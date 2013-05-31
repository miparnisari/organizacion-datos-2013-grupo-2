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

void Heap::ReestructurarHeap(RegistroVariable vectorAOrdenar[],int desde,int hasta)
{
    int hijoIzq=2*desde+1;
    int hijoDer=2*desde+2;

    int posicionMax=desde;
    if (hijoIzq<=hasta)
    	if(comparar_registros_variables(vectorAOrdenar[hijoIzq],vectorAOrdenar[posicionMax])==1)
    	{
    		posicionMax=hijoIzq;
    	}

    if(hijoDer<=hasta)
    	if( comparar_registros_variables(vectorAOrdenar[hijoDer],vectorAOrdenar[posicionMax])==1)
    	{
    		posicionMax=hijoDer;
    	}

    if(posicionMax!=desde)
    {
    	RegistroVariable auxIntercambio=vectorAOrdenar[posicionMax];
        vectorAOrdenar[posicionMax]=vectorAOrdenar[desde];
        vectorAOrdenar[desde]=auxIntercambio;

        ReestructurarHeap(vectorAOrdenar,posicionMax,hasta);
    }
}


void Heap::CargarVectorYTransformarEnHeap(RegistroVariable vectorACargar[])
{
    int UltimoConHijo=(tamanio-2)/2;

    for(int raiz=UltimoConHijo; raiz>=0; raiz--)
    {
        this->ReestructurarHeap(vectorACargar,raiz,tamanio-1);
    }
}

void Heap::ordenar(RegistroVariable vectorAOrdenar[],int tamanio)
{

    this->tamanio=tamanio;

    CargarVectorYTransformarEnHeap(vectorAOrdenar);

    while (tamanio>0)
    {
    	RegistroVariable auxIntercambio=vectorAOrdenar[tamanio-1];
    	vectorAOrdenar[tamanio-1]=vectorAOrdenar[0];
    	vectorAOrdenar[0]=auxIntercambio;

    	ReestructurarHeap(vectorAOrdenar,0,tamanio-2);
    	tamanio--;
    }
}

int Heap::comparar_registros_variables(RegistroVariable reg1,RegistroVariable reg2)
{
	char* campo1Reg1 = new char[reg1.get_tamanio_campo(0)+1];
	reg1.recuperar_campo(campo1Reg1,0);
	campo1Reg1[reg1.get_tamanio_campo(0)]='\0';
	string campo1Reg1Str=campo1Reg1;


	char* campo1Reg2 = new char[reg2.get_tamanio_campo(0)+1];
	reg2.recuperar_campo(campo1Reg2,0);
	campo1Reg2[reg2.get_tamanio_campo(0)]='\0';
	string campo1Reg2Str=campo1Reg2;

	int resPrimerCampo= campo1Reg1Str.compare(campo1Reg2Str);

	resPrimerCampo= strcmp(campo1Reg1,campo1Reg2);

	if (campo1Reg1Str==campo1Reg2Str)
	{
		if (reg1.get_cantidad_campos()>=2)
		{
			char* campo2Reg1 = new char[reg1.get_tamanio_campo(1)];
			reg1.recuperar_campo(campo2Reg1,1);
			string campo2Reg1Str=campo2Reg1;

			char* campo2Reg2 = new char[reg2.get_tamanio_campo(1)];
			reg2.recuperar_campo(campo2Reg2,1);
			string campo2Reg2Str=campo2Reg2;

			if (campo2Reg1Str==campo2Reg2Str)
			{
				delete[] campo2Reg1;
				delete[] campo2Reg2;
				return 0;
			}else if (campo2Reg1Str>campo2Reg2Str)
			{
				delete[] campo2Reg1;
				delete[] campo2Reg2;
				return 1;
			}else{
				delete[] campo2Reg1;
				delete[] campo2Reg2;
				return -1;
			}

		}else
		{
			delete[] campo1Reg1;
			delete[] campo1Reg2;
			return 0;
		}
	}else{
		if (campo1Reg1Str>campo1Reg2Str)
		{
			delete[] campo1Reg1;
			delete[] campo1Reg2;
			return 1;
		}else
		{
			delete[] campo1Reg1;
			delete[] campo1Reg2;
			return -1;
		}
	}
}
