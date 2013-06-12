/*
 * ModeloProbabilistico.h
 *
 *  Created on: 12/06/2013
 *      Author: Betty
 */

#ifndef MODELOPROBABILISTICO_H_
#define MODELOPROBABILISTICO_H_

#include "../../Constantes.h"

typedef unsigned int Uint;

class ModeloProbabilistico {

	private:
		Uint frecuenciasSimbolos[TAMANIO_ALFABETO];
		void inicializar_dinamico();

	public:
		ModeloProbabilistico();
		virtual ~ModeloProbabilistico();


		void incrementar_frecuencia (char simbolo);
		Uint calcular_low_count (const char simbolo);
		Uint calcular_high_count (const char simbolo);
		Uint calcular_total_frecuencias();

		Uint obtener_simbolo(Uint valor);
};

#endif /* MODELOPROBABILISTICO_H_ */
