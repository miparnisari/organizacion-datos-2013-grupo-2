#ifndef BLOQUE_H_
#define BLOQUE_H_

#include <fstream>
#include <climits>
#include <sstream>
#include "RegistroVariable.h"
#include "../Constantes.h"

/*los registros se guardan de la siguietne manera: tamanioRegistro|datosRegistro */

class Bloque {

	protected:
		char* bufferBloque;
		unsigned int espacioLibre;
		/*espacio disponible en buffer. Esta variable ocupa espacio en el buffer*/
		unsigned int tamanioBloque;
		unsigned int espacioLibreOffset;

		unsigned int minNumeroRegistros;
		unsigned int maxNumeroRegistros;

		virtual void obtener_espacio_libre()const throw();
		virtual void escribir_espacio_libre()throw();
		virtual unsigned int obtener_offset_final()const throw();
		virtual unsigned int obtener_offset_registro(unsigned short numeroRegistro)const throw();
		virtual unsigned int calcular_resto_bloque(unsigned int offsetRegistro)const throw();
		virtual void limpiar_buffer()throw();

	public:
		Bloque(unsigned int tamBloque = BLOQUE_TAM_DEFAULT, unsigned int min = 0, unsigned int max = BLOQUE_TAM_DEFAULT);
		/*Si no se le pasa un tamanio, se le asignara BLOQUE_TAM_DEFAULT.*/
		virtual ~Bloque();



		virtual unsigned short get_cantidad_registros_almacenados()const throw();

		virtual unsigned int get_espacio_libre()const throw(){
			return this->espacioLibre;}

		unsigned int get_tamanio_bloque()const throw(){
			return this->tamanioBloque;}

		virtual char* get_buffer()const throw(){
			return this->bufferBloque;}

		virtual bool esta_vacio()const throw();

		virtual int recuperar_registro(RegistroVariable* registro, unsigned short numeroRegistro)throw();
		/*se recupera un registro de una posicion determinada y se retorna la cantidad de bytes leidos.
		 * En caso de ocurrir un error se retorna 0 o valores negativos. */
		/*Los registros se enumeran desde el 0!!!*/

		virtual int remover_registro(unsigned short numeroRegistro)throw();

		virtual int agregar_registro(RegistroVariable* registro)throw();
		/*se hace un APPEND de un registro, es decir que el mismo se agrega al final .*/

		virtual void desempaquetar(const char* datos)throw();
		/*reconstruye un Bloque a partir de un buffer de caracteres que contenga un bloque.
		 * "datos" tiene que tener un tamanio igual o superior al del objeto Bloque */
		virtual void empaquetar(char* copia)const throw();
		/*exporta el bloque como datos sin formato en la variable copia. La variable "copia" debe
		 * ser un buffer de caracteres reservados en memoria de tamanio igual o superior al tamanio
		 * del bloque.*/

		virtual bool fue_eliminado();
		/*verifica si un bloque se marco como eliminado .*/

		virtual int actualizar_ref_prox_bloque(unsigned int primerBloque);
		/* si el bloque NO ESTABA ELIMINADO, la funcion lo marca como eliminado y
		 * agrega el valor de referencia a proximo bloque libre , se retorna RES_OK .
		 * si el bloque ESTABA ELIMINADO , se actualiza la referencia al proximo bloque
		 * del mismo y se retorna RES_ERROR */

		virtual int obtener_ref_prox_bloque();
		/*si el bloque se marco como eliminado se retorna la referencia al proximo bloque
		 * de lo contrario se retorna RES_BLOQUE_OCUPADO*/



/*---------------------------------------------------------------------------------------------------*/
/*LOS SIGUIENTES METODOS SON PUBLICOS EXCLUSIVAMENTE POR CUESTION DE PRUEBAS O PARA CUMPLIR CON
 * CONSIGNAS DEL TP0. */

		/*LA SIGUIENTE PRIMITIVA ES USADA POR ManejadorBloques. Servia para resolver una consigna
		 * del TP0, NO PRESTAR ATENCION!!!*/
		virtual void listar_registros()throw();
		/*lista el byte offset de cada registro contenido en el bloque*/
		virtual unsigned int calcular_espacio_usable()const throw();
		/*calcula espacio asignado al bloque para guardar registros exclusivamente*/
		virtual unsigned int calcular_espacio_ocupado()const throw();
		/*calcula espacio ocupado por registros exclusivamente*/
		virtual unsigned int calcular_espacio_libre()const throw();
		/*este metodo puede ser alterado en clases hijas para agregar un colchon de amortiguacion de
		 * datos de 75% por ejemplo*/
		virtual int agregar_registro(char* dato,unsigned short tamanioDato)throw();
		//TODO hacerlo private

		virtual int agregar_registro(char* dato,unsigned short tamanioDato,unsigned short posicionRegistro)throw();
		/*en caso de no tener los registros guardados en variables RegistroVariable, es posible guardarlos desde un
		 * buffer de caracteres con datos .*/
		//TODO (by maine) creo que este metodo se deberia eliminar

		virtual int agregar_registro(RegistroVariable* registro, unsigned short posicionRegistro)throw();
		/*se agrega un registro en una posicion deseada. Los registros se enumeran desde el 0 */
		//TODO (by maine) creo que este metodo se deberia eliminar


};

#endif /* BLOQUE_H_ */
