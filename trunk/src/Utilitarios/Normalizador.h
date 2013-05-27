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
#include <stdio.h>
#include <string.h>
#include "../Constantes.h"

using namespace std;

class Normalizador {

	protected:
		char filtrar_char(wchar_t c);

	public:

		char transformar_char(wchar_t c);
		wstring a_minuscula(wstring & linea);
		string normalizar_string(wstring & linea);
		int normalizar(string pathArchivoEntrada,string pathArchivoSalida);
		/*normaliza un archivo de texto con el nombre pathArchivoEntrada, removiendo
		 * dieresis, acentos y cambiando el texto a lowerCase y el archivo resultado lo
		 * almacena en la direccion de pathArchivoSalida.*/
		string normalizar_input( string linea );
		/*dado un string "linea" introducido desde consola, se lo normaliza y se retorna el
		 * string normalizado*/

};

#endif /* NORMALIZADOR_H_ */
