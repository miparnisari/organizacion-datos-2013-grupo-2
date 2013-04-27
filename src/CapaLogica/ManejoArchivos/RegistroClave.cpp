/*
 * RegistroClave.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: martin
 */

#include "RegistroClave.h"

RegistroClave::RegistroClave():RegistroVariable(){}


RegistroClave::RegistroClave(Clave* c):RegistroVariable(){

	int const TAMANIO_CLAVE= c->get_tamanio();
	char* bufferClave= new char[TAMANIO_CLAVE];

	c->empaquetar(bufferClave);
	this->agregar_campo(bufferClave,TAMANIO_CLAVE);

	delete[] bufferClave;

}

RegistroClave::~RegistroClave(){}


bool RegistroClave::solo_clave(){

	return this->get_cantidad_campos()== 1;

}


int RegistroClave::get_clave(Clave* clave){

	if(buffer== NULL)
		return RES_ERROR;
	if(this->fue_eliminado() || clave== NULL)
		return RES_ERROR;

	int const TAMANIO_CAMPO_CLAVE= this->get_tamanio_campo(NUMERO_CAMPO_CLAVE);
	char* bufferClave= new char[TAMANIO_CAMPO_CLAVE];
	this->recuperar_campo(bufferClave,NUMERO_CAMPO_CLAVE);
	clave->desempaquetar(bufferClave,TAMANIO_CAMPO_CLAVE);

	delete[] bufferClave;

	return RES_OK;

}


int RegistroClave::set_clave(Clave* clave){

	if(this->fue_eliminado())
		return RES_ERROR;

	const int TAMANIO_CLAVE= clave->get_tamanio();
	char* bufferClave= new char[TAMANIO_CLAVE];
	clave->empaquetar(bufferClave);

	if(buffer== NULL){
		this->agregar_campo(bufferClave,TAMANIO_CLAVE);
		delete[] bufferClave;
		return RES_OK;
	}


	RegistroVariable registroAuxiliar;
	const int CANTIDAD_CAMPOS= this->get_cantidad_campos();
	registroAuxiliar.agregar_campo(bufferClave,TAMANIO_CLAVE);

	for(int i=1;i<CANTIDAD_CAMPOS;i++){
		int tamanioCampo= this->get_tamanio_campo(i);
		char* campo= new char[tamanioCampo];
		this->recuperar_campo(campo,i);
		registroAuxiliar.agregar_campo(campo,tamanioCampo);
		delete[] campo;
	}


	int const TAMANIO_REGISTRO_AUXILIAR= registroAuxiliar.get_tamanio();
	char* bufferRegistroAuxiliar= new char[TAMANIO_REGISTRO_AUXILIAR];
	registroAuxiliar.empaquetar(bufferRegistroAuxiliar);
	this->desempaquetar(bufferRegistroAuxiliar);
	delete[] bufferRegistroAuxiliar;

	delete[] bufferClave;

	return RES_OK;

}
