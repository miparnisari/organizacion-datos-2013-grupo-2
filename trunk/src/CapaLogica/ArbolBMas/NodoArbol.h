/*
 * Nodo.h
 *
 *  Created on: May 4, 2013
 *      Author: maine
 */

#ifndef NODOARBOL_H_
#define NODOARBOL_H_

#include <vector>
#include <iostream>
#include <algorithm>    // std::sort
#include <math.h>		// ceil( )

#include "../../Constantes.h"
#include "../../CapaFisica/RegistroVariable.h"
#include "../../CapaFisica/Bloque.h"
#include "../ManejoArchivos/Clave.h"
#include "../ManejoArchivos/ClaveX.h"
#include "../ManejoArchivos/RegistroClave.h"

class NodoArbol {
	public:
		NodoArbol(char tipoNodo);
		virtual ~NodoArbol();

		virtual int desempaquetar(const Bloque* bloque);

		virtual bool es_hoja();
		virtual bool es_interno();

	protected:
		char tipoNodo; //Si es interno o de secuencia


};

#endif /* NODOARBOL_H_ */
