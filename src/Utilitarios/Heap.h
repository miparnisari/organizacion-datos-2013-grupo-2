#ifndef HEAP_H
#define HEAP_H

#include <iostream>
#include "../CapaLogica/ManejoArchivos/RegistroClave.h"

using namespace std;

class Heap
{
private:
    int tamanio;
    void CargarVectorYTransformarEnHeap(RegistroClave vector[]);
    void ReestructurarHeap(RegistroClave vectorAOrdenar[],int desde,int hasta);

public:
    Heap();
    ~Heap();

    void ordenar(RegistroClave vectorAOrdenar[],int tamanio);
};

#endif // HEAP_H
