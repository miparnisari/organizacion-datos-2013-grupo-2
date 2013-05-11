/*
 * Nodo.cpp
 *
 *  Created on: May 4, 2013
 *      Author: maine
 */

#include "NodoArbol.h"

NodoArbol::NodoArbol(char p_tipoNodo) {
	this->tipoNodo = p_tipoNodo;

}

NodoArbol::~NodoArbol() {
	// TODO Auto-generated destructor stub
}

bool NodoArbol::es_hoja(){

	return this->tipoNodo == TIPO_HOJA;

}

bool NodoArbol::es_interno(){

	return this->tipoNodo == TIPO_INTERNO;

}


int NodoArbol::desempaquetar(const Bloque* bloque){

	if(bloque== NULL)
		return RES_ERROR;

	const unsigned int TAMANIO_BLOQUE= bloque->get_tamanio_bloque();
	char* bloqueEmpaquetado= new char[TAMANIO_BLOQUE];
	bloque->empaquetar(bloqueEmpaquetado);

	this->tipoNodo= bloqueEmpaquetado[0];

	delete[] bloqueEmpaquetado;
	return RES_OK;


}
