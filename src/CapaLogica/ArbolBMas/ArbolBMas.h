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
		unsigned int numeroBloqueRaiz;
		unsigned int tamanioMaximoNodo;

		void _imprimir_recursivo(TipoHijo& numeroBloqueActual);

		int _obtener_nodo_arbol(unsigned int numeroNodoArbol,NodoArbol& nodo);

		int _obtener_nodo_interno(unsigned int numeroNodoInterno,NodoInterno& nodo);

		void _imprimir_recursivo(unsigned int numNodo);
		int _buscar_nodo_con_puntero(int punteroAbuscar);


		int _persistir_nodo(NodoArbol* nodo,unsigned int numeroNodo);


		/*version distinta del _quitar_recursivo.*/
		int _quitar_recursivo(unsigned int& numeroBloqueActual,RegistroClave& registroEliminar,
				int& tipoUnderflow);
		/*esta version trabaja con persistencias DE MAS por seguridad*/
		int _merge_secuenciales(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow,
				unsigned int numeroNodoHermanoUnderflow,bool numeroNodoUnderflowEsUltimoHijo ,
				unsigned short posicionClaveEliminar,unsigned short posicionHijoEliminar);
		/*combina dos nodos secuenciales. El nodo anteriormente en underflow es persistido, el nodo hermano es liberado. EL NODO PADRE
		 * NO ES PERSISTIDO*/
		int _resolver_underflow_hoja(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow);
		int _liberar_nodo(unsigned int numeroNodo);
		int _balancear_secuenciales(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow,
				unsigned int numeroNodoHermanoUnderflow,bool numeroNodoUnderflowEsUltimoHijo);
		/*balancea dos nodos secuenciales . Todos los nodos excepto el padre son persistidos*/
		int _resolver_underflow_interno(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow);
		int _merge_internos(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow,
				unsigned int numeroNodoHermanoUnderflow,bool numeroNodoUnderflowEsUltimoHijo);
		int _balancear_internos(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow,
				unsigned int numeroNodoHermanoUnderflow,bool numeroNodoUnderflowEsUltimoHijo);


	public:
		static const int FACTOR_CARGA= 50;

		ArbolBMas();
		ArbolBMas(const ArbolBMas& otro);
		virtual ~ArbolBMas();

		int crear (std::string fileName, unsigned int tamBloque = BLOQUE_TAM_DEFAULT);
		int eliminar (std::string fileName);

		int abrir (std::string fileName, std::string mode);
		int cerrar ();

		int agregar(RegistroClave & reg);
		int quitar(RegistroClave & reg);

		int buscar(RegistroClave & reg);
		int _buscar(RegistroClave & reg, unsigned int & numBloque);

		bool esta_vacio();

		unsigned int get_cant_minima_nodo();
		unsigned int get_cant_maxima_nodo();


		/*ESTOS METODOS DEBERIAN SER PRIVADOS, SE HACEN PUBLICOS PARA TESTEAR*/
		int _set_header();
		int _get_header();
		int _obtener_nodo_secuencial(int numNodoSecuencial,NodoSecuencial& nodoSecuencialActual);

		int _hallar_hijo_correspondiente(RegistroClave* registro,
				NodoInterno* nodoBuscar,TipoHijo& numeroBloqueHijoCorrespondiente);
		/*dado un nodoInterno y un registro, se busca el siguiente hijo a explorar*/
		int _insertar_recursivo(unsigned int& numeroBloqueActual ,
				 RegistroClave* registro , TipoHijo& hijoPromocionado,ClaveX* clavePromocionada);
		int _hallar_hoja(RegistroClave* registro,unsigned int& numeroBloque);
		/*retorna el numero de bloque donde el registro 'registro se encuentra .
		 * En caso de no hallarlo retorna RES_ERROR'*/
		int _insertar_recursivo_hoja(Bloque* bloqueActual ,
				RegistroClave* registro , TipoHijo& hijoPromocionado);

		int _split_hoja(NodoSecuencial* nodoActual,vector<RegistroClave>* registrosOverflow,
				TipoHijo& hijoPromocionado,ClaveX* clavePromocionada);
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

		void obtener_primer_nodo_secuencial(TipoHijo& numeroPrimerNodo);
		/*guarda en el parametro nodo el primer nodo secuencial*/
};

#endif /* ARBOLBMAS_H */ 
