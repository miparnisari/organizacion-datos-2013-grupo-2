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
#include <vector>
#include "../../Constantes.h"

typedef unsigned int Uint;
typedef std::bitset<PRECISION> BitsPiso;
typedef std::bitset<PRECISION-1> BitsTecho;

class Intervalo {

	private:
		Uint contadorUnderflow;
		Uint rango;
		BitsPiso* piso;
		BitsTecho* techo;

	public:
		Intervalo();
		virtual ~Intervalo();

		Uint get_piso();
		Uint get_techo();

		std::vector<bool> normalizar();
		void resolver_underflow();
		void resolver_overflow();

		bool hay_overflow() const;
		bool hay_underflow() const;

		char calcular_valor(char byteActual);

		void actualizar_rango();
		void actualizar_piso_techo(Uint low_count, Uint high_count, Uint total);
};

#endif /* INTERVALO_H_ */
