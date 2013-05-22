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
		IterArbolBMas(std::string unArchivo);
		//FIXME este constructor es horrible, poner const de copia del arbol y ya
		//y sacar get_ y set_ruta del manejador de bloques!
		virtual ~IterArbolBMas();
		int start(std::string operador, ClaveX claveInicial);
		//start(">=","arjona")
		//arjona0 .... arjonb0.. NULL
		RegistroClave* readNext();

	private:
		ArbolBMas* arbol;
		unsigned int numRegActual;
		int numNodoActual;
		ClaveX claveCorte;
		ClaveX* claveActual;
		ClaveX claveCorte;
		std::string operador;
};

#endif /* ITERARBOLBMAS_H_ */
