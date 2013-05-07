/*
 * Nodo.h
 *
 *  Created on: May 4, 2013
 *      Author: maine
 */

#ifndef NODO_H_
#define NODO_H_

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

class Nodo {
	public:
		Nodo(char tipoNodo);
		virtual ~Nodo();

	protected:
		char tipoNodo; //Si es interno o de secuencia
};

#endif /* NODO_H_ */
