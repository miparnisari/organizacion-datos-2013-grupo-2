/*
 * ManejadorRegistrosVariables.h
 *
 *  Created on: Mar 22, 2013
 *      Author: martin
 */

#ifndef MANEJADORREGISTROSVARIABLES_H_
#define MANEJADORREGISTROSVARIABLES_H_

#include"ManejadorArchivos.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include"RegistroVariable.h"

using namespace std;

class ManejadorRegistrosVariables:public ManejadorArchivos {

	public:
		struct Header{
			long offsetPrimerRegistroLibre;
			unsigned int tamanioArchivo;
			unsigned int cantidadRegistros;/*es la cantidad de registros totales contando
			eliminados y no eliminados*/
			unsigned int cantidadRegistrosLibres;
		};
		struct HeaderRegistroLibre{
			unsigned short espacioLibre;
			char marcaBorrado;
			long offsetProximoRegistroLibre;
		};

	protected:
		Header header;
		string nombreArchivo;

		virtual void _leer_header();
		virtual void _cerrar_archivo(fstream* archivo);
		/*cierra el fstream. Se debe invocar para cada manipulacion del archivo*/
		virtual void _resetear_header();
		virtual void _guardar_header();
		virtual void _desvincular();
		/*desvincula el manejadorRegistrosVariables de el archivo que este manejando*/
		virtual long _append_registro(RegistroVariable* registro);
		/*agrega un registro al final del archivo y retorna el valor del offset donde
		 * se agrego*/
		virtual bool _registro_fue_eliminado(unsigned short numeroRegistro);
		virtual long _get_offset_registro(unsigned short numeroRegistro);


		virtual long _buscar_registro_libre(unsigned short espacioNecesario);
		/*retorna el valor de offset del primer registro libre con tamanio >=
		  espacioNecesario. En caso de no existir tal registro, se retorna RES_ERROR
		  NOTA: espacioNecesario de un registro deberia ser el espacio de empaquetamiento*/

		virtual void _pegar_registro(RegistroVariable* registro,long offset);
		/*esta primitiva se invoca en caso que el espacio libre de un registro libre
		 * coincide EXACTAMENTE con el tamanio de empaquetamiento del registro a insertar.*/



	public:
		static const unsigned int OFFSET_PRIMER_REGISTRO= sizeof(header);

		ManejadorRegistrosVariables();

		virtual int crear_archivo(string nombreArchivo);
		/*intenta crear el archivo con el path nombreArchivo. En caso que el mismo
		 * exista, se retorna RES_ERROR.*/

		virtual int abrir_archivo(string nombreArchivo);
		/*se abre un archivo para su modificacion. Los datos se cargan al header .
		 * En caso de que el archivo no exista se retorna RES_ERROR*/

		virtual long agregar_registro(RegistroVariable* registro);
		/*agrega un registro donde haya espacio disponible en el archivo*/

		virtual int get_cantidad_registros();
		/* retorna la CANTIDAD DE REGISTROS TOTAL DEL ARCHIVO (ocupados + eliminados) .
		 * Retorna RES_ERROR en caso que el archivo vinculado no exista*/
		virtual int get_cantidad_registros_ocupados();
		/*retorna la cantidad de registros ocupados con datos (no libres) */

		virtual long get_tamanio_archivo();
		/*retorna el tamanio del archivo, en caso que el mismo no exista, se retorna
		 * RES_ERROR*/

		virtual long get_registro(RegistroVariable* registro,
				unsigned short numeroRegistro);
		/*recupera un registro del archivo. En caso que el mismo no se hallase se retorna
		 * RES_ERROR . si el mismo es hallado se retorna el offset del registro en el
		 * archivo .
		 * Los registros se cuentan desde el 0! */

		virtual long eliminar_registro(unsigned short numeroRegistro);
		/*elimina en un registro en la posicion numeroRegistro. En caso de no poder
		 * hacerlo retorna RES_ERROR . En caso de lograrlo se retorna el offset del
		 * registro eliminado*/


};/*el archivo se manejara con la politica first-fit. Los espacios libres se particionan
para reducir la fragmentacion interna*/

#endif /* MANEJADORREGISTROSVARIABLES_H_ */
