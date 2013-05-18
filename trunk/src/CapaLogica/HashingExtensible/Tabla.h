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

		void set_rutaTabla(string ruta);

		int get_tamanio();

		int obtener_valor(int posicion);
		int cambiar_valor(int posicion, int nuevoValor);
		int dividir_tabla();
		int duplicar_tabla();
		int eliminar_tabla();
		int crear_archivo_tabla(string rutaArchivo);
};

#endif /* TABLA_H_ */
