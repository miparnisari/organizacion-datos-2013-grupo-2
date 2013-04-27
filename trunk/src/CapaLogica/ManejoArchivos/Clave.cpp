#include"Clave.h"

bool Clave::operator <(const Clave& otraClave){	return false;}
bool Clave::operator >(const Clave& otraClave){	return false;}
bool Clave::operator ==(const Clave& otraClave){return false;}


bool Clave::operator <=(const Clave& otraClave){

	return ( (*this)<otraClave || (*this)==otraClave );

}

bool Clave::operator >=(const Clave& otraClave){

	return ( (*this)>otraClave || (*this)==otraClave );

}

bool Clave::operator !=(const Clave& otraClave){

	return ( (*this)>otraClave || (*this)<otraClave );

}

void Clave::operator =(const Clave& otraClave){}
Clave::Clave(){}
Clave::~Clave(){}
