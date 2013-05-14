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

	/*recupero la clave del registro a buscar*/

	ClaveX unaClave;
	unsigned short indiceClave= 0;

	while( nodoBuscar->get_clave(indiceClave,unaClave)!= RES_ERROR ){

		if(unaClave > registro->get_clave()){
			nodoBuscar->get_hijo(hijoCorrespondiente,indiceClave);
			return RES_OK;
		}/*si la clave del nodo que estoy examinando es mayor que la clave de
		parametro -> el hijo correspondiente es el izquierdo a aquel que examino.*/

		if(unaClave == registro->get_clave()){
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


int ArbolBMas::_split_hoja(NodoSecuencial* nodoActual,RegistroClave* registro,
		TipoHijo& hijoPromocionado){

	return RES_OK;

}//TODO


int ArbolBMas::_insertar_recursivo_hoja(Bloque* bloqueActual ,
		RegistroClave* registro , TipoHijo& hijoPromocionado){

	const unsigned int MIN_BYTES=0,MAX_BYTES=3000;//FIXME eliminar constantes
	NodoSecuencial nodoActual(MIN_BYTES,MAX_BYTES);
	nodoActual.desempaquetar(bloqueActual);
	delete[] bloqueActual;
	/*cargo el bloqueActual en el nodo secuencial y libero al bloque*/

	vector<RegistroClave> registrosOverflow;
	int resultadoInsercion= nodoActual.insertar(*registro,registrosOverflow);

	if(resultadoInsercion== RES_OK)
		return RES_OK;
	if(resultadoInsercion== RES_RECORD_EXISTS)
		return RES_RECORD_EXISTS;

	_split_hoja(&nodoActual,registro,hijoPromocionado);
	/*en registro se guardara el registro promocionado*/

	return RES_OVERFLOW;

}



int ArbolBMas::_split_interno(NodoInterno* nodo,ClaveX* clavePromocionada,
		TipoHijo& bloquePromocionado){

	return RES_OK;

}



int ArbolBMas::_insertar_recursivo(unsigned int& numeroBloqueActual ,
		RegistroClave* registro , TipoHijo& hijoPromocionado,ClaveX* clavePromocionada)
{

	Bloque* bloqueActual= this->archivoNodos.obtener_bloque(numeroBloqueActual);
	NodoArbol nodoActual(TIPO_HOJA);
	nodoActual.desempaquetar(bloqueActual);

	if(nodoActual.es_hoja()){

		int resultadoInsercion=
				_insertar_recursivo_hoja(bloqueActual,registro,hijoPromocionado);

		if(resultadoInsercion== RES_OVERFLOW){
			ClaveX claveRegistroPromocionado= registro->get_clave();
			(*clavePromocionada)= claveRegistroPromocionado;
		}

		return resultadoInsercion;

	}/*si el nodo actual es del tipo hoja, insertar el registro. En caso de ocurrir un overflow se guarda en clavePromocionada
	la clave del registro promocionado.*/


	NodoInterno nodoActualInterno;//TODO agregar el tamanio apropiado del nodo interno en minCantBytes y maxCantBytes
	nodoActualInterno.desempaquetar(bloqueActual);
	delete[] bloqueActual;
	unsigned int numeroBloqueHijo;
	this->_hallar_hijo_correspondiente(registro,&nodoActualInterno,
			numeroBloqueHijo);
	/*busco el siguiente hijo*/

	int resultadoInsercion= this->_insertar_recursivo(numeroBloqueHijo,registro,
			hijoPromocionado,clavePromocionada);

	if(resultadoInsercion== RES_OK)
		return RES_OK;/*no hay promocion / overflow*/
	if(resultadoInsercion== RES_RECORD_EXISTS)
		return RES_RECORD_EXISTS;/*registro que se busco insertar ya existia -> se retorna como error RES_RECORD_EXISTS*/


	/*si el resultado de la insercion no es ninguno de los superiores -> ocurrio un overflow**/
	unsigned short ocurrenciaInsercion;
	nodoActualInterno.insertar_clave(*clavePromocionada,ocurrenciaInsercion);
	int resultadoInsercionNodoInterno= nodoActualInterno.insertar_hijo_derecho(*clavePromocionada,hijoPromocionado);

	if(resultadoInsercionNodoInterno== RES_OK)
		return RES_OK;
	if(resultadoInsercionNodoInterno== RES_ERROR)
		return RES_ERROR;

	_split_interno(&nodoActualInterno,clavePromocionada,hijoPromocionado);
	/*realiza el split. y guarda el nodo nuevo en el archivo*/


	return RES_OVERFLOW;


}
