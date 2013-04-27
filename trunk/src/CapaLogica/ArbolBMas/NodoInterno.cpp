#include "NodoInterno.h"


unsigned short NodoInterno::get_cantidad_claves(){

	return listaClaves.size();

}

unsigned short NodoInterno::get_cantidad_hijos(){

	if(esta_vacio())
		return 0;
	return (get_cantidad_claves() + 1);

}


bool NodoInterno::esta_vacio(){

	return listaClaves.size()== 0;

}


int NodoInterno::get_clave(Clave* clave,unsigned short numeroClave){

	if(esta_vacio())
		return RES_ERROR;
	if(numeroClave >= this->get_cantidad_claves())
		return RES_ERROR;

	list<Clave*>::iterator it= this->listaClaves.begin();
	for(unsigned short i=0; i< numeroClave; i++ ,it++);

	clave= *it;

	return RES_OK;

}


int NodoInterno::get_hijo(unsigned short& hijo,unsigned short numeroHijo){

	if(esta_vacio())
		return RES_ERROR;
	if(numeroHijo > this->get_cantidad_hijos())
		return RES_ERROR;

	list<unsigned short>::iterator it= this->listaHijos.begin();
	for(unsigned short i=0;i<numeroHijo;i++, it++);

	hijo= *it;

	return RES_OK;

}


int NodoInterno::get_clave_mitad(Clave* clave){

	if(this->esta_vacio())
		return RES_ERROR;

	const unsigned short CANTIDAD_CLAVES= listaClaves.size();
	const unsigned short MITAD= (unsigned short)(CANTIDAD_CLAVES/2);

	return this->get_clave(clave,MITAD);

}


int NodoInterno::buscar_clave(Clave* clave,unsigned short& posicionClave){

	if(this->esta_vacio())
		return RES_ERROR;

	Clave* c= NULL;

	for(unsigned short i=0; i< this->get_cantidad_claves() ; i++){

		this->get_clave(c,i);
		if( (*c)==(*clave) ){
			posicionClave= i;
			return posicionClave;
		}

	}

	return RES_ERROR;

}


int NodoInterno::get_hijo_izquierdo(unsigned short& hijo,Clave* clave){

	if(this->esta_vacio())
		return RES_ERROR;

	unsigned short posicionClave;
	if( this->buscar_clave(clave,posicionClave)== RES_ERROR )
		return RES_ERROR;

	return this->get_hijo(hijo,posicionClave);

}

int NodoInterno::get_hijo_derecho(unsigned short& hijo,Clave* clave){

	if(this->esta_vacio())
		return RES_ERROR;

	unsigned short posicionClave;
	if( this->buscar_clave(clave,posicionClave)== RES_ERROR )
		return RES_ERROR;

	return this->get_hijo(hijo,posicionClave+1);

}


int NodoInterno::remover_clave(Clave* clave,unsigned short numeroClave){

	if(this->get_clave(clave,numeroClave)== RES_ERROR)
		return RES_ERROR;

	return this->remover_clave(clave);

}


int NodoInterno::remover_clave(Clave* clave){

	unsigned short posicionClave;
	if(this->buscar_clave(clave,posicionClave)== RES_ERROR)
		return RES_ERROR;

	list<Clave*>::iterator it= listaClaves.begin();
	for(int i=0;i<posicionClave;i++ , it++);
	listaClaves.erase(it);

	return posicionClave;

}


NodoInterno::NodoInterno(unsigned int t){

	this->tamanio= t;
	espacioLibre= tamanio - sizeof(espacioLibre);

}

NodoInterno::~NodoInterno(){

}
