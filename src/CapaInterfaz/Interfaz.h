/*
 * Interfaz.h
 *
 *  Created on: Jul 4, 2013
 *      Author: maine
 */

#ifndef INTERFAZ_H_
#define INTERFAZ_H_

#include "Controlador.h"
#include "../Constantes.h"
#include "../Utilitarios/Utilitarios.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using std::ofstream;
using std::string;
using std::stringstream;

class Interfaz {

	private:
		int _transformar_operacion(string& s);
		void mostrar_operaciones();
		int pedir_operacion();
		std::string pedir_directorio_salida();
		std::string pedir_directorio_entrada();
		std::string pedir_consulta();
		int pedir_id_cancion();

	public:
		Interfaz();
		virtual ~Interfaz();

		void ejecutar();

};

#endif /* INTERFAZ_H_ */
