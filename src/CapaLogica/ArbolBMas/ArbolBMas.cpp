#include "ArbolBMas.h"

ArbolBMas::ArbolBMas()
{
	numeroBloqueRaiz = NUMERO_BLOQUE_RAIZ; // el bloque 0 tiene el header y el 1 la raiz
	raiz = NULL;
	header.maxCantBytesClaves = 0;
	header.minCantBytesClaves = 0;
	tamanioMaximoNodo = BLOQUE_TAM_DEFAULT;
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
	header.minCantBytesClaves = sizeof(TipoHijo);
	header.maxCantBytesClaves = tamanioMaximoNodo;
	registroHeader.agregar_campo((char*)&header.minCantBytesClaves,sizeof(header.minCantBytesClaves));
	registroHeader.agregar_campo((char*)&header.maxCantBytesClaves,sizeof(header.maxCantBytesClaves));
	int res = bloqueHeader->agregar_registro(&registroHeader);
	if (res != RES_OK)
	{
		delete bloqueHeader;
		return RES_ERROR;
	}
	res = archivoNodos.escribir_bloque(bloqueHeader);
	delete bloqueHeader;
	if (res != 0)
		return RES_ERROR;

	// El bloque 1 debe tener la raiz
	Bloque* bloqueRaiz = archivoNodos.crear_bloque();
	raiz = new NodoSecuencial(header.minCantBytesClaves, header.maxCantBytesClaves);
	raiz->empaquetar(bloqueRaiz);

	res = archivoNodos.escribir_bloque(bloqueRaiz);
	delete bloqueRaiz;
	if (res != NUMERO_BLOQUE_RAIZ)
		return RES_ERROR;

	return RES_OK;
}/* PRECONDICION: el archivo debe estar abierto.
POSTCONDICION: se debe cerrar el archivo. */

int ArbolBMas::_get_header()
{

	Bloque* bloqueHeader = archivoNodos.obtener_bloque(0);
	if (bloqueHeader != NULL)
	{
		RegistroVariable registroHeader;
		int res = bloqueHeader->recuperar_registro(&registroHeader,0);

		res += registroHeader.recuperar_campo((char*)&header.minCantBytesClaves,0);
		res += registroHeader.recuperar_campo((char*)&header.maxCantBytesClaves,1);

		delete bloqueHeader;
		if (res >= RES_OK)
			return RES_OK;

		return RES_ERROR;
	}
	return RES_ERROR;
}

unsigned int ArbolBMas::get_cant_minima_nodo()
{
	return header.minCantBytesClaves;
}

unsigned int ArbolBMas::get_cant_maxima_nodo()
{
	return header.maxCantBytesClaves;
}

int ArbolBMas::crear (std::string fileName, unsigned int tamBloque)
{
	this->tamanioMaximoNodo = tamBloque * 0.9;
	int res = archivoNodos.crear_archivo(fileName,tamBloque);
	if (res != RES_OK)
		return res;

	res = archivoNodos.abrir_archivo(fileName,"rb+");
	if (res == RES_OK)
	{
		_set_header();
		return archivoNodos.cerrar_archivo();
	}

	return RES_ERROR;
}/* POSTCONDICION: el archivo se cierra. */

int ArbolBMas::eliminar (std::string fileName)
{
	return archivoNodos.eliminar_archivo(fileName);
}

int ArbolBMas::abrir (std::string fileName, std::string mode)
{
	int res = archivoNodos.abrir_archivo(fileName,mode);
	if (res == RES_OK)
	{
		return _get_header();
	}
	return res;
}

int ArbolBMas::cerrar ()
{
	return archivoNodos.cerrar_archivo();
}

int ArbolBMas::agregar(RegistroClave & reg)
{
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

int ArbolBMas::buscar(RegistroClave & reg, unsigned int  & p_numBloque)
{
	unsigned int numBloque = 1;
	_hallar_hoja(&reg,numBloque);
	p_numBloque = numBloque;

	NodoSecuencial nodoSecuencial (header.minCantBytesClaves, header.maxCantBytesClaves);

	Bloque* bloqueNodoSecuencial = archivoNodos.obtener_bloque(numBloque);

	nodoSecuencial.desempaquetar(bloqueNodoSecuencial);


	delete bloqueNodoSecuencial;

	RegistroClave* regCopia = &reg;
	int res = nodoSecuencial.buscar(reg.get_clave(),&regCopia);

	reg = *regCopia;

	std::cout << "tamanio campo 1 =" << reg.get_tamanio_campo(1) << std::endl;

	return res;

}


int ArbolBMas::_hallar_hijo_correspondiente(RegistroClave* registro,
		NodoInterno* nodoBuscar,TipoHijo& hijoCorrespondiente){

	if(registro== NULL || nodoBuscar== NULL)
		return RES_ERROR;

	/*recupero la clave del registro a buscar*/

	ClaveX unaClave;
	ClaveX claveRegistro= registro->get_clave();
	unsigned short indiceClave= 0;

	nodoBuscar->get_clave(0,unaClave);
	if(claveRegistro < unaClave){
		nodoBuscar->get_hijo(hijoCorrespondiente , 0);
		return RES_OK;
	}/*si la clave de registro es menor que la primera clave de nodoBuscar, entonces el hijo a retornar es el 'mas izquierdo'*/

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
	std::cout << "Num bloque = " << numeroBloque << std::endl;
	Bloque* bloqueActual= archivoNodos.obtener_bloque(numeroBloque);
	if(bloqueActual== NULL)
		return RES_ERROR;
	na.desempaquetar(bloqueActual);

	if( na.es_hoja() )
	{
		delete bloqueActual;
		return RES_OK;
	}

	NodoInterno ni (header.minCantBytesClaves,header.maxCantBytesClaves);
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

	Bloque* bloqueNodoSecuencialNuevo = archivoNodos.crear_bloque();
	nodoSecuencialNuevo.empaquetar(bloqueNodoSecuencialNuevo);
	int resultadoEscritura= this->archivoNodos.escribir_bloque(bloqueNodoSecuencialNuevo);
	delete (bloqueNodoSecuencialNuevo);
	if(resultadoEscritura!= RES_ERROR)
		hijoPromocionado= (unsigned int)resultadoEscritura;

	return resultadoEscritura;

}//TODO




int ArbolBMas::_split_interno(NodoInterno* nodo,ClaveX* clavePromocionada,
		TipoHijo& bloquePromocionado){

	NodoInterno nodoNuevo;
	nodoNuevo.limpiar();
	/*remuevo el hijo x defecto del nodo nuevo*/
	Bloque* bloqueNodoNuevo = archivoNodos.crear_bloque();


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


	nodoNuevo.empaquetar(bloqueNodoNuevo);
	int resEscritura= this->archivoNodos.escribir_bloque(bloqueNodoNuevo);

	if(resEscritura!= RES_ERROR)
		bloquePromocionado= resEscritura;


	delete bloqueNodoNuevo;
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
		delete bloqueActual;

		vector<RegistroClave> registrosOverflow;
		int resultadoInsercionSecuencial= nodoSecuencialActual.insertar(*registro,registrosOverflow);
		if(resultadoInsercionSecuencial== RES_RECORD_EXISTS)
			return RES_RECORD_EXISTS;
		if(resultadoInsercionSecuencial== RES_OK)
		{

			Bloque* bloqueModificado = archivoNodos.crear_bloque();
			nodoSecuencialActual.empaquetar(bloqueModificado);

			int res = archivoNodos.sobreescribir_bloque(bloqueModificado,numeroBloqueActual);
			delete bloqueModificado;
			return res;
		}
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
	delete bloqueActual;
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
