#include "ClaveNumerica.h"


void ClaveNumerica::resetear(){

	this->dato= 0;

}


void ClaveNumerica::desempaquetar(char* buffer,unsigned short tamanioBuffer){

	stringstream stream;
	stream.write(buffer,tamanioBuffer);
	stream.seekg(0,ios::beg);

	stream.read((char*)&dato,sizeof(int));

}


int ClaveNumerica::empaquetar(char* buffer){

	stringstream stream;
	stream.write( (char*)&dato , sizeof(int) );
	stream.seekg(0,ios::beg);

	stream.read(buffer,sizeof(int));

	return sizeof(int);

}

ostream& ClaveNumerica::operator<<(ostream& os)const
{
	os << dato;
	return os;
}

void ClaveNumerica::imprimir_dato()const
{
	std::cout << dato << std::endl;
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


void ClaveNumerica::set_dato(int p_dato){
	this->dato= p_dato;

}

int ClaveNumerica::get_dato(){

	return dato;

}


int ClaveNumerica::get_tamanio(){

	return sizeof(int);

}


ClaveNumerica::ClaveNumerica(){

	dato= 0;

}

ClaveNumerica::ClaveNumerica(int dato){

	this->dato= dato;

}

ClaveNumerica::~ClaveNumerica(){}
