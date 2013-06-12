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
//	Si primer bit de techo es 1 y primer bit de piso es 0 Y
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
	std::vector<bool> bits_a_emitir;
	while (hay_overflow())
	{
		// Emito el primer bit del techo
		bits_a_emitir.push_back(piso->test(PRECISION-1));
		resolver_overflow();

		//TODO: emitir tantos digitos negados como contadorUnderflow



		contadorUnderflow = 0;

		if (hay_underflow())
		{
			contadorUnderflow ++;
			resolver_underflow();
		}
	}

	return bits_a_emitir;
}


/*
 * eliminar los segundos dígitos de piso y techo,
 * correr los demás dígitos a la derecha.
 *
 */
void Intervalo::resolver_underflow()
{
	// quitar 2do bit techo, y agregarle un 1 al final

	std::bitset<PRECISION -1> techo_aux;
	techo_aux.set(PRECISION-2,techo->test(PRECISION-2));
	for (Uint i = PRECISION -3; i > 1; i--)
		techo_aux.set(i,techo->test(i-1));

	techo_aux.set(0,1);
	delete techo;
	techo = new std::bitset<PRECISION -1> (techo_aux);

	// quitar 2do bit piso y agregarle un 0 al final

	std::bitset<PRECISION> piso_aux;
	piso_aux.set(PRECISION-1,piso->test(PRECISION-1));
	for (Uint i = PRECISION -2; i > 1; i--)
		piso_aux.set(i,piso->test(i-1));

	piso_aux.set(0,0);
	delete piso;
	piso = new std::bitset<PRECISION> (piso_aux);

}

void Intervalo::resolver_overflow()
{
	// Agregar 1 al techo en la posicion 0 (la mas derecha)
	*techo <<= 1;
	techo->set(0,1);
	// Agregar 1 al piso en la posicion 0 (la mas derecha)
	*piso <<= 1;
	piso->set(0,0);
}
