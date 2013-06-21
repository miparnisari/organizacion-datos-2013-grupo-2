/*
 * ModeloProbabilistico.h
 *
 *  Created on: 12/06/2013
 *      Author: maine
 */

#ifndef MODELOPROBABILISTICO_H_
#define MODELOPROBABILISTICO_H_

#include "../../Constantes.h"
#include <iostream>
#include <vector>
#include <string.h>

using std::vector;

typedef unsigned int Uint;

class ModeloProbabilistico {

	private:
		Uint* frecuenciasSimbolos;
		short tamanioAlfabeto;

		Uint calcular_total_frecuencias();

	public:
		ModeloProbabilistico(short un_tamanio_alfabeto = TAMANIO_ALFABETO);
		virtual ~ModeloProbabilistico();

		ModeloProbabilistico& operator = (const ModeloProbabilistico& otro);

		void inicializar_frecuencias_en_1();
		void inicializar_frecuencias_en_1(vector<unsigned short>& v);
		/*todas las frecuencias de los mensajes estaran en 0 excepto la de aquellos mensajes contendidos en v*/


		void incrementar_frecuencia (const char simbolo);
		double calcular_low_count (const char simbolo);
		double calcular_high_count (const char simbolo);


		short get_tamanio_alfabeto();


		void resetear();


		Uint obtener_simbolo(Uint valor);
		/*se recupera el valor ASCII de un simbolo a partir de un valor*/
};

#endif /* MODELOPROBABILISTICO_H_ */
