/*
 * ClaveString.h
 *
 *  Created on: Apr 22, 2013
 *      Author: martin
 */

#ifndef CLAVESTRING_H_
#define CLAVESTRING_H_

#include "Clave.h"
#include <string>
#include <sstream>
#include <cstring>
#include <iostream>

using std::string;
using std::stringstream;

class ClaveString:public Clave{

	protected:
		string dato;

	public:
		static const char DATO_DEFECTO= '~';

		virtual void desempaquetar(char* buffer,unsigned short tamanioBuffer= 0);
		/*importa un string a partir de una cadena de caracteres. Si tamanioBuffer no es
		 * especificado, se toma como 0 -> como tamanio del buffer se usa strlen(buffer) .*/

		virtual int empaquetar(char* buffer);
		/*exporta el string como una cadena de caracteres, se retorna la longitud de
		 * dicha cadena*/

		virtual bool operator >(const Clave& otraClave);
		virtual bool operator ==(const Clave& otraClave);
		virtual bool operator <(const Clave& otraClave);
		virtual bool operator >=(const Clave& otraClave);
		virtual bool operator <=(const Clave& otraClave);
		virtual bool operator !=(const Clave& otraClave);
		virtual void operator =(const Clave& otraClave);
		virtual void operator = (const string& unString);
		virtual std::ostream& operator<<(std::ostream& os)const;
		/*redefino los operadores para que antes de comparar las cadenas remueva los espacios
		 * de las mismas*/

		static string remover_espacios(const std::string linea);
		/*remueve los espacios en blanco de linea y retorna un nuevo string*/

		void remover_espacios();
		/*remueve los espacios del string contenido definitivamente*/

		virtual void imprimir_dato() const;

		void set_dato(const string&);
		string get_dato();
		/*getters y setters*/

		int get_tamanio();

		void resetear();

		ClaveString();
		ClaveString(const string& linea);
		ClaveString(const ClaveString& copia);
		virtual ~ClaveString();


};

#endif /* CLAVESTRING_H_ */
