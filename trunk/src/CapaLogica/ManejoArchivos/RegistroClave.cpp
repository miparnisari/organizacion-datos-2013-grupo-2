/*
 * RegistroClave.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: martin
 */

#include "RegistroClave.h"

RegistroClave::RegistroClave()
	:RegistroVariable()
{
	clave = NULL;
//	_agregar_campo_clave();
}

RegistroClave::RegistroClave(const RegistroClave& otro)
	: RegistroVariable(otro)
{
	clave = new ClaveX(*(otro.clave));
//	_agregar_campo_clave();
//	this->buffer= new char[tamanio];
//	stringstream ss;
//	ss.write(otro.buffer,tamanio);
//	ss.seekg(0,ios::beg);
//	ss.read(this->buffer,tamanio);

}

RegistroClave::~RegistroClave()
{
	delete(clave);
}


void RegistroClave::_agregar_campo_clave()
{
	unsigned short tamanioClaveEmpaquetada = clave->get_tamanio_empaquetado();
	char* bufferClaveEmpaquetada= new char[tamanioClaveEmpaquetada]();

	clave->empaquetar(bufferClaveEmpaquetada);

	this->agregar_campo(bufferClaveEmpaquetada,tamanioClaveEmpaquetada);

	delete[] bufferClaveEmpaquetada;
}

void RegistroClave::limpiar_campos() throw(){

	RegistroVariable::limpiar_campos();
	_agregar_campo_clave();

}

void RegistroClave::set_clave(const ClaveX& clave){

	// Si ya tenia una clave no hacemos nada
	if (this->clave != NULL)
		return;

	this->clave = new ClaveX(clave);
	_agregar_campo_clave();

//	const unsigned short CANTIDAD_CAMPOS= this->get_cantidad_campos();
//	RegistroVariable registroCopia;
//	unsigned short tamanioEmpaquetamiento= this->get_tamanio_empaquetado();
//	char* bufferRegistroCopia= new char[tamanioEmpaquetamiento]();
//	this->empaquetar(bufferRegistroCopia);
//	registroCopia.desempaquetar(bufferRegistroCopia);
//	//hago una copia del registro original
//
//	this->limpiar_campos();
//
//
//	if(CANTIDAD_CAMPOS>1)
//		for(unsigned short i=1;i<CANTIDAD_CAMPOS;i++){
//
//			unsigned short tamanioCampo= registroCopia.get_tamanio_campo(i);
//			char* bufferCampo= new char[tamanioCampo]();
//			registroCopia.recuperar_campo(bufferCampo,i);
//			this->agregar_campo(bufferCampo,tamanioCampo);
//
//			delete[] bufferCampo;
//
//		}
//		//agrego los campos viejos en el registro nuevamente
//
//	delete[] bufferRegistroCopia;

}


ClaveX RegistroClave::get_clave()
{
	return (*this->clave);
}

int RegistroClave::desempaquetar(const char* copia)throw(){

	int resultado= RegistroVariable::desempaquetar(copia);
	if(resultado== RES_ERROR)
		return RES_ERROR;

	unsigned short tamanioCampoClave= this->get_tamanio_campo(NUMERO_CAMPO_CLAVE);
	char* bufferClave= new char[tamanioCampoClave]();
	this->recuperar_campo(bufferClave , tamanioCampoClave);

	this->clave->desempaquetar(bufferClave,tamanioCampoClave);

	delete[] bufferClave;
	return RES_OK;

}

bool RegistroClave::operator <(const RegistroClave& rc)const
{
	return (this->clave < rc.clave);

}

bool RegistroClave::operator >(const RegistroClave& rc)const
{
	return (this->clave > rc.clave);
}

bool RegistroClave::operator ==(const RegistroClave& rc)const{

	return (this->clave == rc.clave);

}

bool RegistroClave::operator <=(const RegistroClave& rc)const{

	return (this->clave <= rc.clave);

}

bool RegistroClave::operator >=(const RegistroClave& rc)const{

	return (this->clave >= rc.clave);

}
