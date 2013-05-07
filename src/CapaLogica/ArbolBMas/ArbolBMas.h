#ifndef ARBOLBMAS_H
#define ARBOLBMAS_H

#include <string>
#include "../../Constantes.h"
#include "../../CapaFisica/RegistroVariable.h"
#include "../../CapaFisica/ManejadorBloques.h"
#include "../../CapaFisica/Bloque.h"
#include "../ManejoArchivos/RegistroClave.h"

class NodoInterno;
class NodoSecuencial;

struct header_arbol
{
	unsigned int minCantBytesClaves;
	unsigned int maxCantBytesClaves;
};

class ArbolBMas
{
	private:
		header_arbol header;
		ManejadorBloques archivoNodosInternos;
		ManejadorBloques archivoNodosSecuencia;
		NodoInterno* raiz;

	public:
		ArbolBMas();
		virtual ~ArbolBMas();

		int crear (std::string dir, std::string fileName);
		int eliminar (std::string dir, std::string fileName);

		int abrir (std::string dir, std::string fileName, std::string mode);
		int cerrar ();
		
		int agregar(RegistroVariable & reg);
		int eliminar(RegistroVariable & reg);
		int buscar(RegistroClave & reg);
		/* Devuelve el numero de nodo secuencial donde esta o deberia estar reg. */

};

#endif /* ARBOLBMAS_H */ 
