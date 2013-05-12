/*
 * Tabla.cpp
 *
 *  Created on: Apr 22, 2013
 *      Author: nico
 */

#include "Tabla.h"

/*Crea la tabla si no esta en la ruta especificada, con tamanio 1, y contenido -1,
 * en caso de estar el archivo, carga su tamanio.
 * */
Tabla::Tabla(string rutaArchivo) {
	//en principio la tabla se crea con tamanio 1
	ManejadorArchivoDatosBasicos<int> archivo;
	int tamanio;

	if(archivo.abrir_archivo(rutaArchivo, "rb") == RES_ERROR){
		archivo.crear_archivo(rutaArchivo);
	}else{
		this->set_tamanio(archivo.leer(&tamanio, 0));
	}

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


int Tabla::obtener_valor(string rutaArchivo, int posicion){
	ManejadorArchivoDatosBasicos<int> archivo;
	int contenido;

	archivo.abrir_archivo(rutaArchivo, "rb");

	archivo.leer(&contenido,posicion);

	return contenido;
	archivo.cerrar_archivo();
}

void Tabla::cambiar_valor(string rutaArchivo, int posicion, int nuevoValor){
	ManejadorArchivoDatosBasicos<int> archivo;
	int contenido;

	archivo.abrir_archivo(rutaArchivo, "wb");

	archivo.escribir(contenido,posicion);

	archivo.cerrar_archivo();
}

void Tabla::dividir_tabla(string rutaArchivo){

	this->set_tamanio((this->get_tamanio() / 2));
}

void Tabla::duplicar_tabla(string rutaArchivo){
	int unDato;
	ManejadorArchivoDatosBasicos<int> archivo;

	archivo.abrir_archivo(rutaArchivo, "wb");

	for (int i = 1; i < this->get_tamanio(); ++i) {
		archivo.leer(&unDato, sizeof(int)*i);
		archivo.escribir(unDato, sizeof(int)*(i + this->get_tamanio()));
	}

	//modifico el tamanio
	this->set_tamanio((this->get_tamanio() * 2));

	archivo.cerrar_archivo();
}

void eliminar_tabla(string rutaArchivo){
	ManejadorArchivoDatosBasicos<int> archivo;

	archivo.eliminar_archivo(rutaArchivo);
}
