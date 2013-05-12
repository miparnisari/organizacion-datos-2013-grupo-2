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
	int tamanio;

public:
	Tabla(string rutaArchivo);
	virtual ~Tabla();

	void set_tamanio(int cantidad);
	int get_tamanio();

	int obtener_valor(string rutaArchivo, int posicion);
	void cambiar_valor(string rutaArchivo, int posicion, int nuevoValor);
	void dividir_tabla(string rutaArchivo);
	void duplicar_tabla(string rutaArchivo);
	void eliminar_tabla(string rutaArchivo);
};

#endif /* TABLA_H_ */
