/*
 * DescompresorAritmetico.cpp
 *
 *  Created on: 12/06/2013
 *      Author: Betty
 */

#include "DescompresorAritmetico.h"

DescompresorAritmetico::DescompresorAritmetico() {
	byteActual = '0';
}

DescompresorAritmetico::~DescompresorAritmetico() {
}


char DescompresorAritmetico::decodificar_objetivo ()
{
	intervalo.actualizar_rango();
	char valor =  intervalo.calcular_valor(byteActual);
	return valor;
}

void DescompresorAritmetico::decodificar(Uint low_count, Uint high_count, Uint total)
{
	intervalo.actualizar_piso_techo(low_count,high_count,total);
}/* Actualiza los valores de piso y techo */

void DescompresorAritmetico::descomprimir(const char byte)
{
	byteActual = byte;
	char valor = decodificar_objetivo();
	Uint low_count = modelo.calcular_low_count(valor);
	Uint high_count = modelo.calcular_high_count(valor);
	Uint total = modelo.calcular_total_frecuencias();
	decodificar(low_count, high_count, total);
}
