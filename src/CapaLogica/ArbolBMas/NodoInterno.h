#ifndef NODOINTERNO_H_
#define NODOINTERNO_H_

#include "Nodo.h"

class NodoInterno : public Nodo {

	public:
		typedef vector<ClaveX>::iterator IteradorVectorClaves;

	protected:

		unsigned int maxCantidadBytes;
		unsigned int minCantidadBytes;
		unsigned int cantidadBytesOcupados;
		std::vector<ClaveX> vectorClaves;
		std::vector<unsigned short> vectorHijos;

		ClaveX* get_clave(unsigned short numeroClave);
		virtual int get_hijo(unsigned short& hijo,unsigned short numeroHijo);
		ClaveX* get_clave_mitad();
		virtual int buscar_clave(const ClaveX* clave,unsigned short& posicionClave);
		virtual int get_hijo_izquierdo(unsigned short& hijo, ClaveX* clave);
		virtual int get_hijo_derecho(unsigned short& hijo,ClaveX* clave);
		virtual int remover_clave(const ClaveX* clave,unsigned short numeroClave);
		virtual int remover_clave(const ClaveX* clave);
		virtual int _insertar_clave( ClaveX* claveInsertar ,
				ClaveX& claveMitad , unsigned short& posicionInsercion);
		/*inserta una clave en el nodo . Si ocurre overflow, la clave del medio es
		 * expulsada y guardada en ClaveMitad*/

		virtual int _insertar_si_overflow(ClaveX* claveInsertar,unsigned short& posicionInsertar ,
				unsigned short& posicionClavePromocionar);
		virtual bool _hay_overflow();
		virtual bool _hay_underflow();


	public:
		NodoInterno(unsigned int minCantidadBytesClaves, unsigned int maxCantidadBytesClaves);
		virtual ~NodoInterno();

		bool esta_vacio();
		int agregar_clave(ClaveX* clave,
				ClaveX* clavePromocion,
				std::vector<Clave> clavesSplit,
				std::vector<unsigned short> hijosClaveSplit);
		int get_tamanio_ocupado();

		unsigned short get_cantidad_claves();
		unsigned short get_cantidad_hijos();
		int set_hijo_izquierdo(ClaveX* clave, unsigned short valor);
		int set_hijo_derecho(ClaveX* clave, unsigned short valor);

		int empaquetar(Bloque* bloque); //TODO
		int desempaquetar(const Bloque* bloque); //TODO


};

#endif /* NODOINTERNO_H_ */
