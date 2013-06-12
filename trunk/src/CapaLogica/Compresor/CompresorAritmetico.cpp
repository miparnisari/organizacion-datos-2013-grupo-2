/*
 * CompresorAritmetico.cpp
 *
 *  Created on: Jun 6, 2013
 *      Author: martin
 */

#include "CompresorAritmetico.h"

CompresorAritmetico::CompresorAritmetico()
{
	byteActual = '0';
}

CompresorAritmetico::~CompresorAritmetico()
{

}

std::vector<bool> CompresorAritmetico::comprimir(const char simbolo)
{
	intervalo.actualizar_rango();

	Uint low_count_simbolo = modelo.calcular_low_count(simbolo);
	Uint high_count_simbolo = modelo.calcular_high_count(simbolo);
	Uint total_frecuencias = modelo.calcular_total_frecuencias();

	/*
	 * Nuevo Piso = 0 + 256 * 3 / 6 = 128
	 * Nuevo Techo = 0 + 256 * 5 / 6 - 1 = 212
	 */
	intervalo.actualizar_piso_techo(low_count_simbolo, high_count_simbolo, total_frecuencias);

	/*
	 * Emision: 1
	 * Contador de underflow: 0
	 * Modifica piso y techo.
	 */
	std::vector<bool> bits_a_emitir = intervalo.normalizar();

	modelo.incrementar_frecuencia(simbolo);

	return bits_a_emitir;
}

