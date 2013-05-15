#ifndef MANEJADORDATOSBASICOS_H_
#define MANEJADORDATOSBASICOS_H_

#include "ManejadorArchivos.h"
#include <errno.h>
#include <string.h>

struct mdb_header
{
	unsigned int cantidadDatosGuardados;
};

template<typename TipoDato>
class ManejadorArchivoDatosBasicos: public ManejadorArchivos {

	public:
		ManejadorArchivoDatosBasicos()
			: ManejadorArchivos()
		{
			file_handler = NULL;
			nombreArchivo = "";
			header.cantidadDatosGuardados = 0;
		}

		virtual ~ManejadorArchivoDatosBasicos()
		{
		}

		int truncar(long cantidadDatosDeseados)
		{
			if (file_handler == NULL)
				return RES_ERROR;

			int res = ftruncate(fileno(file_handler), cantidadDatosDeseados*sizeof(TipoDato));
			if (res == 0)
			{
				header.cantidadDatosGuardados = cantidadDatosDeseados;
				return RES_OK;
			}

			std::cout << strerror(errno) << std::endl;
			return RES_ERROR;
		}/* PRECONDICION: abrir el archivo en modo "rb+". */

		int agregar(const TipoDato dato)
		{
			int posicion = header.cantidadDatosGuardados;
			if (posicion == RES_ERROR)
				return RES_ERROR;
			fseek(file_handler,posicion*sizeof(TipoDato)+sizeof(header),SEEK_SET);
			int res = fwrite(&dato,sizeof(dato),1,file_handler);
			if (res != 1)
				return RES_ERROR;

			header.cantidadDatosGuardados ++;
			return posicion;

		}/* PRECONDICION: abrir el archivo en modo "ab".
		Devuelve la posicion donde escribio el dato.
		POSTCONDICION: cerrar el archivo.  */
		int escribir(const TipoDato dato, int pos)
		{
			if (file_handler == NULL)
				return RES_ERROR;

			fseek(file_handler,pos*sizeof(TipoDato)+sizeof(header),SEEK_SET);
			int res = fwrite(&dato,sizeof(dato),1,file_handler);
			if (res != 1)
				return RES_ERROR;

			header.cantidadDatosGuardados ++;

			return RES_OK;
		}/* PRECONDICION: abrir el archivo en modo "wb+".
		POSTCONDICION: cerrar el archivo. */
		
		int leer(TipoDato* dato, unsigned int pos)
		{
			if (pos >= header.cantidadDatosGuardados)
				return RES_ERROR;
			if (file_handler == NULL)
				return RES_ERROR;

			int res = fseek(file_handler,pos*sizeof(TipoDato)+sizeof(header),SEEK_SET);
			if (res != 0)
				return RES_ERROR;
			res = fread(dato,sizeof(TipoDato),1,file_handler);
			if (res == 1)
				return RES_OK;
			return RES_ERROR;
		}/* PRECONDICION: abrir el archivo en modo "rb+".
		POSTCONDICION: cerrar el archivo. */

		int crear_archivo(std::string p_nombreArchivo)
		{
			if (! utilitarios::validFileName(p_nombreArchivo))
				return RES_INVALID_FILENAME;

			nombreArchivo = p_nombreArchivo;
			file_handler = fopen(nombreArchivo.c_str(), "wb+");
			if (file_handler == NULL)
				return RES_ERROR;

			int res = __set_header();
			if (res == RES_ERROR)
				return RES_ERROR;

			res = cerrar_archivo();
			return res;
		} /* Crea un archivo nuevo, sin datos.
		Si el archivo ya existia, sus contenidos se pisaran. */

		int eliminar_archivo(std::string p_nombreArchivo)
		{
			if (! utilitarios::validFileName(p_nombreArchivo))
				return RES_INVALID_FILENAME;
			int res = remove (p_nombreArchivo.c_str() );
			if (res == 0)
				return RES_OK;
			return RES_FILE_DOESNT_EXIST;
		} /* Borra un archivo del disco. */

		int abrir_archivo(std::string p_nombreArchivo, std::string modo)
		{
			if (! utilitarios::validFileName(p_nombreArchivo))
			{
				std::cout << "MADB: invalid filename." << std::endl;
				return RES_INVALID_FILENAME;
			}
			nombreArchivo = p_nombreArchivo;
			file_handler = fopen(nombreArchivo.c_str(), modo.c_str());
			if (file_handler == NULL)
				return RES_ERROR;

			return __get_header();

		}/* Abre un archivo con un modo de apertura especifico.
		Para info sobre los modos posibles: http://www.cplusplus.com/reference/cstdio/fopen/
		*/
		int cerrar_archivo()
		{
			if (file_handler == NULL)
				return RES_OK;
			__set_header();
			int res = fclose(file_handler);
			file_handler = NULL;
			if (res == 0)
				return RES_OK;
			return RES_ERROR;
		}/* Cierra el archivo. */

		int get_tamanio_archivo()
		{
			return header.cantidadDatosGuardados*sizeof(TipoDato);
		}

	private:
		struct mdb_header header;
		FILE* file_handler;
		std::string nombreArchivo;
		int __get_header()
		{
			int res = fseek(this->file_handler,0,SEEK_SET);
			if (res != 0)
				return RES_ERROR;

			res = fread(&(this->header),sizeof(this->header),1,this->file_handler);
			if (res != 1)
				return RES_ERROR;

			return RES_OK;
		}/*PRECONDICION: el archivo debe estar abierto. */

		int __set_header()
		{
			int res = fseek(this->file_handler,0,SEEK_SET);
			if (res != 0)
				return RES_ERROR;

			res = fwrite(&(this->header),sizeof(this->header),1,this->file_handler);
			if (res != 1)
				return RES_ERROR;

			return RES_OK;
		}/*PRECONDICION: el archivo debe estar abierto. */
		
};

#endif /* MANEJADORDATOSBASICOS_H_ */
