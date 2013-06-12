/*
 * Intervalo.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: maine
 */

#include "Intervalo.h"

Intervalo::Intervalo() {

	std::string stringCeros, stringUnos;
	for (unsigned int i=0; i < PRECISION-1; i++)
	{
		stringCeros.push_back('0');
		stringUnos.push_back('1');
	}

	stringCeros.push_back('0');
	contadorUnderflow = 0;
	// El piso tendra 32 bits en cero
	piso = new std::bitset<PRECISION>(stringCeros);
	// El techo tendra 31 bits en uno (para evitar overflow al sumar piso y techo)
	techo = new std::bitset<PRECISION-1>(stringUnos);
	rango = 0;
	actualizar_rango();
}

Intervalo::~Intervalo() {
	delete piso;
	delete techo;
}

Uint Intervalo::get_piso()
{
	return (piso->to_ulong());
}

Uint Intervalo::get_techo()
{
	return (techo->to_ulong());
}

void Intervalo::actualizar_rango()
{
	unsigned int u_piso = piso->to_ulong();
	unsigned int u_techo = techo->to_ulong();
	rango = (u_techo - u_piso + 1);
}

char Intervalo::calcular_valor(char byteActual)
{
	char valor = '0';
	valor = (byteActual - piso->to_ulong()) / rango;
	return valor;
}

//
// Verificacion de overflow:
//	Cantidad de coincidiencias de los primeros N bits entre techo y piso. Esas coincidencias son las que se emiten.
//	Al emitir, se completa a byte de la siguiente manera:
//		- Techo: se agregan 1 por la derecha
//		- Piso:  se agregan 0 por la derecha
//	Ej: techo= "0011 1010" piso= "0000 1111", Se emite: "00"; techo queda: "1110 1011" (EB); piso queda: "0011 1100" (3C)
bool Intervalo::hay_overflow()const
{
	//WARNING Order positions are counted from the rightmost bit, which is order position 0.
	bool primerBitPiso = piso->test(PRECISION-1);
	bool primerBitTecho = techo->test(PRECISION-2);
	return (primerBitPiso == primerBitTecho);
}

//Verificacion de Underflow:
//	Si primer bit de techo es 1 y primer bit de piso es 0:
//		si el segundo bit de techo es 0 y el segundo bit de piso es 1
bool Intervalo::hay_underflow()const
{
	//WARNING Order positions are counted from the rightmost bit, which is order position 0.
	if (piso->test(PRECISION-1) == 0 && techo->test(PRECISION-2) == 1)
	{
		if (piso->test(PRECISION-2) == 1 && techo->test(PRECISION-3) == 0)
			return true;
	}
	return false;
}

void Intervalo::actualizar_piso_techo(Uint low_count, Uint high_count, Uint total)
{
	Uint uint_piso = piso->to_ulong() + rango * (low_count / total);
	Uint uint_techo =  piso->to_ulong() + rango * (high_count / total) - 1;

	delete (piso);
	piso = new std::bitset<PRECISION>(uint_piso);

	delete (techo);
	techo = new std::bitset<PRECISION-1>(uint_techo);
}

std::vector<bool> Intervalo::normalizar()
{
	// TODO: dentro de esta funcion hay que modificar el piso y el techo
	std::vector<bool> bits_a_emitir;
	while (hay_overflow())
	{
		bits_a_emitir.push_back(normalizar_overflow());
		if (hay_underflow())
		{
			normalizar_underflow();
		}
	}

	return bits_a_emitir;
}

void Intervalo::normalizar_underflow()
{
	//TODO
}

bool Intervalo::normalizar_overflow()
{
	bool bit_a_emitir = 0;
	//TODO
	return bit_a_emitir;
}
