/*
 * TestParserCanciones.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: maine
 */

#include "TestParserCanciones.h"

TestParserCanciones::TestParserCanciones() {
}

TestParserCanciones::~TestParserCanciones() {
}

void TestParserCanciones::ejecutar()
{
	test_leer_de_archivo();
}

void TestParserCanciones::test_leer_de_archivo()
{
	ParserCanciones parser;
	string na= "../songs/";
	assert(parser.crear(na) == RES_OK);
	RegistroCancion regCancion, regCancionNormalizada;
	std::string nombreArchivo = "";

	// Para cada cancion que tengamos...
	while (parser.fin_directorio() == false)
	{
		if (parser.obtener_proxima_cancion(regCancion, regCancionNormalizada, nombreArchivo) == RES_OK)
		{
			// Autores
			for (unsigned short j=0; j < regCancion.get_cantidad_autores(); j++)
			{
				std::string autor = regCancion.get_autor(j);
				assert(autor.size() > 1);
			}

			// Atributos de la cancion (NO la letra)
//			for(unsigned short i=1;i<regCancion.contar_parametros();i++)
//			{
//				regCancion.obtener_parametro(i,parametro);
//				assert(parametro.size() > 1);
//			}

			// 50 caracteres de la letra
			std::string letra = regCancion.get_letra();
			std::string parteLetra = "";
			for (int i = 0; i < 50; i++)
			{
				parteLetra.push_back(letra[i]);
			}
			assert(parteLetra.size() == 50);
		}
	}
	print_test_ok("test_leer_de_archivo");
}
