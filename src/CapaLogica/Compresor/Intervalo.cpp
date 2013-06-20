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
	// El piso tendra 32 bits en cero
	piso = new std::bitset<PRECISION>(stringCeros);
	// El techo tendra 32 bits en uno
	techo = new std::bitset<PRECISION>(stringUnos);
	rango = 0;
	calcular_rango();
}

Intervalo::~Intervalo() {
	delete piso;
	delete techo;
}


void Intervalo::set_piso(std::string unPiso)
{
	if (unPiso.length() != PRECISION)
	{
		std::cout << "Error de longitud." << std::endl;
		return;
	}

	delete(piso);
	piso = new std::bitset<PRECISION>(unPiso);

}
void Intervalo::set_techo(std::string unTecho)
{
	if (unTecho.length() != PRECISION)
	{
		std::cout << "Error de longitud." << std::endl;
		return;
	}

	delete(techo);
	techo = new std::bitset<PRECISION>(unTecho);
}

BitsPiso& Intervalo::get_piso()
{
	return (*piso);
}

BitsTecho& Intervalo::get_techo()
{
	return (*techo);
}

Ulonglong Intervalo::get_rango()
{
	return (rango);
}

void Intervalo::calcular_rango()
{
	unsigned int u_piso = piso->to_ulong();
	unsigned int u_techo = techo->to_ulong();
	std::cout << rango << std::endl;
	rango = u_techo - u_piso;
	rango += 1;
	std::cout << rango << std::endl;
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
	bool primerBitTecho = techo->test(PRECISION-1);
	return (primerBitPiso == primerBitTecho);
}

//Verificacion de Underflow:
//	Si primer bit de techo es 1 y primer bit de piso es 0 Y
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

void Intervalo::actualizar_piso_techo(double low_count, double high_count)
{
	Ulonglong uint_piso = floor(piso->to_ulong() + rango * low_count);
	Ulonglong uint_techo = floor(piso->to_ulong() + rango * high_count) - 1;

	delete (piso);
	piso = new std::bitset<PRECISION>(uint_piso);

	delete (techo);
	techo = new std::bitset<PRECISION>(uint_techo);
}

std::vector<bool> Intervalo::normalizar( Byte& cOverflow,Byte& cUnderflow )
{
	std::vector<bool> bits_a_emitir;
	cOverflow= cUnderflow= 0;

	while (hay_overflow())
	{
		// Emito el primer bit del techo
		bool bitAemitir = piso->test(PRECISION-1);
		bits_a_emitir.push_back(bitAemitir);
		resolver_overflow();

		//emitir tantos digitos negados como contadorUnderflow
		for (Uint i = 0; i < contadorUnderflow; i++)
		{
			bits_a_emitir.push_back(! bitAemitir);
		}

		cOverflow++;
		/*todo revisar si es correcto*/

		contadorUnderflow = 0;
	}

	while (hay_underflow())
	{
		cUnderflow++;
		/*todo revisar si es correcto*/

		contadorUnderflow ++;
		resolver_underflow();
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

	std::bitset<PRECISION> techo_aux;
	techo_aux.set(PRECISION-1,techo->test(PRECISION-1));
	for (Uint i = PRECISION -2; i > 0; i--)
		techo_aux.set(i,techo->test(i-1));

	techo_aux.set(0,1);
	delete techo;
	techo = new std::bitset<PRECISION> (techo_aux);

	// quitar 2do bit piso y agregarle un 0 al final

	std::bitset<PRECISION> piso_aux;
	piso_aux.set(PRECISION-1,piso->test(PRECISION-1));
	for (Uint i = PRECISION -2; i > 0; i--)
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
	// Agregar 0 al piso en la posicion 0 (la mas derecha)
	*piso <<= 1;
	piso->set(0,0);
}
