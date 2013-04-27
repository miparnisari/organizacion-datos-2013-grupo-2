/*
 * Normalizador.h
 *
 *  Created on: Apr 25, 2013
 *      Author: martin
 */

#ifndef NORMALIZADOR_H_
#define NORMALIZADOR_H_

#include <fstream>
#include <functional>
#include <locale>
#include <algorithm>
#include <iostream>
#include <string>
#include "../Constantes.h"

using namespace std;

class Normalizador {

	protected:
		char transformar_char(wchar_t c);
		char filtrar_char(wchar_t c);

	public:
		int normalizar(string pathArchivoEntrada,string pathArchivoSalida);
		/*normaliza un archivo de texto con el nombre pathArchivoEntrada, removiendo
		 * dieresis, acentos y cambiando el texto a lowerCase y el archivo resultado lo
		 * almacena en la direccion de pathArchivoSalida.*/

};

#endif /* NORMALIZADOR_H_ */
