/*
 * Tabla.cpp
 *
 *  Created on: Apr 22, 2013
 *      Author: nico
 */

#include "Tabla.h"

Tabla::Tabla() {
	//en principio la tabla se crea con tamanio 1
	int valorNulo = -1;

	this->tamanio = 1;
	this->getBloques()->agregar_nuevo_dato(valorNulo);

}

Tabla::~Tabla() {

}

void Tabla::set_tamanio(int cantidad){
	this->tamanio = cantidad;
}

int Tabla::get_tamanio(){
	return this->tamanio;
}


int Tabla::obtener_valor(int posicion){
	//devuelve el contenido de una posicion de la tabla

	int bloqueReferenciado;

	if (posicion > this->tamanio){
		bloqueReferenciado = -1;
		return bloqueReferenciado;
	}else{
		bloqueReferenciado = this->getBloques()->obtener_valor(posicion);
		return bloqueReferenciado;
	}
}

void Tabla::cambiar_valor(int posicion, int nuevoValor){
	this->getBloques()->cambiar_valor(posicion, nuevoValor);
}

void Tabla::dividir_tabla(){

	int cantPosiciones = (this->getBloques()->getCantidadNodos()) / 2;

	for (int i = 1; i < cantPosiciones; ++i) {
		this->getBloques()->eliminar_ultimo();
	}

	this->setTamanio(cantPosiciones);
}

void Tabla::duplicar_tabla(){
	ListaDE<int>* nuevaLista = new ListaDE<int>();

	//creo una nueva lista con los datos de la que ya tengo
	for (int i = 1; i < this->getBloques()->getCantidadNodos(); ++i) {
		nuevaLista->agregar_nuevo_dato(this->getBloques()->obtener_nodo(i)->getDato());
	}

	//uno las listas
	this->getBloques()->concatenar_listas(nuevaLista);

}

