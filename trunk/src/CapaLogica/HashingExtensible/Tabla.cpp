/*
 * Tabla.cpp
 *
 *  Created on: Apr 22, 2013
 *      Author: nico
 */

#include "Tabla.h"

Tabla::Tabla()
{

}

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

void Tabla::set_rutaTabla(string ruta){
	this->rutaTabla = ruta;
}

string Tabla::get_rutaTabla(){
	return this->rutaTabla;
}

int Tabla::obtener_valor(int posicion){
	ManejadorArchivoDatosBasicos<int> archivo;
	int contenido;

	archivo.abrir_archivo(this->get_rutaTabla(), "rb");

	archivo.leer(&contenido,posicion);

	return contenido;
	archivo.cerrar_archivo();
}

void Tabla::cambiar_valor(int posicion, int nuevoValor){
	ManejadorArchivoDatosBasicos<int> archivo;
	int contenido;

	archivo.abrir_archivo(this->get_rutaTabla(), "wb");

	archivo.escribir(contenido,posicion);

	archivo.cerrar_archivo();
}

void Tabla::dividir_tabla(){

	this->set_tamanio((this->get_tamanio() / 2));
}

void Tabla::duplicar_tabla(){
	int unDato;
	ManejadorArchivoDatosBasicos<int> archivo;

	archivo.abrir_archivo(this->get_rutaTabla(), "wb");

	for (int i = 1; i < this->get_tamanio(); ++i) {
		archivo.leer(&unDato, sizeof(int)*i);
		archivo.escribir(unDato, sizeof(int)*(i + this->get_tamanio()));
	}

	//modifico el tamanio
	this->set_tamanio((this->get_tamanio() * 2));

	archivo.cerrar_archivo();
}

void Tabla::eliminar_tabla(){
	ManejadorArchivoDatosBasicos<int> archivo;

	archivo.eliminar_archivo(this->get_rutaTabla());
}
