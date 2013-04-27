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

		virtual int exportar(char* buffer)= 0;
		/*exporta los datos como un buffer de char. Se retorna la cantidad de bytes escritos*/
		virtual void importar(char* buffer,unsigned short tamanioBuffer)= 0;
		/*importa una clave desde un buffer de char. */


		Clave();
		virtual ~Clave();

};

#endif /* CLAVE_H */ 
