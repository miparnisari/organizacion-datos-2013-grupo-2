/*
 * TablaOrdenes.cpp
 *
 *  Created on: Jul 2, 2013
 *      Author: maine
 */

#include "TablaOrdenes.h"

TablaOrdenes::TablaOrdenes(int cantidadOrdenes)
{
	cantidad_ordenes = cantidadOrdenes;
	inicializar();
}

TablaOrdenes::~TablaOrdenes()
{
	limpiar_ordenes();
}

void TablaOrdenes::inicializar()
{
	_inicializar_orden_menosuno();
	_inicializar_orden_cero();

	// ORDEN 1 a "orden"
	for (unsigned short i = 1; i < cantidad_ordenes+1; i++)
	{
		TablaModelos* contexto_vacio = new TablaModelos();
		mapa_ordenes.insert(pair<int,TablaModelos*> (i,contexto_vacio));
	}
}

void TablaOrdenes::_inicializar_orden_menosuno()
{
	ModeloProbabilistico* mod_menos_uno = new ModeloProbabilistico(TAMANIO_ALFABETO-1);
	mod_menos_uno->inicializar_frecuencias_en_1();

	TablaModelos* contexto_menos_uno = new TablaModelos();
	contexto_menos_uno->agregar_modelo("-1",mod_menos_uno);
	mapa_ordenes.insert(pair<int,TablaModelos*> (-1,contexto_menos_uno));
}

void TablaOrdenes::_inicializar_orden_cero()
{
	ModeloProbabilistico* mod_cero = new ModeloProbabilistico(TAMANIO_ALFABETO);
	vector<unsigned short> vector;
	vector.push_back(VALOR_DEL_ESCAPE);
	mod_cero->inicializar_frecuencias_en_1(vector);

	TablaModelos* ctx_cero = new TablaModelos();
	ctx_cero->agregar_modelo("0",mod_cero);
	mapa_ordenes.insert(pair<int,TablaModelos*> (0,ctx_cero));
}

void TablaOrdenes::imprimir()
{
	stringstream salida;

	for (int orden = -1; orden < cantidad_ordenes + 1; orden++)
	{
		salida << '\n' << "---------------- ORDEN " << orden << " ----------------" << '\n';
		map<string, ModeloProbabilistico*> mapa_modelos = mapa_ordenes[orden]->get_mapa_modelos();
		map<string, ModeloProbabilistico*>::iterator iterador = mapa_modelos.begin();
		while (iterador != mapa_modelos.end())
		{
			salida << "~~~~~~ CONTEXTO " << (*iterador).first << " ~~~~~~" << '\n';
			ModeloProbabilistico* modelo = (*iterador).second;
			salida <<'\t';
			if (orden == -1 )
			{
				if (modelo->todas_frecuencias_en_uno())
				{
					salida << "Todas las frecuencias valen 1. ";
				} else {
					salida << "ERROR! Hay una frecuencia que no es 1! ";
				}
			}
			else
				salida << modelo->imprimir();
			++ iterador;
		}
	}

	salida <<'\n';
	Logger::info("PPMC",salida.str());
}

void TablaOrdenes::limpiar_ordenes()
{
	for (int orden = -1; orden < cantidad_ordenes + 1; orden++)
	{
		delete mapa_ordenes[orden];
	}
	mapa_ordenes.clear();
}

void TablaOrdenes::crear_modelo (int orden, string nombre_modelo)
{
	ModeloProbabilistico* modelo = new ModeloProbabilistico(TAMANIO_ALFABETO);
	std::vector<unsigned short> vector;
	vector.push_back(VALOR_DEL_ESCAPE);
	modelo->inicializar_frecuencias_en_1(vector);

	TablaModelos* contexto = mapa_ordenes[orden];
	contexto->agregar_modelo(nombre_modelo,modelo);
	mapa_ordenes[orden] = contexto;
}

void TablaOrdenes::actualizar_modelo(int orden, Uint simbolo, string contexto_del_simbolo)
{
	// Si el orden es -1 no se debe actualizar nada
	if (orden == -1)
		return;

	ModeloProbabilistico* modelo = NULL;
	mapa_ordenes[orden]->devolver_modelo(contexto_del_simbolo,&modelo);

	if ((modelo->get_frecuencia(VALOR_DEL_ESCAPE)==1) && (modelo->calcular_total_frecuencias()==1)) {
		// Si solo existia el escape, solo aumento la frecuencia del simbolo
	}
	else if (modelo->get_frecuencia(simbolo) >= 1)
	{
		// Si el simbolo tenia frecuencia >= 1, solo aumento la frecuencia del simbolo
	}
	else{
		mapa_ordenes[orden]->incrementar_frecuencia(VALOR_DEL_ESCAPE,contexto_del_simbolo);
	}

	mapa_ordenes[orden]->incrementar_frecuencia(simbolo,contexto_del_simbolo);


	if (orden != cantidad_ordenes)
	{
		// En el contexto superior, creamos el contexto "contexto + simbolo"
		// solo con el ESCAPE

		string ctx = "";
		if ((orden != -1) && (orden != 0))
			ctx += contexto_del_simbolo;
		ctx += simbolo;

		ModeloProbabilistico* modelo_contexto_superior = NULL;
		int res = mapa_ordenes[orden+1]->devolver_modelo(ctx,&modelo_contexto_superior);

		if (res == RES_ERROR)
		{
			crear_modelo(orden+1,ctx);
		}
	}

}

ModeloProbabilistico* TablaOrdenes::devolver_modelo(int orden, string& contexto)
{
	ModeloProbabilistico* modelo_actual = NULL;
	mapa_ordenes[orden]->devolver_modelo(contexto,&modelo_actual);
	return modelo_actual;
}
