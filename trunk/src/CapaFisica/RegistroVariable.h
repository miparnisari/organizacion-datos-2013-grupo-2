/*
 * RegistroVariable.h
 *
 *  Created on: Mar 22, 2013
 *      Author: martin
 */

#ifndef REGISTROVARIABLE_H_
#define REGISTROVARIABLE_H_

#include <iostream>
#include <sstream>
#include <cstring>
#include "../Constantes.h"
#include "../CapaLogica/Compresor/Compresor.h"

using namespace std;

class RegistroVariable {

	protected:
		unsigned short tamanio;
		char* buffer;

		virtual int remover_datos(unsigned short offset,unsigned short tamanioBorrar)throw();
		/*remueve una parte de los datos*/
		virtual int seek_numero_campo(unsigned short numeroCampo);

		virtual std::string mostrar_campo(unsigned short numeroCampo);

	public:
		RegistroVariable();
		virtual ~RegistroVariable();

		virtual unsigned short get_cantidad_campos()throw();
		virtual char* get_buffer()throw();
		virtual unsigned short get_tamanio()throw();

		virtual int eliminar()throw();


		virtual int agregar_campo(const char* campo,unsigned short tamanioCampo)throw();
		/*agrega (append) un campo al registro : tamanioCampo|datosCampo */

		virtual int get_tamanio_campo(unsigned short numeroCampo);

		virtual int recuperar_campo(char* copia,unsigned short numeroCampo)throw();
		/*recupera un campo en la posicion numeroCampo , el mismo es almacenado en el buffer copia.
		 * Se retorna el tamanio del campo leido*/

		virtual void limpiar_buffer()throw();

		virtual int empaquetar(char* copia)throw();
		/*exporta los datos del registro variable como un buffer de caracteres con el siguiente
		 * formato: tamanioRegistro|datosRegistro */
		virtual int get_tamanio_empaquetado();
		/*retorna el tamanio del registroVariable junto con el size del tamanio del mismo
		 * es decir : get_tamanio_empaquetado = sizeof(tamanio) + tamanio ; */

		virtual int desempaquetar(const char* copia)throw();
		/*construye un objeto RegistroVariable a partir de un registro variable contenido en un buffer
		 * de caracteres "copia" */

		virtual bool fue_eliminado()throw();

		int comprimir (Compresor & compresor);

		bool esta_limpio(){return tamanio==0;};
		/*retorna true si el registru fue limpiado y el mismo no se cargo de nuevo con datos.*/

/*--------------------------------------------------------------------------------------------------*/
/*LOS SIGUIENTES METODOS SE CREARON POR CUESTION DE PRUEBAS O PARA CUMPLIR CON CONSIGNAS DEL TP0*/

		virtual int emitir()throw();

		virtual std::string mostrar()throw();
		virtual int agregar_datos(const char* datos,unsigned short tamanioDatos)throw();
		/*agrega datos al final del registro (append) sin agregar delimitador de tamanio del
		 * dato agregado*/
		// TODO eliminarla? es lo mismo q agregar un campo..
		virtual int guardar(ostream& stream)throw();
		/*guarda los datos del registro variable en un objeto ostream: tamanioRegsitro|datosRegistro */

};/*RegistroVariable
*/

#endif /* REGISTROVARIABLE_H_ */
