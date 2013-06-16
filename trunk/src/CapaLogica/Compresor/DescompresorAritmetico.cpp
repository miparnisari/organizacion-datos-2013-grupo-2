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
	intervalo.calcular_rango();
	char valor =  intervalo.calcular_valor(byteActual);
	return valor;
}

void DescompresorAritmetico::decodificar(double low_count, double high_count)
{
	intervalo.actualizar_piso_techo(low_count,high_count);
}/* Actualiza los valores de piso y techo */

void DescompresorAritmetico::descomprimir(const char byte)
{
	byteActual = byte;
	char valor = decodificar_objetivo();
	double low_count = modelo.calcular_low_count(valor);
	double high_count = modelo.calcular_high_count(valor);
	decodificar(low_count, high_count);
}
