/*
 * CompresorAritmetico.h
 *
 *  Created on: Jun 6, 2013
 *      Author: martin
 */

#ifndef ARIMETICO_H_
#define ARIMETICO_H_

#include "../../Constantes.h"
#include "Compresor.h"
#include "Intervalo.h"
#include "ModeloProbabilistico.h"
#include "BufferBits.h"

#include <cmath>
#include <sstream>

using std::stringstream;
using std::vector;

const TamanioBitset TAMANIO_BUFFER_FINAL= 4096;
const TamanioBitset TAMANIO_BUFFER_BITS_BYTES= 1024;
const TamanioBitset TAMANIO_BUFFER_BITS_BITS= TAMANIO_BUFFER_BITS_BYTES*8;
const TamanioBitset TAMANIO_BUFFER_BITS= PRECISION*2;


class Aritmetico : public Compresor {

	private:
		Intervalo* intervalo;
		ModeloProbabilistico* modelo;
		char byteActual;

		std::vector<bool> _comprimir_ultimo_paso();
		void _resetear();

	public:

		Aritmetico (ModeloProbabilistico* modelo);
		Aritmetico(Uint tamanioAlfabeto=TAMANIO_ALFABETO);
		virtual ~ Aritmetico ();

		void inicializar_frecuencias_en_1(vector<unsigned short>& v);

		Uint descomprimir(Uint valor);

		std::vector<bool> comprimir(const Uint simbolo,Byte& cOverflow,Byte& cUnderflow);

		virtual int comprimir_todo
			(const char* buffer_a_comprimir,
			const unsigned int tamanio,
			char* bufferCompresion);

		virtual int descomprimir_todo
			(char* buffer,
			int tamanio,
			char* descomprimido,
			unsigned int precision,
			unsigned int cantidadCaracteresOriginal);

		int set_modelo(ModeloProbabilistico* mp);


};

#endif /* ARIMETICO_H_ */
