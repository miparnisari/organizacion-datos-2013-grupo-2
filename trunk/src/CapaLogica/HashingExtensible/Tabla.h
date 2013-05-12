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

public:
	Tabla(string rutaArchivo);
	Tabla();
	virtual ~Tabla();

	void set_tamanio(int cantidad);
	int get_tamanio();

	void set_rutaTabla(string ruta);
	string get_rutaTabla();

	int obtener_valor(int posicion);
	void cambiar_valor(int posicion, int nuevoValor);
	void dividir_tabla();
	void duplicar_tabla();
	void eliminar_tabla();
};

#endif /* TABLA_H_ */
