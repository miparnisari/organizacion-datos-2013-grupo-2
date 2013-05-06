/*
 * Lista.cpp
 *
 *  Created on: Apr 22, 2013
 *      Author: nico
 */

#include "ListaDE.h"

using namespace std;


template<class T>
ListaDE<T>::ListaDE()
{
  this->cantElementos = 0;
  this->inicio = NULL;
}

template<class T>
Nodo<T> ListaDE<T>::getPrimeraPosicion(){
	 return this->inicio;
}

template<class T>
int ListaDE<T>::getCantidadNodos(){
	 return this->cantElementos;
}

template<class T>
void ListaDE<T>::setCantidadNodos(int cantidad){
	this->cantElementos = cantidad;
}

template<class T>
Nodo<T>* ListaDE<T>::cargarSiguienteNodo(Nodo<T>* unNodo){
	Nodo<T>* nuevoNodo;
	nuevoNodo->setDato(unNodo->getSiguiente()->getDato());
	nuevoNodo->setSiguiente(unNodo->getSiguiente()->getSiguiente());
	nuevoNodo->setAnterior(unNodo->getSiguiente()->getAnterior());

	return nuevoNodo;
}


template<class T>
Nodo<T>* ListaDE<T>::obtener_nodo(int posicion){
	Nodo<T>* nodoAux;
	int i;

	if(posicion > this->cantElementos){
		return -1;//es correcto usar este numero chicos?
	}else{
		//obtengo el primer nodo
		nodoAux = this->getPrimeraPosicion();

		for (i = 0; i < posicion; ++i) {
			this->cargarSiguienteNodo(nodoAux);
		}
		return nodoAux;
	}
}


template<class T>
T ListaDE<T>::obtener_valor(int posicion){
	return (this->obtener_nodo(posicion))->getDato();
}

template<class T>
void ListaDE<T>::cambiar_valor(int posicion, T nuevoValor){
	Nodo<T>* nodoAModificar = this->obtener_nodo(posicion);

	nodoAModificar->setDato(nuevoValor);
}

template<class T>
void ListaDE<T>::agregar_nuevo_dato(T dato){
	Nodo<T>* nuevoNodo = new Nodo<T>(dato);
	Nodo<T>* ultimoNodo;

	if(this->inicio == NULL){
		this->inicio = new Nodo<T>(dato);
		this->setCantidadNodos(1);

	}else {
		ultimoNodo = this->obtener_nodo(this->getCantidadNodos());

		ultimoNodo->setSiguiente(nuevoNodo);
		nuevoNodo->setAnterior(ultimoNodo);
		this->setCantidadNodos((this->getCantidadNodos() + 1));
	}


}


template<class T>
void ListaDE<T>::concatenar_listas(ListaDE<T>* otraLista){

	Nodo<T>* ultimoNodo;

	ultimoNodo = this->obtener_nodo(this->getCantidadNodos());
	ultimoNodo->setSiguiente(otraLista->getPrimeraPosicion());
	otraLista->getPrimeraPosicion().setAnterior(ultimoNodo);

}

template<class T>
void ListaDE<T>::eliminar_ultimo(){
	Nodo<T>* anteultimoNodo;
	Nodo<T>* ultimoNodo;

	anteultimoNodo = this->obtener_nodo((this->getCantidadNodos() - 1));
	ultimoNodo = this->obtener_nodo(this->getCantidadNodos());

	anteultimoNodo->setSiguiente(NULL);
	delete ultimoNodo;

	this->setCantidadNodos((this->getCantidadNodos() - 1));

}
