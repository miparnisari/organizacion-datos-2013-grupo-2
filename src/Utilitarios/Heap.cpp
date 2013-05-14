/*
 * TestNodoSecuencial.cpp
 *
 *  Created on: May 11, 2013
 *      Author: juan
 */

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
    if (hijoIzq<=hasta)
    	if((vectorAOrdenar[hijoIzq]>vectorAOrdenar[posicionMax]))
    	{
    		posicionMax=hijoIzq;
    	}

    if(hijoDer<=hasta)
    	if((vectorAOrdenar[hijoDer]>vectorAOrdenar[posicionMax]))
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

void Heap::ordenar(RegistroClave vectorAOrdenar[],int tamanio)
{

    this->tamanio=tamanio;

    CargarVectorYTransformarEnHeap(vectorAOrdenar);

    while (tamanio>0)
    {
    	RegistroClave auxIntercambio=vectorAOrdenar[tamanio-1];
    	vectorAOrdenar[tamanio-1]=vectorAOrdenar[0];
    	vectorAOrdenar[0]=auxIntercambio;

    	ReestructurarHeap(vectorAOrdenar,0,tamanio-2);
    	tamanio--;
    }
}
