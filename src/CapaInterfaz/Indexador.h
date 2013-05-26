#ifndef INDEXADOR_H
#define INDEXADOR_H

#include <string>

#include "../CapaFisica/ManejadorRegistrosVariables.h"
#include "../CapaFisica/RegistroVariable.h"

#include "../CapaLogica/HashingExtensible/HashingExtensible.h"
#include "../CapaLogica/ArbolBMas/ArbolBMas.h"
#include "../CapaLogica/ArbolBMas/IterArbolBMas.h"

#include "../CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "../CapaLogica/ManejoArchivos/ClaveNumerica.h"
#include "../CapaLogica/Parser/ParserCanciones.h"
#include "../CapaLogica/Compresor/CompresorPPMC.h"


class Indexador
{
	public:
		Indexador();
		virtual ~Indexador();
	
		int indexar(std::string & directorioEntrada, std::string & directorioSalida);
		int consultar_autor(std::string & directorioSalida, std::string & unAutor);
		int consultar_titulo(std::string & directorioSalida, std::string & titulo);

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
