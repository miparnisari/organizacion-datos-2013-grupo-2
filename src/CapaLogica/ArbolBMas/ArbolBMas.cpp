#include "ArbolBMas.h"

ArbolBMas::ArbolBMas()
{
	numeroBloqueRaiz = NUMERO_BLOQUE_RAIZ; // el bloque 0 tiene el header y el 1 la raiz
	header.maxCantBytesClaves = 0;
	header.minCantBytesClaves = 0;
	tamanioMaximoNodo = BLOQUE_TAM_DEFAULT;
}

ArbolBMas::ArbolBMas(const ArbolBMas& otro)
{
	this->numeroBloqueRaiz = otro.numeroBloqueRaiz;
	this->header.maxCantBytesClaves = otro.header.maxCantBytesClaves;
	this->header.minCantBytesClaves = otro.header.minCantBytesClaves;
	this->tamanioMaximoNodo = otro.tamanioMaximoNodo;
	this->archivoNodos=otro.archivoNodos;
}

ArbolBMas::~ArbolBMas()
{
}

int ArbolBMas::_set_header()
{
	// El bloque 0 debe tener datos del arbol
	Bloque* bloqueHeader = archivoNodos.crear_bloque();
	if (bloqueHeader == NULL)
		return RES_ERROR;

	RegistroVariable registroHeader;
	//header.minCantBytesClaves = sizeof(TipoHijo);
	header.minCantBytesClaves = 1;
	//FIXME cambiado 25/5/13. Esta seteado en 1 tal que los underflow puedan existir,  de lo contrario
	//si minCantBytesCLaves== 0 no habra underflow
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
	NodoSecuencial* raiz = new NodoSecuencial(header.minCantBytesClaves, header.maxCantBytesClaves);
	raiz->empaquetar(bloqueRaiz);

	res = archivoNodos.escribir_bloque(bloqueRaiz);
	delete raiz;
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

int ArbolBMas::_persistir_nodo(NodoArbol* nodo, unsigned int numeroNodo){

	Bloque* bloqueNodo= archivoNodos.crear_bloque();
	nodo->empaquetar(bloqueNodo);
	int res = archivoNodos.sobreescribir_bloque(bloqueNodo,numeroNodo);

	delete bloqueNodo;
	return res;
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
	this->tamanioMaximoNodo = tamBloque * FACTOR_CARGA /100;
	/*FIXME Tests andan con FACTOR_CARGA=0.6*/

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
	int resultadoInsercion= _insertar_recursivo(raiz,&reg,hijoPromocionado,&clavePromocionada);

	if(resultadoInsercion!= RES_OVERFLOW)
		return resultadoInsercion;


	NodoInterno nodoRaizNuevo(header.minCantBytesClaves , header.maxCantBytesClaves);
	unsigned short ocurrenciaInsercion= -1;
	nodoRaizNuevo.insertar_clave(clavePromocionada,ocurrenciaInsercion);
	nodoRaizNuevo.insertar_hijo_derecho(clavePromocionada,hijoPromocionado);
	/*instancio un nodo nuevo para la raiz (interno). Inserto la clave promocionada y el numero de bloque de split como hijo derecho de
	 * dicha clave .*/

	Bloque* bloqueRaizAntigua= archivoNodos.obtener_bloque(NUMERO_BLOQUE_RAIZ);
	int numeroBloqueRaizAntigua= archivoNodos.escribir_bloque(bloqueRaizAntigua);
	nodoRaizNuevo.modificar_hijo(numeroBloqueRaizAntigua,0);
	/*escribo la vieja raiz ya modificada en un bloque apropiado (nuevo o reciclado). Modifico el hijo izquierdo de la nueva raiz para que
	 * apunte a el bloque de la vieja raiz.*/

	int res = _persistir_nodo(&nodoRaizNuevo,NUMERO_BLOQUE_RAIZ);

	delete bloqueRaizAntigua;
	return res;

}




int ArbolBMas::_obtener_nodo_interno(unsigned int numeroNodoInterno,NodoInterno& nodo){

	Bloque* bloque= archivoNodos.obtener_bloque(numeroNodoInterno);
	if(bloque==NULL)
		return RES_ERROR;
	nodo.desempaquetar(bloque);
	delete bloque;
	return RES_OK;

}



int ArbolBMas::_obtener_nodo_secuencial(int numNodoSecuencial,NodoSecuencial& nodoSecuencialActual)
{
	Bloque* bloqueSecuencialActual = archivoNodos.obtener_bloque(numNodoSecuencial);
	if(bloqueSecuencialActual== NULL)
		return RES_ERROR;
	nodoSecuencialActual.desempaquetar(bloqueSecuencialActual);
	delete bloqueSecuencialActual;
	return RES_OK;

}

int ArbolBMas::_buscar_nodo_con_puntero(int punteroBuscar)
{
	TipoHijo numNodoSecuencial;
	this->obtener_primer_nodo_secuencial(numNodoSecuencial);
	if (numNodoSecuencial == (unsigned) punteroBuscar)
		return -1; //El primer nodo secuencial no es apuntado por nadie

	NodoSecuencial nodoSecuencialActual(header.minCantBytesClaves,header.maxCantBytesClaves) ;
	_obtener_nodo_secuencial(numNodoSecuencial,nodoSecuencialActual);

	while (nodoSecuencialActual.get_proximo_nodo() != punteroBuscar)
	{
		numNodoSecuencial= nodoSecuencialActual.get_proximo_nodo();
		_obtener_nodo_secuencial(nodoSecuencialActual.get_proximo_nodo(),nodoSecuencialActual);
		if (nodoSecuencialActual.get_proximo_nodo() == -1)
			return -1; //No deberia llegar nunca aca
	}

	return numNodoSecuencial;


}/* Devuelve el numero del bloque del nodo secuencial cuyo puntero es el parametro. */




int ArbolBMas::quitar(RegistroClave& registroEliminar){


	int tipoUnderflow;
	unsigned int numeroNodoActual= NUMERO_BLOQUE_RAIZ;
	int resultadoQuitar= this->_quitar_recursivo(numeroNodoActual,registroEliminar,tipoUnderflow);

	if(resultadoQuitar== RES_ERROR || resultadoQuitar== RES_RECORD_DOESNT_EXIST || resultadoQuitar== RES_OK)
		return resultadoQuitar;

	/*a continuacion se tratara el caso en que la raiz haya quedado en underflow*/
	NodoArbol nodoRaizArbol(TIPO_HOJA);
	this->_obtener_nodo_arbol(NUMERO_BLOQUE_RAIZ,nodoRaizArbol);


	if(nodoRaizArbol.es_hoja())
		return RES_OK;

	NodoInterno nodoRaizInterno(header.minCantBytesClaves,header.maxCantBytesClaves);
	this->_obtener_nodo_interno(NUMERO_BLOQUE_RAIZ,nodoRaizInterno);
	TipoHijo hijoRaiz;
	nodoRaizInterno.get_hijo(hijoRaiz,0);
	NodoArbol nodoHijoRaizArbol(TIPO_HOJA);
	this->_obtener_nodo_arbol(hijoRaiz , nodoHijoRaizArbol);


	if(nodoHijoRaizArbol.es_hoja()){

		NodoSecuencial nodoHijoRaizSecuencial(header.minCantBytesClaves,header.maxCantBytesClaves);
		this->_obtener_nodo_secuencial(hijoRaiz,nodoHijoRaizSecuencial);
		this->_persistir_nodo(&nodoHijoRaizSecuencial,NUMERO_BLOQUE_RAIZ);

	}else{

		NodoInterno nodoHijoRaizInterno(header.minCantBytesClaves,header.maxCantBytesClaves);
		this->_obtener_nodo_interno(hijoRaiz,nodoHijoRaizInterno);
		this->_persistir_nodo(&nodoHijoRaizInterno,NUMERO_BLOQUE_RAIZ);

	}


	this->_liberar_nodo(hijoRaiz);
	return RES_OK;

}



int ArbolBMas::_quitar_recursivo(unsigned int& numeroNodoActual,
		RegistroClave& registroEliminar,int& tipoUnderflow){

	NodoArbol nodoActualArbol(TIPO_HOJA);
	if( _obtener_nodo_arbol(numeroNodoActual,nodoActualArbol)== RES_ERROR )
		return RES_ERROR;

	if(nodoActualArbol.es_hoja()){

		NodoSecuencial nodoActualSecuencial(header.minCantBytesClaves,header.maxCantBytesClaves);
		if(_obtener_nodo_secuencial(numeroNodoActual,nodoActualSecuencial)== RES_ERROR)
			return RES_ERROR;

		ClaveX claveRegistroEliminar= registroEliminar.get_clave();
		vector<RegistroClave> vectorUnderflow;
		int resultadoEliminacionRegistro= nodoActualSecuencial.eliminar(claveRegistroEliminar,vectorUnderflow);

		if(resultadoEliminacionRegistro== RES_OK){
			this->_persistir_nodo(&nodoActualSecuencial,numeroNodoActual);
			tipoUnderflow= RES_NO_UNDERFLOW;
			return RES_OK;
		}/*el borrado del registro en el nodoSecuencial ocurrio exitosamente sin ocurrin underflow. El nodoSecuencial
		modificado es persistido en el archivo*/

		if(resultadoEliminacionRegistro== RES_ERROR)
			return RES_ERROR;
		/*se produjo un error en el borrado del registro. No hay persistencia*/
		if(resultadoEliminacionRegistro== RES_RECORD_DOESNT_EXIST)
			return RES_RECORD_DOESNT_EXIST;

		/*a continuacion se llevaran los pasos a llevar a cabo en caso de underflow del nodoSecuencial*/
		tipoUnderflow= RES_UNDERFLOW_HOJA;
		this->_persistir_nodo(&nodoActualSecuencial , numeroNodoActual);
		/*FIXME esta persistencia deberia ocurrir desde una instancia superior */
		return RES_UNDERFLOW;

	}/*resolucion si el nodoActual es hoja*/


	/*a continuacion se resolvera el caso que el nodoActual sea interno*/
	NodoInterno nodoActualInterno(header.minCantBytesClaves,header.maxCantBytesClaves);
	if( this->_obtener_nodo_interno(numeroNodoActual,nodoActualInterno) == RES_ERROR)
		return RES_ERROR;


	TipoHijo proximoNodoInspeccionar;
	this->_hallar_hijo_correspondiente(&registroEliminar , &nodoActualInterno , proximoNodoInspeccionar);


	int resultadoQuitarInferior= this->_quitar_recursivo(proximoNodoInspeccionar,registroEliminar,tipoUnderflow);

	if(resultadoQuitarInferior== RES_ERROR)
		return RES_ERROR;
	if(resultadoQuitarInferior== RES_OK)
		return RES_OK;
	/*si la insercion de un nivel inferior resulta en un exito(sin underflow) o un error, no ocurre persistencia en disco y se retorna
	 * el valor pertinente a la instancia superior.*/


	if(resultadoQuitarInferior== RES_UNDERFLOW){

		if(tipoUnderflow== RES_UNDERFLOW_HOJA){
			this->_resolver_underflow_hoja(&nodoActualInterno,proximoNodoInspeccionar);
		}/*nodos hijos involucrados deben balancearse / mergear y ser persistidos*/
		else{
			this->_resolver_underflow_interno(&nodoActualInterno,proximoNodoInspeccionar);
		}

		this->_persistir_nodo( &nodoActualInterno,numeroNodoActual );

		if( nodoActualInterno.hay_underflow() ){
			tipoUnderflow= RES_UNDERFLOW_INTERNO;
			return RES_UNDERFLOW;
		}

		return RES_OK;

	}

	return RES_OK;

}


int ArbolBMas::_resolver_underflow_hoja(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow){

	const unsigned short CANTIDAD_CLAVES_PADRE= nodoPadre->get_cantidad_claves();

	NodoSecuencial nodoUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
	if( this->_obtener_nodo_secuencial(numeroNodoUnderflow,nodoUnderflow)== RES_ERROR )
		return RES_ERROR;

	const unsigned short CANTIDAD_HIJOS_NODO_PADRE= nodoPadre->get_cantidad_hijos();
	unsigned short posicionNumeroNodoUnderflow;
	if( nodoPadre->buscar_hijo(numeroNodoUnderflow,posicionNumeroNodoUnderflow) == RES_ERROR )
		return RES_ERROR;

	bool numeroNodoUnderflowEsUltimoHijo= ( posicionNumeroNodoUnderflow == (CANTIDAD_HIJOS_NODO_PADRE - 1) );
	TipoHijo numeroNodoHermanoUnderflow;
	if(numeroNodoUnderflowEsUltimoHijo){
		nodoPadre->get_hijo(numeroNodoHermanoUnderflow,posicionNumeroNodoUnderflow-1);
	}else{
		nodoPadre->get_hijo(numeroNodoHermanoUnderflow,posicionNumeroNodoUnderflow+1);
	}


	NodoSecuencial nodoHermanoUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
	this->_obtener_nodo_secuencial(numeroNodoHermanoUnderflow,nodoHermanoUnderflow);
	if(nodoHermanoUnderflow.tiene_carga_minima()){

		unsigned short posicionClaveEliminar;
		unsigned short posicionHijoEliminar;


		if(!numeroNodoUnderflowEsUltimoHijo){
			posicionClaveEliminar= posicionNumeroNodoUnderflow;
			posicionHijoEliminar= posicionClaveEliminar+1;
		}
		else{
			posicionClaveEliminar= CANTIDAD_CLAVES_PADRE-1;//posicionClaveEliminar es CANTIDAD_CLAVES_PADRE-1
			posicionHijoEliminar= CANTIDAD_HIJOS_NODO_PADRE-2;//es CANTIDAD_HIJOS_PADRE- 2
		}

		if(_merge_secuenciales(nodoPadre,numeroNodoUnderflow,numeroNodoHermanoUnderflow,numeroNodoUnderflowEsUltimoHijo,
				posicionClaveEliminar,posicionHijoEliminar)== RES_ERROR)
			return RES_ERROR;
		return RES_MERGE;

	}/*si el nodo hermano tiene carga minima debo mergear*/

	_balancear_secuenciales(nodoPadre,numeroNodoUnderflow,numeroNodoHermanoUnderflow,numeroNodoUnderflowEsUltimoHijo);
	return RES_BALANCEO;


}


int ArbolBMas::_merge_secuenciales(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow,
		unsigned int numeroNodoHermanoUnderflow,bool numeroNodoUnderflowEsUltimoHijo ,
		unsigned short posicionClaveEliminar,unsigned short posicionHijoEliminar){

	NodoSecuencial nodoUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
	if( this->_obtener_nodo_secuencial(numeroNodoUnderflow,nodoUnderflow) == RES_ERROR)
		return RES_ERROR;
	NodoSecuencial nodoHermanoUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
	if( this->_obtener_nodo_secuencial(numeroNodoHermanoUnderflow,nodoHermanoUnderflow) == RES_ERROR){
		return RES_ERROR;
	}

	vector<RegistroClave> registrosHermanoUnderflow= nodoHermanoUnderflow.get_registros();
	vector<RegistroClave> vu;
	const unsigned short CANTIDAD_REGISTROS_HERMANO_UNDERFLOW= registrosHermanoUnderflow.size();

	for(unsigned short i=0;i<CANTIDAD_REGISTROS_HERMANO_UNDERFLOW;i++){
		RegistroClave unRegistro= registrosHermanoUnderflow.at(i);
		nodoUnderflow.insertar(unRegistro,vu);
	}

	if(numeroNodoUnderflowEsUltimoHijo){
		int numeroNodoIzquierdo= this->_buscar_nodo_con_puntero(numeroNodoHermanoUnderflow);
		if(numeroNodoIzquierdo!= -1){
			NodoSecuencial nodoIzquierdo(header.minCantBytesClaves,header.maxCantBytesClaves);
			this->_obtener_nodo_secuencial(numeroNodoIzquierdo,nodoIzquierdo);
			nodoIzquierdo.set_proximo_nodo(numeroNodoUnderflow);
			this->_persistir_nodo(&nodoIzquierdo,numeroNodoIzquierdo);
		}
	}else{

		int numeroNodoDerecho= nodoHermanoUnderflow.get_proximo_nodo();
		nodoUnderflow.set_proximo_nodo(numeroNodoDerecho);

	}


	this->_persistir_nodo(&nodoUnderflow,numeroNodoUnderflow);
	this->_liberar_nodo(numeroNodoHermanoUnderflow);

	ClaveX cr;
	nodoPadre->remover_clave(posicionClaveEliminar,cr);
	nodoPadre->remover_hijo(posicionHijoEliminar);

	return RES_OK;

}


int ArbolBMas::_balancear_secuenciales(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow,
		unsigned int numeroNodoHermanoUnderflow,bool numeroNodoUnderflowEsUltimoHijo){

	unsigned short posicionNumeroNodoUnderflow, posicionNumeroNodoHermanoUnderflow;
	nodoPadre->buscar_hijo(numeroNodoHermanoUnderflow,posicionNumeroNodoHermanoUnderflow);
	nodoPadre->buscar_hijo(numeroNodoUnderflow,posicionNumeroNodoUnderflow);

	unsigned short posicionClaveDivision;
	ClaveX claveDivision;
	if(numeroNodoUnderflowEsUltimoHijo)
		nodoPadre->get_clave(posicionNumeroNodoHermanoUnderflow,claveDivision);
	else
		nodoPadre->get_clave(posicionNumeroNodoUnderflow,claveDivision);
	nodoPadre->buscar_clave(claveDivision,posicionClaveDivision);

	NodoSecuencial nodoUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
	NodoSecuencial nodoHermano(header.minCantBytesClaves,header.maxCantBytesClaves);
	this->_obtener_nodo_secuencial(numeroNodoUnderflow,nodoUnderflow);
	this->_obtener_nodo_secuencial(numeroNodoHermanoUnderflow,nodoHermano);
	vector<RegistroClave> registrosHermano= nodoHermano.get_registros();
	const unsigned short CANTIDAD_REGISTROS_HERMANO= registrosHermano.size();

	if(numeroNodoUnderflowEsUltimoHijo){
		const unsigned short PRIMER_REGISTRO_PASAR= (short)(CANTIDAD_REGISTROS_HERMANO/2);
		vector<RegistroClave> vu,vo;

		for(unsigned short i=PRIMER_REGISTRO_PASAR;i<CANTIDAD_REGISTROS_HERMANO;i++){
			RegistroClave unRegistro= registrosHermano.at(i);
			nodoUnderflow.insertar(unRegistro,vo);
			ClaveX claveRegistro= unRegistro.get_clave();
			nodoHermano.eliminar(claveRegistro,vu);
		}/*si el nodo en underflow es el ultimo hijo del padre -> paso la mitad superior de los registros del hermano a el (el hermnano es IZQUIERDO)*/
	}else{
		const unsigned short FINAL_REGISTROS_PASAR= (short)(CANTIDAD_REGISTROS_HERMANO/2);
		vector<RegistroClave> vu,vo;

		for(unsigned short i=0;i<FINAL_REGISTROS_PASAR;i++){
			RegistroClave unRegistro= registrosHermano.at(i);
			nodoUnderflow.insertar(unRegistro,vo);
			ClaveX claveRegistro= unRegistro.get_clave();
			nodoHermano.eliminar(claveRegistro,vu);
		}
	}/*si el nodo en underflow no es el ultimo -> paso la mitad inderior de los registros del hermano a el (el hermano es DERECHO) */

	nodoPadre->remover_clave(claveDivision,posicionClaveDivision);
	vector<RegistroClave> registrosUnderflowActualizados= nodoUnderflow.get_registros();
	vector<RegistroClave> registrosHermanoActualizados= nodoHermano.get_registros();
	ClaveX primeraClaveUnderflow= registrosUnderflowActualizados.at(0).get_clave();
	ClaveX primeraClaveHermano= registrosHermanoActualizados.at(0).get_clave();

	if(primeraClaveUnderflow > primeraClaveHermano)
		nodoPadre->insertar_clave(primeraClaveUnderflow,posicionClaveDivision);
	else
		nodoPadre->insertar_clave(primeraClaveHermano,posicionClaveDivision);
	/*actualizo la nueva clave de division*/


	this->_persistir_nodo(&nodoUnderflow,numeroNodoUnderflow);
	this->_persistir_nodo(&nodoHermano,numeroNodoHermanoUnderflow);

	return RES_OK;


}


int ArbolBMas::_liberar_nodo(unsigned int numeroNodo){
	Bloque* bloqueVacio= archivoNodos.crear_bloque();
	int liberacion= archivoNodos.sobreescribir_bloque(bloqueVacio,numeroNodo);
	delete bloqueVacio;

	return liberacion;
}


int ArbolBMas::_resolver_underflow_interno(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow){


	NodoInterno nodoUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
	if( this->_obtener_nodo_interno(numeroNodoUnderflow,nodoUnderflow)== RES_ERROR )
		return RES_ERROR;

	const unsigned short CANTIDAD_HIJOS_NODO_PADRE= nodoPadre->get_cantidad_hijos();
	unsigned short posicionNumeroNodoUnderflow;
	if( nodoPadre->buscar_hijo(numeroNodoUnderflow,posicionNumeroNodoUnderflow) == RES_ERROR )
		return RES_ERROR;

	bool numeroNodoUnderflowEsUltimoHijo= ( posicionNumeroNodoUnderflow == (CANTIDAD_HIJOS_NODO_PADRE - 1) );
	TipoHijo numeroNodoHermanoUnderflow;

	if(numeroNodoUnderflowEsUltimoHijo)
		nodoPadre->get_hijo(numeroNodoHermanoUnderflow,posicionNumeroNodoUnderflow-1);
	else
		nodoPadre->get_hijo(numeroNodoHermanoUnderflow,posicionNumeroNodoUnderflow+1);

	NodoInterno nodoHermanoUnderflow;
	if( this->_obtener_nodo_interno(numeroNodoHermanoUnderflow,nodoHermanoUnderflow)== RES_ERROR )
		return RES_ERROR;
	bool nodoHermanoUnderflowTieneCargaMinima= (nodoHermanoUnderflow.get_cantidad_claves() == 1);
	/*todo , la condicion de carga minima sigue siendo tener un solo registro o clave*/

	if( nodoHermanoUnderflowTieneCargaMinima ){

		this->_merge_internos(nodoPadre,numeroNodoUnderflow,
				numeroNodoHermanoUnderflow,numeroNodoUnderflowEsUltimoHijo);
		return RES_MERGE;

	}


	this->_balancear_internos(nodoPadre,numeroNodoUnderflow,numeroNodoHermanoUnderflow,numeroNodoUnderflowEsUltimoHijo);
	return RES_BALANCEO;



}



int ArbolBMas::_merge_internos(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow,
		unsigned int numeroNodoHermanoUnderflow,bool numeroNodoUnderflowEsUltimoHijo){


	NodoInterno nodoUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
	NodoInterno nodoHermano(header.minCantBytesClaves,header.maxCantBytesClaves);

	if( this->_obtener_nodo_interno(numeroNodoUnderflow,nodoUnderflow)== RES_ERROR ||
			this->_obtener_nodo_interno(numeroNodoHermanoUnderflow,nodoHermano)== RES_ERROR)
		return RES_ERROR;

	unsigned short posicionNumeroNodoUnderflow, posicionNumeroNodoHermanoUnferflow;
	nodoPadre->buscar_hijo(numeroNodoHermanoUnderflow,posicionNumeroNodoHermanoUnferflow);
	nodoPadre->buscar_hijo(numeroNodoUnderflow,posicionNumeroNodoUnderflow);

	ClaveX claveSeparadora;
	if( numeroNodoUnderflowEsUltimoHijo )
		nodoPadre->get_clave(posicionNumeroNodoUnderflow - 1,claveSeparadora);
	else
		nodoPadre->get_clave(posicionNumeroNodoUnderflow,claveSeparadora);


	vector<ClaveX> clavesHermano= nodoHermano.get_claves();
	vector<TipoHijo> hijosHermano= nodoHermano.get_hijos();
	const unsigned short CANTIDAD_CLAVES_HERMANO= clavesHermano.size();

	if( numeroNodoUnderflowEsUltimoHijo ){

		unsigned short posicionOcurrencia;
		nodoUnderflow.insertar_clave(claveSeparadora,posicionOcurrencia);
		nodoPadre->remover_hijo(posicionNumeroNodoUnderflow-1 );
		nodoPadre->remover_clave(claveSeparadora,posicionOcurrencia);

		TipoHijo hijoMayor;
		nodoUnderflow.get_hijo(hijoMayor,0);
		/*todo . este codigo funciona unicamente si la condicion de un underflow es 0 claves!*/


		for(unsigned short i=0;i<CANTIDAD_CLAVES_HERMANO;i++){
			ClaveX unaClave= clavesHermano.at(i);
			if( nodoUnderflow.insertar_clave(unaClave,posicionOcurrencia) ==RES_ERROR)
				return RES_ERROR;
			TipoHijo unHijo= hijosHermano.at(i+1);
			if( nodoUnderflow.insertar_hijo_derecho(unaClave,unHijo)== RES_ERROR)
				return RES_ERROR;
		}

		TipoHijo primerHijoHermano= hijosHermano.at(0);
		nodoUnderflow.modificar_hijo(primerHijoHermano,0);



		nodoUnderflow.insertar_hijo(hijoMayor);
		/*todo . este codigo funciona unicamente si la condicion de un underflow es 0 claves!*/


	}else{

		unsigned short posicionOcurrencia;
		nodoUnderflow.insertar_clave(claveSeparadora,posicionOcurrencia);
		nodoPadre->remover_hijo(posicionNumeroNodoUnderflow+1 );
		nodoPadre->remover_clave(claveSeparadora,posicionOcurrencia);

		nodoUnderflow.insertar_hijo( hijosHermano.at(0) );

		for(unsigned short i=0;i<CANTIDAD_CLAVES_HERMANO;i++){
			ClaveX unaClave= clavesHermano.at(i);
			if( nodoUnderflow.insertar_clave(unaClave,posicionOcurrencia) ==RES_ERROR)
				return RES_ERROR;
			TipoHijo unHijo= hijosHermano.at(i+1);
			if( nodoUnderflow.insertar_hijo_derecho(unaClave,unHijo)== RES_ERROR)
				return RES_ERROR;
		}


	}

	this->_persistir_nodo( &nodoUnderflow,numeroNodoUnderflow );
	this->_liberar_nodo(numeroNodoHermanoUnderflow);


	return RES_OK;

}//todo terminarrr!!!



int ArbolBMas::_balancear_internos(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow,
		unsigned int numeroNodoHermanoUnderflow,bool numeroNodoUnderflowEsUltimoHijo){

	NodoInterno nodoUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
	NodoInterno nodoHermano(header.minCantBytesClaves,header.maxCantBytesClaves);

	if( this->_obtener_nodo_interno(numeroNodoUnderflow,nodoUnderflow)== RES_ERROR ||
			this->_obtener_nodo_interno(numeroNodoHermanoUnderflow,nodoHermano)== RES_ERROR)
		return RES_ERROR;

	unsigned short posicionNumeroNodoUnderflow, posicionNumeroNodoHermanoUnferflow;
	nodoPadre->buscar_hijo(numeroNodoHermanoUnderflow,posicionNumeroNodoHermanoUnferflow);
	nodoPadre->buscar_hijo(numeroNodoUnderflow,posicionNumeroNodoUnderflow);


	vector<ClaveX> clavesHermano= nodoHermano.get_claves();
	vector<TipoHijo> hijosHermano= nodoHermano.get_hijos();
	const unsigned short CANTIDAD_CLAVES_HERMANO= clavesHermano.size();

	const unsigned short CANTIDAD_HIJOS_PASAR= (short)( CANTIDAD_CLAVES_HERMANO/2 );

	unsigned short ocurrenciaInsercion;
	unsigned short ocurrenciaBorrado;


	if (numeroNodoUnderflowEsUltimoHijo){

		for(unsigned short i=0;i<CANTIDAD_HIJOS_PASAR;i++){

			ClaveX claveSeparadora;
			unsigned short cantidadClavesPadre= nodoPadre->get_cantidad_claves();
			nodoPadre->get_clave(cantidadClavesPadre-1,claveSeparadora);
			nodoUnderflow.insertar_clave(claveSeparadora,ocurrenciaInsercion);
			nodoPadre->remover_clave(claveSeparadora,ocurrenciaBorrado);
			/*en el nodo en underflow inserto la clave separadora del hermano y el nUnderflow y la remuevo del nPadre*/

			unsigned short cantidadClavesHermano= nodoHermano.get_cantidad_claves();
			unsigned short cantidadHijosHermano= nodoHermano.get_cantidad_hijos();
			ClaveX claveHermano;
			TipoHijo hijoHermano;
			nodoHermano.get_hijo(hijoHermano,cantidadHijosHermano-1);
			nodoHermano.remover_clave(cantidadClavesHermano-1,claveHermano);
			nodoHermano.remover_hijo(cantidadHijosHermano-1);
			nodoPadre->insertar_clave(claveHermano,ocurrenciaInsercion);
			nodoUnderflow.insertar_hijo_izquierdo(claveSeparadora,hijoHermano);
			/*obtengo la ultima clave y el ultimo hijo del nHermano. La clave obtenida la inserto en el nPadre y el hijo obtenido lo
			 * inserto como hijo izquierdo de la claveSeparadora insertada en el nUnderflow*/


		}

	}else{


		for (unsigned short i = 0; i < CANTIDAD_HIJOS_PASAR; i++) {
			ClaveX claveSeparadora;
			nodoPadre->get_clave(posicionNumeroNodoUnderflow,claveSeparadora);
			nodoUnderflow.insertar_clave(claveSeparadora,ocurrenciaInsercion);
			nodoPadre->remover_clave(claveSeparadora,ocurrenciaBorrado);
			/*en el nodo en underflow inserto la clave separadora del hermano y el nUnderflow y la remuevo del nPadre*/


			ClaveX claveHermano;
			TipoHijo hijoHermano;
			nodoHermano.remover_clave(0,claveHermano);
			nodoHermano.get_hijo(hijoHermano,0);
			nodoHermano.remover_hijo(0);
			nodoUnderflow.insertar_hijo_derecho(claveSeparadora,hijoHermano);
			nodoPadre->insertar_clave(claveHermano,ocurrenciaInsercion);
		}



	}

	//cambio la clave del padre al final

	this->_persistir_nodo( &nodoUnderflow,numeroNodoUnderflow );
	this->_persistir_nodo( &nodoHermano,numeroNodoHermanoUnderflow );


	return RES_OK;

}




int ArbolBMas::_obtener_nodo_arbol(unsigned int numeroNodo,NodoArbol& nodo){

	Bloque* bloqueNodo= archivoNodos.obtener_bloque(numeroNodo);
	if(bloqueNodo== NULL)
		return RES_ERROR;
	nodo.desempaquetar(bloqueNodo);
	delete bloqueNodo;

	return RES_OK;

}

int ArbolBMas::buscar(RegistroClave & reg)
{
	unsigned int numBloque;
	return _buscar(reg,numBloque);
}

int ArbolBMas::_buscar(RegistroClave & reg, unsigned int  & p_numBloque)
{
	unsigned int numBloque = 1;
	_hallar_hoja(&reg,numBloque);
	p_numBloque = numBloque;

	NodoSecuencial nodoSecuencial (header.minCantBytesClaves, header.maxCantBytesClaves);

	Bloque* bloqueNodoSecuencial = archivoNodos.obtener_bloque(numBloque);

	nodoSecuencial.desempaquetar(bloqueNodoSecuencial);


	delete bloqueNodoSecuencial;

	RegistroClave* regCopia = NULL;
	int res = nodoSecuencial.buscar(reg.get_clave(),&regCopia);
	if (res == RES_RECORD_DOESNT_EXIST)
	{
		delete regCopia;
		return RES_RECORD_DOESNT_EXIST;
	}

	reg = *regCopia;

	delete regCopia;
	return RES_OK;
}


int ArbolBMas::_hallar_hijo_correspondiente(RegistroClave* registro,
		NodoInterno* nodoBuscar,TipoHijo& hijoCorrespondiente){

	if(registro== NULL || nodoBuscar== NULL)
		return RES_ERROR;

	/*recupero la clave del registro a buscar*/

	ClaveX unaClave;
	ClaveX claveRegistro= registro->get_clave();

	nodoBuscar->get_clave(0,unaClave);
	if(claveRegistro < unaClave){
		nodoBuscar->get_hijo(hijoCorrespondiente , 0);
		return RES_OK;
	}/*si la clave de registro es menor que la primera clave de nodoBuscar, entonces el hijo a retornar es el 'mas izquierdo'*/


//	while( nodoBuscar->get_clave(indiceClave,unaClave)!= RES_ERROR ){
//
//		if(unaClave > claveRegistro){
//			nodoBuscar->get_hijo(hijoCorrespondiente,indiceClave);
//			return RES_OK;
//		}/*si la clave del nodo que estoy examinando es mayor que la clave de
//		parametro -> el hijo correspondiente es el izquierdo a aquel que examino.*/
//
//		if(unaClave == claveRegistro){
//			nodoBuscar->get_hijo(hijoCorrespondiente,indiceClave+1);
//			return RES_OK;
//		}/*si la clave que estoy examinando es identica a la clave del registro de
//		parametro -> el hijo correspondiente es el derecho de la clave examinada*/
//
//		indiceClave++;
//
//	}
//
//
//	nodoBuscar->get_hijo(hijoCorrespondiente,indiceClave);//
//	/*en caso que la clave del registro sea mayor que todas las claves del nodo ->
//	 * el hijo correspondiente a examinar es el de posicion ultima*/
//
//	return RES_OK;


	const unsigned short CANTIDAD_CLAVES= nodoBuscar->get_cantidad_claves();
	for(unsigned short i=0;i<CANTIDAD_CLAVES;i++){

		nodoBuscar->get_clave(i,unaClave);
		if(claveRegistro >= unaClave)
			nodoBuscar->get_hijo(hijoCorrespondiente,i+1);

	}

	return RES_OK;


}


int ArbolBMas::_hallar_hoja(RegistroClave* registro,
		unsigned int& numeroBloque){

	if(registro== NULL)
		return RES_ERROR;

	NodoArbol na(TIPO_INTERNO);
	Bloque* bloqueActual= archivoNodos.obtener_bloque(numeroBloque);
	if(bloqueActual == NULL)
		return RES_ERROR;

	na.desempaquetar(bloqueActual);

	if( na.es_hoja() )
	{
		delete bloqueActual;
		return RES_OK;
	}

	NodoInterno ni (header.minCantBytesClaves,header.maxCantBytesClaves);
	ni.desempaquetar(bloqueActual);

	delete bloqueActual;
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

	unsigned int siguienteDelOverflow= nodoActual->get_proximo_nodo();

	vector<RegistroClave> ro;
	for(int i=0;i<CANTIDAD_REGISTROS_OVERFLOW;i++){
		nodoSecuencialNuevo.insertar(registrosOverflow->at(i),ro);
	}/*en el nodo secuencial nuevo agrego los registros en overflow y remuevo dichos registros del nodoSecuencialActual*/
	nodoSecuencialNuevo.set_proximo_nodo(siguienteDelOverflow);

	Bloque* bloqueNodoSecuencialNuevo = archivoNodos.crear_bloque();
	nodoSecuencialNuevo.empaquetar(bloqueNodoSecuencialNuevo);
	int resultadoEscritura= this->archivoNodos.escribir_bloque(bloqueNodoSecuencialNuevo);
	delete (bloqueNodoSecuencialNuevo);
	if(resultadoEscritura!= RES_ERROR)
		hijoPromocionado= (unsigned int)resultadoEscritura;

	nodoActual->set_proximo_nodo(hijoPromocionado);

	return resultadoEscritura;

}//




int ArbolBMas::_split_interno(NodoInterno* nodo,ClaveX* clavePromocionada,
		TipoHijo& bloquePromocionado){

	NodoInterno nodoNuevo(header.minCantBytesClaves,header.maxCantBytesClaves);
	nodoNuevo.limpiar();
	/*remuevo el hijo x defecto del nodo nuevo*/
	Bloque* bloqueNodoNuevo = archivoNodos.crear_bloque();

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


	for(unsigned short i=numeroClaveMitad;i<CANTIDAD_CLAVES;i++){
		TipoHijo unHijo;
		int res=nodo->get_hijo(unHijo , i+1);
		if(res!= RES_ERROR){
			nodoNuevo.insertar_hijo(unHijo);//FIXME
		}
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
			return _persistir_nodo(&nodoSecuencialActual,numeroBloqueActual);
		}
		/*si no ocurre overflow en la insercion del registro, la ejecucion finaliza*/

		_split_hoja(&nodoSecuencialActual,&registrosOverflow,hijoPromocionado,
				clavePromocionada);
		/*divido el nodoSecuencial en overflow , guardo el nodo nuevo en el archivo y en hijoPromocionado retorno el numero de
		 * bloque donde el nodoNuevo se guardo. En clavePromocionada guardo la clave que debera ser insertada en el nodoInterno
		 * padre.*/

		_persistir_nodo(&nodoSecuencialActual,numeroBloqueActual);

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

	/*determino el nodoSecuencialSiguiente ------------------------------------------------------------------*/
	//nodoActualInterno.obtener_hijo_siguiente_a(numeroBloqueHijo,nodoSecuencialSiguiente);
	/*determino el nodoSecuencialSiguiente ------------------------------------------------------------------*/
	//FIXME estamos haciendo esto mal

	int resultadoInsercion= this->_insertar_recursivo(numeroBloqueHijo,registro,
			hijoPromocionado,clavePromocionada);

	if(resultadoInsercion== RES_OK)
		return RES_OK;
	if(resultadoInsercion== RES_RECORD_EXISTS)
		return RES_RECORD_EXISTS;/*registro que se busco insertar ya existia -> se retorna como error RES_RECORD_EXISTS*/


	/*si el resultado de la insercion no es ninguno de los superiores -> ocurrio un overflow**/
	unsigned short ocurrenciaInsercion;
	nodoActualInterno.insertar_clave(*clavePromocionada,ocurrenciaInsercion);
	int resultadoInsercionNodoInterno= nodoActualInterno.insertar_hijo_derecho(*clavePromocionada,hijoPromocionado);
	/*inserto en el nodoInterno la clave promocionada desde un secuencial y el hijo derecho de dicha clave*/


	if(resultadoInsercionNodoInterno== RES_OK){
		_persistir_nodo(&nodoActualInterno,numeroBloqueActual);
		return RES_OK;
	}
	if(resultadoInsercionNodoInterno== RES_ERROR)
		return RES_ERROR;


	_split_interno(&nodoActualInterno,clavePromocionada,hijoPromocionado);
	/*realiza el split. y guarda el nodo nuevo en el archivo. En hijoPromocionado se devolvera el hijo del nodoInterno nuevo creado, en
	 * clavePromocionada se retorna la clave a insertar en un nodoInterno en una instancia superior de _insertar_recursivo . El nodoInterno
	 * nuevo creado es guardado en el archivo*/

	_persistir_nodo(&nodoActualInterno,numeroBloqueActual);
	return RES_OVERFLOW;

}


void ArbolBMas::obtener_primer_nodo_secuencial(TipoHijo& numeroPrimerNodo){

	NodoArbol nodoRaiz(TIPO_INTERNO);
	Bloque* bloqueRaiz= archivoNodos.obtener_bloque(NUMERO_BLOQUE_RAIZ);
	nodoRaiz.desempaquetar(bloqueRaiz);
	delete bloqueRaiz;
	if(nodoRaiz.es_hoja()){
		numeroPrimerNodo= NUMERO_BLOQUE_RAIZ;

		return;
	}


	NodoInterno nodoActualInterno(header.minCantBytesClaves,header.maxCantBytesClaves);
	NodoArbol nodoActual(TIPO_INTERNO);
	TipoHijo primerHijo= NUMERO_BLOQUE_RAIZ;


	while(nodoActual.es_interno()){

		Bloque* bloqueActual = archivoNodos.obtener_bloque(primerHijo);
		nodoActual.desempaquetar(bloqueActual);

		if(nodoActual.es_interno())
			nodoActualInterno.desempaquetar(bloqueActual);
		else{
			delete bloqueActual;
			break;
		}

		nodoActualInterno.get_hijo(primerHijo,0);
		delete bloqueActual;

	}

	numeroPrimerNodo= primerHijo;


}

void ArbolBMas::imprimir(){
	_imprimir_recursivo(NUMERO_BLOQUE_RAIZ);
}

void ArbolBMas::_imprimir_recursivo(unsigned int numNodo)
{
	Bloque* bloque = archivoNodos.obtener_bloque(numNodo);
	NodoArbol nodo(TIPO_HOJA);
	nodo.desempaquetar(bloque);

	std::cout << "NUM BLOQUE = " << numNodo << "---->";

	if (nodo.es_interno())
	{
		NodoInterno nodoInterno(header.minCantBytesClaves,header.maxCantBytesClaves);
		nodoInterno.desempaquetar(bloque);

		nodoInterno.imprimir();

		std::cout << std::endl;

		for (unsigned int i = 0; i < nodoInterno.get_cantidad_hijos(); i ++)
		{
			TipoHijo unHijo;
			nodoInterno.get_hijo(unHijo,i);
			_imprimir_recursivo((unsigned)unHijo);
		}
	}

	else
	{
		NodoSecuencial nodoSecuencial(header.minCantBytesClaves,header.maxCantBytesClaves);
		nodoSecuencial.desempaquetar(bloque);
		nodoSecuencial.imprimir();
	}


	delete(bloque);
}
