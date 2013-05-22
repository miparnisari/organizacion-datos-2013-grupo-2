/*
 * IterArbolBMas.cpp
 *
 *  Created on: May 21, 2013
 *      Author: maine
 */

#include "IterArbolBMas.h"

IterArbolBMas::IterArbolBMas(std::string unArchivo)
{
	arbol = new ArbolBMas (unArchivo);
	numRegActual = 0;
	numNodoActual = 0;

	claveActual = new ClaveX();
}

IterArbolBMas::~IterArbolBMas() {
	delete claveActual;
	delete arbol;
}

int IterArbolBMas::start(std::string unOperador, ClaveX claveInicial)
{
	operador = unOperador;
	claveCorte = claveInicial;

	unsigned int numNodoSecuencialInicial;
	NodoSecuencial nodoSecInicial(arbol->get_cant_minima_nodo(),arbol->get_cant_maxima_nodo());

	if (operador==">=" || operador==">")
	{
		RegistroClave regConClave;
		regConClave.set_clave(claveInicial);

		if (arbol->_hallar_hoja(&regConClave,numNodoSecuencialInicial) != RES_OK)
			return RES_ERROR;

	}else//es <= o <
	{
		arbol->obtener_primer_nodo_secuencial(numNodoSecuencialInicial);
	}

	numNodoActual = numNodoSecuencialInicial;

	arbol->_obtener_nodo_secuencial(numNodoSecuencialInicial,nodoSecInicial);

	IterNodoSecuencial iterNodo(nodoSecInicial,operador);

	numRegActual = iterNodo.get_pos_reg(operador, claveInicial);

	return RES_OK;
}

RegistroClave* IterArbolBMas::readNext()
{

	if (numNodoActual != -1)
	{
		NodoSecuencial nodoActual (arbol->get_cant_minima_nodo(), arbol->get_cant_maxima_nodo());
		arbol->_obtener_nodo_secuencial(numNodoActual,nodoActual);

//TODO refactorizar y pasar el avance en los nodos a su iterador
		if (numRegActual < nodoActual.get_cantidad_registros())
		{
			numRegActual++;

			ClaveX claveActual;

			claveActual = (nodoActual.get_registros())[numRegActual].get_clave();
			//para los operadores <= y <
			//si la clave actual es mayor (o igual) dejo de avanzar y retorno NULL.
			if ( operador == "<" && claveActual >= claveCorte)
				return NULL;
			if ( operador == "<=" && claveActual > claveCorte)
				return NULL;
		}
		//llegue al final de un nodo, avanzo al siguiente.
		if ( (numRegActual+1) == nodoActual.get_cantidad_registros())
		{
			numNodoActual = nodoActual.get_proximo_nodo();
		}
	}else
	{
		return NULL;
	}
//TODO agregar casos de RES_ERROR
	NodoSecuencial nodoActual (arbol->get_cant_minima_nodo(), arbol->get_cant_maxima_nodo());
	arbol->_obtener_nodo_secuencial(numNodoActual,nodoActual);

	return &(nodoActual.get_registros()[numRegActual]);
}

