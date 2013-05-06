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
#include "Nodo.h"

using namespace std;

template <class T>
class ListaDE
{
  private:
    int cantElementos;
    Nodo<T>* inicio;

    Nodo<T>* cargarSiguienteNodo(Nodo<T>* unNodo);

  public:
    ListaDE();
    ~ListaDE();

    Nodo<T> getPrimeraPosicion();
    int getCantidadNodos();
    void setCantidadNodos(int cantidad);
    Nodo<T>* obtener_nodo(int posicion);

    T obtener_valor(int posicion);
    void cambiar_valor(int posicion, T nuevoValor);
    void agregar_nuevo_dato(T dato);
    void concatenar_listas(ListaDE<T>* otraLista);
    void eliminar_ultimo();

};

#endif /* LISTA_H_ */
