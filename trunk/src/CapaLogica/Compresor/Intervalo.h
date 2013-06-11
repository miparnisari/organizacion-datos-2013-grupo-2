/*
 * Intervalo.h
 *
 *  Created on: Jun 10, 2013
 *      Author: maine
 */

#ifndef INTERVALO_H_
#define INTERVALO_H_
#include <bitset>
#include <string>
#include "../../Constantes.h"

typedef unsigned int Uint;
typedef std::bitset<PRECISION> BitsPiso, BitsTecho;

class Intervalo {

	private:
		Uint contadorUnderflow;
		Uint rango;
		BitsPiso* piso;
		BitsTecho* techo;

	public:
		Intervalo();
		virtual ~Intervalo();


		void normalizar_underflow();
		void normalizar_overflow();
		bool hay_overflow() const;
		bool hay_underflow() const;
		Uint calcular_rango(BitsPiso & piso, BitsTecho & techo);
		// TODO hacerlas private


		std::bitset<PRECISION> normalizar();
};

#endif /* INTERVALO_H_ */
