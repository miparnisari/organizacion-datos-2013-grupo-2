/*
 * ModeloProbabilistico.cpp
 *
 *  Created on: 12/06/2013
 *      Author: Betty
 */

#include "ModeloProbabilistico.h"

ModeloProbabilistico::ModeloProbabilistico(Uint un_tamanio_alfabeto)
{
	tamanioAlfabeto = un_tamanio_alfabeto;
	frecuenciasSimbolos = new Uint[tamanioAlfabeto]();
	totalFrecuencias = 0;
}

ModeloProbabilistico::~ModeloProbabilistico()
{
	delete[] frecuenciasSimbolos;
}

ModeloProbabilistico::ModeloProbabilistico(const ModeloProbabilistico& otro)
{
	this->tamanioAlfabeto = otro.tamanioAlfabeto;
	this->totalFrecuencias = otro.totalFrecuencias;
	this->frecuenciasSimbolos = new Uint[tamanioAlfabeto]();
	memcpy(frecuenciasSimbolos,otro.frecuenciasSimbolos,tamanioAlfabeto * sizeof(Uint));
}

bool ModeloProbabilistico::todas_frecuencias_en_uno()const
{
	bool todas_en_uno = true;
	for (Uint i = 0; i < tamanioAlfabeto; i++)
	{
		if (frecuenciasSimbolos[i] != 1)
		{
			todas_en_uno = false;
		}
	}
	return todas_en_uno;
}

std::string ModeloProbabilistico::imprimir()const
{
	stringstream res;
	for (Uint i = 0; i < tamanioAlfabeto; i++)
	{
		if (frecuenciasSimbolos[i] != 0)
		{
			res << "char = " << i << ", freq = ";
			res << utilitarios::int_a_string(frecuenciasSimbolos[i]);
			res << ".";
		}
	}
	res << '\n';
	return res.str();
}

ModeloProbabilistico& ModeloProbabilistico::operator = (const ModeloProbabilistico& otro)
{
	if (this != &otro)
	{
		delete[] frecuenciasSimbolos;
		this->tamanioAlfabeto = otro.tamanioAlfabeto;
		this->totalFrecuencias = otro.totalFrecuencias;
		this->frecuenciasSimbolos = new Uint[tamanioAlfabeto]();
		memcpy(frecuenciasSimbolos,otro.frecuenciasSimbolos,tamanioAlfabeto * sizeof(Uint));
	}
	return (*this);
}

void ModeloProbabilistico::resetear()
{
	totalFrecuencias = 0;
	for(Uint i=0;i<tamanioAlfabeto;i++)
	{
		if( this->frecuenciasSimbolos[i] != 0 )
		{
			this->frecuenciasSimbolos[i] = 1;
			totalFrecuencias ++;
		}
	}
}

/*
 * PRECAUCION al utilizar este metodo.
 * Comparar doubles por igualdad no siempre funciona bien.
 */
double ModeloProbabilistico::get_probabilidad(Uint simbolo)const
{
	double high = calcular_high_count(simbolo);
	double low = calcular_low_count(simbolo);
	return (high - low);
}

Uint ModeloProbabilistico::get_tamanio_alfabeto()const
{

	return this->tamanioAlfabeto;

}

Uint ModeloProbabilistico::get_frecuencia(Uint simbolo)const
{
	return frecuenciasSimbolos[simbolo];
}

void ModeloProbabilistico::inicializar_frecuencias_en_1(vector<unsigned short>& vector){

	const unsigned short TAMANIO_V= vector.size();

	if(TAMANIO_V== 0)
		return;

	totalFrecuencias = TAMANIO_V;

	for(unsigned int i=0;i<tamanioAlfabeto;i++)
		this->frecuenciasSimbolos[i]= 0;

	for(unsigned short i=0;i<TAMANIO_V;i++)
		this->frecuenciasSimbolos[vector.at(i)]= 1;

}

void ModeloProbabilistico::inicializar_frecuencias_en_1()
{
	for (unsigned int i = 0; i < tamanioAlfabeto; i++)
		frecuenciasSimbolos[i]= 1;

	totalFrecuencias = tamanioAlfabeto;
}

void ModeloProbabilistico::incrementar_frecuencia (const Uint simbolo)
{
	Uint posicionSimbolo = simbolo;
	frecuenciasSimbolos [posicionSimbolo] += 1;
	totalFrecuencias ++;
}

double ModeloProbabilistico::calcular_low_count (const Uint simbolo)const
{
	Uint posicionSimbolo = simbolo;
	double contador = 0;
	for (Uint i = 0; i < posicionSimbolo; i++)
		contador += frecuenciasSimbolos[i];

	double totalFrecuencias= (double)(calcular_total_frecuencias());
	double low_count= (double)(contador / totalFrecuencias);
	return low_count;
}

double ModeloProbabilistico::calcular_high_count (const Uint simbolo)const
{
	Uint posicionSimbolo = simbolo;
	double contador = 0;
	for (Uint i = 0; i < posicionSimbolo + 1; i++)
		contador += frecuenciasSimbolos[i];

	double totalFrecuencias= (double)(calcular_total_frecuencias());
	double high_count= (double)(contador / totalFrecuencias);
	return high_count;
}

Uint ModeloProbabilistico::obtener_simbolo(double probabilidad)const
{
	for (Uint simbolo = 0; simbolo < tamanioAlfabeto; simbolo++)
	{
		if (calcular_low_count(simbolo) <= probabilidad && probabilidad <= calcular_high_count(simbolo))
			return simbolo;
	}

	return tamanioAlfabeto; //ultimo simbolo
}

Uint ModeloProbabilistico::calcular_total_frecuencias()const
{
	return totalFrecuencias;
}
