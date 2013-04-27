/*
 * GeneradorClave.h
 *
 *  Created on: Apr 12, 2013
 *      Author: martin
 */

#ifndef GENERADORCLAVE_H_
#define GENERADORCLAVE_H_

template<class TipoClave>
class GeneradorClave {

	public:
		virtual TipoClave generar_clave()const throw()= 0;
		virtual ~GeneradorClave(){};

};

#endif /* GENERADORCLAVE_H_ */
