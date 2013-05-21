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

	if (operador==">=")
	{
		RegistroClave regConClave;
		regConClave.set_clave(claveInicial);


		unsigned int numNodoSecuencialInicial;
		if (arbol->_hallar_hoja(&regConClave,numNodoSecuencialInicial) != RES_OK)
			return RES_ERROR;


		NodoSecuencial nodoSecInicial(arbol->get_cant_minima_nodo(),arbol->get_cant_maxima_nodo());
		arbol->_obtener_nodo_secuencial(numNodoSecuencialInicial,nodoSecInicial);

		numNodoActual = numNodoSecuencialInicial;

		IterNodoSecuencial iterNodo(nodoSecInicial);

		numRegActual = iterNodo.get_pos_reg(unOperador, claveInicial);
	}else//es <=
	{
		numNodoActual = 0;//FIXME numero del primer nodo secuencial
	}

	return RES_OK;
}

RegistroClave* IterArbolBMas::readNext()
{


	while (numNodoActual != -1)
	{
		NodoSecuencial nodoActual (arbol->get_cant_minima_nodo(), arbol->get_cant_maxima_nodo());
		arbol->_obtener_nodo_secuencial(numNodoActual,nodoActual);


		while (numRegActual < nodoActual.get_cantidad_registros())
		{
			std::vector<RegistroClave> registrosDelNodo = nodoActual.get_registros();
			unsigned int i = 0;
			for (; i < registrosDelNodo.size(); i ++)
			{
				ClaveX clave;

				clave = registrosDelNodo.at(i).get_clave();

				if (operador == ">=" && clave == (*claveActual))
					numRegActual ++;

			}
		}

		numNodoActual = nodoActual.get_proximo_nodo();

	}



	return NULL;
}

