#include "NodoInterno.h"

NodoInterno::NodoInterno(unsigned int p_minCantidadBytesClaves, unsigned int p_maxCantidadBytesClaves)
{
//	tamanio = tamanioNodo;
//	espacioLibre = tamanio - sizeof(espacioLibre);
	unsigned int minCantidadBytesClaves = p_minCantidadBytesClaves;
	unsigned int maxCantidadBytesClaves = p_maxCantidadBytesClaves;
}

NodoInterno::~NodoInterno()
{
	for (unsigned int i = 0; i < vectorClaves.size(); i++)
		delete(vectorClaves.at(i));
}

unsigned short NodoInterno::get_cantidad_claves()
{
	return vectorClaves.size();
}

unsigned short NodoInterno::get_cantidad_hijos()
{
	return vectorHijos.size();
}

bool NodoInterno::esta_vacio()
{
	return (vectorClaves.size()== 0);
}


ClaveX* NodoInterno::get_clave(unsigned short numeroClave)
{
	if (esta_vacio())
		return NULL;
	if (numeroClave >= this->get_cantidad_claves())
		return NULL;

	return vectorClaves.at(numeroClave);
}


int NodoInterno::get_hijo(unsigned short& hijo,unsigned short numeroHijo)
{
	if (esta_vacio())
		return RES_ERROR;
	if (numeroHijo >= this->get_cantidad_hijos())
		return RES_ERROR;

	hijo = vectorHijos.at(numeroHijo);

	return RES_OK;
}


ClaveX* NodoInterno::get_clave_mitad()
{
	if (this->esta_vacio())
		return NULL;

	const unsigned short CANTIDAD_CLAVES = vectorClaves.size();
	const unsigned short MITAD = (unsigned short)(CANTIDAD_CLAVES/2);

	return this->get_clave(MITAD);

}

int NodoInterno::buscar_clave(const ClaveX* clave,unsigned short& posicionClave)
{

	const unsigned short CANTIDAD_CLAVES= this->get_cantidad_claves();
	for(int i=0;i<CANTIDAD_CLAVES;i++){

		ClaveX* unaClave= this->get_clave(i);
		if( (*clave)== (*unaClave) ){
			posicionClave= i;
			return i;
		}

	}

	return RES_ERROR;


}//FIXME Para que sirve este metodo!? Es lo mismo que get_clave(pos)...


int NodoInterno::get_hijo_izquierdo(unsigned short& hijo, ClaveX* clave){

	if(this->esta_vacio())
		return RES_ERROR;

	unsigned short posicionClave;
	if( this->buscar_clave(clave,posicionClave)== RES_ERROR )
		return RES_ERROR;

	return this->get_hijo(hijo,posicionClave);

}

int NodoInterno::get_hijo_derecho(unsigned short& hijo,ClaveX* clave){

	if(this->esta_vacio())
		return RES_ERROR;

	unsigned short posicionClave;
	if( this->buscar_clave(clave,posicionClave)== RES_ERROR )
		return RES_ERROR;

	return this->get_hijo(hijo,posicionClave+1);

}


int NodoInterno::remover_clave(const ClaveX* clave,unsigned short numeroClave){

	if(this->get_clave(numeroClave)== NULL)
		return RES_ERROR;

	return this->remover_clave(clave);

}


int NodoInterno::remover_clave(const ClaveX* clave){

	unsigned short posicionClave;
	if (this->buscar_clave(clave,posicionClave)== RES_ERROR)
		return RES_ERROR;

	vectorClaves.erase(vectorClaves.begin() + posicionClave);

	return posicionClave;

}

int NodoInterno::empaquetar(Bloque* bloque)
{
	return RES_OK;
}

int NodoInterno::desempaquetar(const Bloque* bloque)
{
	return RES_OK;
}
