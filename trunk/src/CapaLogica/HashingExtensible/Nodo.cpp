/*
 * Nodo.cpp
 *
 *  Created on: Apr 22, 2013
 *      Author: nico
 */

#include "Nodo.h"

template<class T>
Nodo<T>::Nodo()
{
  dato = NULL;
  siguiente = NULL;
  anterior = NULL;
}


template<class T>
Nodo<T>::Nodo(T unDato)
{
  dato = unDato;
  siguiente = NULL;
  anterior = NULL;
}


template<class T>
void Nodo<T>::setDato(T dato)
{
 this->dato = dato;
}

template<class T>
T Nodo<T>::getDato()
{
 return this->dato;
}


template<class T>
void Nodo<T>::setSiguiente(Nodo* nodoSiguiente){
	this->siguiente = nodoSiguiente;
}

template<class T>
Nodo<T>* Nodo<T>::getSiguiente(){
	return this->siguiente;
}

template<class T>
void Nodo<T>::setAnterior(Nodo* nodoAnterior){
	this->anterior = nodoAnterior;
}

template<class T>
Nodo<T>* Nodo<T>::getAnterior(){
	return this->anterior;
}

template<class T>
Nodo<T>::~Nodo(){}

