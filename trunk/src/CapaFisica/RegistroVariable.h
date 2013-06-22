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
		TamanioCampos tamanio;
		char* buffer;

		virtual int seek_numero_campo(TamanioCampos numeroCampo);

		virtual std::string mostrar_campo(TamanioCampos numeroCampo);
		void _inicializar_buffer();

	public:
		static const TamanioCampos CAMPO_ENCADENAMIENTO_LIBRES= 1;

		RegistroVariable();
		RegistroVariable& operator = (const RegistroVariable& otro);
		virtual ~RegistroVariable();
		RegistroVariable(const RegistroVariable& otro);

		virtual TamanioCampos get_cantidad_campos()throw();
		virtual char* get_buffer()throw();
		virtual TamanioCampos get_tamanio()throw();

		virtual int eliminar()throw();


		virtual int agregar_campo(const char* campo,TamanioCampos tamanioCampo)throw();
		/*agrega (append) un campo al registro : tamanioCampo|datosCampo */

		virtual int get_tamanio_campo(TamanioCampos numeroCampo);

		virtual int recuperar_campo(char* copia,TamanioCampos numeroCampo)throw();
		/*recupera un campo en la posicion numeroCampo , el mismo es almacenado en el buffer copia.
		 * Se retorna el tamanio del campo leido*/

		virtual void limpiar_campos()throw();

		virtual int empaquetar(char* copia)throw();
		/*exporta los datos del registro variable como un buffer de caracteres con el siguiente
		 * formato: tamanioRegistro|datosRegistro */
		virtual int get_tamanio_empaquetado()const;
		/*retorna el tamanio del registroVariable junto con el size del tamanio del mismo
		 * es decir : get_tamanio_empaquetado = sizeof(tamanio) + tamanio ; */

		virtual int desempaquetar(const char* copia)throw();
		/*construye un objeto RegistroVariable a partir de un registro variable contenido en un buffer
		 * de caracteres "copia" */

		virtual bool fue_eliminado()throw();

		RegistroVariable* comprimir (Compresor * compresor);
		int descomprimir(Compresor * compresor, RegistroVariable* reg_descomp);

		virtual bool esta_limpio(){return tamanio==0;};
		/*retorna true si el registru fue limpiado y el mismo no se cargo de nuevo con datos.*/

/*--------------------------------------------------------------------------------------------------*/
/*LOS SIGUIENTES METODOS SE CREARON POR CUESTION DE PRUEBAS O PARA CUMPLIR CON CONSIGNAS DEL TP0*/

		virtual std::string mostrar()throw();
		virtual int agregar_datos(const char* datos,TamanioCampos tamanioDatos)throw();
		/*agrega datos al final del registro (append) sin agregar delimitador de tamanio del
		 * dato agregado*/
		// TODO eliminarla? es lo mismo q agregar un campo..

};
#endif /* REGISTROVARIABLE_H_ */
