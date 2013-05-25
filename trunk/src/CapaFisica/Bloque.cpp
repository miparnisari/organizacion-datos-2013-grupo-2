#include "Bloque.h"

Bloque::Bloque(unsigned int tamBloque)
{
	this->tamanioBloque= tamBloque;
	this->espacioLibre= tamanioBloque - sizeof(espacioLibre);
	this->espacioLibreOffset= espacioLibre;
	this->bufferBloque= new char[tamanioBloque]();
	_limpiar_buffer();
}

Bloque::~Bloque()
{
	delete[] bufferBloque;
}

Bloque::Bloque(const Bloque& otro)
{
	tamanioBloque = otro.tamanioBloque;
	espacioLibre = otro.espacioLibre;
	espacioLibreOffset = otro.espacioLibreOffset;
	bufferBloque = new char[tamanioBloque]();
	/*limpiar_buffer antes se invocaba despues del memcpy , deberia invocarse antes.*/

	memcpy(this->bufferBloque,otro.bufferBloque,tamanioBloque);
}

bool Bloque::fue_eliminado(){

	if(!this->esta_vacio())
		return false;

	stringstream stream;
	stream.write(bufferBloque,tamanioBloque);
	stream.seekg(0,ios::beg);

	char caracterBorrado;
	stream.read( (char*)&caracterBorrado , sizeof(caracterBorrado) );

	return caracterBorrado== MARCA_BORRADO;

}


int Bloque::actualizar_ref_prox_bloque(unsigned int primerBloque){

	int res;

	if(!this->fue_eliminado()){
		this->_limpiar_buffer();
		res= RES_OK;
	}
	else
		res= RES_ERROR;
	/*vacio el bloque, marco que su espacio libre es total*/

	stringstream stream;
	stream.write(bufferBloque , tamanioBloque);
	stream.seekp(0,ios::beg);
	char marcaBorrado= MARCA_BORRADO;
	stream.write( (char*)&marcaBorrado , sizeof(marcaBorrado) );

	stream.write( (char*)&primerBloque , sizeof(primerBloque) );
	stream.seekg(0,ios::beg);
	stream.read( bufferBloque , tamanioBloque );


	return res;

}


int Bloque::obtener_ref_prox_bloque(){

	if(!this->fue_eliminado())
		return RES_BLOQUE_NO_BORRADO;

	stringstream stream;
	stream.write( bufferBloque , tamanioBloque );
	char caracterEliminacion;
	stream>>caracterEliminacion;

	int proximoBloque;
	stream.read( (char*)&proximoBloque , sizeof(proximoBloque) );

	return proximoBloque;

}

unsigned int Bloque::_calcular_espacio_libre()const throw(){

	return  espacioLibre;

}/*calcula el espacio libre disponible para una siguiente alta de registro*/


unsigned int Bloque::_calcular_espacio_usable()const throw(){

	return (tamanioBloque - sizeof(espacioLibre));

}/*retorna la cantidad de espacio del bloque usable para guardar registros*/


unsigned int Bloque::_calcular_espacio_ocupado()const throw(){

	unsigned int espacioUsable= _calcular_espacio_usable();
	unsigned int espacioOcupado= espacioUsable - espacioLibre;

	return espacioOcupado;

}/*retorna el espacio usado por registros*/


unsigned int Bloque::_obtener_offset_final()const throw(){

	unsigned int offsetFinal;
	offsetFinal= tamanioBloque - espacioLibre - sizeof(espacioLibre);

	return offsetFinal;

}/*devuelve el byte offset del final del buffer*/


bool Bloque::esta_vacio()const throw()
{
	return this->_calcular_espacio_ocupado() == 0;

}/*El bloque se considera vacio si su espacio ocupado
es unicamente debido al almacenamiento del espacio libre disponible en buffer */


void Bloque::_limpiar_buffer()throw(){

	/*unsigned int espacioOcupado= this->calcular_espacio_ocupado();
	if(espacioOcupado){
		char* limpio= new char[espacioOcupado];
		strcpy( limpio , "~");
		memcpy( bufferBloque , limpio , espacioOcupado );
		delete[] limpio;
	}*/

	strcpy( bufferBloque , "*****" );

	espacioLibre= tamanioBloque - sizeof(espacioLibre);

	_escribir_espacio_libre();

}/*limpia el buffer*/


void Bloque::desempaquetar(const char* datos)throw(){

	stringstream stream(ios::in |ios::out);

	stream.write(datos,tamanioBloque);
	stream.seekg(0,stream.beg);
	stream.read(bufferBloque,tamanioBloque);
	_obtener_espacio_libre();

}/*carga un bloque contenido en datos en la estructura*/


int Bloque::_agregar_registro(char* dato, unsigned short tamanioDato)throw(){

	if(dato == NULL)
		return RES_ERROR;
	if(tamanioDato >= _calcular_espacio_libre())
		return RES_INSUFFICIENT_SPACE;

	stringstream stream(ios::in |ios::out);
	stream.write(bufferBloque,tamanioBloque); //copia en stream lo de bufferBloque

	unsigned int offsetFinal= this->_obtener_offset_final();
	if(!this->esta_vacio())
		stream.seekp(offsetFinal,stream.beg);
	else
		stream.seekp(0,stream.beg);
	stream.write((char*)(&tamanioDato),sizeof(tamanioDato));
	espacioLibre-= sizeof(tamanioDato);
	stream.write(dato,tamanioDato);
	espacioLibre-= tamanioDato;

	stream.seekg(0,stream.beg);
	stream.read(bufferBloque,tamanioBloque);

	_escribir_espacio_libre();
	return RES_OK;

}/*agrega los datos contenidos en dato como un registro de tamanio variable.
La estructura de dato debe ser tamanio_dato|dato . Retorna ESPACIO_INSUFICIENTE , DATO_NULO o RES_OK*/


int Bloque::agregar_registro(RegistroVariable* registro)throw(){

	if(registro== NULL)
		return RES_ERROR;
	if(registro->esta_limpio())
		return RES_ERROR;

	unsigned short tamanioDato= registro->get_tamanio();
	return this->_agregar_registro( registro->get_buffer() , tamanioDato );

}/*agrega un registro variable al bloque*/

unsigned short Bloque::get_cantidad_registros_almacenados()const throw(){

	if(this->esta_vacio())
		return 0;

	unsigned int espacioUsado= this->_obtener_offset_final();
	unsigned int cuentaBytes= 0;
	unsigned short cuentaRegistros= 0;
	stringstream stream(ios::in | ios::out);
	stream.write(bufferBloque,tamanioBloque);
	stream.seekg(0,stream.beg);


	while(cuentaBytes< espacioUsado){

		unsigned short tamanioBufferAux;
		stream.read( (char*)(&tamanioBufferAux) , sizeof(tamanioBufferAux) );
		char* bufferAux = new char[tamanioBufferAux];
		stream.read(bufferAux,tamanioBufferAux);

		cuentaRegistros++;

		cuentaBytes+= sizeof(tamanioBufferAux);
		cuentaBytes+= tamanioBufferAux;
		delete[] bufferAux;
	}

	return cuentaRegistros;

}/*cuenta la cantidad de registros variables guardados*/


int Bloque::recuperar_registro(RegistroVariable* registro,unsigned short numeroRegistro)const throw()
{
	if(registro== NULL)
		return RES_ERROR;
	if(this->esta_vacio())
		return RES_ERROR;
	if(this->get_cantidad_registros_almacenados() <= numeroRegistro)
		return RES_ERROR;

	unsigned short cuentaRegistros= 0;
	stringstream stream(ios::in | ios::out);
	stream.write(bufferBloque,tamanioBloque); //copia en stream lo de bufferbloque
	
	unsigned short tamanioBufferAux;
	while(cuentaRegistros < numeroRegistro){

		stream.read( (char*)(&tamanioBufferAux),sizeof(tamanioBufferAux) );
		char* bufferAux = new char[tamanioBufferAux];
		stream.read(bufferAux , tamanioBufferAux);
		delete[] bufferAux;
		cuentaRegistros++;

	}

	stream.read( (char*)(&tamanioBufferAux),sizeof(tamanioBufferAux) );
	char* bufferAux = new char[tamanioBufferAux + 1];
	stream.read(bufferAux , tamanioBufferAux);
	/*se recupera el registro. bufferAux tendra la siguiente estructura:
	 * 	tamanioCampo1|campo1|tamanioCampo2|campo2|...  */

	stringstream streamEscritura;
	const int TAMANIO_BUFFER_ESCRITURA= tamanioBufferAux + sizeof(tamanioBufferAux);
	char* bufferEscritura= new char[ TAMANIO_BUFFER_ESCRITURA ];
	streamEscritura.write( (char*)&tamanioBufferAux , sizeof(tamanioBufferAux) );
	streamEscritura.write( bufferAux , tamanioBufferAux );
	streamEscritura.seekg(0,ios::beg);

	streamEscritura.read(bufferEscritura , TAMANIO_BUFFER_ESCRITURA);
	registro->desempaquetar(bufferEscritura);


	//registro->agregar_datos(bufferAux,tamanioBufferAux);


	delete[] bufferAux;
	delete[] bufferEscritura;
	return tamanioBufferAux;
}/*recupera el registro con numero numeroRegistro. Los registros se cuentan a partir del 0.*/


void Bloque::empaquetar(char* copia)const throw(){

	stringstream stream(ios::in |ios::out);
	stream.write(bufferBloque,tamanioBloque);

	stream.seekg(0,stream.beg);
	stream.read(copia,tamanioBloque);

}/*exporta los datos del buffer en copia. Se exporta el bloque COMPLETO. copia debe tener datos reservados*/


unsigned int Bloque::_obtener_offset_registro(unsigned short numeroRegistro)const throw(){

	if(numeroRegistro>= get_cantidad_registros_almacenados())
		return RES_INVALID_OFFSET;

	unsigned int offset= 0;
	unsigned short cuentaRegistros= 0;
	stringstream stream;
	stream.write(bufferBloque,tamanioBloque);
	stream.seekg(0,stream.beg);

	while(cuentaRegistros < numeroRegistro){

		unsigned short tamanioBuffer;
		stream.read( (char*)(&tamanioBuffer) , sizeof(tamanioBuffer) );
		char* bufferAux = new char [tamanioBuffer];
		stream.read(bufferAux,tamanioBuffer);

		offset+= sizeof(tamanioBuffer);
		offset+= tamanioBuffer;
		cuentaRegistros++;
		delete[] bufferAux;
	}

	return offset;

}/*retorna el valor del byte offset de un registro determinado*/


int Bloque::eliminar_registro(unsigned short numeroRegistro)throw(){

	if(this->esta_vacio())
		return RES_OK;

	const unsigned int OFFSET_REGISTRO= this->_obtener_offset_registro(numeroRegistro);
	if((int)OFFSET_REGISTRO == RES_INVALID_OFFSET)
		return RES_ERROR;

	stringstream stream;
	stream.write(bufferBloque,tamanioBloque);
	stream.seekg(OFFSET_REGISTRO , stream.beg);
	/*levanto el buffer al stream*/

	unsigned short tamanioRegistro;
	unsigned short tamanioRegistroGrabado= 0;
	stream.read( (char*)(&tamanioRegistro) , sizeof(tamanioRegistro) );
	tamanioRegistroGrabado+= sizeof(tamanioRegistro);
	tamanioRegistroGrabado+= tamanioRegistro;
	char* bufferAux = new char [tamanioRegistro];
	stream.read(bufferAux,tamanioRegistro);
	delete[] bufferAux;
	/*se determina tamanio del registro completo grabado a borrar del bloque (incluido su tamanio en bloque)*/

	unsigned int restoBloque= this->_calcular_espacio_ocupado();
	restoBloque-= OFFSET_REGISTRO;
	restoBloque-= tamanioRegistroGrabado;
	/*se calcula cuanto espacio ocupado por registros hay despues del registro a borrar*/

	if(restoBloque){

		char* bufferRestoBloque = new char[restoBloque];
		stream.read( bufferRestoBloque , restoBloque );
		stream.seekp(OFFSET_REGISTRO , stream.beg);
		stream.write( bufferRestoBloque , restoBloque );
		delete[] bufferRestoBloque;

	}/*si queda espacio ocupado por registros despues de aquel a borrar, estos registros se mueven al
	offset del registro a borrar*/

	stream.seekg(0,stream.beg);
	stream.read(bufferBloque,tamanioBloque);
	espacioLibre+= tamanioRegistroGrabado;
	_escribir_espacio_libre();

	return RES_OK;

}/*remueve un registro indicado por el numeroRegistro*/




void Bloque::_obtener_espacio_libre()const throw(){

	stringstream stream(ios::in | ios::out);
	stream.write(bufferBloque,tamanioBloque);

	stream.seekg(espacioLibreOffset , stream.beg);
	stream.read( (char*)(&espacioLibre) , sizeof(espacioLibre) );

}/*recupera el valor de espacio libre guardado en buffer y lo almacena en espacioLibre*/


void Bloque::_escribir_espacio_libre()throw(){

	stringstream stream(ios::in | ios::out);

	stream.write(bufferBloque,tamanioBloque);
	stream.seekp(espacioLibreOffset , stream.beg);
	stream.write( (char*)(&espacioLibre) , sizeof(espacioLibre) );
	stream.seekg(0,stream.beg);
	stream.read(bufferBloque,tamanioBloque);

}/*escribe en el buffer el espacio libre*/

void Bloque::_listar_registros()throw(){

	if(this->esta_vacio())
		return;

	unsigned short cantidadRegistros= this->get_cantidad_registros_almacenados();
	stringstream stream;
	stream.write(bufferBloque,tamanioBloque);
	stream.seekg(0,ios::beg);

	unsigned short cuentaRegistros= 0;
	unsigned int byteOffset= 0;

	while(cuentaRegistros <cantidadRegistros){

		unsigned short tamanioBufferAux;
		stream.read( (char*)(&tamanioBufferAux),sizeof(tamanioBufferAux) );

		if(stream.good()){
			cout<<"Registro: "<<cuentaRegistros<<" | Offset: "<<byteOffset<<endl;
			cuentaRegistros++;
			byteOffset+= sizeof(tamanioBufferAux);
			byteOffset+= tamanioBufferAux;
			char* bufferAux = new char[tamanioBufferAux];
			stream.read(bufferAux,tamanioBufferAux);
			delete[] bufferAux;
		}

	}

}/*lista el byte offset de cada registro contenido en el bloque*/

