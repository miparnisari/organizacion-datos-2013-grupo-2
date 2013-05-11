#ifndef HEAP_H
#define HEAP_H

#include <iostream>
//cambiar estos include de *******
#include "C:\Users\Juan\workspace\TpDatos\organizacion-datos-2013-grupo-2\src\CapaLogica\ManejoArchivos\RegistroClave.h"
#include "C:\Users\Juan\workspace\TpDatos\organizacion-datos-2013-grupo-2\src\CapaFisica\RegistroVariable.h"

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

    void AgregarAlHeap(RegistroClave vectorAOrdenar[], RegistroClave valor_a_agregar);
    void ordenar(RegistroClave vectorAOrdenar[],int tamanio);
};

#endif // HEAP_H
