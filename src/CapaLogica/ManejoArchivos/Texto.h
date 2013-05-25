/*
 * LetrasCancion.h
 *
 *  Created on: May 23, 2013
 *      Author: maine
 */

#ifndef TEXTO_H_
#define TEXTO_H_
#include "../../Constantes.h"
#include <vector>
#include <string>
#include <iostream>

class Texto {
	public:
		Texto();
		virtual ~Texto();

		/* Parsea un string de palabras a un vector de palabras sin espacios delimitadores ni numeros muy grandes.
		Devuelve la cantidad de palabras en la que se descompuso el texto. */
		int parsear(std::string unTexto);


		/* Devuelve la siguiente palabra del texto a procesar, o RES_FIN si no hay mas palabras. */
		int get_proxima_palabra(std::string & palabra);

	private:
	    std::vector<std::string> elTexto;
	    unsigned int contador_palabra;

	    bool _es_delimitador(char c);
	    char _to_lowercase (char c);

};

#endif /* LETRASCANCION_H_ */
