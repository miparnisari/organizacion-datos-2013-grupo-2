/*
 * RegistroVariable.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: martin
 */

#include "RegistroVariable.h"

RegistroVariable::RegistroVariable()
{
	buffer= NULL;
	tamanio= 0;
}

RegistroVariable& RegistroVariable::operator = (const RegistroVariable& otro)
{
	if (this != &otro)
	{
		delete[] buffer;
		this->tamanio = otro.tamanio;
		this->buffer = new char[otro.tamanio];
		memcpy(buffer,otro.buffer,tamanio);
	}
	return (*this);
}

RegistroVariable::~RegistroVariable()
{
	delete[] buffer;
}

RegistroVariable::RegistroVariable(const RegistroVariable& otro)
{
	tamanio = otro.tamanio;
	buffer = new char[tamanio];
	stringstream ss;
	ss.write(otro.buffer,tamanio);
	ss.seekg(0,ios::beg);
	ss.read(this->buffer,tamanio);
}


void RegistroVariable::_inicializar_buffer(){

	if (buffer == NULL)
		return;
	for(unsigned short i=0;i<this->tamanio;i++)
		buffer[i]= '*';
}

int RegistroVariable::agregar_datos(const char* datos,unsigned short tamanioDatos)throw(){

	if (this->fue_eliminado() )
	{
		return RES_ERROR;
	}
	unsigned short tamanioFinal = tamanio + tamanioDatos;
	if (tamanioFinal > REG_VAR_MAX_TAM)
	{
		return RES_RECORD_TOO_LONG;
	}

	stringstream stream(ios::in | ios::out);
	if (buffer)
		stream.write(buffer,tamanio); //Inserts the first n characters of the array pointed by s into the stream.
	stream.write(datos,tamanioDatos);
	RegistroVariable::limpiar_campos();

	tamanio= tamanioFinal;
	buffer= new char[tamanioFinal];
	_inicializar_buffer();
	stream.seekg(0,ios::beg);
	stream.read(buffer,tamanio);

	return RES_OK;

}/*agrega datos al buffer menor que REG_VAR_MAX_TAM.
Los datos se agregan de forma directa sin separadores*/


int RegistroVariable::agregar_campo(const char* campo,unsigned short tamanioCampo)throw(){

	int res=this->agregar_datos( (char*)&tamanioCampo, sizeof(tamanioCampo) );

	if(res!= RES_OK)
		return RES_ERROR;

	return this->agregar_datos(campo,tamanioCampo);

}/*agrega un campo al registro junto con su delimitador de espacio.*/


void RegistroVariable::limpiar_campos()throw()
{
	if(buffer){
		delete[] buffer;
		buffer = NULL;
		tamanio = 0;
	}
}


std::string RegistroVariable::mostrar_campo(unsigned short numeroCampo){

	if(buffer==NULL)
		return "***";
	if(this->fue_eliminado())
		return "REGISTRO_ELIMINADO";
	if(numeroCampo >= this->get_cantidad_campos())
		return "***";

	const unsigned short TAMANIO_CAMPO= this->get_tamanio_campo(numeroCampo);
	char* campo= new char[TAMANIO_CAMPO]();
	this->recuperar_campo(campo,numeroCampo);
	string retornar(campo,TAMANIO_CAMPO);
	delete[] campo;
	return retornar;

}


std::string RegistroVariable::mostrar()throw()
{
	string retornar;

	for(unsigned short i= 0; i<this->get_cantidad_campos() ; i++){
		retornar+= mostrar_campo(i) + '|';
	}

	return retornar;
}


int RegistroVariable::eliminar()throw(){

	if(this->fue_eliminado())
		return RES_OK;

	stringstream stream;
	stream.write(buffer,tamanio);
	unsigned short uno= 1;
	stream.seekp(0,ios::beg);

	stream.write( (char*)(&uno),sizeof(uno) );
	stream.write( (char*)(&MARCA_BORRADO) , sizeof(MARCA_BORRADO) );
	stream.seekg(0,stream.beg);

	stream.read(buffer,tamanio);

	return RES_OK;
}/*marca al registro como eliminado:
en el primer campo, en su longitud, pone "1", y en el valor pone
un caracter de marca de borrado * */

bool RegistroVariable::fue_eliminado()throw(){

	if(buffer==NULL)
		return false;

	stringstream stream;
	stream.write(buffer,tamanio);
	stream.seekg(0,stream.beg);
	unsigned short tamanioPrimerCampo= 0;
	char primerCampo=1;

	stream.read( (char*)(&tamanioPrimerCampo) , sizeof(tamanioPrimerCampo) );
	if(tamanioPrimerCampo > 1)
		return false;

	stream.read( (char*)(&primerCampo) , sizeof(primerCampo) );
	if( primerCampo!= MARCA_BORRADO )
		return false;

	return true;


}/*verifica si un registro fue marcado como eliminado*/


int RegistroVariable::empaquetar(char* copia)throw(){

	if(buffer== NULL || copia== NULL)
		return RES_ERROR;

	stringstream stream;

	stream.write( (char*)(&tamanio) , sizeof(tamanio) );
	stream.write( buffer,tamanio );
	stream.seekg(0,stream.beg);
	stream.read(copia, tamanio+sizeof(tamanio) );

	return RES_OK;

}/*exporta en un arreglo de caracteres
 el contenido del registro junto con
 el tamanio del mismo insertado al principio del arreglo*/


unsigned short RegistroVariable::get_cantidad_campos()throw(){

	if(this->fue_eliminado())
		return 0;
	if(buffer== NULL)
		return 0;

	unsigned short contadorOffset= 0;
	unsigned short contadorCampos= 0;
	stringstream stream;
	stream.write(buffer,tamanio);

	while(contadorOffset < tamanio){

		unsigned short tamanioCampo;
		stream.read( (char*)&tamanioCampo , sizeof(tamanioCampo) );
		contadorOffset+= sizeof(tamanioCampo);
		contadorCampos++;
		char* bufferCampo = new char[tamanioCampo];
		stream.read(bufferCampo,tamanioCampo);
		contadorOffset+= tamanioCampo;
		delete[] bufferCampo;
	}

	return contadorCampos;

}/*cuenta la cantidad de campos almacenados en el registro*/


int RegistroVariable::seek_numero_campo(unsigned short numeroCampo){

	if (this->fue_eliminado())
			return RES_ERROR;
	if (numeroCampo >= this->get_cantidad_campos())
		return RES_ERROR;

	stringstream stream;
	stream.write(buffer,tamanio);
	stream.seekg(0,ios::beg);

	unsigned short contadorCampo= 0;
	unsigned short offsetCampo= 0;

	while(contadorCampo < numeroCampo){

		unsigned short tamanioCampo;
		stream.read( (char*)&tamanioCampo , sizeof(tamanioCampo) );
		offsetCampo+= sizeof(tamanioCampo)+tamanioCampo;
		char* dummyBuffer= new char[tamanioCampo];
		stream.read(dummyBuffer,tamanioCampo);
		delete[] dummyBuffer;

		contadorCampo++;
	}

	return offsetCampo;
}


int RegistroVariable::get_tamanio_campo(unsigned short numeroCampo)
{
	if (this->fue_eliminado())
		return RES_ERROR;
	if (numeroCampo >= this->get_cantidad_campos())
		return RES_ERROR;

	stringstream stream;
	stream.write(buffer,tamanio);
	stream.seekg( this->seek_numero_campo(numeroCampo) , ios::beg );

	unsigned short tamanioCampo;
	stream.read( (char*)&tamanioCampo , sizeof(tamanioCampo) );

	return tamanioCampo;
}

int RegistroVariable::recuperar_campo(char* copia,unsigned short numeroCampo)throw(){

	if(this->fue_eliminado() && numeroCampo> CAMPO_ENCADENAMIENTO_LIBRES)
		return RES_ERROR;
	if (numeroCampo >= this->get_cantidad_campos())
		return RES_ERROR;

	stringstream stream;
	stream.write(buffer,tamanio);
	unsigned short contadorCampos= 0;

	while(contadorCampos < numeroCampo){

		unsigned short tamanioCampo;
		stream.read( (char*)&tamanioCampo , sizeof(tamanioCampo) );
		char* bufferCampo = new char[tamanioCampo];
		stream.read(bufferCampo,tamanioCampo);
		delete[] bufferCampo;
		contadorCampos++;
	}

	unsigned short tamanioCampo;
	stream.read( (char*)&tamanioCampo,sizeof(tamanioCampo) );
	stream.read(copia,tamanioCampo);

	return tamanioCampo;

}/*recupera un campo del registro y lo inserta en el buffer copia.
Retorna la cantidad de bytes leidos*/


int RegistroVariable::desempaquetar(const char* copia)throw()
{
	if(copia== NULL) {
		return RES_ERROR;
	}

	limpiar_campos();
	stringstream stream;
	stream.write(copia,sizeof(tamanio)); //Inserts the first n characters of the array pointed by s into the stream.
	stream.seekg(0,stream.beg);
	unsigned short tamanioLeido;

	stream.read( (char*)(&tamanioLeido),sizeof(tamanioLeido) );
	if(tamanioLeido > REG_VAR_MAX_TAM)
		return RES_RECORD_TOO_LONG;

	tamanio= tamanioLeido;
	buffer= new char[tamanio]();
	memcpy(buffer , copia+sizeof(tamanio) , tamanio);


	return RES_OK;

}/*importa un registro a partir de un arreglo
con el formato tamanioRegistro|tamanioPrimerCampo|datos...*/


int RegistroVariable::get_tamanio_empaquetado()const
{
	return (sizeof(tamanio)+tamanio) ;
}


char* RegistroVariable::get_buffer()throw()
{
	return buffer;

}

unsigned short RegistroVariable::get_tamanio()throw()
{
	return tamanio;
}

int RegistroVariable::comprimir (Compresor & compresor)
{
	//TODO
	return RES_OK;
}
