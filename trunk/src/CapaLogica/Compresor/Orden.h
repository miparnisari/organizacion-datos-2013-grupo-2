/*
 * Orden.h
 *
 *  Created on: Jun 21, 2013
 *      Author: nico
 */

#ifndef ORDEN_H_
#define ORDEN_H_

#include "ModeloProbabilistico.h"
#include <map>

using namespace std;

class Orden {

	private:
		map<string, ModeloProbabilistico*> modelos;

	public:
		Orden();
		~Orden();

		/*a la lista de modelos, se le agrega un nuevo modelo en el cual, la unica frecuencia con
		 * probabilidad 1 es el caracter escape*/
		int agregar_modelo(string nombreModelo, ModeloProbabilistico* modelo);

		/*EN caso de que se encuentre el modelo buscado, se incrementa en uno la frecuencia del caracter, en caso contrario
		 * devuelve error
		 * */
		int incrementar_frecuencia(long caracter, string nombreModelo);

		int devolver_modelo(string nombreModelo, ModeloProbabilistico** modeloADevolver);

		//FIXME esto solo es necesario para debuggear
		map<string, ModeloProbabilistico*> get_mapa_modelos();
};

#endif /* ORDEN_H_ */
