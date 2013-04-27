#ifndef PARSERCANCIONES_H
#define PARSERCANCIONES_H

#include <string>
#include <vector>
#include <fstream>
#include "../../Constantes.h"
#include "../ManejoArchivos/RegistroCancion.h"
#include "../../Utilitarios/Utilitarios.h"

using namespace utilitarios;

class ParserCanciones
{
	public:
		ParserCanciones();
		~ParserCanciones();
		int create (std::string dir);
		
		int				getCantidadCancionesLeidas();
		bool 			finDirectorio();
		int 			setFile(std::string path);
		int				getNextCancion(RegistroCancion & reg);
		bool eof ();
		void closeFile ();
	
	private:
		std::vector<std::string> archivos;
		unsigned int contadorArchivosLeidos;
		
		
		char estandarizar(char caracter);
};

#endif /* PARSERCANCIONES_H */ 
