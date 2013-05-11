
#include "Heap.h"

Heap::Heap()
{
    tamanio=0;
}

Heap::~Heap()
{

}

void Heap::ReestructurarHeap(RegistroClave vectorAOrdenar[],int desde,int hasta)
{
    int hijoIzq=2*desde+1;
    int hijoDer=2*desde+2;

    int posicionMax=desde;

    if((vectorAOrdenar[hijoIzq]>vectorAOrdenar[posicionMax])&&(hijoIzq<=hasta))
    {
        posicionMax=hijoIzq;
    }

    if((vectorAOrdenar[hijoDer]>vectorAOrdenar[posicionMax])&&(hijoDer<=hasta))
    {
        posicionMax=hijoDer;
    }

    if(posicionMax!=desde)
    {
        RegistroClave auxIntercambio=vectorAOrdenar[posicionMax];
        vectorAOrdenar[posicionMax]=vectorAOrdenar[desde];
        vectorAOrdenar[desde]=auxIntercambio;

        ReestructurarHeap(vectorAOrdenar,posicionMax,hasta);
    }

}


void Heap::CargarVectorYTransformarEnHeap(RegistroClave vectorACargar[])
{
    int UltimoConHijo=(tamanio-2)/2;

    for(int raiz=UltimoConHijo; raiz>=0; raiz--)
    {
        this->ReestructurarHeap(vectorACargar,raiz,tamanio-1);
    }
}


void Heap::AgregarAlHeap(RegistroClave vectorAOrdenar[], RegistroClave valor_a_agregar)
{
    bool cambio=false;//inicializo variables necesarias
    int posInicial=tamanio;

    vectorAOrdenar[posInicial]=valor_a_agregar;//agrego al final
    int padre=(posInicial-1)/2;//busco el padre
    if(vectorAOrdenar[padre]<vectorAOrdenar[posInicial])
    {
        //intercambio
        RegistroClave auxIntercambio=vectorAOrdenar[padre];
        vectorAOrdenar[padre]=vectorAOrdenar[posInicial];
        vectorAOrdenar[posInicial]=auxIntercambio;

        cambio=true;
        posInicial=padre;//ahora el valor agregado esta donde estaba el padre
        padre=(posInicial-1)/2;//busco el nuevo padre
    }

    while (cambio)//mientras pueda tener que seguir subiendo...
    {
        cambio=false;
        if(vectorAOrdenar[padre]<vectorAOrdenar[posInicial])
        {
            //intercambio
            RegistroClave auxIntercambio=vectorAOrdenar[padre];
            vectorAOrdenar[padre]=vectorAOrdenar[posInicial];
            vectorAOrdenar[posInicial]=auxIntercambio;

            cambio=true;
            posInicial=padre;//ahora el valor agregado esta donde estaba el padre
            padre=(posInicial-1)/2;//busco el nuevo padre
        }
    }

    tamanio++;//aumento tamanio total del vector
}

void Heap::ordenar(RegistroClave vectorAOrdenar[],int tamanio)
{

    this->tamanio=tamanio;

    CargarVectorYTransformarEnHeap(vectorAOrdenar);

    RegistroClave auxIntercambio=vectorAOrdenar[tamanio-1];
    vectorAOrdenar[tamanio-1]=vectorAOrdenar[0];
    vectorAOrdenar[0]=auxIntercambio;

    ReestructurarHeap(vectorAOrdenar,0,tamanio-2);
    tamanio--;
}

