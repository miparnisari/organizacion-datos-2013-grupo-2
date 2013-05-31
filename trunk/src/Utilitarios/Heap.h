#ifndef HEAP_H
#define HEAP_H

#include <iostream>
#include <string.h>

#include "../CapaLogica/ManejoArchivos/RegistroClave.h"
#include "../CapaFisica/RegistroVariable.h"

using namespace std;

class Heap
{
private:
    int tamanio;
    void CargarVectorYTransformarEnHeap(RegistroVariable vector[]);
    void ReestructurarHeap(RegistroVariable vectorAOrdenar[],int desde,int hasta);

public:
    Heap();
    ~Heap();

    void ordenar(RegistroVariable vectorAOrdenar[],int tamanio);

    int comparar_registros_variables(RegistroVariable reg1,RegistroVariable reg2);
};

#endif // HEAP_H
