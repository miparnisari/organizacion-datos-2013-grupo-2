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
}

RegistroClave::RegistroClave(const RegistroClave& otro)
	: RegistroVariable(otro)
{
	clave = otro.clave;
}

RegistroClave& RegistroClave::operator=(const RegistroClave& otro)
{
	if (this != &otro)
	{
		this->clave = otro.clave;
		this->tamanio = otro.tamanio;
	//	buffer = new char[tamanio];
	//	strcpy(buffer,otro.buffer);
		delete[] buffer;
		this->buffer= new char[tamanio];
		stringstream ss;
		ss.write(otro.buffer,tamanio);
		ss.seekg(0,ios::beg);
		ss.read(this->buffer,tamanio);
	}

	return *this;
}

RegistroClave::~RegistroClave()
{
}


void RegistroClave::_agregar_campo_clave()
{
	unsigned short tamanioClaveEmpaquetada = clave.get_tamanio_empaquetado();
	char* bufferClaveEmpaquetada= new char[tamanioClaveEmpaquetada]();

	clave.empaquetar(bufferClaveEmpaquetada);

	this->agregar_campo(bufferClaveEmpaquetada,tamanioClaveEmpaquetada);

	delete[] bufferClaveEmpaquetada;
}

void RegistroClave::limpiar_campos() throw(){

	RegistroVariable::limpiar_campos();

}

void RegistroClave::set_clave(const ClaveX& clave){
	this->clave = clave;
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


ClaveX RegistroClave::get_clave()const
{
	return clave;
}

int RegistroClave::desempaquetar(const char* copia)throw(){

	int resultado= RegistroVariable::desempaquetar(copia);
	if (resultado == RES_ERROR)
		return RES_ERROR;

	unsigned short tamanioCampoClave= this->get_tamanio_campo(NUMERO_CAMPO_CLAVE);
	char* bufferClave= new char[tamanioCampoClave + 1]();
	bufferClave[tamanioCampoClave] = '\0';

	if (this->recuperar_campo(bufferClave , 0) > 0)
	{
		ClaveX clave;
		clave.desempaquetar(bufferClave,tamanioCampoClave);

		set_clave(clave);

		delete[] bufferClave;
		return RES_OK;
	}

	delete[] bufferClave;
	return RES_ERROR;

}

bool RegistroClave::operator <(const RegistroClave& rc)const
{
	return ((this->clave) < (rc.clave));
}

bool RegistroClave::operator >(const RegistroClave& rc)const
{
	return ((this->clave) > (rc.clave));
}

bool RegistroClave::operator ==(const RegistroClave& rc)const
{
	return ((this->clave) == (rc.clave));
}

bool RegistroClave::operator <=(const RegistroClave& rc)const
{
	return ((this->clave) <= (rc.clave));
}

bool RegistroClave::operator >=(const RegistroClave& rc)const
{
	return ((this->clave) >= (rc.clave));
}
