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
		char currentWorkingDirectory[1024];
		Normalizador normalizador;
		std::vector<std::string> archivos;
		unsigned int contadorArchivosLeidos;

		char _estandarizar(char caracter);

	public:
		ParserCanciones();
		~ParserCanciones();
		
		int crear (std::string dir);
		bool fin_directorio()const;
		int	getNextCancion(RegistroCancion & reg, std::string nombreArchivo);

		unsigned int get_cantidad_archivos()const;
	

};

#endif /* PARSERCANCIONES_H */ 
