/*
 * DescompresorAritmetico.h
 *
 *  Created on: 12/06/2013
 *      Author: Betty
 */

#ifndef DESCOMPRESORARITMETICO_H_
#define DESCOMPRESORARITMETICO_H_
#include "Intervalo.h"
#include "ModeloProbabilistico.h"

class DescompresorAritmetico {
private:
	Intervalo intervalo;
	char byteActual;
	ModeloProbabilistico modelo;

	char decodificar_objetivo ();
	/* Determina el intervalo que contiene el simbolo.
	 * Esto se logra calculando el codigo del simbolo. */

	void decodificar(Uint low_count, Uint high_count, Uint total);

public:
	DescompresorAritmetico();
	virtual ~DescompresorAritmetico();

	void descomprimir(const char byte);
};

#endif /* DESCOMPRESORARITMETICO_H_ */
