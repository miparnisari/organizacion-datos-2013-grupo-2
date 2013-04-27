/*
 * RegistroClave.h
 *
 *  Created on: Apr 27, 2013
 *      Author: martin
 */

#ifndef REGISTROCLAVE_H_
#define REGISTROCLAVE_H_


#include "../../CapaFisica/RegistroVariable.h"
#include "Clave.h"


class RegistroClave:public RegistroVariable {

	protected:

	public:
		static const unsigned short NUMERO_CAMPO_CLAVE= 0;

		RegistroClave();
		RegistroClave(Clave* c);
		virtual ~RegistroClave();

		virtual int get_clave(Clave* clave);
		/*recupera la clave del registro del primer campo y lo almacena en la variable
		 * clave*/
		virtual int set_clave(Clave* clave);
		/*guarda la clave en el primer campo del registro sobreescribiendo el primer
		 * campo ya existente*/

		bool solo_clave();
		/*retorna true si el registro unicamente es una clave*/


};/*registro variable donde el primer campo coincide con una clave de identificacion*/

#endif /* REGISTROCLAVE_H_ */
