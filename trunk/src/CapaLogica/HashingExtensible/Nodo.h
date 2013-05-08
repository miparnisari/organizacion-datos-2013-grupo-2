/*
 * Nodo.h
 *
 *  Created on: Apr 22, 2013
 *      Author: nico
 */

#ifndef NODO_H_
#define NODO_H_
#include <iostream>
#include <cstddef>

using namespace std;
template <class T>
class Nodo
{
private:
	T dato;
	Nodo* siguiente;
	Nodo* anterior;

  public:
	Nodo()
	{
	  dato = NULL;
	  siguiente = NULL;
	  anterior = NULL;
	}
    Nodo(T unDato)
    {
      dato = unDato;
      siguiente = NULL;
      anterior = NULL;
    }
    ~Nodo()
    {

    }

    T getDato()
    {
     return this->dato;
    }

    Nodo<T>* getSiguiente(){
    	return this->siguiente;
    }
    Nodo<T>* getAnterior(){
    	return this->anterior;
    }
    void setAnterior(Nodo* nodoAnterior){
    	this->anterior = nodoAnterior;
    }
    void setDato(T dato) {
    	this->dato = dato;
    }
    void setSiguiente(Nodo* nodoSiguiente){
    	this->siguiente = nodoSiguiente;
    }
};

#endif /* NODO_H_ */
