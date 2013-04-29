/*
 * ClaveX.h
 *
 *  Created on: Apr 27, 2013
 *      Author: martin
 */

#ifndef CLAVEX_H_
#define CLAVEX_H_

#include "ClaveString.h"
#include "ClaveNumerica.h"

class ClaveX {

	public:
		static const int CANTIDAD_TIPOS_CLAVE= 2;
		enum TipoClave {CLAVE_STRING , CLAVE_NUMERICA};

	protected:
		TipoClave tipoClave;
		Clave* claves[CANTIDAD_TIPOS_CLAVE];

		void _resetear_datos();

	public:

		ClaveX(TipoClave tipoClave);

		virtual ~ClaveX();

		virtual int empaquetar(char* buffer);
		/*empaqueta la clave como un buffer de caracteres con el siguiente formato:
		 * tipoClave|clave . tipoClave se guarda como un char .
		 * se retorna el tamanio del buffer empaquetado = sizeof(char)+tamanioClave */

		virtual void desempaquetar(char* buffer,unsigned short tamanioBuffer);
		/* reconstruye un objeto ClaveX a partir de un buffer de caracteres que tenga el
		 * formato: tipoClave|clave . */

		virtual bool es_incompatible_con(const ClaveX& clavex);
		/*retorna true si un objeto ClaveX es incompatible de tipo con otro objeto ClaveX*/


		virtual void set_tipo_clave(TipoClave tc);
		/*cambia el tipo de clave a manejar y resetea el dato de la clave*/


		virtual int set_clave(string clave);
		virtual int set_clave(int clave);
		/*se retorna RES_ERROR en caso que el tipoClave sea incompatible*/

		virtual bool operator<(const ClaveX& clavex);
		virtual bool operator>(const ClaveX& clavex);
		virtual bool operator==(const ClaveX& clavex);

};

#endif /* CLAVEX_H_ */
