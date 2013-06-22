/*
 * Contexto.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: nico
 */

#include "Contexto.h"

Contexto::Contexto(){

}

Contexto::~Contexto()
{
	map<string, ModeloProbabilistico*>::iterator iterador = this->listaDeModelos.begin();
	while (iterador != this->listaDeModelos.end())
	{
		ModeloProbabilistico* unModelo = (*iterador).second;
		delete unModelo;
		iterador ++;
	}
}

int Contexto::incrementar_frecuencia(long caracter, string nombreModelo)
{
	if(this->listaDeModelos.find(nombreModelo) == this->listaDeModelos.end()){
		return RES_ERROR;
	}

	ModeloProbabilistico* unModelo = this->listaDeModelos.at(nombreModelo);
	unModelo->incrementar_frecuencia(caracter);

	return RES_OK;
}

int Contexto::devolver_modelo(string nombreModelo, ModeloProbabilistico** modeloADevolver)
{
	if(this->listaDeModelos.find(nombreModelo) == this->listaDeModelos.end()){
		return RES_ERROR;
	}
	*modeloADevolver = this->listaDeModelos.at(nombreModelo);
	return RES_OK;
}

void Contexto::agregar_modelo(string nombreModelo, ModeloProbabilistico* modelo)
{
//	ModeloProbabilistico* nuevoModelo = new ModeloProbabilistico(TAMANIO_ALFABETO);
//	vector<unsigned short> vectorDeProbabilidadesINiciales;
//	//vectorDeProbabilidadesINiciales.push_back(VALOR_DEL_ESCAPE);
//
//	//nuevoModelo->inicializar_frecuencias_en_1(vectorDeProbabilidadesINiciales);
//
//	nuevoModelo->incrementar_frecuencia(VALOR_DEL_ESCAPE);
	this->listaDeModelos.insert(pair<string, ModeloProbabilistico*>(nombreModelo, modelo));
}
