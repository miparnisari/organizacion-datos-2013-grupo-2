/*
 * Texto.cpp
 *
 *  Created on: May 23, 2013
 *      Author: maine
 */

#include "Texto.h"

Texto::Texto() {
	contador_palabra = 0;
	elTexto.clear();
}

Texto::~Texto() {
	elTexto.clear();
}

bool Texto::_es_delimitador(char c)
{
	const char delimit[31] = {'=','`','\'',' ','\n','\t','\b','#','&','.',';',':',',','_','~','(',')', '[', ']','{','}','?','!','<','>','"','@','/','$','%','*'};

	unsigned int i;
	for (i = 0; i < 31; i++) {
		if (c == delimit[i]) {
			return true;
		}
	}

	return false;
}

char Texto::_to_lowercase (char c) {
	if (c >= 65 && c <= 90) {
		return c + 32;
	}
	return c;
}

int Texto::parsear(std::string unTexto)
{
	if (unTexto.size() == 0)
		return 0;
    std::string palabraActual = "";
    unsigned int numberChars = 0; // Contador de digitos leidos en una palabra
    unsigned int i = 0;
    unsigned int tam = unTexto.size();
    char caracterActual = unTexto.at(i);

    while (i  < tam-1 )
    {
    	// Convierto a minusculas lo que sea que vino
		caracterActual = _to_lowercase(caracterActual);

		// Si vino un delimitador, lo salteo
    	if (_es_delimitador(caracterActual) )
    	{
    		if (palabraActual.size() > 0) {
    			this->elTexto.push_back(palabraActual);
    			palabraActual.clear();
    			numberChars = 0;
    		}
    	}

    	// Si vino un numero, lo parseo como un numero de hasta 4 digitos
    	else if ( isdigit(caracterActual) )
    	{
			numberChars ++;
			palabraActual.push_back(caracterActual);
			// Si una palabra tiene 4 digitos consecutivos O MENOS, los devuelvo
			if (palabraActual.size() == numberChars && numberChars == LONGITUD_MAXIMA_NUMERO_INDEXABLE)
			{
				numberChars = 0;
				this->elTexto.push_back(palabraActual);
				palabraActual.clear();
			}
    	}
    	else {
	    	palabraActual.push_back(caracterActual);
    	}
		i ++;
		caracterActual = unTexto.at(i);
    }
    palabraActual.push_back(caracterActual);
    this->elTexto.push_back(palabraActual);
    return this->elTexto.size();
} /* Parsea un string de palabras a un vector de palabras sin espacios delimitadores ni numeros muy grandes.
Devuelve la cantidad de palabras en la que se descompuso el texto. */

int Texto::get_proxima_palabra(std::string & palabra)
{
	unsigned int cantidadPalabras = elTexto.size();
	if (contador_palabra +1 > cantidadPalabras)
		return RES_ERROR;

	palabra = elTexto.at(contador_palabra);
	contador_palabra++;
	return RES_OK;
} /* Devuelve la siguiente palabra del texto a procesar, o RES_ERROR si no hay mas palabras. */

