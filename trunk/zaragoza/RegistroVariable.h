/*
 * RegistroVariable.h
 *
 *  Created on: Mar 22, 2013
 *      Author: martin
 */

#ifndef REGISTROVARIABLE_H_
#define REGISTROVARIABLE_H_

#include<iostream>
#include<sstream>
#include<cstring>

using namespace std;

class RegistroVariable {

	protected:
		unsigned int tamanio;
		char* buffer;

	public:
		static const unsigned int TAMANIO_MAXIMO= 1048576*2;/*2 Mb*/
		static const int EXITO= 1;
		static const int FRACASO= 0;
		static const int TAMANIO_EXCEDIDO= -1;
		static const char MARCA_BORRADO= '*';

		virtual int agregar_datos(char* datos,unsigned int tamanioDatos)throw();
		virtual void limpiar()throw();
		virtual int copiar_datos(char* copia,unsigned int tamanioCopia,unsigned int offset= 0)throw();
		virtual char* get_buffer()throw(){return buffer;}/*retorna el puntero al buffer*/
		virtual unsigned int get_tamanio()throw(){return tamanio;}/*devuelve el tamanio del registro*/
		virtual void mostrar()throw();
		virtual int guardar(ostream& stream)throw();
		virtual int emitir()throw();
		virtual int exportar(char* copia)throw();
		virtual int importar(char* copia)throw();
		virtual int eliminar()throw();
		virtual bool fue_eliminado()throw();

		virtual ~RegistroVariable();
		RegistroVariable();

};/*RegistroVariable
esta diseniado para guardar un buffer de mas de un mega de datos.*/

#endif /* REGISTROVARIABLE_H_ */
