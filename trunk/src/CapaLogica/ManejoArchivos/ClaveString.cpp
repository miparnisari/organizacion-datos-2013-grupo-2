/*
 * ClaveString.cpp
 *
 *  Created on: Apr 22, 2013
 *      Author: martin
 */

#include "ClaveString.h"


void ClaveString::resetear(){

	this->dato= DATO_DEFECTO;

}


void ClaveString::desempaquetar(char* buffer,unsigned short tamanioBuffer){

	if(tamanioBuffer== 0)
		tamanioBuffer= strlen(buffer);

	string copia(buffer,tamanioBuffer);
	dato= copia;

}


int ClaveString::empaquetar(char* buffer){

	//memcpy(buffer,dato.c_str(),dato.length());
	stringstream ss;
	ss.write(dato.c_str(),dato.length());
	ss.seekg(0,ss.beg);
	ss.read(buffer,dato.length());

	return dato.length();

}


string ClaveString::remover_espacios(string linea){

	for(unsigned short i=0;i<linea.length();i++){
		char c= linea.at(i);
		if( c== ' ' )
			linea.erase(i,1);
	}

	return linea;

}


void ClaveString::operator =(const string& unString){

	this->dato= unString;

}


bool ClaveString::operator !=(const Clave& otraClave){

	ClaveString* cs= (ClaveString*)&otraClave;
	return (remover_espacios(dato) != remover_espacios(cs->dato)  );

}

bool ClaveString::operator >(const Clave& otraClave){

	ClaveString* cs= (ClaveString*)&otraClave;
	return (remover_espacios(dato) > remover_espacios(cs->dato)  );

}


bool ClaveString::operator <(const Clave& otraClave){

	ClaveString* cs= (ClaveString*)&otraClave;
	return (remover_espacios(dato) < remover_espacios(cs->dato)  );

}


bool ClaveString::operator ==(const Clave& otraClave){

	ClaveString* cs= (ClaveString*)&otraClave;
	return (remover_espacios(dato) == remover_espacios(cs->dato)  );

}


bool ClaveString::operator >=(const Clave& otraClave){

	ClaveString* cs= (ClaveString*)&otraClave;
	return (remover_espacios(dato) >= remover_espacios(cs->dato)  );

}


bool ClaveString::operator <=(const Clave& otraClave){

	ClaveString* cs= (ClaveString*)&otraClave;
	return (remover_espacios(dato) <= remover_espacios(cs->dato)  );

}

void ClaveString::operator =(const Clave& otraClave){

	ClaveString* cs= (ClaveString*)&otraClave;
	this->dato= cs->dato;

}

void ClaveString::remover_espacios(){

	string copia= remover_espacios(dato);

	dato= copia;

}


void ClaveString::set_dato(const string& dato){

	this->dato= dato;

}

string ClaveString::get_dato(){

	return dato;

}


int ClaveString::get_tamanio(){

	return dato.length();

}


ClaveString::ClaveString(){

	dato= "~";/*ASCII 126*/

}


ClaveString::ClaveString(const string& linea){

	dato= linea;

}


ClaveString::~ClaveString(){}
