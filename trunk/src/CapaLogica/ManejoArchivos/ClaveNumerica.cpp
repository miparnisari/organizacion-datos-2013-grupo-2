#include "ClaveNumerica.h"

void ClaveNumerica::importar(char* buffer,unsigned short tamanioBuffer){

	stringstream stream;
	stream.write(buffer,tamanioBuffer);
	stream.seekg(0,ios::beg);

	stream.write((char*)&dato,sizeof(int));

}


int ClaveNumerica::exportar(char* buffer){

	stringstream stream;
	stream.write( (char*)&dato , sizeof(int) );
	stream.seekg(0,ios::beg);

	stream.read(buffer,sizeof(int));

	return sizeof(int);

}


bool ClaveNumerica::operator <(const Clave& otraClave){

	ClaveNumerica* cn= (ClaveNumerica*)&otraClave;
	return this->dato < cn->dato;

}

bool ClaveNumerica::operator >(const Clave& otraClave){

	ClaveNumerica* cn=(ClaveNumerica*)&otraClave;
	return this->dato > cn->dato;

}

bool ClaveNumerica::operator ==(const Clave& otraClave){

	ClaveNumerica* cn=(ClaveNumerica*)&otraClave;
	return this->dato == cn->dato;

}


bool ClaveNumerica::operator >=(const Clave& otraClave){

	ClaveNumerica* cn=(ClaveNumerica*)&otraClave;
	return this->dato >= cn->dato;

}



bool ClaveNumerica::operator <=(const Clave& otraClave){

	ClaveNumerica* cn=(ClaveNumerica*)&otraClave;
	return this->dato <= cn->dato;

}


bool ClaveNumerica::operator !=(const Clave& otraClave){

	ClaveNumerica* cn=(ClaveNumerica*)&otraClave;
	return this->dato != cn->dato;

}


void ClaveNumerica::operator =(const Clave& otraClave){

	ClaveNumerica* cn=(ClaveNumerica*)&otraClave;
	this->dato= cn->dato;

}

int ClaveNumerica::incrementar(){

	return ++dato;

}


void ClaveNumerica::set_dato(int dato){

	this->dato= dato;

}

int ClaveNumerica::get_dato(){

	return dato;

}


ClaveNumerica::ClaveNumerica(){

	dato= 0;

}

ClaveNumerica::ClaveNumerica(int dato){

	this->dato= dato;

}

ClaveNumerica::~ClaveNumerica(){}
