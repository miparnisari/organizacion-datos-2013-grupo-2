#ifndef NODOINTERNO_H_
#define NODOINTERNO_H_

#include <iostream>
#include <vector>
#include "../ManejoArchivos/Clave.h"
#include "../../Constantes.h"
#include "../../CapaFisica/Bloque.h"
#include "../ManejoArchivos/ClaveX.h"

class NodoInterno {

	protected:
//		unsigned int espacioLibre;
//		unsigned int tamanio;
		unsigned int minCantidadBytesClaves;
		unsigned int maxCantidadBytesClaves;
		std::vector<ClaveX*> vectorClaves;
		std::vector<unsigned short> vectorHijos;

		ClaveX* get_clave(unsigned short numeroClave);
		virtual int get_hijo(unsigned short& hijo,unsigned short numeroHijo);
		ClaveX* get_clave_mitad();
		virtual int buscar_clave(const ClaveX* clave,unsigned short& posicionClave);
		virtual int get_hijo_izquierdo(unsigned short& hijo, ClaveX* clave);
		virtual int get_hijo_derecho(unsigned short& hijo,ClaveX* clave);
		virtual int remover_clave(const ClaveX* clave,unsigned short numeroClave);
		virtual int remover_clave(const ClaveX* clave);

	public:
		NodoInterno(unsigned int minCantidadBytesClaves, unsigned int maxCantidadBytesClaves);
		virtual ~NodoInterno();

		bool esta_vacio();

		unsigned short get_cantidad_claves();
		unsigned short get_cantidad_hijos();

		int empaquetar(Bloque* bloque); //TODO
		int desempaquetar(const Bloque* bloque); //TODO


};

#endif /* NODOINTERNO_H_ */
