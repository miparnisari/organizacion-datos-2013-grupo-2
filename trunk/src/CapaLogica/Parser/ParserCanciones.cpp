#include "ParserCanciones.h"


ParserCanciones::ParserCanciones()
{
	contadorArchivosLeidos = 0;
	currentWorkingDirectory[0] = '\0';
}

ParserCanciones::~ParserCanciones()
{
	chdir(currentWorkingDirectory); // De vuelta al directorio original
}

unsigned int ParserCanciones::get_cantidad_archivos()const
{
	return archivos.size();
}

bool ParserCanciones::fin_directorio()const
{
	return (contadorArchivosLeidos > get_cantidad_archivos());
}

char ParserCanciones::_estandarizar (char caracter)
{
	return normalizador.transformar_char(caracter);
}

int ParserCanciones::crear (std::string dir)
{
	getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory));
	int res = chdir(dir.c_str()); // Cambia el directorio al del repositorio, para poder indexar
	if (res == -1)
		return RES_DIRECTORY_DOESNT_EXIST;
	utilitarios::listpath (dir, archivos, ".txt");
	return RES_OK;
}

int ParserCanciones::obtener_proxima_cancion(RegistroCancion& reg, std::string nombreArchivo)
{
	// Abrimos el archivo
	nombreArchivo = archivos[contadorArchivosLeidos];
	std::ifstream archivo(nombreArchivo.c_str(), std::ios::in);
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
	char * bufferCancion = new char[s_contenidoArchivo.size() + 1];
	std::copy(s_contenidoArchivo.begin(), s_contenidoArchivo.end(), bufferCancion);
	bufferCancion[s_contenidoArchivo.size()] = '\0';

	// el tamaño del registro
	unsigned short sizeOfBuffer = s_contenidoArchivo.size();
	
	// lo cargamos en el registro
	reg.limpiar_buffer();
	int resultado = reg.cargar(bufferCancion, sizeOfBuffer);
	
	delete[] bufferCancion;
	return resultado;
}
