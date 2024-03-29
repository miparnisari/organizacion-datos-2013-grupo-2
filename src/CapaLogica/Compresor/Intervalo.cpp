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
	piso = new std::bitset<PRECISION>(stringCeros);
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

Uint Intervalo::get_contador_underflow(){
	return contadorUnderflow;
}

void Intervalo::calcular_rango()
{
	unsigned int u_piso = piso->to_ulong();
	unsigned int u_techo = techo->to_ulong();
	rango = u_techo - u_piso;
	rango += 1;
}

/* Devuelve true si el primer bit de piso y techo son iguales.
 * Si son iguales se emitiran en el archivo. */
bool Intervalo::hay_overflow()const
{
	//WARNING Order positions are counted from the rightmost bit, which is order position 0.
	bool primerBitPiso = piso->test(PRECISION-1);
	bool primerBitTecho = techo->test(PRECISION-1);
	return (primerBitPiso == primerBitTecho);
}

/* Verificacion de Underflow:
Si primer bit de techo es 1 y primer bit de piso es 0 Y
si el segundo bit de techo es 0 y el segundo bit de piso es 1
 * */
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

/* Actualiza el piso y techo de acuerdo a:
 * PISO = floor ( piso + rango * low_count )
 * TECHO = floor (piso + rango * high_count ) + 1
 */
void Intervalo::actualizar_piso_techo(double low_count, double high_count)
{
	Ulonglong uint_piso = floor(piso->to_ulong() + rango * low_count);
	Ulonglong uint_techo = floor(piso->to_ulong() + rango * high_count) - 1;

	delete (piso);
	piso = new std::bitset<PRECISION>(uint_piso);

	delete (techo);
	techo = new std::bitset<PRECISION>(uint_techo);
}

/* Resuelve underflows y overflows y retorna lo que se debe emitir en archivo.
 * Devuelve en los parametros la cantidad de overflows y underflows
 * que se produjeron. */
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

		contadorUnderflow = 0;
	}

	while (hay_underflow())
	{
		cUnderflow++;

		contadorUnderflow ++;
		resolver_underflow();
	}


	return bits_a_emitir;
}

/*
 * Eliminar los segundos dígitos de piso y techo,
 * correr los demás dígitos a la derecha.
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
