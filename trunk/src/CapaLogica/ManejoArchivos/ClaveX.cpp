/*
 * ClaveX.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: martin
 */

#include "ClaveX.h"

ClaveX::ClaveX(){

	this->tipoClave = CLAVE_STRING; //tipo por defecto
	claves[CLAVE_NUMERICA]= new ClaveNumerica;
	claves[CLAVE_STRING]= new ClaveString;

}


ClaveX::ClaveX(const ClaveX& clavex){

	claves[CLAVE_NUMERICA]= new ClaveNumerica;
	claves[CLAVE_STRING]= new ClaveString;

	(*this)= clavex;

}


ClaveX::~ClaveX(){

	for (unsigned int i = 0; i < CANTIDAD_TIPOS_CLAVE; i++)
		delete(claves[i]);
}


ClaveX::TipoClave ClaveX::get_tipo_clave(){

	return tipoClave;

}


void ClaveX::_resetear_datos(){

	for(unsigned int i=0;i<CANTIDAD_TIPOS_CLAVE;i++)
		claves[i]->resetear();

}


int ClaveX::get_tamanio_empaquetado(){

	return this->get_tamanio_clave() + sizeof(char);

}

int ClaveX::get_tamanio_clave(){

	return this->claves[tipoClave]->get_tamanio();

}


int ClaveX::empaquetar(char* buffer){

	unsigned short tamanioClave= this->get_tamanio_clave();
	char tipoClaveChar= (char)tipoClave;
	unsigned short tamanioEmpaquetado= this->get_tamanio_empaquetado();

	stringstream stream;
	stream.write( (char*)&tipoClaveChar , sizeof(tipoClaveChar) );
	char* empaquetado= new char[tamanioClave];
	claves[tipoClave]->empaquetar(empaquetado);
	stream.write( empaquetado,tamanioClave );

	stream.seekg(0,ios::beg);
	stream.read( buffer,tamanioEmpaquetado );


	delete[] empaquetado;
	return tamanioEmpaquetado;


}

void ClaveX::desempaquetar(char* buffer,unsigned short tamanioBuffer){

	unsigned short tamanioClave= tamanioBuffer - sizeof(char);
	char* bufferClave= new char[tamanioClave];

	stringstream stream;
	stream.write( buffer,tamanioBuffer );
	stream.seekg( 0 ,ios::beg);

	char tipoClaveChar;
	stream.read( (char*)&tipoClaveChar , sizeof(tipoClaveChar) );
	tipoClave= (TipoClave)tipoClaveChar;

	stream.read( bufferClave,tamanioClave );
	claves[tipoClave]->desempaquetar(bufferClave , tamanioClave);

	delete[] bufferClave;


}

void ClaveX::set_clave(string clave){

	tipoClave= CLAVE_STRING;

	*(ClaveString*)claves[CLAVE_STRING] = clave;

}

void ClaveX::set_clave(int clave){

	tipoClave= CLAVE_NUMERICA;

	ClaveNumerica* cn= (ClaveNumerica*)claves[CLAVE_NUMERICA];
	cn->set_dato(clave);


}




bool ClaveX::operator <(const ClaveX& clavex)const{


	return ( (*claves[tipoClave]) < (*clavex.claves[tipoClave]) );

}

bool ClaveX::operator >(const ClaveX& clavex)const{


	return ( (*claves[tipoClave]) > (*clavex.claves[tipoClave]) );

}

bool ClaveX::operator ==(const ClaveX& clavex)const{

	if(this->tipoClave!= clavex.tipoClave)
		return false;
	return ( (*claves[tipoClave]) == (*clavex.claves[tipoClave]) );

}


bool ClaveX::operator !=(const ClaveX& clavex)const{

	bool sonIguales= (*this)==clavex;
	return !sonIguales;

}


bool ClaveX::operator <=(const ClaveX& clavex)const{

	return ( (*this)<clavex || (*this)==clavex );

}


bool ClaveX::operator >=(const ClaveX& clavex)const{

	return ( (*this)>clavex || (*this)==clavex );

}


void ClaveX::operator =(const ClaveX& clavex){

	this->tipoClave= clavex.tipoClave;

	for(unsigned int i=0;i<CANTIDAD_TIPOS_CLAVE;i++)
		(*claves[i]) = (*clavex.claves[i]);

}


int ClaveX::get_clave(int& clave)const{

	if(tipoClave!= CLAVE_NUMERICA)
		return RES_ERROR;

	ClaveNumerica* cn= (ClaveNumerica*)claves[CLAVE_NUMERICA];
	clave= cn->get_dato();

	return RES_OK;

}

int ClaveX::get_clave(string& clave)const{

	if(tipoClave!= CLAVE_STRING)
		return RES_ERROR;

	ClaveString* cs= (ClaveString*)claves[CLAVE_STRING];
	clave= cs->get_dato();

	return RES_OK;

}
