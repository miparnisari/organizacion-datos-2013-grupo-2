/*
 * Controlador.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: maine
 */

#include "Controlador.h"

Controlador::Controlador() {

}

Controlador::~Controlador() {
}

void Controlador::mostrar_resultado(const vector<IDdocumento_t> & documentos)
{
	const unsigned int CANTIDAD_DOCS = documentos.size();
	for (unsigned int i = 0; i < CANTIDAD_DOCS; i ++)
	{
		IDdocumento_t id_doc = documentos[i];
		generador_estadisticas.aumentar_frecuencia_tema(id_doc);
		RegistroCancion* reg = resolvedor_consultas.get_reg_completo(id_doc);

		std::cout << "----------------------------------------------" << std::endl;
		std::cout << "- ID DE LA CANCIÓN = " << id_doc << std::endl;
		std::cout << "- AUTOR/ES  = ";

		const unsigned short CANTIDAD_AUTORES = reg->get_cantidad_autores();
		for (unsigned short i = 0; i < CANTIDAD_AUTORES; i++)
		{
			std::cout << reg->get_autor(i);
			if (i != CANTIDAD_AUTORES - 1) std::cout<< ", ";
		}
		std::cout << std::endl;
		std::cout << "- TITULO = " << reg->get_titulo() << std::endl;
		std::cout << "- LETRAS = " << std::endl;
		std::cout << reg->get_letra() << std::endl;
		std::cout << "----------------------------------------------" << std::endl;

		delete reg;
	}

}

int Controlador::indexar (std::string & directorioEntrada, std::string & directorioSalida )
{
	resolvedor_consultas.set_directorio_indice(directorioSalida);
	return indexador.indexar(directorioEntrada, directorioSalida, resolvedor_consultas);
}

int Controlador::consultar_titulo( std::string & directorioSalida, std::string & titulo)
{
	std::string tituloNormalizado = normalizador_consultas.normalizar_input(titulo);

	generador_estadisticas.aumentar_frecuencia_titulo(tituloNormalizado);
	resolvedor_consultas.set_directorio_indice(directorioSalida);
	std::vector <IDdocumento_t> ids = resolvedor_consultas.get_ids_cancion_titulo(titulo);
	if (ids.size() == 0)
	{
		std::cout << "No se encontraron canciones con título '" << titulo << "'." << std::endl;
	}
	mostrar_resultado(ids);
	return RES_OK;
}

int Controlador::consultar_autor( std::string & directorioSalida, std::string & autor)
{
	std::string autorNormalizado = normalizador_consultas.normalizar_input(autor);

	generador_estadisticas.aumentar_frecuencia_autor(autorNormalizado);
	resolvedor_consultas.set_directorio_indice(directorioSalida);
	std::vector <IDdocumento_t> ids = resolvedor_consultas.get_id_canciones_autor(autorNormalizado);
	if (ids.size() == 0)
	{
		std::cout << "El autor '" << autor << "' no fue encontrado." << std::endl;
	}
	mostrar_resultado(ids);
	return RES_OK;
}

int Controlador::consultar_frase( std::string & directorioSalida, std::string & frase)
{
	std::string fraseNormalizada = normalizador_consultas.normalizar_input(frase);

	generador_estadisticas.aumentar_frecuencia_frase(fraseNormalizada);
	resolvedor_consultas.set_directorio_indice(directorioSalida);
	std::vector <IDdocumento_t> ids = resolvedor_consultas.get_ids_canciones_frases(fraseNormalizada);
	if (ids.size() == 0)
	{
		std::cout << "La frase '" << frase << "' no fue encontrada." << std::endl;
	}
	mostrar_resultado(ids);
	return RES_OK;
}

int Controlador::borrar_cancion( std::string & directorioSalida, int id_cancion)
{
	resolvedor_consultas.set_directorio_indice(directorioSalida);

	RegistroCancion* reg = resolvedor_consultas.get_reg_completo (id_cancion);

	if (reg == NULL)
	{
		std::cout << "La canción con ID " << id_cancion << " no existe." << std::endl;
		return RES_RECORD_DOESNT_EXIST;
	}


	return indexador.borrar_cancion(directorioSalida ,id_cancion, *reg);
}

void Controlador::mostrar_estadisticas(std::string tipo)
{
	if (tipo == "titulo")
	{
		std::string tituloMasConsultado = generador_estadisticas.get_titulo_mas_consultado();
		if (tituloMasConsultado != "")
			std::cout << "Título más consultado: '" << tituloMasConsultado << "'" << std::endl;
	}
	else if (tipo == "autor")
	{
		std::string autorMasConsultado = generador_estadisticas.get_autor_mas_consultado();
		if (autorMasConsultado != "")
			std::cout << "Autor más consultado: '" << autorMasConsultado << "'" << std::endl;
	}
	else if (tipo == "frase")
	{
		std::string fraseMasConsultada = generador_estadisticas.get_frase_mas_consultado();
		if (fraseMasConsultada != "")
			std::cout << "Frase más consultada: '" << fraseMasConsultada << "'" << std::endl;
	}
	else if (tipo == "tema")
	{
		int temaMasConsultado = generador_estadisticas.get_tema_mas_consultado();
		if (temaMasConsultado != -1)
			std::cout << "ID tema más consultado: '" << temaMasConsultado << "'" << std::endl;
	}
}
