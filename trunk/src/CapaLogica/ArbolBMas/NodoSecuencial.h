#ifndef NODOSECUENCIAL_H_
#define NODOSECUENCIAL_H_

#include "NodoArbol.h"

typedef int TipoPuntero;

class NodoSecuencial : public NodoArbol {

	private:
		// En memoria
		unsigned int minCantidadBytesOcupados;
		unsigned int maxCantidadBytesOcupados;
		unsigned int bytesOcupados;

		// Al disco y en memoria
		std::vector<RegistroClave > vectorRegistros;
		TipoPuntero proximoNodo;

		void __resolver_overflow(std::vector<RegistroClave>& regsOverflow);
		/* Devuelve en un vector los registros que se sacaron del nodo. */
		void __resolver_underflow(std::vector<RegistroClave>& regsUnderflow);
		/* Devuelve en un vector los registros que se sacaron del nodo. */
		void __limpiar();

	public:
		NodoSecuencial(unsigned int minBytesOcupados, unsigned int maxBytesOcupados);
		virtual ~NodoSecuencial();

		void set_proximo_nodo(TipoPuntero p_prox);

		int insertar(const RegistroClave & registro, std::vector<RegistroClave> & regsOveflow);
		/* Inserta un registro en el nodo. Si ya estaba, devuelve RES_RECORD_EXISTS.
		Si no estaba, lo inserta. Si al insertar hubo overflow, devuelve RES_OVERFLOW.
		Si no, devuelve RES_OK.
		Si hubo error, devuelve RES_ERROR. */

		int eliminar(const ClaveX & clave, std::vector<RegistroClave>& regsUnderflow);
		/* Elimina una clave del Nodo.
		 * Si se produce underflow al borrar, se eliminan los registros restantes del Nodo,
		 * y se los guarda en la variable "regsUnderflow".
		 */
		int buscar(const ClaveX & claveBuscada, RegistroClave** regDevuelto);

		int empaquetar(Bloque* bloque);
		int desempaquetar(const Bloque*);

		unsigned int get_bytes_ocupados();
		TipoPuntero get_proximo_nodo();
		unsigned int get_cantidad_registros();
		bool esta_vacio();
};

#endif /* NODOSECUENCIAL_H_ */
