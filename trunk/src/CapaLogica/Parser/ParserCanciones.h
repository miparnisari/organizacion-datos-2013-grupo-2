#ifndef PARSERCANCIONES_H
#define PARSERCANCIONES_H

#include <string>
#include <vector>
#include <fstream>
#include "Normalizador.h"
#include "../../Constantes.h"
#include "../ManejoArchivos/RegistroCancion.h"
#include "../../Utilitarios/Utilitarios.h"


using namespace utilitarios;

class ParserCanciones
{
	private:
		Normalizador normalizador;
		std::vector<std::string> archivos;
		unsigned int contadorArchivosLeidos;

		void _init();

	public:
		ParserCanciones();
		~ParserCanciones();
		
		int crear (std::string & dir);
		bool fin_directorio()const;
		int	obtener_proxima_cancion(RegistroCancion& regNormalizado,
				RegistroCancion & regNoNormalizado,
				std::string & nombreArchivo);

		unsigned int get_cantidad_archivos()const;
};

#endif /* PARSERCANCIONES_H */ 
