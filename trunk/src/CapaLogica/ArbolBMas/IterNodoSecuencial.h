/*
 * IterNodoSecuencial.h
 *
 *  Created on: May 21, 2013
 *      Author: maine
 */

#ifndef ITERNODOSECUENCIAL_H_
#define ITERNODOSECUENCIAL_H_

#include "NodoSecuencial.h"

class IterNodoSecuencial {
public:
	NodoSecuencial* nodo;
	std::string operador;


	IterNodoSecuencial(const NodoSecuencial& nodoSec,std::string operador);
	virtual ~IterNodoSecuencial();

	int get_pos_reg(std::string unOperador, ClaveX claveInicial);
	//esta basado en el buscar de nodo secuencial!
};

#endif /* ITERNODOSECUENCIAL_H_ */
