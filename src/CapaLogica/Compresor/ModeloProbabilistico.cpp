/*
 * ModeloProbabilistico.cpp
 *
 *  Created on: 12/06/2013
 *      Author: Betty
 */

#include "ModeloProbabilistico.h"

ModeloProbabilistico::ModeloProbabilistico(Uint un_tamanio_alfabeto) {
	tamanioAlfabeto = un_tamanio_alfabeto;
	frecuenciasSimbolos = new Uint[tamanioAlfabeto]();
}

ModeloProbabilistico::~ModeloProbabilistico() {
	delete[] frecuenciasSimbolos;
}

ModeloProbabilistico::ModeloProbabilistico(const ModeloProbabilistico& otro){

	this->tamanioAlfabeto = otro.tamanioAlfabeto;
	this->frecuenciasSimbolos = new Uint[tamanioAlfabeto]();
//	memcpy(frecuenciasSimbolos,otro.frecuenciasSimbolos,tamanioAlfabeto);

	for (Uint i = 0; i < otro.tamanioAlfabeto; i++)
		this->frecuenciasSimbolos[i] = otro.frecuenciasSimbolos[i];

}

void ModeloProbabilistico::imprimir(ofstream* archivoSalida)
{
	for (Uint i = 0; i < tamanioAlfabeto; i++)
	{
		if (frecuenciasSimbolos[i] != 0)
			(*archivoSalida) << "i " << i << " = " <<frecuenciasSimbolos[i] << ";";
	}
	(*archivoSalida) << endl;
}

ModeloProbabilistico& ModeloProbabilistico::operator = (const ModeloProbabilistico& otro)
{
	if (this != &otro)
	{
		delete[] frecuenciasSimbolos;
		this->tamanioAlfabeto = otro.tamanioAlfabeto;
		this->frecuenciasSimbolos = new Uint[tamanioAlfabeto]();
		for (Uint i = 0; i < otro.tamanioAlfabeto; i++)
			this->frecuenciasSimbolos[i] = otro.frecuenciasSimbolos[i];
//		memcpy(frecuenciasSimbolos,otro.frecuenciasSimbolos,tamanioAlfabeto);
	}
	return (*this);
}

void ModeloProbabilistico::resetear()
{
	for(Uint i=0;i<tamanioAlfabeto;i++)
	{
		if( this->frecuenciasSimbolos[i] != 0 )
			this->frecuenciasSimbolos[i] = 1;
	}
}

/*
 * PRECAUCION al utilizar este metodo.
 * Comparar doubles por igualdad no siempre funciona bien.
 */
double ModeloProbabilistico::get_probabilidad(Uint simbolo)
{
	double high = calcular_high_count(simbolo);
	double low = calcular_low_count(simbolo);
	return (high - low);
}

Uint ModeloProbabilistico::get_tamanio_alfabeto(){

	return this->tamanioAlfabeto;

}

Uint ModeloProbabilistico::get_frecuencia(Uint simbolo)
{
	return frecuenciasSimbolos[simbolo];
}

void ModeloProbabilistico::inicializar_frecuencias_en_1(vector<unsigned short>& v){

	const unsigned short TAMANIO_V= v.size();
	if(TAMANIO_V== 0)
		return;

	for(unsigned int i=0;i<tamanioAlfabeto;i++)
		this->frecuenciasSimbolos[i]= 0;

	for(unsigned short i=0;i<TAMANIO_V;i++){
		unsigned short p= v.at(i);
		this->frecuenciasSimbolos[p]= 1;
	}

}

void ModeloProbabilistico::inicializar_frecuencias_en_1()
{
	for (unsigned int i = 0; i < tamanioAlfabeto; i++)
		frecuenciasSimbolos[i]= 1;
}

void ModeloProbabilistico::incrementar_frecuencia (const Uint simbolo)
{
	Uint posicionSimbolo = simbolo;
	frecuenciasSimbolos [posicionSimbolo] += 1;
}

double ModeloProbabilistico::calcular_low_count (const Uint simbolo)
{
	Uint posicionSimbolo = simbolo;
	double contador = 0;
	for (Uint i = 0; i < posicionSimbolo; i++)
		contador += frecuenciasSimbolos[i];

	double totalFrecuencias= (double)(calcular_total_frecuencias());
	double low_count= (double)(contador / totalFrecuencias);
	return low_count;
}

double ModeloProbabilistico::calcular_high_count (const Uint simbolo)
{
	Uint posicionSimbolo = simbolo;
	double contador = 0;
	for (Uint i = 0; i < posicionSimbolo + 1; i++)
		contador += frecuenciasSimbolos[i];

	double totalFrecuencias= (double)(calcular_total_frecuencias());
	double high_count= (double)(contador / totalFrecuencias);
	return high_count;
}

Uint ModeloProbabilistico::obtener_simbolo(double probabilidad)
{
	for (Uint simbolo = 0; simbolo < tamanioAlfabeto; simbolo++)
	{
		if (calcular_low_count(simbolo) <= probabilidad && probabilidad <= calcular_high_count(simbolo))
			return simbolo;
	}

	return tamanioAlfabeto; //ultimo simbolo
}

Uint ModeloProbabilistico::calcular_total_frecuencias()
{
	Uint total = 0;
	for (Uint i = 0; i < tamanioAlfabeto; i++)
		total += frecuenciasSimbolos[i];

	return total;
}
