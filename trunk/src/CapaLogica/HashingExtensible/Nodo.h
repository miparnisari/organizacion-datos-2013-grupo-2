/*
 * Nodo.h
 *
 *  Created on: Apr 22, 2013
 *      Author: nico
 */

#ifndef NODO_H_
#define NODO_H_
#include <iostream>

using namespace std;
template <class T>
class Nodo
{
private:
	T dato;
	Nodo* siguiente;
	Nodo* anterior;

  public:
    Nodo();
    Nodo(T);
    ~Nodo();

    T getDato();
    Nodo<T>* getSiguiente();
    Nodo<T>* getAnterior();
    void setAnterior(Nodo* nodoAnterior);
    void setDato(T dato);
    void setSiguiente(Nodo* nodoSiguiente);
};

#endif /* NODO_H_ */
