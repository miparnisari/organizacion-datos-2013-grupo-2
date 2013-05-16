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
		NodoInterno* raiz;
		unsigned int numeroBloqueRaiz;




	public:
		ArbolBMas();
		virtual ~ArbolBMas();

		int crear (std::string fileName, unsigned int tamNodo = BLOQUE_TAM_DEFAULT);
		int eliminar (std::string fileName);

		int abrir (std::string fileName, std::string mode);
		int cerrar ();

		int agregar(RegistroVariable & reg);
		int eliminar(RegistroVariable & reg);
		int buscar(RegistroClave & reg);
		/* Devuelve el numero de nodo secuencial donde esta o deberia estar reg. */


		/*ESTOS METODOS DEBERIAN SER PRIVADOS, SE HACEN PUBLICOS PARA TESTEAR*/
		int _set_header();
		int _get_header();

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

};

#endif /* ARBOLBMAS_H */ 
