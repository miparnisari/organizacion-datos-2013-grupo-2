#ifndef NODOINTERNO_H_
#define NODOINTERNO_H_

#include <iostream>
#include <vector>
#include "../ManejoArchivos/Clave.h"
#include "../../Constantes.h"
#include "../../CapaFisica/Bloque.h"

class NodoInterno {

	protected:
		unsigned int espacioLibre;
		unsigned int tamanio;
		std::vector<Clave*> vectorClaves;
		std::vector<unsigned short> vectorHijos;

		Clave* get_clave(unsigned short numeroClave);
		virtual int get_hijo(unsigned short& hijo,unsigned short numeroHijo);
		Clave* get_clave_mitad();
		virtual int buscar_clave(const Clave* clave,unsigned short& posicionClave);
		virtual int get_hijo_izquierdo(unsigned short& hijo, Clave* clave);
		virtual int get_hijo_derecho(unsigned short& hijo,Clave* clave);
		virtual int remover_clave(const Clave* clave,unsigned short numeroClave);
		virtual int remover_clave(const Clave* clave);

	public:
		NodoInterno(unsigned int tamanioNodo);
		virtual ~NodoInterno();

		bool esta_vacio();

		unsigned short get_cantidad_claves();
		unsigned short get_cantidad_hijos();

		int empaquetar(Bloque* bloque); //TODO
		int desempaquetar(const Bloque* bloque); //TODO


};

#endif /* NODOINTERNO_H_ */
