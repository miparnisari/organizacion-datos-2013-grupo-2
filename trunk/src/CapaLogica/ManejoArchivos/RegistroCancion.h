#ifndef REGISTROCANCION_H_
#define REGISTROCANCION_H_

#include <stack>
#include "../Compresor/Compresor.h"
#include "../../CapaFisica/RegistroVariable.h"
#include "Idioma.h"
#include "Anio.h"

class RegistroCancion : public RegistroVariable{

	protected:
		std::string* 		autores;
		unsigned short		cantidadAutores;
		Idioma				idioma;
		Anio 				anioGrabacion;
		string 				titulo;
		string 				letra;
		unsigned short 	cantidadParametros;

		virtual void limpiar_autores()throw();
		virtual unsigned short obtener_autores()throw();
		/*recupera los autores en un arreglo de strings. Retorna la cantidad de autores
		 * recuperados*/


		
	public:
		RegistroCancion();
		~RegistroCancion();
		
		/*METODOS DE ACCESO PUBLICO --------------------------------------------------------*/

		string generar_clave()const throw();
		/*una clave unica de identificacion de la cancion se generara a partir de los
		 * parametros de la misma.*/
		int cargar(const char* dato,unsigned short tamanioDato)throw();
		/*se carga una cancion como una cadena de caracteres que contiene en la primer linea
		 * los parametros de la cancion y en las lineas subsiguientes la letra. */

		unsigned short get_cantidad_autores()const throw();
		string get_autor(unsigned short numeroAutor)throw();
		int get_anio()throw();
		string get_idioma()throw();
		string get_letra()throw();
		string get_titulo()throw();
		/*gets*/

		void limpiar_buffer()throw();
		/*limpia el buffer y restaura el valor de las variables a sus respectivos DEFAULTS*/
		unsigned short get_cantidad_parametros();
		
		/*METODOS DE ACCESO PUBLICO --------------------------------------------------------*/

		virtual unsigned short contar_parametros()throw();
		/*cuenta la cantidad de parametros de la cancion.*/
		virtual bool obtener_parametro(unsigned short numeroParametro,string& parametro)throw();
		/*recupera un determinado parametro. los mismos se cuentan desde el 0*/
		virtual int obtener_anio_grabacion()throw();
		/*recupera el anio de grabacion de la cancion.
		 * En caso que no este incluido en la linea, se retorna -1*/
		virtual bool obtener_titulo()throw();
		/*recupera el titulo de la linea de caracteres y lo almacena en la variable titulo*/
		virtual bool obtener_idioma()throw();
		/*recupera el idioma de la cadena de caracteres y lo almacena en la variable idioma*/
		virtual bool obtener_letra()throw();
		/*recupera la letra de la cancion y la almacena en la variable letra*/
		int empaquetar(char* copia) throw();
		int desempaquetar(const char* copia) throw();


		int comprimir (Compresor & compresor);
		/*cuenta la cantidad de parametros en la linea.*/
};

#endif /* REGISTROCANCION_H_ */
