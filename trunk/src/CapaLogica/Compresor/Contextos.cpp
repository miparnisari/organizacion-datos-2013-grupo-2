/*
 * Contexto.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: nico
 */

#include "Contextos.h"

Contextos::Contextos(){

}

Contextos::~Contextos()
{
	map<string, ModeloProbabilistico*>::iterator iterador = this->listaDeModelos.begin();
	while (iterador != this->listaDeModelos.end())
	{
		ModeloProbabilistico* unModelo = (*iterador).second;
		delete unModelo;
		iterador ++;
	}
}

int Contextos::incrementar_frecuencia(long caracter, string nombreModelo)
{
	if(this->listaDeModelos.find(nombreModelo) == this->listaDeModelos.end()){
		cout << "No pude incrementar frecuencia de " << caracter << " en modelo " << nombreModelo << endl;
		return RES_ERROR;
	}

	ModeloProbabilistico* unModelo = this->listaDeModelos.at(nombreModelo);
	unModelo->incrementar_frecuencia(caracter);

	return RES_OK;
}

int Contextos::devolver_modelo(string nombreModelo, ModeloProbabilistico** modeloADevolver)
{
	if (listaDeModelos.size() == 0)
		return RES_ERROR;

	if(this->listaDeModelos.find(nombreModelo) == this->listaDeModelos.end())
		return RES_ERROR;

	(*modeloADevolver) = this->listaDeModelos.at(nombreModelo);
	return RES_OK;
}

int Contextos::agregar_modelo(string nombreModelo, ModeloProbabilistico* modelo)
{
	if(modelo == NULL)
		return RES_ERROR;

	if( listaDeModelos.size()!=0 )
	if(this->listaDeModelos.find(nombreModelo) != this->listaDeModelos.end())
		return RES_ERROR;

	this->listaDeModelos.insert(pair<string, ModeloProbabilistico*>(nombreModelo, modelo));
	return RES_OK;
}

map<string, ModeloProbabilistico*> Contextos::get_mapa_modelos()
{
	return this->listaDeModelos;
}
