#include "ArbolBMas.h"

ArbolBMas::ArbolBMas()
{
	raiz = NULL;
}
ArbolBMas::~ArbolBMas()
{
	delete raiz;
}

int ArbolBMas::crear (std::string dir, std::string fileName)
{
	unsigned int tamanioBloque = BLOQUE_TAM_DEFAULT;
	unsigned int minCantidadBytes = 0;
	unsigned int maxCantidadBytes = 0;
	Bloque* bloque = new Bloque(tamanioBloque);
	raiz = new NodoInterno(minCantidadBytes, maxCantidadBytes);
	raiz->empaquetar(bloque);
	return RES_OK;
}

int ArbolBMas::eliminar (std::string dir, std::string fileName)
{
	return RES_OK;
}

int ArbolBMas::abrir (std::string dir, std::string fileName, std::string mode)
{
	return RES_OK;
}

int ArbolBMas::cerrar ()
{
	return RES_OK;
}

int ArbolBMas::agregar(RegistroVariable & reg)
{
	return RES_OK;
}

int ArbolBMas::eliminar(RegistroVariable & reg)
{
	return RES_OK;
}

int ArbolBMas::buscar(RegistroClave & reg)
{
	return 0;
}


int ArbolBMas::_hallar_hijo_correspondiente(RegistroClave* registro,
		NodoInterno* nodoBuscar,TipoHijo& hijoCorrespondiente){

	if(registro== NULL || nodoBuscar== NULL)
		return RES_ERROR;

	ClaveX claveRegistro;
	registro->get_clave(claveRegistro);
	/*recupero la clave del registro a buscar*/

	ClaveX unaClave;
	unsigned short indiceClave= 0;

	while( nodoBuscar->get_clave(indiceClave,unaClave)!= RES_ERROR ){

		if(unaClave > claveRegistro){
			nodoBuscar->get_hijo(hijoCorrespondiente,indiceClave);
			return RES_OK;
		}/*si la clave del nodo que estoy examinando es mayor que la clave de
		parametro -> el hijo correspondiente es el izquierdo a aquel que examino.*/

		if(unaClave == claveRegistro){
			nodoBuscar->get_hijo(hijoCorrespondiente,indiceClave+1);
			return RES_OK;
		}/*si la clave que estoy examinando es identica a la clave del registro de
		parametro -> el hijo correspondiente es el derecho de la clave examinada*/

		indiceClave++;

	}


	nodoBuscar->get_hijo(hijoCorrespondiente,indiceClave+1);
	/*en caso que la clave del registro sea mayor que todas las claves del nodo ->
	 * el hijo correspondiente a examinar es el de posicion ultima*/

	return RES_OK;


}


int ArbolBMas::_hallar_hoja(RegistroClave* registro,
		unsigned int& numeroBloque){

	if(registro== NULL)
		return RES_ERROR;

	NodoArbol na(TIPO_INTERNO);
	Bloque* bloqueActual= archivoNodos.obtener_bloque(numeroBloque);
	if(bloqueActual== NULL)
		return RES_ERROR;
	na.desempaquetar(bloqueActual);

	if( na.es_hoja() )
		return RES_OK;

	NodoInterno ni;
	ni.desempaquetar(bloqueActual);
	TipoHijo numeroBloqueHijoCorrespondiente;
	this->_hallar_hijo_correspondiente(registro,&ni,numeroBloqueHijoCorrespondiente);
	numeroBloque= numeroBloqueHijoCorrespondiente;

	return _hallar_hoja(registro,numeroBloque);


}



int ArbolBMas::_insertar_recursivo(unsigned int& numeroBloqueActual ,
		RegistroClave* registro , TipoHijo& hijoPromocionado){




}
