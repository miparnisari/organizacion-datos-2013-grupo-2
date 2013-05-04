#ifndef ARBOLBMAS_H
#define ARBOLBMAS_H

#include <string>
#include "../../Constantes.h"
#include "../../CapaFisica/RegistroVariable.h"
#include "../../CapaFisica/ManejadorBloques.h"
#include "NodoInterno.h"

typedef struct header_arbol
{
	unsigned int minCantBytesClaves;
	unsigned int maxCantBytesClaves;
}header_arbol;

class ArbolBMas
{
	private:
		header_arbol header;
		ManejadorBloques archivoNodosInternos;
		ManejadorBloques archivoNodosSecuencia;
		NodoInterno* nodoInternoRaiz;

	public:
		ArbolBMas();
		virtual ~ArbolBMas();

		int crear (std::string dir, std::string fileName);
		int eliminar (std::string dir, std::string fileName);

		int abrir (std::string dir, std::string fileName, std::string mode);
		int cerrar ();
		
		int agregar(RegistroVariable & reg);
		int eliminar(RegistroVariable & reg);
		int buscar(RegistroVariable & reg);

};

#endif /* ARBOLBMAS_H */ 
