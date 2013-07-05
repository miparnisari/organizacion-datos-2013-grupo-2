/*
 * ModeloProbabilistico.h
 *
 *  Created on: 12/06/2013
 *      Author: maine
 */

#ifndef MODELOPROBABILISTICO_H_
#define MODELOPROBABILISTICO_H_

#include "../../Constantes.h"
#include "../../Utilitarios/Utilitarios.h"
#include <iostream>
#include <vector>
#include <string.h>
#include <cmath>
#include <fstream>

using std::ofstream;
using std::vector;

typedef unsigned int Uint;

class ModeloProbabilistico {

	private:
		Uint* frecuenciasSimbolos;
		Uint tamanioAlfabeto;
		Uint totalFrecuencias;

	public:
		ModeloProbabilistico(Uint un_tamanio_alfabeto = TAMANIO_ALFABETO);
		ModeloProbabilistico(const ModeloProbabilistico& otro);
		virtual ~ModeloProbabilistico();

		ModeloProbabilistico& operator = (const ModeloProbabilistico& otro);

		void inicializar_frecuencias_en_1();
		void inicializar_frecuencias_en_1(vector<unsigned short>& v);
		/*todas las frecuencias de los mensajes estaran en 0 excepto la de aquellos mensajes contendidos en v*/

		double get_probabilidad(Uint simbolo)const;
		Uint get_frecuencia(Uint simbolo)const;
		Uint get_tamanio_alfabeto()const;

		void incrementar_frecuencia (const Uint simbolo);
		double calcular_low_count (const Uint simbolo)const;
		double calcular_high_count (const Uint simbolo)const;
		Uint calcular_total_frecuencias()const;

		bool todas_frecuencias_en_uno()const;

		void resetear();

		std::string imprimir()const;
};

#endif /* MODELOPROBABILISTICO_H_ */
