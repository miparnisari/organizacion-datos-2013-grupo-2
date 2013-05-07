#include "ParserCanciones.h"


ParserCanciones::ParserCanciones()
{
	contadorArchivosLeidos = 0;
}

ParserCanciones::~ParserCanciones()
{
	
}

bool ParserCanciones::finDirectorio()
{
	return (contadorArchivosLeidos > archivos.size());
}

char ParserCanciones::_estandarizar (char caracter)
{
	//TODO Martin: quitar acentos, dieresis, etc.
	return caracter;	
}

int ParserCanciones::crear (std::string dir)
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	int res = chdir(dir.c_str()); // Cambia el directorio al del repositorio, para poder indexar
	if (res == -1)
		return RES_DIRECTORY_DOESNT_EXIST;
	utilitarios::listpath (dir, archivos, ".txt");
	chdir(cwd); // De vuelta al directorio original
	return RES_OK;
}

int ParserCanciones::getNextCancion(RegistroCancion& reg)
{
	// Abrimos el archivo
	std::ifstream archivo(archivos[contadorArchivosLeidos].c_str(), std::ios::in); 
	std::string s_contenidoArchivo;

	// Lo leemos
	while (archivo.good())
	{
		char c = archivo.get();
		_estandarizar(c);
		if (archivo.good())
			s_contenidoArchivo.push_back(c);
	}

	archivo.close();
	contadorArchivosLeidos ++;
	
	// el registro
	char * regCancion = new char[s_contenidoArchivo.size() + 1];
	std::copy(s_contenidoArchivo.begin(), s_contenidoArchivo.end(), regCancion);
	regCancion[s_contenidoArchivo.size()] = '\0'; // don't forget the terminating 0

	// el tamaño del registro
	unsigned short sizeofregCancion = s_contenidoArchivo.size();
	
	// lo cargamos en el registro
	reg.limpiar_buffer();
	int resultado = reg.cargar(regCancion, sizeofregCancion);
	
	delete[] regCancion;
	
	return resultado;
}
