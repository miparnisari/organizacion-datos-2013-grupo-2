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
		resto_modelos.insert(pair<int,TablaModelos*> (i,contexto_vacio));
	}
}

void TablaOrdenes::_inicializar_orden_menosuno()
{
	modelo_menos_uno = new ModeloProbabilistico(TAMANIO_ALFABETO-1);
	modelo_menos_uno->inicializar_frecuencias_en_1();
}

void TablaOrdenes::_inicializar_orden_cero()
{
	modelo_cero = new ModeloProbabilistico(TAMANIO_ALFABETO);
	vector<unsigned short> vector;
	vector.push_back(VALOR_DEL_ESCAPE);
	modelo_cero->inicializar_frecuencias_en_1(vector);
}

void TablaOrdenes::imprimir()
{
	stringstream salida;

	if (modelo_menos_uno->todas_frecuencias_en_uno())
	{
		salida << "ORDEN -1: Todas las frecuencias valen 1. ";
	} else {
		salida << "ORDEN -1: ERROR! Hay una frecuencia que no es 1! ";
	}

	salida << '\n' << "---------------- ORDEN 0 ----------------" << '\n';
	salida << '\t' << modelo_cero->imprimir();

	for (int orden = 1; orden < cantidad_ordenes + 1; orden++)
	{
		salida << '\n' << "---------------- ORDEN " << orden << " ----------------" << '\n';
		map<string, ModeloProbabilistico*> mapa_modelos = resto_modelos[orden]->get_mapa_modelos();
		map<string, ModeloProbabilistico*>::iterator iterador = mapa_modelos.begin();
		while (iterador != mapa_modelos.end())
		{
			salida << "~~~~~~ CONTEXTO " << (*iterador).first << " ~~~~~~" << '\n';
			ModeloProbabilistico* modelo = (*iterador).second;
			salida << '\t' << modelo->imprimir();
			++ iterador;
		}
	}

	salida <<'\n';
	Logger::info("PPMC",salida.str());
}

void TablaOrdenes::limpiar_ordenes()
{
	delete modelo_menos_uno;
	delete modelo_cero;
	for (int orden = 1; orden < cantidad_ordenes + 1; orden++)
	{
		delete resto_modelos[orden];
	}
	resto_modelos.clear();
}

void TablaOrdenes::crear_modelo (int orden, string nombre_modelo)
{
	assert(orden >= 1);
	ModeloProbabilistico* modelo = new ModeloProbabilistico(TAMANIO_ALFABETO);
	std::vector<unsigned short> vector;
	vector.push_back(VALOR_DEL_ESCAPE);
	modelo->inicializar_frecuencias_en_1(vector);

	TablaModelos* contexto = resto_modelos[orden];
	contexto->agregar_modelo(nombre_modelo,modelo);
	resto_modelos[orden] = contexto;
}

void TablaOrdenes::actualizar_modelo(int orden, Uint simbolo, string contexto_del_simbolo)
{
	// Si el orden es -1 no se debe actualizar nada
	if (orden == -1)
		return;


	// Actualizo el modelo actual en el contexto actual
	ModeloProbabilistico* modelo = NULL;
	if (orden == 0)
		modelo = modelo_cero;
	else
		resto_modelos[orden]->devolver_modelo(contexto_del_simbolo,&modelo);

	if ((modelo->get_frecuencia(VALOR_DEL_ESCAPE)==1) && (modelo->calcular_total_frecuencias()==1)) {
		// Si solo existia el escape, solo aumento la frecuencia del simbolo
	}
	else if (modelo->get_frecuencia(simbolo) >= 1)
	{
		// Si el simbolo tenia frecuencia >= 1, solo aumento la frecuencia del simbolo
	}
	else{
		if (orden == 0)
			modelo_cero->incrementar_frecuencia(VALOR_DEL_ESCAPE);
		else
			resto_modelos[orden]->incrementar_frecuencia(VALOR_DEL_ESCAPE,contexto_del_simbolo);
	}


	if (orden == 0)
		modelo_cero->incrementar_frecuencia(simbolo);
	else
		resto_modelos[orden]->incrementar_frecuencia(simbolo,contexto_del_simbolo);


	// En el contexto superior, creamos el contexto "contexto + simbolo"
	// solo con el ESCAPE

	if (orden != cantidad_ordenes)
	{

		string ctx = "";
		if (orden != 0)
			ctx += contexto_del_simbolo;
		ctx += simbolo;

		ModeloProbabilistico* modelo_contexto_superior = NULL;
		int res = resto_modelos[orden+1]->devolver_modelo(ctx,&modelo_contexto_superior);

		if (res == RES_ERROR)
		{
			crear_modelo(orden+1,ctx);
		}
	}

}

ModeloProbabilistico* TablaOrdenes::devolver_modelo(int orden, string& contexto)
{
	if (orden == -1)
		return modelo_menos_uno;

	if (orden == 0)
		return modelo_cero;

	ModeloProbabilistico* modelo_actual = NULL;
	resto_modelos[orden]->devolver_modelo(contexto,&modelo_actual);
	return modelo_actual;
}
