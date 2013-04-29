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

		ClaveX(TipoClave tipoClave= CLAVE_STRING);
		/*por defecto una ClaveX es de string*/

		virtual ~ClaveX();

		virtual int empaquetar(char* buffer);
		/*empaqueta la clave como un buffer de caracteres con el siguiente formato:
		 * tipoClave|clave . tipoClave se guarda como un char .
		 * se retorna el tamanio del buffer empaquetado = sizeof(char)+tamanioClave */

		virtual void desempaquetar(char* buffer,unsigned short tamanioBuffer);
		/* reconstruye un objeto ClaveX a partir de un buffer de caracteres que tenga el
		 * formato: tipoClave|clave . */


		virtual void set_tipo_clave(TipoClave tc);
		/*cambia el tipo de clave a manejar y resetea el dato de la clave*/

		virtual TipoClave get_tipo_clave();


		virtual void set_clave(string clave);
		virtual void set_clave(int clave);
		/*se establece el valor de la clave y su tipo cambia automaticamente.
		 * Si la clave era CLAVE_NUMERICA y se invoca set_clave(string) , tipoClave
		 * cambia a CLAVE_STRING y la clave es guardada*/

		virtual bool operator<(const ClaveX& clavex);
		virtual bool operator>(const ClaveX& clavex);
		virtual bool operator==(const ClaveX& clavex);

};

#endif /* CLAVEX_H_ */
