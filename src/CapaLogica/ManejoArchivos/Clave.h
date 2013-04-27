#ifndef CLAVE_H
#define CLAVE_H

#include "../../Constantes.h"

class Clave{

	protected:


	public:
		virtual bool operator <(const Clave&)= 0;
		virtual bool operator >(const Clave&)= 0;
		virtual bool operator ==(const Clave&)= 0;
		virtual void operator =(const Clave&)= 0;
		virtual bool operator !=(const Clave&)= 0;
		virtual bool operator <=(const Clave&)= 0;
		virtual bool operator >=(const Clave&)= 0;
		/*clases que extiendan a clave tendran que redefinir los operadores de comparacion
		 * > , < y == */

		virtual int empaquetar(char* buffer)= 0;
		/*exporta los datos como un buffer de char. Se retorna la cantidad de bytes escritos*/
		virtual void desempaquetar(char* buffer,unsigned short tamanioBuffer)= 0;
		/*importa una clave desde un buffer de char. */
		virtual int get_tamanio()= 0;
		/*retorna el tamanio de la clave guardada en bytes*/

		Clave();
		virtual ~Clave();

};

#endif /* CLAVE_H */ 
