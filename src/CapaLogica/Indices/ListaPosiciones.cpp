/*
 * ListaPosiciones.cpp
 *
 *  Created on: Jun 29, 2013
 *      Author: maine
 */

#include "ListaPosiciones.h"

ListaPosiciones::ListaPosiciones()
{
}

ListaPosiciones::~ListaPosiciones()
{
}

int ListaPosiciones::crear(std::string p_nombre_archivo)
{
	nombre_archivo = p_nombre_archivo;
	int res = archivo.crear_archivo(nombre_archivo);
	return res;
}
int ListaPosiciones::eliminar()
{
	int res = archivo.eliminar_archivo(nombre_archivo);
	return res;
}

int ListaPosiciones::abrir(std::string p_nombre_archivo)
{
	nombre_archivo = p_nombre_archivo;
	int res = archivo.abrir_archivo(nombre_archivo, "rb+");
	return res;
}
int ListaPosiciones::cerrar()
{
	int res = archivo.cerrar_archivo();
	return res;
}

int ListaPosiciones::_guardar_posicion(const PosicionTermino_t pos)
{
	// Si se quiere comprimir la posicion usando codigos delta, gamma o lo que sea,
	// este es el lugar indicado

	int posicion_guardado = archivo.agregar(pos);
	return posicion_guardado;
}

PosicionTermino_t ListaPosiciones::_devolver_posicion(const int posicion_guardado)
{
	// Si se quiere comprimir la posicion usando codigos delta, gamma o lo que sea,
	// este es el lugar indicado

	PosicionTermino_t pos_leida;

	archivo.leer(&pos_leida,posicion_guardado);
	return pos_leida;
}

ReferenciaListaPos_t ListaPosiciones::guardar_lista (const ListaPos_t & lista)
{
	const unsigned int TAMANIO_LISTA = lista.size();

	if (TAMANIO_LISTA == 0)
		return REFERENCIA_INVALIDA;

	ReferenciaListaPos_t offset_inicio = archivo.agregar(lista[0]);

	for (unsigned int i = 1; i < TAMANIO_LISTA; i ++)
	{
		PosicionTermino_t pos = lista[i];
		_guardar_posicion(pos);
	}

	return offset_inicio;
}

ListaPos_t ListaPosiciones::devolver_lista (const CantidadPosiciones_t cuantos, const ReferenciaListaPos_t desde)
{
	ListaPos_t lista_pos;

	PosicionTermino_t pos;

	for (unsigned int i = desde; i < desde+cuantos; i++)
	{
		pos = this->_devolver_posicion(i);
		lista_pos.push_back(pos);
	}

	return lista_pos;
}
