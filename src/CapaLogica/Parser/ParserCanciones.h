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

	public:
		ParserCanciones();
		~ParserCanciones();
		int crear (std::string dir);
		
		bool fin_directorio();
		int setFile(std::string path);
		int	getNextCancion(RegistroCancion & reg);
		bool eof ();
		void cerrar ();

		int cantidad_archivos();
	

};

#endif /* PARSERCANCIONES_H */ 
