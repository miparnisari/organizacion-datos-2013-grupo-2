#ifndef INDEXADOR_H
#define INDEXADOR_H

#include <string>

#include "../../CapaFisica/ManejadorRegistrosVariables.h"
#include "../../CapaFisica/RegistroVariable.h"

#include "../HashingExtensible/HashingExtensible.h"
#include "../ArbolBMas/ArbolBMas.h"

#include "../ManejoArchivos/RegistroCancion.h"
#include "../ManejoArchivos/ClaveNumerica.h"
#include "../Parser/ParserCanciones.h"
#include "../Compresor/CompresorPPMC.h"


class Indexador
{
	public:
		Indexador();
		virtual ~Indexador();
	
		int indexar(std::string directorioEntrada, std::string directorioSalida);
	private:
		/* add your private declarations */
};

#endif /* INDEXADOR_H */ 
