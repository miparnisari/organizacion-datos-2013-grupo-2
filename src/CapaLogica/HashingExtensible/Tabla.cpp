/*
 * Tabla.cpp
 *
 *  Created on: Apr 22, 2013
 *      Author: nico
 */

#include "Tabla.h"

Tabla::Tabla()
{
	this->set_rutaTabla("");
}

/*Crea la tabla si no esta en la ruta especificada, con tamanio 1, y contenido -1,
 * en caso de estar el archivo, carga su tamanio.
 * */
Tabla::Tabla(string rutaArchivo) {
	//en principio la tabla se crea con tamanio 1
	ManejadorArchivoDatosBasicos<int> archivo;

	if (archivo.abrir_archivo(rutaArchivo, "rb+") == RES_ERROR){
		archivo.crear_archivo(rutaArchivo);
		archivo.abrir_archivo(rutaArchivo,"rb+");
		archivo.agregar(-1);
	}

	this->set_rutaTabla(rutaArchivo);

	archivo.cerrar_archivo();
}

Tabla::~Tabla() {
}

void Tabla::set_rutaTabla(string ruta){
	this->rutaTabla = ruta;
}

string Tabla::_get_ruta_tabla(){
	return this->rutaTabla;
}

int Tabla::obtener_valor(int posicion){
	ManejadorArchivoDatosBasicos<int> archivo;
	int contenido;

	int res = archivo.abrir_archivo(this->_get_ruta_tabla(), "rb+");
	if (res == RES_OK)
	{
		res = archivo.leer(&contenido,posicion);
		archivo.cerrar_archivo();
		if (res != RES_OK)
			return RES_ERROR;

		return contenido;
	}
	return RES_ERROR;
}

int Tabla::cambiar_valor(int posicion, int nuevoValor){
	ManejadorArchivoDatosBasicos<int> archivo;

	if (archivo.abrir_archivo(this->_get_ruta_tabla(), "rb+") == RES_OK)
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
	ManejadorArchivoDatosBasicos<int> archivo;

	if (archivo.abrir_archivo(this->_get_ruta_tabla(), "rb+") == RES_OK)
	{
		return archivo.get_cantidad_datos_guardados();
	}

	return -1;
}

int Tabla::dividir_tabla(){

	ManejadorArchivoDatosBasicos<int> archivo;

	if (archivo.abrir_archivo(this->_get_ruta_tabla(), "rb+") == RES_OK)
	{
		archivo.truncar(archivo.get_cantidad_datos_guardados() / 2);
	}

	return archivo.cerrar_archivo();
}

int Tabla::duplicar_tabla(){
	int unDato;
	ManejadorArchivoDatosBasicos<int> archivo;

	int res = archivo.abrir_archivo(this->_get_ruta_tabla(), "rb+");
	if (res == RES_OK)
	{
		int cantidadDatosGuardadosAntesDeDuplicar = archivo.get_cantidad_datos_guardados();
		for (int i = 0; i < cantidadDatosGuardadosAntesDeDuplicar; ++i) {
			if (archivo.leer(&unDato, i) == RES_OK)
			{
				archivo.agregar(unDato);
			}
			else {
				return RES_ERROR;
			}
		}

		return archivo.cerrar_archivo();
	}

	return RES_ERROR;
}

int Tabla::eliminar_tabla(){
	ManejadorArchivoDatosBasicos<int> archivo;

	return archivo.eliminar_archivo(this->_get_ruta_tabla());
}

int Tabla::crear_archivo_tabla(string rutaArchivo){
	ManejadorArchivoDatosBasicos<int> archivo;

	return archivo.crear_archivo(rutaArchivo);
}
