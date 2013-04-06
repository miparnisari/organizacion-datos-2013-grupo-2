/*
 * Bloque.cpp
 *
 *  Created on: Mar 25, 2013
 *      Author: martin
 */

#include "Bloque.h"


unsigned int Bloque::calcular_espacio_libre()throw(){

	return  espacioLibre;

}/*calcula el espacio libre disponible para una siguiente alta de registro*/


unsigned int Bloque::calcular_espacio_usable()throw(){

	return (tamanioBloque - sizeof(espacioLibre));

}/*retorna la cantidad de espacio del bloque usable para guardar registros*/


unsigned int Bloque::calcular_espacio_ocupado()throw(){

	unsigned int espacioUsable= calcular_espacio_usable();
	unsigned int espacioOcupado= espacioUsable - espacioLibre;

	return espacioOcupado;

}/*retorna el espacio usado por registros*/


unsigned int Bloque::obtener_offset_final()throw(){

	unsigned int offsetFinal;
	offsetFinal= tamanioBloque - espacioLibre - sizeof(espacioLibre);

	return offsetFinal;

}/*devuelve el byte offset del final del buffer*/


bool Bloque::esta_vacio()throw(){

	unsigned int espacioUsado= this->obtener_offset_final();
	return espacioUsado== 0;

}/*verifica si el bolque esta vacio. El mismo se considera vacio si su espacio ocupado es unicamente debido al almacenamiento del espacio libre disponible en buffer*/


void Bloque::limpiar()throw(){

	espacioLibre= tamanioBloque - sizeof(espacioLibre);

	escribir_espacio_libre();

}/*limpia el buffer*/


void Bloque::cargar(char* datos)throw(){

	stringstream stream(ios::in |ios::out);

	stream.write(datos,tamanioBloque);
	stream.read(bufferBloque,tamanioBloque);
	obtener_espacio_libre();

}/*carga un bloque contenido en datos en la estructura*/


int Bloque::agregar_registro(char* dato,unsigned int tamanioDato)throw(){

	if(dato== NULL)
		return DATO_NULO;
	if(tamanioDato >= calcular_espacio_libre())
		return ESPACIO_INSUFICIENTE;

	stringstream stream(ios::in |ios::out);
	stream.write(bufferBloque,tamanioBloque);

	unsigned int offsetFinal= this->obtener_offset_final();
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

	escribir_espacio_libre();
	return EXITO;

}/*agrega los datos contenidos en dato como un registro de tamanio variable. La estructura de dato debe ser tamanio_dato|dato . Retorna ESPACIO_INSUFICIENTE , DATO_NULO o EXITO*/


int Bloque::agregar_registro(RegistroVariable* registro)throw(){

	if(registro== NULL)
		return DATO_NULO;

	unsigned int tamanioDato= registro->get_tamanio();
	return this->agregar_registro( registro->get_buffer() , tamanioDato );

}/*agrega un registro variable al bloque*/


unsigned short Bloque::contar_registros()throw(){

	if(this->esta_vacio())
		return 0;

	unsigned int espacioUsado= this->obtener_offset_final();
	unsigned int cuentaBytes= 0;
	unsigned short cuentaRegistros= 0;
	stringstream stream(ios::in | ios::out);
	stream.write(bufferBloque,tamanioBloque);
	stream.seekg(0,stream.beg);


	while(cuentaBytes< espacioUsado){

		unsigned int tamanioBufferAux;
		stream.read( (char*)(&tamanioBufferAux) , sizeof(tamanioBufferAux) );
		char bufferAux[tamanioBufferAux];
		stream.read(bufferAux,tamanioBufferAux);

		cuentaRegistros++;

		cuentaBytes+= sizeof(tamanioBufferAux);
		cuentaBytes+= tamanioBufferAux;
	}

	return cuentaRegistros;

}/*cuenta la cantidad de registros variables guardados*/


int Bloque::recuperar_registro(RegistroVariable* registro,unsigned short numeroRegistro)throw(){

	if(registro== NULL)
		return DATO_NULO;
	if(this->esta_vacio())
		return BLOQUE_VACIO;
	if(this->contar_registros() <= numeroRegistro)
		return FRACASO;

	unsigned short cuentaRegistros= 0;
	stringstream stream(ios::in | ios::out);
	stream.write(bufferBloque,tamanioBloque);

	while(cuentaRegistros < numeroRegistro){

		unsigned int tamanioBufferAux;
		stream.read( (char*)(&tamanioBufferAux),sizeof(tamanioBufferAux) );
		char bufferAux[tamanioBufferAux];
		stream.read(bufferAux , tamanioBufferAux);
		cuentaRegistros++;

	}

	unsigned int tamanioBufferAux;
	stream.read( (char*)(&tamanioBufferAux),sizeof(tamanioBufferAux) );
	char bufferAux[tamanioBufferAux];
	stream.read(bufferAux , tamanioBufferAux);

	registro->agregar_datos(bufferAux,tamanioBufferAux);
	return EXITO;


}/*recupera el registro con numero numeroRegistro. Los registros se cuentan a partir del 0.*/


void Bloque::exportar_bloque(char* copia)throw(){

	stringstream stream(ios::in |ios::out);
	stream.write(bufferBloque,tamanioBloque);

	stream.seekg(0,stream.beg);
	stream.read(copia,tamanioBloque);

}/*exporta los datos del buffer en copia. Se exporta el bloque COMPLETO. copia debe tener datos reservados*/


int Bloque::guardar(ostream& salida)throw(){

	if(!salida.good())
		return FRACASO;

	salida.write(bufferBloque,tamanioBloque);
	return EXITO;

}/*guarda los datos del buffer en un objeto ostream.*/


void Bloque::listar_registros()throw(){

	if(this->esta_vacio())
		return;

	unsigned short cantidadRegistros= this->contar_registros();
	stringstream stream;
	stream.write(bufferBloque,tamanioBloque);
	stream.seekg(0,ios::beg);

	unsigned short cuentaRegistros= 0;
	unsigned int byteOffset= 0;

	while(cuentaRegistros <cantidadRegistros){

		unsigned int tamanioBufferAux;
		stream.read( (char*)(&tamanioBufferAux),sizeof(tamanioBufferAux) );

		if(stream.good()){
			cout<<"Registro: "<<cuentaRegistros<<" | Offset: "<<byteOffset<<endl;
			cuentaRegistros++;
			byteOffset+= sizeof(tamanioBufferAux);
			byteOffset+= tamanioBufferAux;
			char bufferAux[tamanioBufferAux];
			stream.read(bufferAux,tamanioBufferAux);

		}

	}

}/*lista el byte offset de cada registro contenido en el bloque*/


unsigned int Bloque::obtener_offset_registro(unsigned short numeroRegistro)throw(){

	if(numeroRegistro>= contar_registros())
		return OFFSET_INVALIDO;

	unsigned int offset= 0;
	unsigned short cuentaRegistros= 0;
	stringstream stream;
	stream.write(bufferBloque,tamanioBloque);
	stream.seekg(0,stream.beg);

	while(cuentaRegistros < numeroRegistro){

		unsigned int tamanioBuffer;
		stream.read( (char*)(&tamanioBuffer) , sizeof(tamanioBuffer) );
		char bufferAux[tamanioBuffer];
		stream.read(bufferAux,tamanioBuffer);

		offset+= sizeof(tamanioBuffer);
		offset+= tamanioBuffer;
		cuentaRegistros++;

	}

	return offset;

}/*retorna el valor del byte offset de un registro determinado*/


int Bloque::remover_registro(unsigned short numeroRegistro)throw(){

	if(this->esta_vacio())
		return EXITO;

	const unsigned int OFFSET_REGISTRO= this->obtener_offset_registro(numeroRegistro);
	if(OFFSET_REGISTRO == OFFSET_INVALIDO)
		return FRACASO;

	stringstream stream;
	stream.write(bufferBloque,tamanioBloque);
	stream.seekg(OFFSET_REGISTRO , stream.beg);
	/*levanto el buffer al stream*/

	unsigned int tamanioRegistro;
	unsigned int tamanioRegistroGrabado= 0;
	stream.read( (char*)(&tamanioRegistro) , sizeof(tamanioRegistro) );
	tamanioRegistroGrabado+= sizeof(tamanioRegistro);
	tamanioRegistroGrabado+= tamanioRegistro;
	char bufferAux[tamanioRegistro];
	stream.read(bufferAux,tamanioRegistro);
	/*se determina tamanio del registro completo grabado a borrar del bloque (incluido su tamanio en bloque)*/

	unsigned int restoBloque= this->calcular_espacio_ocupado();
	restoBloque-= OFFSET_REGISTRO;
	restoBloque-= tamanioRegistroGrabado;
	/*se calcula cuanto espacio ocupado por registros hay despues del registro a borrar*/

	if(restoBloque){

		char bufferRestoBloque[restoBloque];
		stream.read( bufferRestoBloque , restoBloque );
		stream.seekp(OFFSET_REGISTRO , stream.beg);
		stream.write( bufferRestoBloque , restoBloque );


	}/*si queda espacio ocupado por registros despues de aquel a borrar, estos registros se mueven al
	offset del registro a borrar*/

	stream.seekg(0,stream.beg);
	stream.read(bufferBloque,tamanioBloque);
	espacioLibre-= tamanioRegistroGrabado;
	escribir_espacio_libre();

	return EXITO;

}/*remueve un registro indicado por el numeroRegistro*/


void Bloque::obtener_espacio_libre()throw(){

	stringstream stream(ios::in | ios::out);
	stream.write(bufferBloque,tamanioBloque);

	stream.seekg(espacioLibreOffset , stream.beg);
	stream.read( (char*)(&espacioLibre) , sizeof(espacioLibre) );

}/*recupera el valor de espacio libre guardado en buffer y lo almacena en espacioLibre*/


void Bloque::escribir_espacio_libre()throw(){

	stringstream stream(ios::in | ios::out);

	stream.write(bufferBloque,tamanioBloque);
	stream.seekp(espacioLibreOffset , stream.beg);
	stream.write( (char*)(&espacioLibre) , sizeof(espacioLibre) );
	stream.seekg(0,stream.beg);
	stream.read(bufferBloque,tamanioBloque);

}/*escribe en el buffer el espacio libre*/
