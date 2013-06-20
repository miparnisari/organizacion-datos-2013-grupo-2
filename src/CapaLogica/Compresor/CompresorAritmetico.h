/*
 * CompresorAritmetico.h
 *
 *  Created on: Jun 6, 2013
 *      Author: martin
 */

#ifndef COMPRESORARITMETICO_H_
#define COMPRESORARITMETICO_H_

#include "../../Constantes.h"
#include "Codigo.h"
#include "Compresor.h"
#include <vector>
#include "Intervalo.h"
#include "ModeloProbabilistico.h"
#include "BufferBits.h"
#include <sstream>
#include <iostream>

using std::stringstream;


const TamanioBitset TAMANIO_BUFFER_FINAL= 4096;
const TamanioBitset TAMANIO_BUFFER_BITS_BYTES= 1024;
const TamanioBitset TAMANIO_BUFFER_BITS_BITS= TAMANIO_BUFFER_BITS_BYTES*8;



class CompresorAritmetico : public Compresor {


	private:
		std::vector<Codigo> vectorCodigos;
		Intervalo* intervalo;
		ModeloProbabilistico* modelo;
		char byteActual;



	public:


		CompresorAritmetico(unsigned int tamanioAlfabeto=TAMANIO_ALFABETO);
		virtual ~ CompresorAritmetico ();

		std::vector<bool> comprimir(const char simbolo);

		virtual int comprimir_todo
			(const char* buffer_a_comprimir,
			const unsigned int tamanio,
			char* bufferCompresion);

};/*Este compresor aplicara las tecnicas de compresion usando objetos del tipo
Codigo.
El compresor contendra una lista de los codigos cada uno con su respectiva frecuencia .*/

#endif /* COMPRESORARITMETICO_H_ */
