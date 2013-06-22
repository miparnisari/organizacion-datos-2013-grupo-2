/*
 * Contexto.h
 *
 *  Created on: Jun 21, 2013
 *      Author: nico
 */

#ifndef CONTEXTOS_H_
#define CONTEXTOS_H_

#include "ModeloProbabilistico.h"
#include <map>

using namespace std;

class Contextos {

	private:
		map<string, ModeloProbabilistico*> listaDeModelos;

	public:
		Contextos();
		~Contextos();

		/*a la lista de modelos, se le agrega un nuevo modelo en el cual, la unica frecuencia con
		 * probabilidad 1 es el caracter escape*/
		void agregar_modelo(string nombreModelo, ModeloProbabilistico* modelo);

		/*EN caso de que se encuentre el modelo buscado, se incrementa en uno la frecuencia del caracter, en caso contrario
		 * devuelve error
		 * */
		int incrementar_frecuencia(long caracter, string nombreModelo);

		int devolver_modelo(string nombreModelo, ModeloProbabilistico** modeloADevolver);
};

#endif /* CONTEXTOS_H_ */
