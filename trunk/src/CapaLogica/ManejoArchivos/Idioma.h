#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "../../Constantes.h"

class Idioma
{
	private:
		std::map < std::string, std::vector<std::string> > mapaIdiomasValidos;
		std::string idioma;
		
		int cargarIdiomasValidos(std::string nombreArchivo);
		bool validar (std::string idioma);
		
	public:
		Idioma();
		~Idioma();
		
		std::string getIdioma();
		bool cargar(std::string idioma);
		
};
