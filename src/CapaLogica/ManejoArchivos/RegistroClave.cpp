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
	_agregar_campo_clave();
}

RegistroClave::RegistroClave(const RegistroClave& otro)
	: RegistroVariable(otro)
{
	clave = otro.clave;
}

void RegistroClave::_agregar_campo_clave()
{
	unsigned short tamanioClaveEmpaquetada= clave.get_tamanio_empaquetado();
	char* bufferClaveEmpaquetada= new char[tamanioClaveEmpaquetada]();

	clave.empaquetar(bufferClaveEmpaquetada);

	this->agregar_campo(bufferClaveEmpaquetada,tamanioClaveEmpaquetada);

	delete[] bufferClaveEmpaquetada;
}

void RegistroClave::limpiar_buffer() throw(){

	RegistroVariable::limpiar_buffer();
	_agregar_campo_clave();

}


void RegistroClave::set_clave(const ClaveX& clave){

	this->clave= clave;
	const unsigned short CANTIDAD_CAMPOS= this->get_cantidad_campos();
	RegistroVariable registroCopia;
	unsigned short tamanioEmpaquetamiento= this->get_tamanio_empaquetado();
	char* bufferRegistroCopia= new char[tamanioEmpaquetamiento]();
	this->empaquetar(bufferRegistroCopia);
	registroCopia.desempaquetar(bufferRegistroCopia);
	//hago una copia del registro original

	this->limpiar_buffer();


	if(CANTIDAD_CAMPOS>1)
		for(unsigned short i=1;i<CANTIDAD_CAMPOS;i++){

			unsigned short tamanioCampo= registroCopia.get_tamanio_campo(i);
			char* bufferCampo= new char[tamanioCampo]();
			registroCopia.recuperar_campo(bufferCampo,i);
			this->agregar_campo(bufferCampo,tamanioCampo);

			delete[] bufferCampo;

		}
		//agrego los campos viejos en el registro nuevamente

	delete[] bufferRegistroCopia;

}


void RegistroClave::get_clave(ClaveX& clave){

	clave= this->clave;

}


int RegistroClave::desempaquetar(const char* copia)throw(){

	int resultado= RegistroVariable::desempaquetar(copia);
	if(resultado== RES_ERROR)
		return RES_ERROR;

	unsigned short tamanioCampoClave= this->get_tamanio_campo(NUMERO_CAMPO_CLAVE);
	char* bufferClave= new char[tamanioCampoClave]();
	this->recuperar_campo(bufferClave , tamanioCampoClave);

	this->clave.desempaquetar(bufferClave,tamanioCampoClave);

	delete[] bufferClave;
	return RES_OK;

}

bool RegistroClave::operator <(const RegistroClave& rc)const
{
//	std::cout << "una clave =";
//	(this->clave.imprimir_dato());
//	std::cout << std::endl;
//	std::cout << "otra clave =";
//	(rc.clave.imprimir_dato());
//	std::cout << std::endl;
	return (this->clave < rc.clave);

}

bool RegistroClave::operator >(const RegistroClave& rc)const{

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
