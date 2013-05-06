#include "ManejadorDatosBasicos.h"

template<typename TipoDato>
ManejadorDatosBasicos<TipoDato>::ManejadorDatosBasicos()
	: ManejadorArchivos()
{
	file_handler = NULL;
	nombreArchivo = "";
}

template<typename TipoDato>
ManejadorDatosBasicos<TipoDato>::~ManejadorDatosBasicos()
{
}

template<typename TipoDato>
int ManejadorDatosBasicos<TipoDato>::abrir_archivo(std::string p_nombreArchivo, std::string modo)
{
	if (! utilitarios::validFileName(p_nombreArchivo))
		return RES_INVALID_FILENAME;
	nombreArchivo = p_nombreArchivo;
	file_handler = fopen(nombreArchivo.c_str(), modo.c_str());
	if (file_handler == NULL)
		return RES_ERROR;

	return RES_OK;

}/* Abre un archivo con un modo de apertura especifico.
Para info sobre los modos posibles: http://www.cplusplus.com/reference/cstdio/fopen/
*/

template<typename TipoDato>
int ManejadorDatosBasicos<TipoDato>::cerrar_archivo()
{
	if (file_handler == NULL)
		return RES_OK;
	int res = fclose(file_handler);
	file_handler = NULL;
	if (res == 0)
		return RES_OK;
	return RES_ERROR;
}

template<typename TipoDato>
int ManejadorDatosBasicos<TipoDato>::crear_archivo(std::string p_nombreArchivo, unsigned int tamBloque)
{
	if (! utilitarios::validFileName(p_nombreArchivo))
		return RES_INVALID_FILENAME;

	nombreArchivo = p_nombreArchivo;
	file_handler = fopen(nombreArchivo.c_str(), "wb+");
	if (file_handler == NULL)
		return RES_ERROR;

	return cerrar_archivo();
} /* Crea un archivo de bloques nuevo, sin bloques para usar.
Si el archivo ya existia, sus contenidos se pisaran. */

template<typename TipoDato>
int ManejadorDatosBasicos<TipoDato>::eliminar_archivo(std::string p_nombreArchivo)
{
	if (! utilitarios::validFileName(p_nombreArchivo))
		return RES_INVALID_FILENAME;
	int res = remove (p_nombreArchivo.c_str() );
	if (res == 0)
		return RES_OK;
	return RES_FILE_DOESNT_EXIST;
} /* Borra un archivo del disco. */

template<typename TipoDato>
int ManejadorDatosBasicos<TipoDato>::get_tamanio_archivo()
{
	if (abrir_archivo(nombreArchivo, "rb") == RES_OK)
	{
		fseek(file_handler,0,SEEK_END);
		int size = ftell(file_handler);
		cerrar_archivo();
		return size;
	}
	return RES_FILE_DOESNT_EXIST;
}

template <class TipoDato>
int ManejadorDatosBasicos<TipoDato>::agregar(const TipoDato dato)
{
	int posicion = get_tamanio_archivo();
	if (posicion == RES_ERROR)
		return RES_ERROR;
	if (fwrite(&dato,sizeof(dato),1,file_handler) != 1)
		return RES_ERROR;

	return posicion;

}/* PRECONDICION: abrir el archivo en modo "a".
Devuelve la posicion donde escribio el dato.
POSTCONDICION: cerrar el archivo.  */

template<typename TipoDato>
int ManejadorDatosBasicos<TipoDato>::escribir(const TipoDato dato, int pos)
{
	if (file_handler == NULL)
		return RES_ERROR;

	fseek(file_handler,pos,SEEK_SET);
	int res = fwrite(&dato,sizeof(dato),1,file_handler);
	if (res == 1)
		return RES_OK;
	return RES_ERROR;
}/* PRECONDICION: abrir el archivo en modo "wb+".
POSTCONDICION: cerrar el archivo. */

template<typename TipoDato>
int ManejadorDatosBasicos<TipoDato>::leer(TipoDato dato, int pos)
{
	if (file_handler == NULL)
		return RES_ERROR;

	fseek(file_handler,pos,SEEK_SET);
	int res = fread(&dato,sizeof(dato),1,file_handler);
	if (res == 1)
		return RES_OK;
	return RES_ERROR;
}/* PRECONDICION: abrir el archivo en modo "rb+".
POSTCONDICION: cerrar el archivo. */

