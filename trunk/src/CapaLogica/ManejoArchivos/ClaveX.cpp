/*
 * ClaveX.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: martin
 */

#include "ClaveX.h"

ClaveX::ClaveX()
{
	this->tipoClave = CLAVE_STRING; //tipo por defecto
	this->claves[CLAVE_NUMERICA]= new ClaveNumerica;
	this->claves[CLAVE_STRING]= new ClaveString;
}


ClaveX::ClaveX(const ClaveX& otro)
{
	this->tipoClave = otro.tipoClave;
	this->claves[CLAVE_NUMERICA]= new ClaveNumerica;
	this->claves[CLAVE_STRING]= new ClaveString;
//	(*this)= clavex;

	if (this->tipoClave == CLAVE_NUMERICA)
	{
		int clave;
		otro.get_clave(clave);
		set_clave(clave);
	}
	else if (this->tipoClave == CLAVE_STRING)
	{
		std::string clave;
		otro.get_clave(clave);
		set_clave(clave);
	}
}


ClaveX::~ClaveX()
{
	for (unsigned int i = 0; i < CANTIDAD_TIPOS_CLAVE; i++)
	{
		delete(claves[i]);
		claves[i] = NULL;
	}
}

void ClaveX::imprimir_dato()const
{

	if (tipoClave == CLAVE_STRING)
	{
		string s;
		this->get_clave(s);
		std::cout<<s;
	}
	else {
		int i;
		this->get_clave(i);
		std::cout<<i;
	}
}


TipoClave ClaveX::get_tipo_clave()const{

	return tipoClave;

}


void ClaveX::_resetear_datos(){

	for(unsigned int i=0;i<CANTIDAD_TIPOS_CLAVE;i++)
		claves[i]->resetear();

}


int ClaveX::get_tamanio_empaquetado()const{

	return this->get_tamanio_clave() + sizeof(char);

}

int ClaveX::get_tamanio_clave()const{

	return this->claves[tipoClave]->get_tamanio();

}


int ClaveX::empaquetar(char* buffer){

	unsigned short tamanioClave= this->get_tamanio_clave();
	char tipoClaveChar= (char)tipoClave;
	unsigned short tamanioEmpaquetado= this->get_tamanio_empaquetado();

	stringstream stream;
	stream.write( (char*)&tipoClaveChar , sizeof(tipoClaveChar) );
	char* empaquetado= new char[tamanioClave]();
	claves[tipoClave]->empaquetar(empaquetado);
	stream.write( empaquetado,tamanioClave );

	stream.seekg(0,ios::beg);
	stream.read( buffer,tamanioEmpaquetado );


	delete[] empaquetado;
	return tamanioEmpaquetado;


}

void ClaveX::desempaquetar(char* buffer,unsigned short tamanioBuffer){

	unsigned short tamanioClave= tamanioBuffer - sizeof(char);
	char* bufferClave= new char[tamanioClave +1]();
	bufferClave[tamanioClave] = '\0';

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

	//*(ClaveString*)claves[CLAVE_STRING] = clave;
	ClaveString* cs= (ClaveString*)claves[CLAVE_STRING];
	cs->set_dato(clave);

}

void ClaveX::set_clave(int clave){

	tipoClave= CLAVE_NUMERICA;

	ClaveNumerica* cn= (ClaveNumerica*)claves[CLAVE_NUMERICA];
	cn->set_dato(clave);


}

std::ostream& ClaveX::operator << (std::ostream& os)const
{
	return os;
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


ClaveX& ClaveX::operator =(const ClaveX& otro)
{
	if (this != &otro)
	{
		this->tipoClave= otro.tipoClave;
//		this->claves[CLAVE_NUMERICA]= new ClaveNumerica;
//		this->claves[CLAVE_STRING]= new ClaveString;

		for(unsigned int i=0;i<CANTIDAD_TIPOS_CLAVE;i++)
			(*claves[i]) = (*otro.claves[i]);
	}
	return (*this);

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
	string s= cs->get_dato();
	string ss(s);
	clave= ss;


	return RES_OK;

}

