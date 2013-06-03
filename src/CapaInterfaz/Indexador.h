#ifndef INDEXADOR_H
#define INDEXADOR_H

#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include "../CapaFisica/ManejadorRegistrosVariables.h"
#include "../CapaFisica/RegistroVariable.h"

#include "../CapaLogica/HashingExtensible/HashingExtensible.h"
#include "../CapaLogica/ArbolBMas/ArbolBMas.h"
#include "../CapaLogica/ArbolBMas/IterArbolBMas.h"

#include "../CapaLogica/Indices/IndiceInvertido.h"

#include "../CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "../CapaLogica/ManejoArchivos/ClaveNumerica.h"
#include "../CapaLogica/Parser/ParserCanciones.h"
#include "../CapaLogica/Compresor/CompresorPPMC.h"

#include "ResolvedorConsultas.h"

class Indexador
{
	public:
		Indexador();
		virtual ~Indexador();
	
		int indexar(std::string & directorioEntrada, std::string & directorioSalida);
		int consultar_autor(std::string & directorioSalida, std::string & unAutor);
		int consultar_titulo(std::string & directorioSalida, std::string & titulo);
		int consultar_frase (std::string & directorioSalida, std::string & frase);
		void borrar_cancion (std::string & directorioSalida, int idCancion);

	private:
		static const int OPCION_INDEXAR = 1;
		static const int OPCION_ANEXAR = 2;

		IndiceInvertido indiceSecundarioFrases;
		HashingExtensible indicePrimario;
		ManejadorRegistrosVariables archivoMaestro;
		ArbolBMas indiceSecundarioAutor;
		HashingExtensible indiceSecundarioTitulo;
		HashingExtensible documentos;
		ParserCanciones parser;
		CompresorPPMC compresor;

		int _crear_archivos_indices(std::string & directorioSalida);
		int _abrir_archivos_indices (std::string & directorioSalida);
		int _finalizar();
		void _agregar_a_los_indices (
				ClaveNumerica & id,
				RegistroCancion & regCancion,
				RegistroCancion & regCancionNoNormalizada,
				std::string nombreArchivo);
		int _anexar(std::string & directorioEntrada, std::string & directorioSalida);
		void _indexar();
		int _mostrar_opciones(std::string & directorioEntrada, std::string & directorioSalida);
		void _mostrar_contenido(int id, RegistroCancion* reg);
};

#endif /* INDEXADOR_H */ 
