#ifndef PARSERCANCIONES_H
#define PARSERCANCIONES_H

#include <string>
#include <vector>
#include <fstream>
#include "../../Constantes.h"
#include "../ManejoArchivos/RegistroCancion.h"
#include "../../Utilitarios/Utilitarios.h"
#include "../../Utilitarios/Normalizador.h"

using namespace utilitarios;

class ParserCanciones
{
	private:
		Normalizador normalizador;
		std::vector<std::string> archivos;
		unsigned int contadorArchivosLeidos;

		char _estandarizar(char caracter);
		void _init();

	public:
		ParserCanciones();
		~ParserCanciones();
		
		int crear (std::string dir);
		bool fin_directorio()const;
		int	obtener_proxima_cancion(RegistroCancion & reg, std::string & nombreArchivo);

		unsigned int get_cantidad_archivos()const;
};

#endif /* PARSERCANCIONES_H */ 
