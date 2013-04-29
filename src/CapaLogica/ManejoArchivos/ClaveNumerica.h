/*
 * ClaveNumerica.h
 *
 *  Created on: Apr 22, 2013
 *      Author: martin
 */

#ifndef CLAVENUMERICA_H_
#define CLAVENUMERICA_H_

#include"Clave.h"
#include<sstream>

using namespace std;

class ClaveNumerica:public Clave {

	protected:
		int dato;

	public:
		virtual bool operator <(const Clave&);
		virtual bool operator >(const Clave&);
		virtual bool operator ==(const Clave&);
		virtual bool operator >=(const Clave&);
		virtual bool operator <=(const Clave&);
		virtual bool operator !=(const Clave&);
		virtual void operator =(const Clave&);
		/*se redefinen operatores*/


		virtual void desempaquetar(char* buffer,unsigned short tamanioBuffer= sizeof(int));
		/*no es necesario aclarar el tamanio de la clave, los integer siempre tienen el mismo
		 * tamanio.*/

		virtual int empaquetar(char* buffer);

		int incrementar();
		/*incrementa en uno el valor de la clave y retorna el nuevo valor incrementado*/

		void set_dato(int dato);
		int get_dato();
		/*getters y setters*/

		int get_tamanio();

		void resetear();
		/*setea el dato de la clav en 0*/

		ClaveNumerica();
		ClaveNumerica(int dato);
		virtual ~ClaveNumerica();

};

#endif /* CLAVENUMERICA_H_ */
