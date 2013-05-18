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
	this->set_tamanio(0);
}

/*Crea la tabla si no esta en la ruta especificada, con tamanio 1, y contenido -1,
 * en caso de estar el archivo, carga su tamanio.
 * */
Tabla::Tabla(string rutaArchivo) {
	//en principio la tabla se crea con tamanio 1
	ManejadorArchivoDatosBasicos<int> archivo;
	int tamanio = 1;

	if(archivo.abrir_archivo(rutaArchivo, "rb+") == RES_ERROR){
		archivo.crear_archivo(rutaArchivo);
		archivo.abrir_archivo(rutaArchivo, "rb+");
		archivo.escribir(1,0);
		archivo.escribir(-1,1);
	}else{
		archivo.leer(&tamanio, 0);
	}

	this->set_rutaTabla(rutaArchivo);
	this->set_tamanio(tamanio);

	archivo.cerrar_archivo();
}

Tabla::~Tabla() {
}

void Tabla::set_tamanio(int cantidad){
	this->tamanio = cantidad;
}

int Tabla::get_tamanio(){
	return this->tamanio;
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

void Tabla::dividir_tabla(){

	ManejadorArchivoDatosBasicos<int> archivo;

	archivo.truncar(this->get_tamanio() / 2);

	this->set_tamanio((this->get_tamanio() / 2));
}

int Tabla::duplicar_tabla(){
	int unDato;
	ManejadorArchivoDatosBasicos<int> archivo;

	int res = archivo.abrir_archivo(this->_get_ruta_tabla(), "rb+");
	if (res == RES_OK)
	{
		for (int i = 1; i < this->get_tamanio(); ++i) {
			archivo.leer(&unDato, i);
			archivo.agregar(unDato);
		}

		//modifico el tamanio
		this->set_tamanio((this->get_tamanio() * 2));

		return archivo.cerrar_archivo();
	}

	return RES_ERROR;
}

void Tabla::eliminar_tabla(){
	ManejadorArchivoDatosBasicos<int> archivo;

	archivo.eliminar_archivo(this->_get_ruta_tabla());
}

void Tabla::crear_archivo_tabla(string rutaArchivo){
	ManejadorArchivoDatosBasicos<int> archivo;

	archivo.crear_archivo(rutaArchivo);
}
