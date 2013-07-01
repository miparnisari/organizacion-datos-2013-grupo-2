/*
 * Texto.cpp
 *
 *  Created on: May 23, 2013
 *      Author: maine
 */

#include "TextoNormalizado.h"

TextoNormalizado::TextoNormalizado() {
	contador_palabras_parseadas = 0;
	texto_parseado.clear();
}

TextoNormalizado::~TextoNormalizado() {
	texto_parseado.clear();
}

bool TextoNormalizado::_es_delimitador(char c)
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

char TextoNormalizado::_to_lowercase (char c)
{
	if (c >= 65 && c <= 90)
	{
		return c + 32;
	}
	return c;
}

int TextoNormalizado::parsear(std::string unTexto)
{
	if (unTexto.size() == 0)
		return 0;
    std::string palabraActual = "";
    unsigned int numberChars = 0; // Contador de digitos leidos en una palabra
    unsigned int i = 0;
    unsigned int tam = unTexto.size();
    char caracterActual;

    while (i  < tam )
    {
    	caracterActual = unTexto.at(i);
    	// Convierto a minusculas lo que sea que vino
		caracterActual = _to_lowercase(caracterActual);

		// Si vino un delimitador, lo salteo
    	if (_es_delimitador(caracterActual) )
    	{
    		if (palabraActual.size() > 0) {
    			this->texto_parseado.push_back(palabraActual);
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
				this->texto_parseado.push_back(palabraActual);
				palabraActual.clear();
			}
    	}
    	else {
	    	palabraActual.push_back(caracterActual);
    	}
		i ++;

    }
    if (! _es_delimitador(caracterActual) && ! palabraActual.empty())
    	this->texto_parseado.push_back(palabraActual);

    return this->texto_parseado.size();
}

int TextoNormalizado::get_proxima_palabra(std::string & proxima_palabra)
{
	unsigned int cantidadPalabras = texto_parseado.size();
	if (contador_palabras_parseadas +1 > cantidadPalabras)
		return RES_FIN;

	proxima_palabra = texto_parseado.at(contador_palabras_parseadas);
	contador_palabras_parseadas++;
	return RES_OK;
}

