/*
 * Lista.h
 *
 *  Created on: Apr 22, 2013
 *      Author: nico
 */

#ifndef LISTA_H_
#define LISTA_H_

#include <iostream>
#include <fstream>
#include <string>
#include <cstddef>
#include "Nodo.h"

using namespace std;

template <class T>
class ListaDE
{
  private:
    int cantElementos;
    Nodo<T>* inicio;

    Nodo<T>* cargarSiguienteNodo(Nodo<T>* unNodo){
    	Nodo<T>* nuevoNodo;
    	nuevoNodo->setDato(unNodo->getSiguiente()->getDato());
    	nuevoNodo->setSiguiente(unNodo->getSiguiente()->getSiguiente());
    	nuevoNodo->setAnterior(unNodo->getSiguiente()->getAnterior());

    	return nuevoNodo;
    }

  public:
    ListaDE()
    {
      this->cantElementos = 0;
      this->inicio = NULL;
    }
    ~ListaDE();

    Nodo<T> getPrimeraPosicion(){
   	 return (*this->inicio);
   }
    int getCantidadNodos(){
   	 return this->cantElementos;
   }

    void setCantidadNodos(int cantidad){
    	this->cantElementos = cantidad;
    }
    Nodo<T>* obtener_nodo(int posicion){
    	Nodo<T>* nodoAux;
    	int i;

    	if(posicion > this->cantElementos){
    		return NULL;
    	}else{
    		//obtengo el primer nodo
    		*nodoAux = this->getPrimeraPosicion();

    		for (i = 0; i < posicion; ++i) {
    			this->cargarSiguienteNodo(nodoAux);
    		}
    		return nodoAux;
    	}
    }

    T obtener_valor(int posicion){
    	return (this->obtener_nodo(posicion))->getDato();
    }

    void cambiar_valor(int posicion, T nuevoValor){
    	Nodo<T>* nodoAModificar = this->obtener_nodo(posicion);

    	nodoAModificar->setDato(nuevoValor);
    }

    void agregar_nuevo_dato(T dato){
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

    void concatenar_listas(ListaDE<T>* otraLista){

    	Nodo<T>* ultimoNodo;

    	ultimoNodo = this->obtener_nodo(this->getCantidadNodos());
    	Nodo<T> nodo = otraLista->getPrimeraPosicion();
    	ultimoNodo->setSiguiente(&nodo);
    	otraLista->getPrimeraPosicion().setAnterior(ultimoNodo);

    }

    void eliminar_ultimo(){
    	Nodo<T>* anteultimoNodo;
    	Nodo<T>* ultimoNodo;

    	anteultimoNodo = this->obtener_nodo((this->getCantidadNodos() - 1));
    	ultimoNodo = this->obtener_nodo(this->getCantidadNodos());

    	anteultimoNodo->setSiguiente(NULL);
    	delete ultimoNodo;

    	this->setCantidadNodos((this->getCantidadNodos() - 1));

    }

};

#endif /* LISTA_H_ */
