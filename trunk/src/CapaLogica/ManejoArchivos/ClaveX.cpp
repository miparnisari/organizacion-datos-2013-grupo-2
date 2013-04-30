/*
 * ClaveX.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: martin
 */

#include "ClaveX.h"

ClaveX::ClaveX(ClaveX::TipoClave tipoClave){

	this->tipoClave= tipoClave;
	claves[CLAVE_NUMERICA]= new ClaveNumerica;
	claves[CLAVE_STRING]= new ClaveString;

}


ClaveX::~ClaveX(){

	delete claves[CLAVE_NUMERICA];
	delete claves[CLAVE_STRING];

}


ClaveX::TipoClave ClaveX::get_tipo_clave(){

	return tipoClave;

}


void ClaveX::_resetear_datos(){

	for(int i=0;i<CANTIDAD_TIPOS_CLAVE;i++)
		claves[i]->resetear();

}


void ClaveX::set_tipo_clave(TipoClave tc){

	tipoClave= tc;
	_resetear_datos();

}


int ClaveX::empaquetar(char* buffer){

	const unsigned short TAMANIO_CLAVE= claves[tipoClave]->get_tamanio();
	char* bufferClave= new char[TAMANIO_CLAVE];
	claves[tipoClave]->empaquetar(bufferClave);
	const char TIPO_CLAVE_C= (char)tipoClave;

	stringstream stream;
	stream<<TIPO_CLAVE_C;
	stream.write(bufferClave,TAMANIO_CLAVE);
	stream.seekg(0,ios::beg);
	stream.write(buffer,TAMANIO_CLAVE+sizeof(char));

	delete[] bufferClave;

	return (TAMANIO_CLAVE+sizeof(char));

}

void ClaveX::desempaquetar(char* buffer,unsigned short tamanioBuffer){

	stringstream stream;
	stream.write(buffer,tamanioBuffer);
	char tipoClave_c;
	stream>>tipoClave_c;

	this->tipoClave= (TipoClave)tipoClave_c;
	const unsigned short TAMANIO_CLAVE= tamanioBuffer - sizeof(char);
	char* bufferClave= new char[TAMANIO_CLAVE];

	stream.read(bufferClave,TAMANIO_CLAVE);
	claves[tipoClave]->desempaquetar(bufferClave,TAMANIO_CLAVE);

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




bool ClaveX::operator <(const ClaveX& clavex){


	return ( (*claves[tipoClave]) < (*clavex.claves[tipoClave]) );

}

bool ClaveX::operator >(const ClaveX& clavex){


	return ( (*claves[tipoClave]) > (*clavex.claves[tipoClave]) );

}

bool ClaveX::operator ==(const ClaveX& clavex){


	return ( (*claves[tipoClave]) == (*clavex.claves[tipoClave]) );

}


