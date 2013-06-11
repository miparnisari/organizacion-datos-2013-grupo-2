/*
 * Intervalo.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: maine
 */

#include "Intervalo.h"

Intervalo::Intervalo() {

	std::string stringCeros, stringUnos;
	for (unsigned int i=0; i < PRECISION; i++)
	{
		stringCeros.push_back('0');
		stringUnos.push_back('1');
	}

	contadorUnderflow = 0;
	piso = new std::bitset<PRECISION>(stringCeros); // El piso tendra 32 bits en cero
	techo = new std::bitset<PRECISION>(stringUnos); // El techo tendra 32 bits en uno
	rango = calcular_rango(*piso, *techo); //
}

Intervalo::~Intervalo() {
	delete piso;
	delete techo;
}

Uint Intervalo::calcular_rango(BitsPiso & piso, BitsTecho & techo)
{
	unsigned int u_piso;
	unsigned int u_techo;
	u_piso = piso.to_ulong();
	u_techo = techo.to_ulong();
	return u_techo - u_piso + 1;
}/* Rango = techo - piso + 1 */

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
	bool primerBitTecho = techo->test(PRECISION-1);
	return (primerBitPiso == primerBitTecho);
}

//Verificacion de Underflow:
//	Si primer bit de techo es 1 y primer bit de piso es 0:
//		si el segundo bit de techo es 0 y el segundo bit de piso es 1
bool Intervalo::hay_underflow()const
{
	//WARNING Order positions are counted from the rightmost bit, which is order position 0.
	if (piso->test(PRECISION-1) == 0 && techo->test(PRECISION-1) == 1)
	{
		if (piso->test(PRECISION-2) == 1 && techo->test(PRECISION-2) == 0)
			return true;
	}
	return false;
}
