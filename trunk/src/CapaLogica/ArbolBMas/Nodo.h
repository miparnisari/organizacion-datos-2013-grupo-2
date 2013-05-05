/*
 * Nodo.h
 *
 *  Created on: May 4, 2013
 *      Author: maine
 */

#ifndef NODO_H_
#define NODO_H_

class Nodo {
	public:
		Nodo(char tipoNodo);
		virtual ~Nodo();

	protected:
		char tipoNodo; //Si es interno o de secuencia
};

#endif /* NODO_H_ */
