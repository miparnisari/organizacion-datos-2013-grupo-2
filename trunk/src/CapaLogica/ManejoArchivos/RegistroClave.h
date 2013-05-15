/*
 * RegistroClave.h
 *
 *  Created on: Apr 27, 2013
 *      Author: martin
 */

#ifndef REGISTROCLAVE_H_
#define REGISTROCLAVE_H_


#include "../../CapaFisica/RegistroVariable.h"
#include "ClaveX.h"


class RegistroClave:public RegistroVariable {

	protected:
		ClaveX clave;

		void _agregar_campo_clave();
		/*la clave se agrega como el primer campo del registro. Se considera el
		 * buffer del registro limpio*/

	public:
		static const unsigned short NUMERO_CAMPO_CLAVE= 0;

		RegistroClave();
		~RegistroClave();
		RegistroClave(const RegistroClave& otro);
		RegistroClave& operator=(const RegistroClave& otro);

		virtual void set_clave(const ClaveX& clave);
		virtual ClaveX get_clave() const;

		void limpiar_campos() throw();
		/*limpia el buffer e inserta la clave en el primer campo del registro .
		 * Esto obliga a que la clave SIEMPRE ESTE EN EL PRIMER CAMPO.*/

		virtual int desempaquetar(const char* copia)throw();
		//desempaqueta un RegistroClave recuperando la clave y los demas campos

		virtual bool operator <(const RegistroClave& rc)const;
		virtual bool operator >(const RegistroClave& rc)const;
		virtual bool operator ==(const RegistroClave& rc)const;
		virtual bool operator <=(const RegistroClave& rc)const;
		virtual bool operator >=(const RegistroClave& rc)const;

};/*registro variable donde el primer campo coincide con una clave de identificacion*/

#endif /* REGISTROCLAVE_H_ */
