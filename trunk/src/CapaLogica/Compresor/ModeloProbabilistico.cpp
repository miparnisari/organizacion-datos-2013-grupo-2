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

ModeloProbabilistico& ModeloProbabilistico::operator = (const ModeloProbabilistico& otro)
{
	if (this != &otro)
	{
		delete[] frecuenciasSimbolos;
		this->tamanioAlfabeto = otro.tamanioAlfabeto;
		this->frecuenciasSimbolos = new Uint[tamanioAlfabeto]();
		memcpy(frecuenciasSimbolos,otro.frecuenciasSimbolos,tamanioAlfabeto);
	}
	return (*this);
}


ModeloProbabilistico::ModeloProbabilistico(const ModeloProbabilistico& otro){

	this->tamanioAlfabeto = otro.tamanioAlfabeto;
	this->frecuenciasSimbolos = new Uint[tamanioAlfabeto]();
	memcpy(frecuenciasSimbolos,otro.frecuenciasSimbolos,tamanioAlfabeto);

}


Uint ModeloProbabilistico::get_tamanio_alfabeto(){

	return this->tamanioAlfabeto;

}

void ModeloProbabilistico::inicializar_frecuencias_en_1()
{
	for (unsigned int i = 0; i < tamanioAlfabeto; i++)
		*(frecuenciasSimbolos+i) = 1;
}

void ModeloProbabilistico::incrementar_frecuencia (const Uint simbolo)
{
	Uint posicionSimbolo = simbolo;
	*(frecuenciasSimbolos+posicionSimbolo) += 1;
}


/*
 *  SIMBOLO  |||  FRECUENCIA   ||| LOW_COUNT ||| HIGH_COUNT
 *    A                4              0              4
 *    B                2              4              6
 *    C                1              6              7
 *    D                1              7              8
 */
double ModeloProbabilistico::calcular_low_count (const Uint simbolo)
{
	Uint posicionSimbolo = simbolo;
//	IMPRIMIR_MY_VARIABLE((char)simbolo);
	Uint contador = 0;
	for (Uint i = 0; i < posicionSimbolo; i++)
		contador += *(frecuenciasSimbolos+i);

	double totalFrecuencias= (double)(calcular_total_frecuencias());
	double contadorD= (double)(contador);
	double resultado= (double)(contadorD / totalFrecuencias);
	return resultado;
}

/*
 *  SIMBOLO  |||  FRECUENCIA   ||| LOW_COUNT ||| HIGH_COUNT
 *    A                4              0              4
 *    B                2              4              6
 *    C                1              6              7
 *    D                1              7              8
 */
double ModeloProbabilistico::calcular_high_count (const Uint simbolo)
{
	Uint posicionSimbolo = simbolo;
	Uint contador = 0;
	for (Uint i = 0; i < posicionSimbolo + 1; i++)
		contador += *(frecuenciasSimbolos+i);

	double totalFrecuencias= (double)(calcular_total_frecuencias());
	double contadorD= (double)(contador);
	double resultado= (double)(contadorD / totalFrecuencias);
	return resultado;
}

Uint ModeloProbabilistico::obtener_simbolo(double probabilidad)
{
	for (Uint i = 0; i < tamanioAlfabeto; i++)
	{
		if (calcular_low_count(i) <= probabilidad && probabilidad <= calcular_high_count(i))
			return i;
	}

	return tamanioAlfabeto ;
}

Uint ModeloProbabilistico::calcular_total_frecuencias()
{
	Uint total = 0;
	for (Uint i = 0; i < tamanioAlfabeto; i++)
		total += *(frecuenciasSimbolos + i);

	return total;
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

Uint ModeloProbabilistico::get_frecuencia(Uint simbolo)
{
	return frecuenciasSimbolos[simbolo];
}

void ModeloProbabilistico::resetear()
{
	for(Uint i=0;i<tamanioAlfabeto;i++)
		if( this->frecuenciasSimbolos[i]!= 0 )
			this->frecuenciasSimbolos[i] = 1;
}
