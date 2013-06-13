/*
 * GeneradorEstadisticasConsultas.h
 *
 *  Created on: Jun 12, 2013
 *      Author: maine
 */

#ifndef GENERADORESTADISTICASCONSULTAS_H_
#define GENERADORESTADISTICASCONSULTAS_H_

#include <string>
#include <vector>
#include <iostream>

class GeneradorEstadisticasConsultas
{
	private:
		std::vector < std::pair <std::string,int >>	consultasTitulo;
		std::vector < std::pair <std::string,int >>	consultasAutor;
		std::vector < std::pair <std::string,int >>	consultasFrase;
		std::vector < std::pair <int,int >>			consultasTema;

		template<typename U>
		int _get_pos_max_freq (std::vector<U>& vector);

		template<typename T, typename U>
		void _aumentar_frecuencia (T& valor, std::vector<U>& vector);

	public:
		GeneradorEstadisticasConsultas();
		virtual ~GeneradorEstadisticasConsultas();

		std::string get_titulo_mas_consultado();
		std::string get_frase_mas_consultado();
		std::string get_autor_mas_consultado();
		int get_tema_mas_consultado();

		void aumentar_frecuencia_titulo (std::string & titulo);
		void aumentar_frecuencia_autor (std::string & autor);
		void aumentar_frecuencia_frase (std::string & frase);
		void aumentar_frecuencia_tema (int id_doc);
};

#endif /* GENERADORESTADISTICASCONSULTAS_H_ */
