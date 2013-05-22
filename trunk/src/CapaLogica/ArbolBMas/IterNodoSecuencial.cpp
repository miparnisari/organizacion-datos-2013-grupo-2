/*
 * IterNodoSecuencial.cpp
 *
 *  Created on: May 21, 2013
 *      Author: maine
 */

#include "IterNodoSecuencial.h"

IterNodoSecuencial::IterNodoSecuencial(const NodoSecuencial& nodoSec) {
	this->nodo = new NodoSecuencial(nodoSec);
}

IterNodoSecuencial::~IterNodoSecuencial() {
	delete nodo;
}

int IterNodoSecuencial::get_pos_reg(std::string unOperador, ClaveX claveInicial)
{
	if (unOperador != ">" && unOperador != ">=")
		return RES_ERROR;

	unsigned int ini = 0;
	unsigned int fin = nodo->get_registros().size();

	for (; ini < fin; ini++)
	{
		ClaveX claveAlmacenada (nodo->get_registros().at(ini).get_clave());

		if (unOperador == ">" && claveAlmacenada > claveInicial)
			return ini;

		if (unOperador == ">=" && claveAlmacenada >= claveInicial)
			return ini;
	}
	return ini+1;
}
