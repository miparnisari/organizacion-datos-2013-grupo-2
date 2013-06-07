/*
 * Codigo.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: martin
 */

#include "Codigo.h"

const char* Codigo::OverflowException::what()const throw(){
		string s= exception::what();
		s+="->OverflowException";
		return s.c_str();
}


Codigo::Codigo(int identificador){
	this->identificador= identificador;
}


Codigo::Codigo(const Codigo& otro){

	this->identificador= otro.identificador;
	this->frecuencia= otro.frecuencia;

}


Codigo::~Codigo(){}


unsigned long Codigo::get_frecuencia()const{
	return frecuencia;
}


unsigned long Codigo::incrementar_frecuencia(unsigned long aumento=1)
	throw(Codigo::OverflowException){

	if( (ULONG_MAX - frecuencia) < aumento ){
		OverflowException oe;
		throw oe;
	}

	frecuencia+= aumento;
	return frecuencia;
}


int Codigo::get_identificador()const{
	return identificador;
}


double Codigo::get_probabilidad(unsigned long frecuenciaTotal)const{

	double probabilidad;

	probabilidad= (double)(frecuencia / frecuenciaTotal);
	return probabilidad;

}


unsigned long Codigo::get_proporcion(unsigned long frecuenciaTotal,unsigned long rango)const{

	double probabilidad= get_probabilidad(frecuenciaTotal);
	unsigned long proporcion= (long)(probabilidad*rango);

	return proporcion;

}


bool Codigo::operator ==(const Codigo& otro)const{

	return ( this->identificador == otro.identificador );

}

bool Codigo::operator >(const Codigo& otro)const{

	return ( this->identificador > otro.identificador );

}

bool Codigo::operator <(const Codigo& otro)const{

	return ( this->identificador < otro.identificador );

}

bool Codigo::operator >=(const Codigo& otro)const{

	return ( this->identificador >= otro.identificador );

}

bool Codigo::operator <=(const Codigo& otro)const{

	return ( this->identificador <= otro.identificador );

}


Codigo& Codigo::operator =(const Codigo& otro){

	this->identificador= otro.identificador;
	this->frecuencia= otro.frecuencia;

	return *this;

}

