#ifndef NODOINTERNO_H_
#define NODOINTERNO_H_

#include "NodoArbol.h"
#include <cmath>

class NodoInterno : public NodoArbol {

	public:
		typedef vector<ClaveX>::iterator IteradorVectorClaves;
		//typedef unsigned short TipoHijo;
		typedef unsigned int TipoHijo;

	protected:

		unsigned int maxCantidadBytes;
		unsigned int minCantidadBytes;
		unsigned int cantidadBytesOcupados;
		/*cantidadBytesOcupados NO TOMA EN CUENTA tamanio de metadatos de registros
		 * variables a la hora de empaquetar tampoco el tamanio de tipoNodo ...
		 * seria sensato que al momento de crear un NodoInterno, el parametro de
		 * maxCantidadBytes sea determinado a partir del tamanio de Bloque donde
		 * sera empaquetado en el futuro.*/
		std::vector<ClaveX> vectorClaves;
		std::vector<TipoHijo> vectorHijos;



	public:
		const static unsigned short POSICION_FIN_VECTOR_HIJOS= USHRT_MAX;
		const static unsigned int MINIMA_CANTIDAD_BYTES_DEFAULT = 0;
		const static unsigned int MAXIMA_CANTIDAD_BYTES_DEFAULT = 4096;//4k


		NodoInterno(unsigned int minCantidadBytes= MINIMA_CANTIDAD_BYTES_DEFAULT,
				unsigned int maxCantidadBytes= MAXIMA_CANTIDAD_BYTES_DEFAULT);
		virtual ~NodoInterno();

		bool esta_vacio()const;
		int get_tamanio_ocupado();

		unsigned short get_cantidad_claves();
		unsigned short get_cantidad_hijos();
		int set_hijo_izquierdo(const ClaveX& clave, TipoHijo valor);
		int set_hijo_derecho(const ClaveX& clave, TipoHijo valor);
		virtual int get_hijo_izquierdo(TipoHijo& hijo,const ClaveX& clave);
		virtual int get_hijo_derecho(TipoHijo& hijo,const ClaveX& clave);
		/*se retornar los hijos izquierdo y derecho respectivamente a partir de una
		 * clave.*/


		int get_clave(unsigned short numeroClave,ClaveX& clave);
		virtual int get_hijo(TipoHijo& hijo,unsigned short numeroHijo);
		/*se recupera el hijo en la posicion numeroHijo y se guarda en el parametro
		 * 'hijo'*/
		int get_clave_mitad(ClaveX& clave);
		virtual int buscar_clave(const ClaveX& clave,unsigned short& posicionClave);
		/*se busca la posicion donde se encuentra la clave 'clave' y su valor es devuelto
		 * en el parametro 'posicionClave'*/

		/*METODOS AGREGADOS POR MARTIN -----------------------------------------------*/

		int insertar_clave(const ClaveX& clave,
				unsigned short& posicionOcurrenciaInsercion);
		/*se inserta una clave, se retorna la posicion donde se inserto
		 * en el parametro posicionInsercion . Se retorna RES_OVERFLOW,
		 * en caso que despues de la insersion el nodo quede en estado de OVERFLOW ,
		 * de lo contrario RES_OK */
		int insertar_hijo(TipoHijo hijo,
				unsigned short posicion= POSICION_FIN_VECTOR_HIJOS);
		/*se inserta un hijo 'hijo' en la posicion 'posicion' . en caso de que no se
		 * especifique el parametro posicion, el hijo se agrega al final . En caso que
		 * despues de la insersion el nodo quede en estado OVERFLOW, se retorna
		 * RES_OVERFLOW, en caso contrario se retorna RES_OK . Este metodo puede acoplarse
		 * con insertar_clave tal que despues de insertar clave se usa el parametro
		 * posicionInsercion para agregar el correspondiente hijo derecho' */
		int modificar_hijo(TipoHijo valor , unsigned short numeroHijo);
		/*el hijo en la posicion 'numeroHijo' es modificado por el valor 'valor'.
		 * en caso de pasar un numeroHijo invalido se retorna RES_ERROR */
		int remover_clave( const ClaveX& clave, unsigned short& posicionEliminacion );
		/*elimina la clave 'clave' del nodo interno, se retorna su posicion de eliminacion
		 * en la variable posicionEliminacion . En caso q el nodo quede en UNDERFLOW, se
		 * retrorna RES_UNDERFLOW*/
		int remover_clave(unsigned short numeroClave , ClaveX& clave);
		/*se remueve la clave en la posicion numeroClave, su valor es devuelto en la variable clave.
		 * Retorna RES_UNDERFLOW en caso que el nodo quede en estado de underflow*/
		int remover_hijo(unsigned short numeroHijo);
		/*se remueve el hijo en la posicion numeroHijo. en caso de pasar un parametro
		 * invalido se retorna RES_ERROR . En caso que el nodo interno quede en estado
		 * underflow se retorna RES_UNDERFLOW.*/
		bool es_hoja(){return false;}
		bool es_interno(){return true;}
		virtual bool hay_overflow()const;
		virtual bool hay_underflow()const;


		void imprimir();
		void imprimir_claves()const;
		/*imprime todas las claves guardadas en el nodo. En caso que el mismo
		 * este vacio se imprime NODO_VACIO.*/
		void imprimir_hijos()const;

		int insertar_hijo_derecho(const ClaveX& clave,TipoHijo valor= HIJO_INVALIDO);
		int insertar_hijo_izquierdo(const ClaveX& clave,TipoHijo valor);

		int modificar_hijo_derecho(const ClaveX&,TipoHijo valor );
		int modificar_hijo_izquierdo(const ClaveX&,TipoHijo valor);

		void limpiar();
		/*vacia los vectores de claves e hijos y resetea el espacio usado*/

		int buscar_hijo(TipoHijo valorHijo,unsigned short& posicionHijo);
		int obtener_hijo_siguiente_a(TipoHijo valorHijo,int& hijoSiguiente);




		/*METODOS AGREGADOS POR MARTIN -----------------------------------------------*/



		int empaquetar(Bloque* bloque);
		int desempaquetar(const Bloque* bloque);


};

#endif /* NODOINTERNO_H_ */
