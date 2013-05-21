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
	IterNodoSecuencial(NodoSecuencial nodoSec);
	virtual ~IterNodoSecuencial();

	int get_pos_reg(std::string unOperador, ClaveX claveInicial);
};

#endif /* ITERNODOSECUENCIAL_H_ */
