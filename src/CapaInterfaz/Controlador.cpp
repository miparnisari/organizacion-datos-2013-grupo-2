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

void Controlador::mostrar_contenido(int id, RegistroCancion* reg)
{
	if (reg == NULL)
		return;
	std::cout << "----------- ID CANCION = " << id << " ---------" << std::endl;
	std::cout << "TITULO = " << reg->get_titulo() << std::endl;
	std::cout << reg->get_letra() << std::endl;

}

int Controlador::indexar (std::string & directorioEntrada, std::string & directorioSalida )
{
	return indexador.indexar(directorioEntrada, directorioSalida, resolvedor_consultas);
}

int Controlador::consultar_titulo( std::string & directorioSalida, std::string & titulo)
{
	std::string tituloNormalizado = normalizador_consultas.normalizar_input(titulo);

	generador_estadisticas.aumentar_frecuencia_titulo(tituloNormalizado);
	resolvedor_consultas.set_directorio_indice(directorioSalida);
	int id = resolvedor_consultas.get_id_cancion_titulo(tituloNormalizado);
	if (id != RES_RECORD_DOESNT_EXIST)
	{
		generador_estadisticas.aumentar_frecuencia_tema(id);
		RegistroCancion* reg = resolvedor_consultas.get_reg_completo(id);
		mostrar_contenido(id,reg);
		delete reg;
		return RES_OK;
	}

	return RES_RECORD_DOESNT_EXIST;
}

int Controlador::consultar_autor( std::string & directorioSalida, std::string & autor)
{
	std::string autorNormalizado = normalizador_consultas.normalizar_input(autor);

	generador_estadisticas.aumentar_frecuencia_autor(autorNormalizado);
	resolvedor_consultas.set_directorio_indice(directorioSalida);
	std::vector <int> ids = resolvedor_consultas.get_id_canciones_autor(autorNormalizado);
	if (ids.size() == 0)
	{
		std::cout << "El autor " << autor << " no fue encontrado." << std::endl;
	}
	for (unsigned int i = 0; i < ids.size(); i ++)
	{
		generador_estadisticas.aumentar_frecuencia_tema(ids.at(i));
		RegistroCancion* reg = resolvedor_consultas.get_reg_completo(ids.at(i));
		mostrar_contenido(ids.at(i),reg);
		delete reg;
	}
	return RES_OK;
}

int Controlador::consultar_frase( std::string & directorioSalida, std::string & frase)
{
	std::string fraseNormalizada = normalizador_consultas.normalizar_input(frase);

	generador_estadisticas.aumentar_frecuencia_frase(fraseNormalizada);
	resolvedor_consultas.set_directorio_indice(directorioSalida);
	std::vector <int> ids = resolvedor_consultas.get_ids_canciones_frases(fraseNormalizada);
	for (unsigned int i = 0; i < ids.size(); i ++)
	{
		generador_estadisticas.aumentar_frecuencia_tema(ids.at(i));
		RegistroCancion *reg = resolvedor_consultas.get_reg_completo(ids.at(i));
		mostrar_contenido(ids.at(i),reg);
		delete reg;
	}
	return RES_OK;
}

int Controlador::borrar_cancion( std::string & directorioSalida, int id_cancion)
{
	resolvedor_consultas.set_directorio_indice(directorioSalida);

	RegistroCancion* reg = resolvedor_consultas.get_reg_completo (id_cancion);

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
