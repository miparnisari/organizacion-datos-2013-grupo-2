/*
 * IterNodoSecuencial.cpp
 *
 *  Created on: May 21, 2013
 *      Author: maine
 */

#include "IterNodoSecuencial.h"

IterNodoSecuencial::IterNodoSecuencial(NodoSecuencial & nodoSec, std::string unOperador) {
	nodo= new NodoSecuencial(nodoSec);
	this->operador=unOperador;
}

IterNodoSecuencial::~IterNodoSecuencial() {
	delete nodo;
}

int IterNodoSecuencial::get_pos_reg(std::string unOperador,
		ClaveX claveInicial)
{
	if (operador == "<=" || operador == "<")
		return 0;
	else if	(operador==">") {
		unsigned int i = 0;
		for (; i < nodo->get_registros().size(); i++)
		{
			ClaveX* claveAlmacenada = new ClaveX(nodo->get_registros().at(i).get_clave());
			if (claveInicial < *claveAlmacenada)
			{
				delete (claveAlmacenada);
				return i;
			}
			delete (claveAlmacenada);
		}
		return i;
	}
	else {// caso de >=

		unsigned int i = 0;
		for (; i < nodo->get_registros().size(); i++)
		{
			ClaveX* claveAlmacenada = new ClaveX(nodo->get_registros().at(i).get_clave());

			if (claveInicial <= *claveAlmacenada)
			{
				delete (claveAlmacenada);
				return i;
			}
			delete (claveAlmacenada);
		}
		return i;
	}
}
