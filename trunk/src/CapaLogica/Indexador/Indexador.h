#ifndef INDEXADOR_H
#define INDEXADOR_H

#include <string>

#include "../../CapaFisica/ManejadorRegistrosVariables.h"
#include "../../CapaFisica/RegistroVariable.h"

#include "../HashingExtensible/HashingExtensible.h"
#include "../ArbolBMas/ArbolBMas.h"
#include "../ArbolBMas/IterArbolBMas.h"

#include "../ManejoArchivos/RegistroCancion.h"
#include "../ManejoArchivos/ClaveNumerica.h"
#include "../Parser/ParserCanciones.h"
#include "../Compresor/CompresorPPMC.h"


class Indexador
{
	public:
		Indexador();
		virtual ~Indexador();
	
		int indexar(std::string & directorioEntrada, std::string & directorioSalida);
		int consultar_autor(std::string & directorioSalida, std::string & unAutor);

	private:
		HashingExtensible indicePrimario;
		ManejadorRegistrosVariables archivoMaestro;
		ArbolBMas indiceSecundarioAutor;
		HashingExtensible indiceSecundarioTitulo;
		HashingExtensible documentos;
		ParserCanciones parser;
		CompresorPPMC compresor;

		int _init(std::string & directorioEntrada, std::string & directorioSalida);
		int _finalizar();
};

#endif /* INDEXADOR_H */ 
