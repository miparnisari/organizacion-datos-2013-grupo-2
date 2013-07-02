/*
 * TablaModelos.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: nico
 */

#include "TablaModelos.h"

TablaModelos::TablaModelos(){

}


TablaModelos::~TablaModelos()
{
	std::map<std::string, ModeloProbabilistico*>::iterator iterador = this->modelos.begin();
	while (iterador != this->modelos.end())
	{
		ModeloProbabilistico* unModelo = (*iterador).second;
		delete unModelo;
		++ iterador;
	}
}

std::map<std::string, ModeloProbabilistico*> TablaModelos::get_mapa_modelos()
{
	return this->modelos;
}

int TablaModelos::incrementar_frecuencia(long caracter, string nombreModelo)
{
	if(this->modelos.find(nombreModelo) == this->modelos.end()){
		return RES_ERROR;
	}

	ModeloProbabilistico* unModelo = this->modelos.at(nombreModelo);
	unModelo->incrementar_frecuencia(caracter);

	return RES_OK;
}

int TablaModelos::devolver_modelo(std::string nombreModelo, ModeloProbabilistico** modeloADevolver)
{
	if (modelos.empty())
		return RES_ERROR;

	if(this->modelos.find(nombreModelo) == this->modelos.end())
		return RES_ERROR;

	(*modeloADevolver) = this->modelos.at(nombreModelo);
	return RES_OK;
}

int TablaModelos::agregar_modelo(std::string nombreModelo, ModeloProbabilistico* modelo)
{
	if(modelo == NULL)
		return RES_ERROR;


	if(! modelos.empty() )
		if(this->modelos.find(nombreModelo) != this->modelos.end())
			return RES_ERROR;

	this->modelos.insert(std::pair<std::string, ModeloProbabilistico*>(nombreModelo, modelo));
	return RES_OK;
}

