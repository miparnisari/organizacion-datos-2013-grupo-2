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

		int parsear(std::string unTexto);
		int get_proxima_palabra(std::string & palabra);

	private:
	    std::vector<std::string> elTexto;
	    unsigned int contador_palabra;

	    bool _es_delimitador(char c);
	    char _to_lowercase (char c);

};

#endif /* LETRASCANCION_H_ */
