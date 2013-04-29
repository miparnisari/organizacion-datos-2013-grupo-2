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
			int numerorPrimerRegistroLibre;
			unsigned int tamanioArchivo;
			unsigned int cantidadRegistros;
			unsigned int cantidadRegistrosLibres;
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
		virtual void _append_registro(RegistroVariable* registro);
		/*agrega un registro al final del archivo*/



	public:
		static const unsigned int OFFSET_PRIMER_REGISTRO= sizeof(header);

		ManejadorRegistrosVariables();

		virtual int crear_archivo(string nombreArchivo);
		/*intenta crear el archivo con el path nombreArchivo. En caso que el mismo
		 * exista, se retorna RES_ERROR.*/

		virtual int abrir_archivo(string nombreArchivo);
		/*se abre un archivo para su modificacion. Los datos se cargan al header .
		 * En caso de que el archivo no exista se retorna RES_ERROR*/

		virtual int agregar_registro(RegistroVariable* registro);
		/*agrega un registro donde haya espacio disponible en el archivo*/

		virtual int get_cantidad_registros();
		/*si el archivo no existiese  retorna RES_ERROR*/

		virtual long get_tamanio_archivo();
		/*retorna el tamanio del archivo, en caso que el mismo no exista, se retorna
		 * RES_ERROR*/

		virtual long get_registro(RegistroVariable* registro,
				unsigned short numeroRegistro);
		/*recupera un registro del archivo. En caso que el mismo no se hallase se retorna
		 * RES_ERROR . si el mismo es hallado se retorna el offset del registro en el
		 * archivo .
		 * Los registros se cuentan desde el 0! */


};

#endif /* MANEJADORREGISTROSVARIABLES_H_ */
