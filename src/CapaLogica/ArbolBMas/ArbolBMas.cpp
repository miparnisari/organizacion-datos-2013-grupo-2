#include "ArbolBMas.h"

ArbolBMas::ArbolBMas()
{
	numeroBloqueRaiz = 1; // el bloque 0 tiene el header!
	raiz = NULL;
	header.maxCantBytesClaves = 0;
	header.minCantBytesClaves = 0;
}

ArbolBMas::~ArbolBMas()
{
	delete raiz;
}

int ArbolBMas::_set_header()
{
	// El bloque 0 debe tener datos del arbol
	Bloque* bloqueHeader = archivoNodos.crear_bloque();
	if (bloqueHeader == NULL)
		return RES_ERROR;
	RegistroVariable registroHeader;
	registroHeader.agregar_campo((char*)&header.minCantBytesClaves,sizeof(header.minCantBytesClaves));
	registroHeader.agregar_campo((char*)&header.maxCantBytesClaves,sizeof(header.maxCantBytesClaves));
	int res = bloqueHeader->agregar_registro(&registroHeader);
	if (res != RES_OK) {
		delete bloqueHeader;
		return RES_ERROR;
	}
	res = archivoNodos.sobreescribir_bloque(bloqueHeader,0);
	delete bloqueHeader;
	if (res != RES_OK)
		return RES_ERROR;

	// El bloque 1 debe tener la raiz
	unsigned int minCantidadBytes = 0; //FIXME CANTIDADES PARA TESTEAR!!!
	unsigned int maxCantidadBytes = 4096; //FIXME
	Bloque* bloqueRaiz = archivoNodos.crear_bloque();
	raiz = new NodoInterno(minCantidadBytes, maxCantidadBytes);
	raiz->empaquetar(bloqueRaiz);

	res = archivoNodos.sobreescribir_bloque(bloqueRaiz,1);

	delete bloqueRaiz;

	return res;
}/* PRECONDICION: el archivo debe estar abierto.
POSTCONDICION: se debe cerrar el archivo. */

int ArbolBMas::_get_header()
{

	return RES_OK;
}

int ArbolBMas::crear (std::string fileName, unsigned int tamNodo)
{
	int res = archivoNodos.crear_archivo(fileName,tamNodo);
	if (res != RES_OK)
		return res;

	res = archivoNodos.abrir_archivo(fileName,"wb+");
	if (res == RES_OK)
		return _set_header();
	return RES_ERROR;
}

int ArbolBMas::eliminar (std::string fileName)
{
	return archivoNodos.eliminar_archivo(fileName);
}

int ArbolBMas::abrir (std::string fileName, std::string mode)
{
	return RES_OK;
}

int ArbolBMas::cerrar ()
{
	return RES_OK;
}

int ArbolBMas::agregar(RegistroClave & reg)
{
	//TODO agregar caso que arbol este vacio (se crea un arbol NUEVO) y se busca insertar el primer registro

	TipoHijo raiz= this->numeroBloqueRaiz;
	TipoHijo hijoPromocionado;
	ClaveX clavePromocionada;
	return this->_insertar_recursivo(raiz,&reg,hijoPromocionado,&clavePromocionada);

	//TODO resolver caso que _insertar_recursivo retorne finalmente OVERFLOW -> se debe crear nueva raiz

}

int ArbolBMas::eliminar(RegistroClave & reg)
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
	ClaveX claveRegistro= registro->get_clave();
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


int ArbolBMas::_split_hoja(NodoSecuencial* nodoActual,vector<RegistroClave>* registrosOverflow,
		TipoHijo& hijoPromocionado,ClaveX* clavePromocionada){

	NodoSecuencial nodoSecuencialNuevo(header.minCantBytesClaves,header.maxCantBytesClaves);
	const unsigned short CANTIDAD_REGISTROS_OVERFLOW= registrosOverflow->size();
	if(CANTIDAD_REGISTROS_OVERFLOW== 0)
		return RES_ERROR;
	(*clavePromocionada)= registrosOverflow->at(0).get_clave();

	vector<RegistroClave> ro;
	for(int i=0;i<CANTIDAD_REGISTROS_OVERFLOW;i++){
		nodoSecuencialNuevo.insertar(registrosOverflow->at(i),ro);
	}/*en el nodo secuencial nuevo agrego los registros en overflow y remuevo dichos registros del nodoSecuencialActual*/

	Bloque bloqueNodoSecuencialNuevo;
	nodoSecuencialNuevo.empaquetar(&bloqueNodoSecuencialNuevo);
	int resultadoEscritura= this->archivoNodos.escribir_bloque(&bloqueNodoSecuencialNuevo);

	if(resultadoEscritura!= RES_ERROR)
		hijoPromocionado= (unsigned int)resultadoEscritura;

	return resultadoEscritura;

}//TODO




int ArbolBMas::_split_interno(NodoInterno* nodo,ClaveX* clavePromocionada,
		TipoHijo& bloquePromocionado){

	NodoInterno nodoNuevo;
	nodoNuevo.limpiar();
	/*remuevo el hijo x defecto del nodo nuevo*/
	Bloque bloqueNodoNuevo;


	const unsigned short CANTIDAD_HIJOS= nodo->get_cantidad_hijos();
	const unsigned short CANTIDAD_CLAVES= nodo->get_cantidad_claves();


	ClaveX claveMitad;
	/*guardo la clavePromocionada*/
	unsigned short numeroClaveMitad;
	nodo->get_clave_mitad(claveMitad);
	nodo->buscar_clave(claveMitad , numeroClaveMitad);
	(*clavePromocionada) = claveMitad;


	for(unsigned short i=numeroClaveMitad+1;i<CANTIDAD_CLAVES;i++){
		ClaveX unaClave;
		nodo->get_clave(i,unaClave);
		unsigned short ocurrenciaInsercion;
		nodoNuevo.insertar_clave(unaClave,ocurrenciaInsercion);
	}/*obtengo las claves que tengo que pasar al nodo nuevo (posteriores a la clave de la mitad)*/


	for(unsigned short i=numeroClaveMitad;i<CANTIDAD_HIJOS;i++){
		TipoHijo unHijo;
		nodo->get_hijo(unHijo , i+1);
		nodoNuevo.insertar_hijo(unHijo);
	}/*obtengo los hijos que se deben insertar en el nodo nuevo*/


	for(unsigned short i=numeroClaveMitad;i<CANTIDAD_CLAVES;i++){
		ClaveX unaClave;
		nodo->remover_clave(numeroClaveMitad,unaClave);
		nodo->remover_hijo(numeroClaveMitad+1);
	}/*remuevo las claves e hijos del nodo en overflow*/


	nodoNuevo.empaquetar(&bloqueNodoNuevo);
	int resEscritura= this->archivoNodos.escribir_bloque(&bloqueNodoNuevo);

	if(resEscritura!= RES_ERROR)
		bloquePromocionado= resEscritura;



	return resEscritura;

}



int ArbolBMas::_insertar_recursivo(unsigned int& numeroBloqueActual ,
		RegistroClave* registro , TipoHijo& hijoPromocionado,ClaveX* clavePromocionada)
{

	Bloque* bloqueActual= this->archivoNodos.obtener_bloque(numeroBloqueActual);
	NodoArbol nodoActual(TIPO_HOJA);
	nodoActual.desempaquetar(bloqueActual);
	/*desempaqueto el bloque para verificar que tipo de nodo es*/

	if(nodoActual.es_hoja()){

		NodoSecuencial nodoSecuencialActual(header.minCantBytesClaves,header.maxCantBytesClaves);
		nodoSecuencialActual.desempaquetar(bloqueActual);
		delete[] bloqueActual;

		vector<RegistroClave> registrosOverflow;
		int resultadoInsercionSecuencial= nodoSecuencialActual.insertar(*registro,registrosOverflow);
		if(resultadoInsercionSecuencial== RES_RECORD_EXISTS)
			return RES_RECORD_EXISTS;
		if(resultadoInsercionSecuencial== RES_OK)
			return RES_OK;
		/*si no ocurre overflow en la insercion del registro, la ejecucion finaliza*/

		_split_hoja(&nodoSecuencialActual,&registrosOverflow,hijoPromocionado,
				clavePromocionada);
		/*divido el nodoSecuencial en overflow , guardo el nodo nuevo en el archivo y en hijoPromocionado retorno el numero de
		 * bloque donde el nodoNuevo se guardo. En clavePromocionada guardo la clave que debera ser insertada en el nodoInterno
		 * padre.*/
		Bloque bloqueNodoSecuencialActualModificado;
		nodoSecuencialActual.empaquetar(&bloqueNodoSecuencialActualModificado);
		this->archivoNodos.sobreescribir_bloque(&bloqueNodoSecuencialActualModificado,numeroBloqueActual);
		/*persisto en el archivo el nodoSecuencial modificado*/

		return RES_OVERFLOW;

	}/*si el nodo actual es del tipo hoja, insertar el registro. En caso de ocurrir un overflow se guarda en clavePromocionada
	la clave del registro promocionado.*/


	NodoInterno nodoActualInterno(header.minCantBytesClaves,header.maxCantBytesClaves);
	nodoActualInterno.desempaquetar(bloqueActual);
	delete[] bloqueActual;
	unsigned int numeroBloqueHijo;
	this->_hallar_hijo_correspondiente(registro,&nodoActualInterno,
			numeroBloqueHijo);
	/*busco el siguiente hijo para acercarme a la hoja*/

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
	/*inserto en el nodoInterno la clave promocionada desde un secuencial y el hijo derecho de dicha clave*/


	if(resultadoInsercionNodoInterno== RES_OK)
		return RES_OK;
	if(resultadoInsercionNodoInterno== RES_ERROR)
		return RES_ERROR;


	_split_interno(&nodoActualInterno,clavePromocionada,hijoPromocionado);
	/*realiza el split. y guarda el nodo nuevo en el archivo. En hijoPromocionado se devolvera el hijo del nodoInterno nuevo creado, en
	 * clavePromocionada se retorna la clave a insertar en un nodoInterno en una instancia superior de _insertar_recursivo . El nodoInterno
	 * nuevo creado es guardado en el archivo*/

	Bloque bloqueNodoInternoActualModificado;
	nodoActualInterno.empaquetar(&bloqueNodoInternoActualModificado);
	this->archivoNodos.sobreescribir_bloque(&bloqueNodoInternoActualModificado,numeroBloqueActual);
	/*se sobreescribe el nodo original en el archivo de bloques*/


	return RES_OVERFLOW;


}
