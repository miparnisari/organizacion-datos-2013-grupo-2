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
#include <cmath>
#include <iostream>

typedef unsigned int Uint;
typedef unsigned long long Ulonglong;
typedef std::bitset<PRECISION> BitsPiso;
typedef std::bitset<PRECISION> BitsTecho;

class Intervalo {

	private:
		Uint contadorUnderflow;
		Ulonglong rango; // 8 bytes
		BitsPiso* piso;
		BitsTecho* techo;

	public:
		Intervalo();
		virtual ~Intervalo();

		void set_piso(std::string unPiso);
		void set_techo(std::string unTecho);

		BitsPiso& get_piso();
		BitsTecho& get_techo();
		Ulonglong get_rango();

		std::vector<bool> normalizar();
		/*resuelve underflows y overflows y retorna lo que se debe emitir en archivo*/
		void resolver_underflow();
		void resolver_overflow();

		bool hay_overflow() const;
		bool hay_underflow() const;

		char calcular_valor(char byteActual);
		/*retorna el mensaje correspondiente al valor leido "byteActual" */

		void calcular_rango();
		void actualizar_piso_techo(double low_count, double high_count);
};

#endif /* INTERVALO_H_ */
