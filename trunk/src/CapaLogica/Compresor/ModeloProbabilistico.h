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

using std::vector;

typedef unsigned int Uint;

class ModeloProbabilistico {

	private:
		Uint* frecuenciasSimbolos;

		Uint calcular_total_frecuencias();

	public:
		ModeloProbabilistico(short tamanio_alfabeto = TAMANIO_ALFABETO);
		virtual ~ModeloProbabilistico();

		void inicializar_frecuencias_en_1();
		void inicializar_frecuencias_en_1(vector<unsigned short>& v);
		/*todas las frecuencias de los mensajes estaran en 0 excepto la de aquellos mensajes contendidos en v*/


		void incrementar_frecuencia (const char simbolo);
		double calcular_low_count (const char simbolo);
		double calcular_high_count (const char simbolo);

		Uint obtener_simbolo(Uint valor);
};

#endif /* MODELOPROBABILISTICO_H_ */
