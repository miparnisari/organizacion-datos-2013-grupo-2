/*
 * ClaveX.h
 *
 *  Created on: Apr 27, 2013
 *      Author: martin
 */

#ifndef CLAVEX_H_
#define CLAVEX_H_

#include "../../Constantes.h"
#include "ClaveString.h"
#include "ClaveNumerica.h"

class ClaveX {

	public:

	protected:
		TipoClave tipoClave;
		Clave* claves[CANTIDAD_TIPOS_CLAVE];

		void _resetear_datos();

	public:
		ClaveX();
		/*por defecto una ClaveX es de string*/
		ClaveX( const ClaveX& clavex );

		ClaveX& operator =(const ClaveX& otro);

		~ClaveX();

		virtual int empaquetar(char* buffer);
		/*empaqueta la clave como un buffer de caracteres con el siguiente formato:
		 * tipoClave|clave . tipoClave se guarda como un char .
		 * se retorna el tamanio del buffer empaquetado = sizeof(char)+tamanioClave */

		virtual void desempaquetar(char* buffer,unsigned short tamanioBuffer);
		/* reconstruye un objeto ClaveX a partir de un buffer de caracteres que tenga el
		 * formato: tipoClave|clave . tamanioBuffer debe ser efectivamente el tamanio en
		 * bytes del buffer (tamanioClave + tamanioTipoClave) */

		virtual TipoClave get_tipo_clave()const ;

		virtual void imprimir_dato()const;

		virtual void set_clave(string clave);
		virtual void set_clave(int clave);
		/*se establece el valor de la clave y su tipo cambia automaticamente.
		 * Si la clave era CLAVE_NUMERICA y se invoca set_clave(string) , tipoClave
		 * cambia a CLAVE_STRING y la clave es guardada*/
		virtual int get_tamanio_empaquetado()const;
		virtual int get_tamanio_clave()const;

		virtual int get_clave(int &clave)const;
		virtual int get_clave(string& clave)const;
		/*se recupera la clave en el formato deseado. En caso que tipoClave sea
		 * incompatible se retorna RES_ERROR y el parametro pasado no se altera*/

		virtual bool operator <(const ClaveX& clavex)const;
		virtual std::ostream& operator << (std::ostream& os)const;
		virtual bool operator >(const ClaveX& clavex)const;
		virtual bool operator >=(const ClaveX& clavex)const;
		virtual bool operator <=(const ClaveX& clavex)const;
		virtual bool operator ==(const ClaveX& clavex)const;
		virtual bool operator !=(const ClaveX& clavex)const;


};

#endif /* CLAVEX_H_ */
