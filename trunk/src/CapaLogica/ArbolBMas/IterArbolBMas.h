/*claveActual
 * IterArbolBMas.h
 *
 *  Created on: May 21, 2013
 *      Author: maine
 */

#ifndef ITERARBOLBMAS_H_
#define ITERARBOLBMAS_H_

#include "ArbolBMas.h"
#include"IterNodoSecuencial.h"

class IterArbolBMas {

	public:
		IterArbolBMas(ArbolBMas arbol);
		virtual ~IterArbolBMas();
		int start(std::string operador, ClaveX claveInicial);
		int readNext(RegistroClave & regActual);

	private:
		ArbolBMas* arbol;
		unsigned int numRegActual;
		int numNodoActual;
		ClaveX claveCorte;
		ClaveX* claveActual;
		std::string operador;
};

#endif /* ITERARBOLBMAS_H_ */
