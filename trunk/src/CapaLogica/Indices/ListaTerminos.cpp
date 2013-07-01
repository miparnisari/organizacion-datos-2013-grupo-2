/*
 * ListaTerminos.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: maine
 */

#include "ListaTerminos.h"

ListaTerminos::ListaTerminos() {

}

ListaTerminos::~ListaTerminos() {
}

int ListaTerminos::crear(std::string p_nombre_archivo)
{
	nombre_archivo = p_nombre_archivo;
	int res = archivo.crear_archivo(nombre_archivo);
	return res;
}

int ListaTerminos::eliminar()
{
	int res = archivo.eliminar_archivo(nombre_archivo);
	return res;
}

int ListaTerminos::abrir(std::string p_nombre_archivo)
{
	nombre_archivo = p_nombre_archivo;
	int res = archivo.abrir_archivo(nombre_archivo);
	return res;
}

int ListaTerminos::cerrar()
{
	return RES_OK;
}

IDtermino_t ListaTerminos::agregar(const Termino_t & termino)
{
	// Si se quiere comprimir el termino, este es el lugar para hacerlo.

	RegistroVariable registro;
	registro.agregar_campo(termino.c_str(), termino.size());

	IDtermino_t ID_termino = archivo.get_cantidad_registros_ocupados();

	archivo.agregar_registro(&registro);

	return ID_termino;
}

Termino_t ListaTerminos::devolver(const IDtermino_t & id_termino)
{
	// Si se quiere descomprimir el termino, este es el lugar para hacerlo.

	RegistroVariable registro;

	archivo.get_registro(&registro,id_termino);

	char* c_termino = new char[registro.get_tamanio_campo(0) + 1]();
	c_termino[registro.get_tamanio_campo(0)] = '\0';

	registro.recuperar_campo(c_termino,0);

	Termino_t termino (c_termino);

	delete[] c_termino;

	return termino;
}
