/*
 * DescompresorAritmetico.cpp
 *
 *  Created on: 12/06/2013
 *      Author: Betty
 */

#include "DescompresorAritmetico.h"

DescompresorAritmetico::DescompresorAritmetico() {
	modelo.inicializar_frecuencias_en_1();
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

int DescompresorAritmetico::descomprimir_todo(char* buffer, int tamanio, char* descomprimido,unsigned int precision)
{

	unsigned long tamanioDescompresion= 0;
	unsigned int indiceDescompresion= 0;
	BufferBits<TAMANIO_BUFFER_BITS> bufferBits;
	const unsigned int PRECISION_BYTES= (unsigned int)(precision/8);


	for(unsigned int i=0;i<PRECISION_BYTES;i++){


	}



	/*
		1)-Resolver overflow y underflow

		2)-Definir todos los rangos de los caracteres del intervalo resultante de 1).

		3)Leer los 8 bits del archivo comprimido de la sgte forma:
		  a) Caso Overflow: Descartar los primeros n bits del archivo comprimido como n overflow haya ocurrido.
		  b) Caso Underflow: Leer el 1er bit y luego descartar tantos bits como el contador de underflow me indique.

		  Veo a que rango pertenece(busqueda binaria) y emito el caracter asociado a ese rango.
		  Los nuevos límites del intervalo van a estar dados por el techo y piso del rango de caracter emitido.

		4) Vuelvo a 1)
	 */



	return RES_OK;
}
