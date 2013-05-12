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

	RegistroVariable registroTipoNodo;
	bloque->recuperar_registro(&registroTipoNodo,0);
	char tipoNodoLeido;
	registroTipoNodo.recuperar_campo( (char*)&tipoNodoLeido , sizeof(tipoNodoLeido) );

	this->tipoNodo= tipoNodoLeido;
	return RES_OK;


}