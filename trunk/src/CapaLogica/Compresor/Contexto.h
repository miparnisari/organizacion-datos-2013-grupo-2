/*
 * Contexto.h
 *
 *  Created on: Jun 21, 2013
 *      Author: nico
 */

#ifndef CONTEXTO_H_
#define CONTEXTO_H_

#include "ModeloProbabilistico.h"
#include <map>

namespace std {

class Contexto {

	private:
		map<string, ModeloProbabilistico*> listaDeModelos;




	public:
		Contexto();
		~Contexto();

		/*a la lista de modelos, se le agrega un nuevo modelo en el cual, la unica frecuencia con
		 * probabilidad 1 es el caracter escape*/
		void agregar_modelo(string nombreModelo);

		/*EN caso de que se encuentre el modelo buscado, se incrementa en uno la frecuencia del caracter, en caso contrario
		 * devuelve error
		 * */
		int incrementar_frecuencia(long caracter, string nombreModelo);

		int devolver_modelo(string nombreModelo, ModeloProbabilistico** modeloADevolver);


};

} /* namespace std */
#endif /* CONTEXTO_H_ */
