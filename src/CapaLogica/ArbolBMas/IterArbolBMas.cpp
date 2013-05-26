/*
 * IterArbolBMas.cpp
 *
 *  Created on: May 21, 2013
 *      Author: maine
 */

#include "IterArbolBMas.h"

IterArbolBMas::IterArbolBMas(ArbolBMas otroArbol)
{
	this->arbol = new ArbolBMas(otroArbol);
	numRegActual = 0;
	numNodoActual = 0;
	claveActual = new ClaveX();
}

IterArbolBMas::~IterArbolBMas() {
	delete claveActual;
	delete arbol;
}

int IterArbolBMas::comienzo(std::string unOperador, ClaveX claveInicial)
{
	operador = unOperador;
	claveCorte = claveInicial;

	unsigned int numNodoSecuencialInicial=NUMERO_BLOQUE_RAIZ;
	NodoSecuencial nodoSecInicial(arbol->get_cant_minima_nodo(),arbol->get_cant_maxima_nodo());

	if (operador==">=" || operador==">")
	{
		RegistroClave regConClave;
		regConClave.set_clave(claveCorte);

		if (arbol->_hallar_hoja(&regConClave,numNodoSecuencialInicial) != RES_OK)
			return RES_ERROR;

	}else//es <= o <
	{
		return RES_ERROR;
	}

	numNodoActual = numNodoSecuencialInicial;

	arbol->_obtener_nodo_secuencial(numNodoSecuencialInicial,nodoSecInicial);

	IterNodoSecuencial iterNodo(nodoSecInicial);

	numRegActual = iterNodo.get_pos_reg(operador, claveInicial);

	return RES_OK;
}

int IterArbolBMas::leer_siguiente(RegistroClave & regActual)
{
	NodoSecuencial nodoActual (arbol->get_cant_minima_nodo(), arbol->get_cant_maxima_nodo());

	if (numNodoActual == -1)
		return RES_FIN;

	else {

		arbol->_obtener_nodo_secuencial(numNodoActual,nodoActual);

		// caso especial en que se busca una clave mayor a todas las del arbol
		if (numRegActual >= nodoActual.get_cantidad_registros())
		{
			numNodoActual = nodoActual.get_proximo_nodo();
			numRegActual = 0;
			return leer_siguiente(regActual);
		}
		regActual = (nodoActual.get_registros().at(numRegActual));
		numRegActual++;

		// llegue al final de un nodo, avanzo al siguiente.
		if ( numRegActual == nodoActual.get_cantidad_registros())
		{
			numNodoActual = nodoActual.get_proximo_nodo();
			numRegActual = 0;
		}

		return RES_OK;
	}
}
