/*
 * Tabla.h
 *
 *  Created on: Apr 22, 2013
 *      Author: nico
 */

#ifndef TABLA_H_
#define TABLA_H_
#include <iostream>
#include "ListaDE.h"
#include "../../CapaFisica/ManejadorArchivoDatosBasicos.h"


using namespace std;


class Tabla {
private:
	int tamanio;
	ListaDE<int>* bloques;

public:
	Tabla();
	Tabla(string tablaPersistida);
	virtual ~Tabla();

	ListaDE<int>* getBloques();

	void set_tamanio(int cantidad);
	int get_tamanio();

    string persistir_tabla(); //Esta me dijo nico que iba....
	int obtener_valor(int posicion);
	void cambiar_valor(int posicion, int nuevoValor);
	void dividir_tabla();
	void duplicar_tabla();

	void persistir_tabla(string rutaArchivo);
	void cargar_tabla(string rutaArchivo);

};

#endif /* TABLA_H_ */
