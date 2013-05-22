/*
 * IterNodoSecuencial.cpp
 *
 *  Created on: May 21, 2013
 *      Author: maine
 */

#include "IterNodoSecuencial.h"

IterNodoSecuencial::IterNodoSecuencial(const NodoSecuencial& nodoSec,std::string operador) {
	this->operador=operador;
	nodo = new NodoSecuencial(nodoSec);

}

IterNodoSecuencial::~IterNodoSecuencial() {
	delete nodo;
}

int IterNodoSecuencial::get_pos_reg(std::string unOperador,
		ClaveX claveInicial)
{

	if (operador == ">")
	{
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

	}else{
		if (operador == ">=")
		{
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

		}else//es decir es < o <=
		{
			return 0;//empieza desde el primero
		}
	}
	//no debería pasar pero si el operador es algun otro (probablemente invalido) TODO
	return RES_ERROR;
}
