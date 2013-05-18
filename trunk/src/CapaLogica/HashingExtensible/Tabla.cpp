/*
 * Tabla.cpp
 *
 *  Created on: Apr 22, 2013
 *      Author: nico
 */

#include "Tabla.h"

Tabla::Tabla()
	:rutaArchivo("")
{
}

Tabla::~Tabla() {
}

int Tabla::crear(string unaRuta)
{
	this->rutaArchivo = unaRuta;

	if (archivo.abrir_archivo(rutaArchivo, "rb+") == RES_ERROR){
		archivo.crear_archivo(rutaArchivo);
		archivo.abrir_archivo(rutaArchivo,"rb+");
		archivo.agregar(-1);
	}

	return archivo.cerrar_archivo();
}/* POSTCONDICION: la tabla tiene un solo valor (-1). */

int Tabla::eliminar()
{
	return archivo.eliminar_archivo(this->rutaArchivo);
}

int Tabla::obtener_valor(int posicion){

	int res = archivo.abrir_archivo(this->rutaArchivo, "rb+");
	if (res == RES_OK)
	{
		int contenido;
		res = archivo.leer(&contenido,posicion);
		archivo.cerrar_archivo();
		if (res != RES_OK)
			return RES_ERROR;

		return contenido;
	}
	return RES_ERROR;
}

int Tabla::cambiar_valor(int posicion, int nuevoValor){

	if (archivo.abrir_archivo(this->rutaArchivo, "rb+") == RES_OK)
	{
		int res = archivo.escribir(nuevoValor,posicion);
		archivo.cerrar_archivo();
		if (res == RES_OK)
			return RES_OK;
		return RES_ERROR;
	}
	return RES_ERROR;
}

int Tabla::get_tamanio()
{
	if (archivo.abrir_archivo(this->rutaArchivo,"rb+") == RES_OK)
	{
		return archivo.get_cantidad_datos_guardados();
	}

	return -1;
}

int Tabla::dividir()
{
	if (archivo.abrir_archivo(this->rutaArchivo, "rb+") == RES_OK)
	{
		archivo.truncar(archivo.get_cantidad_datos_guardados() / 2);
	}

	return archivo.cerrar_archivo();
}

int Tabla::duplicar()
{
	int res = archivo.abrir_archivo(this->rutaArchivo, "rb+");
	if (res == RES_OK)
	{
		int cantidadDatosGuardadosAntesDeDuplicar = archivo.get_cantidad_datos_guardados();
		for (int i = 0; i < cantidadDatosGuardadosAntesDeDuplicar; ++i) {
			int unDato;
			if (archivo.leer(&unDato, i) == RES_OK)
			{
				archivo.agregar(unDato);
			}
			else {
				archivo.cerrar_archivo();
				return RES_ERROR;
			}
		}

		return archivo.cerrar_archivo();
	}

	return RES_ERROR;
}
