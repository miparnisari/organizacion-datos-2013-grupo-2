/*
 * ModeloProbabilistico.cpp
 *
 *  Created on: 12/06/2013
 *      Author: Betty
 */

#include "ModeloProbabilistico.h"

ModeloProbabilistico::ModeloProbabilistico(short tamanio_alfabeto) {
	frecuenciasSimbolos = new Uint[tamanio_alfabeto]();
}

ModeloProbabilistico::~ModeloProbabilistico() {
	delete frecuenciasSimbolos;
}

void ModeloProbabilistico::inicializar_frecuencias_en_1()
{
	for (unsigned int i = 0; i < TAMANIO_ALFABETO; i++)
		*(frecuenciasSimbolos+i) = 1;
}

void ModeloProbabilistico::incrementar_frecuencia (const char simbolo)
{
	Uint posicionSimbolo = simbolo;
	*(frecuenciasSimbolos+posicionSimbolo) += 1;
}


/*
 *  SIMBOLO  |||  FRECUENCIA   ||| LOW_COUNT ||| HIGH_COUNT
 *    A                4              0              4
 *    B                2              4              6
 *    C                1              6              7
 *    D                1              7              8
 */
double ModeloProbabilistico::calcular_low_count (const char simbolo)
{
	Uint posicionSimbolo = simbolo;
	Uint contador = 0;
	for (Uint i = 0; i < posicionSimbolo; i++)
		contador += *(frecuenciasSimbolos+i);

	return contador / this->calcular_total_frecuencias();
}

/*
 *  SIMBOLO  |||  FRECUENCIA   ||| LOW_COUNT ||| HIGH_COUNT
 *    A                4              0              4
 *    B                2              4              6
 *    C                1              6              7
 *    D                1              7              8
 */
double ModeloProbabilistico::calcular_high_count (const char simbolo)
{
	Uint posicionSimbolo = simbolo;
	Uint contador = 0;
	for (Uint i = 0; i < posicionSimbolo + 1; i++)
		contador += *(frecuenciasSimbolos+i);

	return contador / this->calcular_total_frecuencias();
}

Uint ModeloProbabilistico::calcular_total_frecuencias()
{
	Uint total = 0;
	for (Uint i = 0; i < TAMANIO_ALFABETO; i++)
		total += *(frecuenciasSimbolos + i);

	return total;
}

Uint ModeloProbabilistico::obtener_simbolo(Uint valor)
{
	for (Uint i = 0; i < TAMANIO_ALFABETO; i++)
	{
		if (valor < *(frecuenciasSimbolos + i))
			return i;
	}

	return TAMANIO_ALFABETO;
}
/* Recibe un numero y devuelve la posicion del vector de frecuencias
 * donde cae ese numero.
 */
