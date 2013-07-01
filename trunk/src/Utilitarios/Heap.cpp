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

void Heap::_reestructurar_heap(RegistroVariable vectorAOrdenar[],int desde,int hasta)
{
    int hijoIzq=2*desde+1;
    int hijoDer=2*desde+2;

    int posicionMax=desde;
    if (hijoIzq<=hasta)
    	if(comparar_registros_variables(vectorAOrdenar[hijoIzq],vectorAOrdenar[posicionMax])==-1)
    	{
    		posicionMax=hijoIzq;
    	}

    if(hijoDer<=hasta)
    	if( comparar_registros_variables(vectorAOrdenar[hijoDer],vectorAOrdenar[posicionMax])==-1)
    	{
    		posicionMax=hijoDer;
    	}

    if(posicionMax!=desde)
    {
    	RegistroVariable auxIntercambio=vectorAOrdenar[posicionMax];
        vectorAOrdenar[posicionMax]=vectorAOrdenar[desde];
        vectorAOrdenar[desde]=auxIntercambio;

        _reestructurar_heap(vectorAOrdenar,posicionMax,hasta);
    }
}

void Heap::transformar_en_heap(RegistroVariable vectorAOrdenar[],int un_tamanio)
{
    this->tamanio=un_tamanio;

    int UltimoConHijo=(un_tamanio-2)/2;

    for(int raiz=UltimoConHijo; raiz>=0; raiz--)
    {
        this->_reestructurar_heap(vectorAOrdenar,raiz,un_tamanio-1);
    }
}

int Heap::comparar_registros_variables(RegistroVariable reg1,RegistroVariable reg2)
{
	int campo1Int = -10;
	reg1.recuperar_campo((char*)&campo1Int,0);

	int campo2Int = -10;
	reg2.recuperar_campo((char*)&campo2Int,0);

	int resPrimerCampo= campo1Int - campo2Int;

	if (resPrimerCampo==0)
	{
			if (reg1.get_cantidad_campos()>=2)
			{

					int campo3Int;
					reg1.recuperar_campo((char*)&campo3Int,1);

					int campo4Int;
					reg2.recuperar_campo((char*)&campo4Int,1);

					int resSegundoCampo= campo3Int - campo4Int;

					if (resSegundoCampo==0)
					{
							return 0;
					}else if (resSegundoCampo>0)
					{
							return 1;
					}else{
							return -1;
					}
			}else
			{
					return 0;
			}
	}else{
			if (resPrimerCampo>0)
			{
					return 1;
			}else
			{
					return -1;
			}
	}
}
