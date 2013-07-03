/*
 * Controlador.h
 *
 *  Created on: Jun 13, 2013
 *      Author: maine
 */

#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include "Indexador.h"
#include "GeneradorEstadisticasConsultas.h"
#include "ResolvedorConsultas.h"
#include "../CapaLogica/Parser/TextoNormalizado.h"

class Controlador {

	private:
		Indexador indexador;
		GeneradorEstadisticasConsultas generador_estadisticas;
		ResolvedorConsultas resolvedor_consultas;
		Normalizador normalizador_consultas;

		void mostrar_resultado(const vector<IDdocumento_t> & documentos);

	public:
		Controlador();
		virtual ~Controlador();

		int indexar (std::string & directorioEntrada, std::string & directorioSalida );
		int borrar_cancion( std::string & directorioSalida, int id_cancion);

		int consultar_titulo( std::string & directorioSalida, std::string & titulo);
		int consultar_autor( std::string & directorioSalida, std::string & autor);
		int consultar_frase( std::string & directorioSalida, std::string & frase);

		void mostrar_estadisticas(std::string tipo);
};

#endif /* CONTROLADOR_H_ */
