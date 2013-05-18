/*
 * Tabla.h
 *
 *  Created on: Apr 22, 2013
 *      Author: nico
 */

#ifndef TABLA_H_
#define TABLA_H_
#include <iostream>
#include <cstddef>
#include "../../Constantes.h"
#include "../../CapaFisica/ManejadorArchivoDatosBasicos.h"


using namespace std;


class Tabla {
	private:
		string rutaArchivo;
		ManejadorArchivoDatosBasicos<int> archivo;

	public:
		Tabla();
		virtual ~Tabla();

		int crear(string rutaArchivo);
		int eliminar();

		int get_tamanio();

		int obtener_valor(int posicion);
		int cambiar_valor(int posicion, int nuevoValor);

		int dividir();
		int duplicar();


};

#endif /* TABLA_H_ */
