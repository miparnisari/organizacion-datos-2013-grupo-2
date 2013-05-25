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
		delete[] buffer;
		this->buffer= new char[tamanio];
//		memcpy(buffer,otro.buffer,tamanio);
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

	RegistroVariable copiaDeDatos;
	for (int i = 1; i < get_cantidad_campos(); i ++)
	{
		char* copiaDato = new char[get_tamanio_campo(i) + 1]();
		copiaDato[get_tamanio_campo(i)] = '\0';
		recuperar_campo(copiaDato,i);
		copiaDeDatos.agregar_campo(copiaDato,strlen(copiaDato));
		delete[] copiaDato;
	}

	limpiar_campos();

	this->clave = clave;
	this->_agregar_campo_clave();

	for (int i = 0; i < copiaDeDatos.get_cantidad_campos(); i++)
	{
		char* copiaDato = new char[copiaDeDatos.get_tamanio_campo(i) +1]();
		copiaDato[copiaDeDatos.get_tamanio_campo(i)] = '\0';
		copiaDeDatos.recuperar_campo(copiaDato,i);
		this->agregar_campo(copiaDato,strlen(copiaDato));
		delete[] copiaDato;
	}

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
