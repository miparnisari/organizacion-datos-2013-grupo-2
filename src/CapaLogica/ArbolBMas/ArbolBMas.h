#ifndef ARBOLBMAS_H
#define ARBOLBMAS_H

#include <string>
#include "../../Constantes.h"
#include "../../CapaFisica/RegistroVariable.h"
#include "../../CapaFisica/ManejadorBloques.h"
#include "../../CapaFisica/Bloque.h"
#include "../ManejoArchivos/RegistroClave.h"
#include "NodoInterno.h"
#include "NodoSecuencial.h"

typedef struct header_arbol
{
	unsigned int minCantBytesClaves;
	unsigned int maxCantBytesClaves;
}header_arbol;

class ArbolBMas
{
	public:
		typedef NodoInterno::TipoHijo TipoHijo;

	private:
		header_arbol header;
		ManejadorBloques archivoNodos;
		NodoSecuencial* raiz;
		unsigned int numeroBloqueRaiz;
		unsigned int tamanioMaximoNodo;
		unsigned int tamanioBloque;

		int _obtener_nodo_interno(unsigned int numeroNodoInterno,NodoInterno& nodo);

		int _obtener_nodo_secuencial(int numNodoSecuencial,NodoSecuencial& nodoSecuencialActual);
		void _imprimir_recursivo(unsigned int numNodo);
		int _buscar_nodo_con_puntero(int punteroAbuscar);
		int _quitar_recursivo(unsigned int& numNodoActual, RegistroClave & registro, int & huboUnderflow);
		int _resolver_underflow_hoja(unsigned int numBloqueUnderflow, unsigned int numBloqueHermano,
				int numBloqueIzquierdo,bool esUltimo,unsigned int numNodoPadre);
		int _merge_nodos_secuenciales(NodoSecuencial & nodoUnderflow, NodoSecuencial & hermano);

		int persistir(NodoArbol* nodo,unsigned int numeroNodo);
		int _balancear_secuenciales(NodoSecuencial& uno,NodoSecuencial& otro,
				NodoInterno& nodoPadre);
		//el balanceo se produce desde el nodo 'uno' a 'otro' pasando registros en ese orden. El nodoPadre se modifica pertinentemente.
		//se pasan registros de uno a otro y la clave de nodoPadre es actualizada.

		int _balancear_internos(NodoInterno& uno,NodoInterno& otro,NodoInterno& padre);
		//se rotan las claves de uno -> padre -> otro como sea necesario.

		int _resolver_underflow_interno();


	public:
		static const int FACTOR_CARGA= 50;

		ArbolBMas();
		virtual ~ArbolBMas();

		int crear (std::string fileName, unsigned int tamBloque = BLOQUE_TAM_DEFAULT);
		int eliminar (std::string fileName);

		int abrir (std::string fileName, std::string mode);
		int cerrar ();

		int agregar(RegistroClave & reg);
		int quitar(RegistroClave & reg);//FIXME el parametro no deberia ser un ClaveX?
		int buscar(RegistroClave & reg, unsigned int & numBloque);//FIXME el parametro no deberia ser un ClaveX?
		/* Devuelve el numero de nodo secuencial donde esta o deberia estar reg. */


		bool esta_vacio();

		unsigned int get_cant_minima_nodo();
		unsigned int get_cant_maxima_nodo();


		/*ESTOS METODOS DEBERIAN SER PRIVADOS, SE HACEN PUBLICOS PARA TESTEAR*/
		int _set_header();
		int _get_header();

		int _hallar_hijo_correspondiente(RegistroClave* registro,
				NodoInterno* nodoBuscar,TipoHijo& numeroBloqueHijoCorrespondiente);
		/*dado un nodoInterno y un registro, se busca el siguiente hijo a explorar*/
		int _insertar_recursivo(unsigned int& numeroBloqueActual ,
				 RegistroClave* registro , TipoHijo& hijoPromocionado,ClaveX* clavePromocionada,TipoPuntero& nodoSecuencialSiguiente);
		int _hallar_hoja(RegistroClave* registro,unsigned int& numeroBloque);
		/*retorna el numero de bloque donde el registro 'registro se encuentra .
		 * En caso de no hallarlo retorna RES_ERROR'*/
		int _insertar_recursivo_hoja(Bloque* bloqueActual ,
				RegistroClave* registro , TipoHijo& hijoPromocionado);

		int _split_hoja(NodoSecuencial* nodoActual,vector<RegistroClave>* registrosOverflow,
				TipoHijo& hijoPromocionado,ClaveX* clavePromocionada,TipoPuntero nodoSecuencialSiguiente);
		/* > a partir de un NodoSecuencial en estado de overflow, se crea un NodoSecuencial
		 * nuevo donde se guardaran la mitad superior de las claves de nodoActual.
		 * > En registro se guardara el RegistroClave promocionado
 	 	   > en hijoPromocionado se guardara el numero de bloque del nodo NUEVO
 	 	   > seria conveniente que este metodo se encargue de guardar en el archivo de nodos el nuevo Bloque*/
		int _split_interno(NodoInterno* nodo,ClaveX* clavePromocionada,
				TipoHijo& hijoPromocionado);
		/* > a partir de un NodoInterno en estado de overflow, se crea un NodoInterno nuevo donde se
		 * guardan las claves e hijos de la mitad derecha de nodo .
		 * > el nodo nuevo es persistido en el archivo de bloques de nodos
		 * > el numero de bloque del nodo nuevo es guardado en el parametro hijoPromocionado*/


		void imprimir();
		void _imprimir_recursivo(TipoHijo& numeroBloqueActual);
		void obtener_primer_nodo_secuencial(TipoHijo& numeroPrimerNodo);
		/*guarda en el parametro nodo el primer nodo secuencial*/
};

#endif /* ARBOLBMAS_H */ 
