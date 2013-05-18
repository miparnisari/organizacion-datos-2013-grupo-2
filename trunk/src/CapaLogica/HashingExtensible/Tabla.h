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
		string rutaTabla;
		int tamanio;

		string _get_ruta_tabla();

	public:
		Tabla(string rutaArchivo);
		Tabla();
		virtual ~Tabla();

		void set_tamanio(int cantidad);
		int get_tamanio();

		void set_rutaTabla(string ruta);


		int obtener_valor(int posicion);
		int cambiar_valor(int posicion, int nuevoValor);
		void dividir_tabla();
		int duplicar_tabla();
		void eliminar_tabla();
		void crear_archivo_tabla(string rutaArchivo);
};

#endif /* TABLA_H_ */
