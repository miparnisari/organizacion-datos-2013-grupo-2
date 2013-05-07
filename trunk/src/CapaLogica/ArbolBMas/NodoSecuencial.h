#ifndef NODOSECUENCIAL_H_
#define NODOSECUENCIAL_H_

#include "Nodo.h"

typedef int TipoPuntero;

class NodoSecuencial : public Nodo {

	private:
		// En memoria
		unsigned int minCantidadBytesOcupados;
		unsigned int maxCantidadBytesOcupados;
		unsigned int bytesOcupados;

		// Al disco y en memoria
		std::vector<RegistroClave > vectorRegistros;
		TipoPuntero proximoNodo;

		void __resolver_overflow(std::vector<RegistroClave>& regsOverflow);
		void __resolver_underflow(std::vector<RegistroClave>& regsUnderflow);

	public:
		NodoSecuencial(unsigned int minBytesOcupados, unsigned int maxBytesOcupados);
		virtual ~NodoSecuencial();

		int insertar(RegistroClave* registro, std::vector<RegistroClave> & regsOveflow);
		int eliminar(ClaveX* clave, std::vector<RegistroClave>& regsUnderflow);
		int buscar(RegistroClave* regDevuelto);

		int empaquetar(Bloque* bloque);
		int desempaquetar(const Bloque*);

		TipoPuntero get_proximo_nodo();
		void set_proximo_nodo(TipoPuntero p_prox);

		unsigned int get_cantidad_registros();

		bool esta_vacio();
};

#endif /* NODOSECUENCIAL_H_ */
