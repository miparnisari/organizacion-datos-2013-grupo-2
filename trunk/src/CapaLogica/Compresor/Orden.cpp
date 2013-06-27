/*
 * Orden.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: nico
 */

#include "Orden.h"

Orden::Orden(){

}


Orden::~Orden()
{
	map<string, ModeloProbabilistico*>::iterator iterador = this->modelos.begin();
	while (iterador != this->modelos.end())
	{
		ModeloProbabilistico* unModelo = (*iterador).second;
		delete unModelo;
		++ iterador;
	}
}

map<string, ModeloProbabilistico*> Orden::get_mapa_modelos()
{
	return this->modelos;
}

int Orden::incrementar_frecuencia(long caracter, string nombreModelo)
{
	if(this->modelos.find(nombreModelo) == this->modelos.end()){
		return RES_ERROR;
	}

	ModeloProbabilistico* unModelo = this->modelos.at(nombreModelo);
	unModelo->incrementar_frecuencia(caracter);

	return RES_OK;
}

int Orden::devolver_modelo(string nombreModelo, ModeloProbabilistico** modeloADevolver)
{
	if (modelos.empty())
		return RES_ERROR;

	if(this->modelos.find(nombreModelo) == this->modelos.end())
		return RES_ERROR;

	(*modeloADevolver) = this->modelos.at(nombreModelo);
	return RES_OK;
}

int Orden::agregar_modelo(string nombreModelo, ModeloProbabilistico* modelo)
{
	if(modelo == NULL)
		return RES_ERROR;


	if(! modelos.empty() )
		if(this->modelos.find(nombreModelo) != this->modelos.end())
			return RES_ERROR;

	this->modelos.insert(pair<string, ModeloProbabilistico*>(nombreModelo, modelo));
	return RES_OK;
}

