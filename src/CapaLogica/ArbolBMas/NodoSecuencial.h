/*
 * NodoSecuencial.h
 *
 *  Created on: May 5, 2013
 *      Author: maine
 */

#ifndef NODOSECUENCIAL_H_
#define NODOSECUENCIAL_H_

#include "Nodo.h"
#include <vector>
#include "../../CapaFisica/RegistroVariable.h"
#include "../../CapaFisica/Bloque.h"
#include "../ManejoArchivos/ClaveX.h"

class NodoSecuencial: public Nodo {

private:
	unsigned int minCantidadBytesOcupados;
	unsigned int maxCantidadBytesOcupados;
	unsigned int bytesOcupados;
	std::vector<RegistroVariable > vectorRegistros;
	int proximoNodo;

public:
	NodoSecuencial(unsigned int minBytesOcupados, unsigned int maxBytesOcupados);
	virtual ~NodoSecuencial();

	int insertar(RegistroVariable* registro);
	int eliminar(RegistroVariable* registro);
	int buscar(ClaveX* clave, RegistroVariable* regDevuelto);

	int empaquetar(Bloque* bloque);
	int desempaquetar(const Bloque*);
};

#endif /* NODOSECUENCIAL_H_ */
