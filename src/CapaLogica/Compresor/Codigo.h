/*
 * Codigo.h
 *
 *  Created on: Jun 6, 2013
 *      Author: martin
 */

#ifndef CODIGO_H_
#define CODIGO_H_

#include <iostream>
#include <exception>
#include "../../Constantes.h"
#include <climits>

using std::exception;
using std::string;


class Codigo {

	public:
		typedef int TipoIdentificador;

	private:
		unsigned long frecuencia;
		TipoIdentificador identificador;
		/*valor de identificacion de un codigo. La asociacion del codigo con un mensaje
		 * debe realizarse por el programa a partir de este valor de identificador.*/

	public:
		class OverflowException:public exception{
			virtual const char* what()const throw();
		};

		Codigo(TipoIdentificador identificador);
		Codigo(const Codigo& otro);
		Codigo(char identificador);
		/*este constructor asigna como identificador el valor ASCII de un char.*/
		~Codigo();

		TipoIdentificador get_identificador()const;
		unsigned long get_frecuencia()const;
		unsigned long incrementar_frecuencia(unsigned long aumento)throw(OverflowException);
		/*aumenta la frecuencia y retorna el valor incrementado*/

		double get_probabilidad(unsigned long frecuenciaTotal)const;
		/*calcula la probabilidad del codigo a partir de la frecuencia total*/

		unsigned long get_proporcion(unsigned long frecuenciaTotal,unsigned long rango)const;
		/*calcula el intervalo de valores que le corresponde de acuerdo a un rango. rango puede ser= techo - piso .*/

		bool operator ==(const Codigo& otro)const;
		bool operator >(const Codigo& otro)const;
		bool operator <(const Codigo& otro)const;
		bool operator >=(const Codigo& otro)const;
		bool operator <=(const Codigo& otro)const;


		bool operator ==(const TipoIdentificador& otroIdentificador)const;
		bool operator >(const TipoIdentificador& otroIdentificador)const;
		bool operator <(const TipoIdentificador& otroIdentificador)const;
		bool operator >=(const TipoIdentificador& otroIdentificador)const;
		bool operator <=(const TipoIdentificador& otroIdentificador)const;

		Codigo& operator =(const Codigo& otro);

};/*Este codigo sera una respresentacion abstracta de un mensaje . A un mensaje
se le asociara un codigo. La asociacion debera ser definida por el programa.
Ejemplo : las identificaciones de codigo son del tipo entero -> uno puede decidir
asociar el mensaje 'A' con el Codigo de identificador 4 si lo desease o con el
valor de ASCII de A = 65. */

#endif /* CODIGO_H_ */
