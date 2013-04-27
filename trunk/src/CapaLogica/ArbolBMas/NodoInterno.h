#ifndef NODOINTERNO_H_
#define NODOINTERNO_H_

#include <iostream>
#include <list>
#include "../ManejoArchivos/Clave.h"
#include "../../Constantes.h"

using std::list;

class NodoInterno {

	protected:
		unsigned int espacioLibre;
		unsigned int tamanio;
		list<Clave*> listaClaves;
		list<unsigned short> listaHijos;

		virtual int get_clave(Clave* clave,unsigned short numeroClave);
		/*recupera la clave en la posicion numeroClave*/

		virtual int get_hijo(unsigned short& hijo,unsigned short numeroHijo);
		/*recupera el hijo en la posicion numeroHijo y se guarda en la variable hijo.*/

		virtual int get_clave_mitad(Clave* clave);
		/*recupera la clave del medio, en caso de no haber claves retorna RES_ERROR. La
		 * clave recuperada la retorna en el puntero clave*/

		virtual int buscar_clave(Clave* clave,unsigned short& posicionClave);
		/*busca una clave y retorna su posicion en posicionClave. En caso de no hallarla
		 * retorna RES_ERROR*/

		virtual int get_hijo_izquierdo(unsigned short& hijo,Clave* clave);
		/*recupera el hijo izquierdo de una clave y lo guarda en la variable hijo*/

		virtual int get_hijo_derecho(unsigned short& hijo,Clave* clave);
		/*recupera el hijo derecho de una clave*/

		virtual int remover_clave(Clave* clave,unsigned short numeroClave);
		/*remueve la clave en la posicion numeroClave y la retorna en el puntero clave.*/

		virtual int remover_clave(Clave* clave);
		/*remueve la clave que sea igual a clave*/

	public:
		NodoInterno(unsigned int t= BLOQUE_TAM_DEFAULT);
		virtual ~NodoInterno();

		bool esta_vacio();

		unsigned short get_cantidad_claves();
		unsigned short get_cantidad_hijos();


};

#endif /* NODOINTERNO_H_ */
