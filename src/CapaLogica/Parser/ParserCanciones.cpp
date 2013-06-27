#include "ParserCanciones.h"


ParserCanciones::ParserCanciones()
{
	_init();
}

ParserCanciones::~ParserCanciones()
{
}


void ParserCanciones::_init()
{
	contadorArchivosLeidos = 0;
}


unsigned int ParserCanciones::get_cantidad_archivos()const
{
	return archivos.size();
}

bool ParserCanciones::fin_directorio()const
{
	return (contadorArchivosLeidos >= get_cantidad_archivos());
}

char ParserCanciones::_estandarizar (char caracter)
{
	return normalizador.transformar_char(caracter);
}

int ParserCanciones::crear (std::string & dir)
{
	_init();
	archivos.clear();
	return utilitarios::listpath (dir, archivos, ".txt");
}

int ParserCanciones::obtener_proxima_cancion(RegistroCancion& regNormalizado,
		RegistroCancion & regNoNormalizado,
		std::string & nombreArchivo)
{

	// Abrimos el archivo
	nombreArchivo = archivos[contadorArchivosLeidos];
	int largoNombreArchivo = nombreArchivo.length();

	// Archivo que tiene la normalizacion
	char* nombreArchivoNormalizado = new char[largoNombreArchivo + 1]();
	nombreArchivoNormalizado[largoNombreArchivo] = '\0';
	strncpy(nombreArchivoNormalizado,nombreArchivo.c_str(),largoNombreArchivo);
	memcpy(nombreArchivoNormalizado+largoNombreArchivo-4,".tmp",4);

	normalizador.normalizar(nombreArchivo, std::string(nombreArchivoNormalizado));

	// Leo el archivo NORMALIZADO
	std::ifstream archivo(nombreArchivoNormalizado, std::ios::in);
	std::string s_contenidoArchivoNormalizado;

	// Lo leemos
	while (archivo.good())
	{
		char c = archivo.get();
		if (archivo.good())
			s_contenidoArchivoNormalizado.push_back(c);
	}

	archivo.close();
	remove(nombreArchivoNormalizado); // Borro el archivo temporal

	// Leo el archivo ORIGINAL (para mandar el registro al archivo maestro)
	archivo.open(nombreArchivo.c_str(), std::ios::in);
	std::string s_contenidoArchivo;

	// Lo leemos
	while (archivo.good())
	{
		char c = archivo.get();
		if (archivo.good())
			s_contenidoArchivo.push_back(c);
	}

	archivo.close();

	memcpy(nombreArchivoNormalizado+largoNombreArchivo-4,".txt",4);
	nombreArchivo = nombreArchivoNormalizado;
	delete[] nombreArchivoNormalizado;

	contadorArchivosLeidos ++;
	
	// el registro
	char * bufferNormalizado = new char[s_contenidoArchivoNormalizado.size() + 1];
	bufferNormalizado[s_contenidoArchivoNormalizado.size()] = '\0';
	std::copy(s_contenidoArchivoNormalizado.begin(), s_contenidoArchivoNormalizado.end(), bufferNormalizado);

	// el tamaño del registro
	unsigned short sizeOfBuffer = s_contenidoArchivoNormalizado.size();
	if (sizeOfBuffer == 0)
	{
		delete[] bufferNormalizado;
		return RES_ERROR;
	}
	
	// el registro original
	char * bufferNoNormalizado = new char[s_contenidoArchivo.size() + 1];
	bufferNoNormalizado[s_contenidoArchivo.size()] = '\0';
	std::copy(s_contenidoArchivo.begin(), s_contenidoArchivo.end(), bufferNoNormalizado);

	regNoNormalizado.limpiar_buffer();
	regNoNormalizado.cargar(bufferNoNormalizado, sizeOfBuffer);

	regNormalizado.limpiar_buffer();
	int resultado = regNormalizado.cargar(bufferNormalizado, sizeOfBuffer);
	
	delete[] bufferNoNormalizado;
	delete[] bufferNormalizado;
	return resultado;
}
