#include "ArbolBMas.h"

ArbolBMas::ArbolBMas()
{
	numeroBloqueRaiz = NUMERO_BLOQUE_RAIZ; // el bloque 0 tiene el header y el 1 la raiz
	header.maxCantBytesClaves = 0;
	header.minCantBytesClaves = 0;
	tamanioMaximoNodo = BLOQUE_TAM_DEFAULT;
}

ArbolBMas::ArbolBMas(std::string nombreArchivo)
{
	archivoNodos.set_ruta(nombreArchivo);
}

ArbolBMas::~ArbolBMas()
{
}

int ArbolBMas::persistir(NodoArbol* nodo, unsigned int numeroNodo){

	Bloque* bloqueNodo= archivoNodos.crear_bloque();
	nodo->empaquetar(bloqueNodo);
	archivoNodos.sobreescribir_bloque(bloqueNodo,numeroNodo);

	delete bloqueNodo;
	return RES_OK;
}

int ArbolBMas::_balancear_secuenciales(NodoSecuencial&, NodoSecuencial&, NodoInterno&){
	return RES_OK;
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
	/*FIXME . Tests funcaban con 0.6*/

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
	TipoPuntero nodoSecuencialSiguiente= -1;
	int resultadoInsercion= _insertar_recursivo(raiz,&reg,hijoPromocionado,&clavePromocionada,nodoSecuencialSiguiente);

	if(resultadoInsercion!= RES_OVERFLOW)
		return resultadoInsercion;


	NodoInterno nodoRaizNuevo(header.minCantBytesClaves , header.maxCantBytesClaves);
	unsigned short ocurrenciaInsercion= -1;
	nodoRaizNuevo.insertar_clave(clavePromocionada,ocurrenciaInsercion);
	cout<<"ocurrencia insercion: "<<ocurrenciaInsercion<<endl;
	nodoRaizNuevo.insertar_hijo_derecho(clavePromocionada,hijoPromocionado);
	/*instancio un nodo nuevo para la raiz (interno). Inserto la clave promocionada y el numero de bloque de split como hijo derecho de
	 * dicha clave .*/

	Bloque* bloqueRaizAntigua= archivoNodos.obtener_bloque(NUMERO_BLOQUE_RAIZ);
	int numeroBloqueRaizAntigua= archivoNodos.escribir_bloque(bloqueRaizAntigua);
	nodoRaizNuevo.modificar_hijo(numeroBloqueRaizAntigua,0);
	/*escribo la vieja raiz ya modificada en un bloque apropiado (nuevo o reciclado). Modifico el hijo izquierdo de la nueva raiz para que
	 * apunte a el bloque de la vieja raiz.*/

	Bloque* bloqueRaizNueva=  archivoNodos.crear_bloque();
	nodoRaizNuevo.empaquetar(bloqueRaizNueva);
	int res= archivoNodos.sobreescribir_bloque(bloqueRaizNueva,NUMERO_BLOQUE_RAIZ);


	delete bloqueRaizNueva;
	delete bloqueRaizAntigua;
	return res;

}

int ArbolBMas::quitar(RegistroClave & reg)
{
	return RES_OK;
}


int ArbolBMas::_obtener_nodo_interno(unsigned int numeroNodoInterno,NodoInterno& nodo){

	Bloque* bloque= archivoNodos.obtener_bloque(numeroNodoInterno);
	if(bloque==NULL)
		return RES_ERROR;
	nodo.desempaquetar(bloque);
	delete bloque;
	return RES_OK;

}


//TODO numBloqueHermano puede ser el izquierdo o el derecho
int ArbolBMas::_resolver_underflow_hoja(unsigned int numBloqueUnderflow, unsigned int numBloqueHermano,
		int numBloqueIzquierdo,bool esUltimo,unsigned int numNodoPadre)
{
	NodoSecuencial nodoUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
	if(_obtener_nodo_secuencial(numBloqueUnderflow,nodoUnderflow)== RES_ERROR)
		return RES_ERROR;

//	Bloque* bUnderflow = archivoNodos.obtener_bloque(numBloqueUnderflow);
//	NodoSecuencial nodoUnderflow(header.minCantBytesClaves, header.maxCantBytesClaves);
//	nodoUnderflow.desempaquetar(bUnderflow);
//	delete bUnderflow;
//
//	Bloque* bHermano = archivoNodos.obtener_bloque(numBloqueHermano);
//	NodoSecuencial hermano(header.minCantBytesClaves, header.maxCantBytesClaves);
//	hermano.desempaquetar(bHermano);
//	delete bHermano;

	NodoSecuencial hermano(header.minCantBytesClaves,header.maxCantBytesClaves) ;
	_obtener_nodo_secuencial(numBloqueHermano,hermano);

	if (hermano.tiene_carga_minima())
	{
		_merge_nodos_secuenciales(nodoUnderflow,hermano);
		if(esUltimo){
			NodoSecuencial nodoIzquierdo(header.minCantBytesClaves,header.maxCantBytesClaves);
			this->_obtener_nodo_secuencial(numBloqueIzquierdo,nodoIzquierdo);
			nodoIzquierdo.set_proximo_nodo(numBloqueUnderflow);
			persistir(&nodoIzquierdo,numBloqueIzquierdo);
		}else{
			int bloqueSiguiente= hermano.get_proximo_nodo();
			nodoUnderflow.set_proximo_nodo(bloqueSiguiente);
		}

		persistir(&nodoUnderflow,numBloqueUnderflow);

		// El hermano vuela del arbol
		Bloque* bloqueVacio = archivoNodos.crear_bloque();
		archivoNodos.sobreescribir_bloque(bloqueVacio,numBloqueHermano);
		delete bloqueVacio;

		return RES_MERGE;
	}
	else
	{
		NodoInterno nodoPadre(header.minCantBytesClaves,header.maxCantBytesClaves);
		if(this->_obtener_nodo_interno(numNodoPadre,nodoPadre)== RES_ERROR)
			return RES_ERROR;

		_balancear_secuenciales(hermano,nodoUnderflow,nodoPadre);
		persistir(&nodoUnderflow,numBloqueUnderflow);
		persistir(&hermano,numBloqueHermano);
		return RES_BALANCEO;
	}

}

int ArbolBMas::_resolver_underflow_interno()
{
	return RES_OK;

}

int ArbolBMas::_merge_nodos_secuenciales(NodoSecuencial & nodoUnderflow, NodoSecuencial & hermano)
{
	std::vector<RegistroClave> registrosDelHermano = hermano.get_registros();
	std::vector<RegistroClave> regsOverflow;

	int res;

	for (unsigned int i = 0; i < registrosDelHermano.size(); i++)
	{
		res = nodoUnderflow.insertar(registrosDelHermano.at(i),regsOverflow);
		if (res == RES_OVERFLOW)
			return RES_ERROR;
	}

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

int ArbolBMas::_buscar_nodo_con_puntero(int punteroAbuscar)
{
	TipoHijo numNodoSecuencial;
	this->obtener_primer_nodo_secuencial(numNodoSecuencial);
	if (numNodoSecuencial == punteroAbuscar)
		return -1; //El primer nodo secuencial no es apuntado por nadie

	NodoSecuencial nodoSecuencialActual(header.minCantBytesClaves,header.maxCantBytesClaves) ;
	_obtener_nodo_secuencial(numNodoSecuencial,nodoSecuencialActual);

	while (nodoSecuencialActual.get_proximo_nodo() != punteroAbuscar)
	{
		_obtener_nodo_secuencial(nodoSecuencialActual.get_proximo_nodo(),nodoSecuencialActual);
		if (nodoSecuencialActual.get_proximo_nodo() == -1)
			return -1; //No deberia llegar nunca aca
	}

	return numNodoSecuencial;
}/* Devuelve el numero del bloque del nodo secuencial cuyo puntero es el parametro. */

int ArbolBMas::_quitar_recursivo(unsigned int& numNodoActual, RegistroClave & registro, int & huboUnderflow)
{

	Bloque* bloqueActual = archivoNodos.obtener_bloque(numNodoActual);
	NodoArbol nodo(TIPO_INTERNO);
	nodo.desempaquetar(bloqueActual);

	if (nodo.es_interno())
	{
		NodoInterno nodoInternoActual (header.minCantBytesClaves,header.maxCantBytesClaves);
		nodoInternoActual.desempaquetar(bloqueActual);

		TipoHijo elHijo;
		unsigned int numBloqueDelHermano;
		_hallar_hijo_correspondiente(&registro,&nodoInternoActual,elHijo);
		/*hallamos el nodo siguiente donde debemos realizar la eliminacion*/
		unsigned int CANT_HIJOS_NODO = nodoInternoActual.get_cantidad_hijos();
		unsigned short posicionElHijo;
		nodoInternoActual.buscar_hijo(elHijo,posicionElHijo);

		//puntero del izquierdo del izquierdo
		int numBloqueDelHermanoDelHermano;
		//la variable sirve en caso que el siguiente nodo a inspeccionar resulte ser el ultimo del nodoInterno actual
		bool esUltimo;

		if (posicionElHijo == CANT_HIJOS_NODO -1)
		{
			// Es el ultimo hijo, devolvemos el hermano izquierdo para resolver mas adelante merge o balanceo
			nodoInternoActual.get_hijo(numBloqueDelHermano,posicionElHijo -1);
			esUltimo= true;

			numBloqueDelHermanoDelHermano = _buscar_nodo_con_puntero(numBloqueDelHermano);
			// FIXME esto puede ser -1 !!!


		}//caso si el  siguiente nodo a inspeccionar es el ultimo del nodoInterno
		else
		{
			esUltimo= false;
			// Devolvemos el hermano derecho
			nodoInternoActual.get_hijo(numBloqueDelHermano,posicionElHijo +1);
		}//caso en el que el siguiente nodo a inspeccionar tiene hermanos a la derecha

		int res = _quitar_recursivo(elHijo,registro, huboUnderflow);
		if (res == RES_OK)
			return res;

		if (res == RES_RECORD_DOESNT_EXIST)
			return RES_RECORD_DOESNT_EXIST;

		// Hubo underflow en un nodo interno o secuencial inferior
		bloqueActual = archivoNodos.obtener_bloque(elHijo);
		NodoArbol nodo(TIPO_INTERNO);
		nodo.desempaquetar(bloqueActual);

		if (nodo.es_hoja())
		{
			NodoSecuencial nodoSecuencialUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
			NodoSecuencial nodoSecuencialHermano(header.minCantBytesClaves,header.maxCantBytesClaves);
			this->_obtener_nodo_secuencial(elHijo,nodoSecuencialUnderflow);
			this->_obtener_nodo_secuencial(numBloqueDelHermano,nodoSecuencialHermano);
			unsigned int numeroNodoPadre;//como la obteneeeemossss??? TODO
			this->_resolver_underflow_hoja(numNodoActual,numBloqueDelHermano,numBloqueDelHermanoDelHermano,
					esUltimo,numeroNodoPadre);
			//FIXME resolver cuanto antesss!!!
			if(!esUltimo){
				int bloqueSiguiente= nodoSecuencialHermano.get_proximo_nodo();
				nodoSecuencialUnderflow.set_proximo_nodo(bloqueSiguiente);
			}else{
				int numeroNodoSecuencialIzquierdo= this->_buscar_nodo_con_puntero(numBloqueDelHermanoDelHermano);
				NodoSecuencial nodoSecuencialIzquierdo(header.minCantBytesClaves,header.maxCantBytesClaves);
				this->_obtener_nodo_secuencial(numeroNodoSecuencialIzquierdo,nodoSecuencialIzquierdo);
				nodoSecuencialIzquierdo.set_proximo_nodo(elHijo);

			}

			//persistir!
		}
		else
		{
			// Hubo underflow en nodo interno
			res = _resolver_underflow_interno();
		}

		delete bloqueActual;
		return res;
	}

	// Quitar de la hoja
	NodoSecuencial nodoSecuencialActual(header.minCantBytesClaves, header.maxCantBytesClaves);
	nodoSecuencialActual.desempaquetar(bloqueActual);

	std::vector<RegistroClave> regsUnderflow;
	int res = nodoSecuencialActual.eliminar(registro.get_clave(),regsUnderflow);

	if (res == RES_UNDERFLOW)
		huboUnderflow = RES_UNDERFLOW_HOJA;

	if (res == RES_OK)
	{
		persistir(&nodoSecuencialActual,numNodoActual);
		return RES_OK;
	}

	for (unsigned int i = 0; regsUnderflow.size(); i++)
	{
		std::vector<RegistroClave> noOverflow;
		nodoSecuencialActual.insertar(regsUnderflow.at(i), noOverflow);
	}

	delete bloqueActual;
	return res;
}


int ArbolBMas::_quitar_recursivo2(unsigned int& numeroNodoActual,
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
			this->persistir(&nodoActualSecuencial,numeroNodoActual);
			tipoUnderflow= RES_NO_UNDERFLOW;
			return RES_OK;
		}/*el borrado del registro en el nodoSecuencial ocurrio exitosamente sin ocurrin underflow. El nodoSecuencial
		modificado es persistido en el archivo*/

		if(resultadoEliminacionRegistro== RES_ERROR)
			return RES_ERROR;
		/*se produjo un error en el borrado del registro. No hay persistencia*/

		/*a continuacion se llevaran los pasos a llevar a cabo en caso de underflow del nodoSecuencial*/
		tipoUnderflow= RES_UNDERFLOW_HOJA;
		this->persistir(&nodoActualSecuencial , numeroNodoActual);
		/*FIXME, esta persistencia deberia ocurrir desde una instancia superior, pero por ahora no se me ocurre otra solucion ... */
		return RES_UNDERFLOW;

	}/*resolucion si el nodoActual es hoja*/


	/*a continuacion se resolvera el caso que el nodoActual sea interno*/
	NodoInterno nodoActualInterno(header.minCantBytesClaves,header.maxCantBytesClaves);
	if( this->_obtener_nodo_interno(numeroNodoActual,nodoActualInterno) == RES_ERROR)
		return RES_ERROR;


	TipoHijo proximoNodoInspeccionar;
	this->_hallar_hijo_correspondiente(&registroEliminar , &nodoActualInterno , proximoNodoInspeccionar);


	int resultadoQuitarInferior= this->_quitar_recursivo2(proximoNodoInspeccionar,registroEliminar,tipoUnderflow);

	if(resultadoQuitarInferior== RES_ERROR)
		return RES_ERROR;
	if(resultadoQuitarInferior== RES_OK)
		return RES_OK;
	/*si la insercion de un nivel inferior resulta en un exito(sin underflow) o un error, no ocurre persistencia en disco y se retorna
	 * el valor pertinente a la instancia superior.*/


	if(resultadoQuitarInferior== RES_UNDERFLOW){

		if(tipoUnderflow== RES_UNDERFLOW_HOJA){
			this->_resolver_underflow_hoja2(&nodoActualInterno,proximoNodoInspeccionar);
		}/*nodos hijos involucrados deben balancearse / mergear y ser persistidos*/
		else{

		}


	}


	return RES_UNDERFLOW;


}


int ArbolBMas::_resolver_underflow_hoja2(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow){


	NodoSecuencial nodoUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
	if( this->_obtener_nodo_secuencial(numeroNodoUnderflow,nodoUnderflow)== RES_ERROR )
		return RES_ERROR;

	const unsigned short CANTIDAD_HIJOS_NODO_PADRE= nodoPadre->get_cantidad_hijos();
	unsigned short posicionNumeroNodoUnderflow;
	if( nodoPadre->buscar_hijo(numeroNodoUnderflow,posicionNumeroNodoUnderflow) == RES_ERROR )
		return RES_ERROR;

	bool numeroNodoUnderflowEsUltimoHijo= ( posicionNumeroNodoUnderflow == (CANTIDAD_HIJOS_NODO_PADRE - 1) );
	TipoHijo numeroNodoHermanoUnderflow;



	NodoSecuencial nodoHermanoUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
	this->_obtener_nodo_secuencial(numeroNodoHermanoUnderflow,nodoHermanoUnderflow);
	if(nodoHermanoUnderflow.tiene_carga_minima()){

		unsigned short posicionClaveEliminar;
		unsigned short posicionHijoEliminar;


		if(!numeroNodoUnderflowEsUltimoHijo){
			nodoPadre->get_hijo(numeroNodoHermanoUnderflow,posicionNumeroNodoUnderflow+1);
			posicionClaveEliminar= posicionNumeroNodoUnderflow;
			posicionHijoEliminar= posicionClaveEliminar+1;
		}
		else{
			nodoPadre->get_hijo(numeroNodoHermanoUnderflow,posicionNumeroNodoUnderflow-1);
			posicionClaveEliminar= posicionNumeroNodoUnderflow-1;
			posicionHijoEliminar= posicionClaveEliminar+1;
		}

		if(_merge_secuenciales2(nodoPadre,numeroNodoUnderflow,numeroNodoHermanoUnderflow,numeroNodoUnderflowEsUltimoHijo,
				posicionClaveEliminar,posicionHijoEliminar)== RES_ERROR)
			return RES_ERROR;
		return RES_MERGE;

	}/*si el nodo hermano tiene carga minima debo mergear*/

	_balancear_secuenciales2(nodoPadre,numeroNodoUnderflow,numeroNodoHermanoUnderflow,numeroNodoUnderflowEsUltimoHijo);
	return RES_BALANCEO;


}


int ArbolBMas::_merge_secuenciales2(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow,
		unsigned int numeroNodoHermanoUnderflow,bool numeroNodoUnderflowEsUltimoHijo ,
		unsigned short posicionClaveEliminar,unsigned short posicionHijoEliminar){

	NodoSecuencial nodoUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
	if( this->_obtener_nodo_secuencial(numeroNodoUnderflow,nodoUnderflow) == RES_ERROR)
		return RES_ERROR;
	NodoSecuencial nodoHermanoUnderflow(header.minCantBytesClaves,header.maxCantBytesClaves);
	if( this->_obtener_nodo_secuencial(numeroNodoUnderflow,nodoHermanoUnderflow) == RES_ERROR){
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
			this->persistir(&nodoIzquierdo,numeroNodoIzquierdo);
		}
	}else{

		int numeroNodoDerecho= nodoHermanoUnderflow.get_proximo_nodo();
		nodoUnderflow.set_proximo_nodo(numeroNodoDerecho);

	}


	this->persistir(&nodoUnderflow,numeroNodoUnderflow);
	this->_liberar_nodo(numeroNodoHermanoUnderflow);

	ClaveX cr;
	nodoPadre->remover_clave(posicionClaveEliminar,cr);
	nodoPadre->remover_hijo(posicionHijoEliminar);

	return RES_OK;

}


int ArbolBMas::_balancear_secuenciales2(NodoInterno* nodoPadre,unsigned int numeroNodoUnderflow,
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


	this->persistir(&nodoUnderflow,numeroNodoUnderflow);
	this->persistir(&nodoHermano,numeroNodoHermanoUnderflow);

	return RES_OK;


}


int ArbolBMas::_liberar_nodo(unsigned int numeroNodo){
	Bloque* bloqueVacio= archivoNodos.crear_bloque();
	int liberacion= archivoNodos.sobreescribir_bloque(bloqueVacio,numeroNodo);
	delete bloqueVacio;

	return liberacion;
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

	reg = *regCopia;

	delete regCopia;
	return res;
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

	delete bloqueActual;
	TipoHijo numeroBloqueHijoCorrespondiente;
	this->_hallar_hijo_correspondiente(registro,&ni,numeroBloqueHijoCorrespondiente);
	numeroBloque= numeroBloqueHijoCorrespondiente;

	return _hallar_hoja(registro,numeroBloque);


}


int ArbolBMas::_split_hoja(NodoSecuencial* nodoActual,vector<RegistroClave>* registrosOverflow,
		TipoHijo& hijoPromocionado,ClaveX* clavePromocionada,TipoPuntero nodoSecuencialSiguiente){

//	{
//		cout<<"imprimiendo registrosOverflow: "<<endl;
//		const unsigned short CANTIDAD_REGISTROS_OVERFLOW= registrosOverflow->size();
//		for(unsigned short i=0;i<CANTIDAD_REGISTROS_OVERFLOW;i++){
//			ClaveX unaClave= registrosOverflow->at(i).get_clave();
//			string datoClave;
//			unaClave.get_clave(datoClave);
//			cout<<datoClave<<endl;
//		}
//
//		cout<<"imprimiendo claves nodo split: "<<endl;
//		nodoActual->imprimir();
//
//	}//TODO BORRAR

	NodoSecuencial nodoSecuencialNuevo(header.minCantBytesClaves,header.maxCantBytesClaves);
	const unsigned short CANTIDAD_REGISTROS_OVERFLOW= registrosOverflow->size();
	if(CANTIDAD_REGISTROS_OVERFLOW== 0)
		return RES_ERROR;
	(*clavePromocionada)= registrosOverflow->at(0).get_clave();

	vector<RegistroClave> ro;
	for(int i=0;i<CANTIDAD_REGISTROS_OVERFLOW;i++){
		nodoSecuencialNuevo.insertar(registrosOverflow->at(i),ro);
	}/*en el nodo secuencial nuevo agrego los registros en overflow y remuevo dichos registros del nodoSecuencialActual*/
	nodoSecuencialNuevo.set_proximo_nodo(nodoSecuencialSiguiente);

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

	cout<<"claves split interno:"<<endl;
	nodo->imprimir_claves();
	cout<<"hijos split interno:"<<endl;
	nodo->imprimir_hijos();
	cout<<"fin imprimir split interno--------------------------------------------"<<endl;


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


	for(unsigned short i=numeroClaveMitad;i<CANTIDAD_CLAVES;i++){
		TipoHijo unHijo;
		int res=nodo->get_hijo(unHijo , i+1);
		if(res!= RES_ERROR){
			nodoNuevo.insertar_hijo(unHijo);//FIXME
			cout<<"hijo a pasar: "<<unHijo<<endl;
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
		RegistroClave* registro , TipoHijo& hijoPromocionado,ClaveX* clavePromocionada, TipoPuntero& nodoSecuencialSiguiente)
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
				clavePromocionada,nodoSecuencialSiguiente);
		/*divido el nodoSecuencial en overflow , guardo el nodo nuevo en el archivo y en hijoPromocionado retorno el numero de
		 * bloque donde el nodoNuevo se guardo. En clavePromocionada guardo la clave que debera ser insertada en el nodoInterno
		 * padre.*/
		Bloque* bloqueNodoSecuencialActualModificado = archivoNodos.crear_bloque();
		nodoSecuencialActual.empaquetar(bloqueNodoSecuencialActualModificado);
		this->archivoNodos.sobreescribir_bloque(bloqueNodoSecuencialActualModificado,numeroBloqueActual);
		/*persisto en el archivo el nodoSecuencial modificado*/

		delete bloqueNodoSecuencialActualModificado;

		return RES_OVERFLOW;

	}/*si el nodo actual es del tipo hoja, insertar el registro. En caso de ocurrir un overflow se guarda en clavePromocionada
	la clave del registro promocionado.*/


	NodoInterno nodoActualInterno(header.minCantBytesClaves,header.maxCantBytesClaves);
	nodoActualInterno.desempaquetar(bloqueActual);
	delete bloqueActual;

	/*TODO BORRAR*/
	cout<<"nodoActualInterno claves:"<<endl;
	nodoActualInterno.imprimir_claves();
	cout<<"nodoActualInterno hijos:"<<endl;
	nodoActualInterno.imprimir_hijos();

	unsigned int numeroBloqueHijo;
	this->_hallar_hijo_correspondiente(registro,&nodoActualInterno,
			numeroBloqueHijo);
	/*busco el siguiente hijo para acercarme a la hoja*/

	/*determino el nodoSecuencialSiguiente ------------------------------------------------------------------*/
	nodoActualInterno.obtener_hijo_siguiente_a(numeroBloqueHijo,nodoSecuencialSiguiente);
	/*determino el nodoSecuencialSiguiente ------------------------------------------------------------------*/


	int resultadoInsercion= this->_insertar_recursivo(numeroBloqueHijo,registro,
			hijoPromocionado,clavePromocionada,nodoSecuencialSiguiente);

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
		Bloque* bloqueNodoInternoActualModificado = archivoNodos.crear_bloque();
		nodoActualInterno.empaquetar(bloqueNodoInternoActualModificado);
		this->archivoNodos.sobreescribir_bloque(bloqueNodoInternoActualModificado,numeroBloqueActual);
		/*se sobreescribe el nodo original en el archivo de bloques*/

		delete bloqueNodoInternoActualModificado;
		return RES_OK;
	}
	if(resultadoInsercionNodoInterno== RES_ERROR)
		return RES_ERROR;


	_split_interno(&nodoActualInterno,clavePromocionada,hijoPromocionado);
	/*realiza el split. y guarda el nodo nuevo en el archivo. En hijoPromocionado se devolvera el hijo del nodoInterno nuevo creado, en
	 * clavePromocionada se retorna la clave a insertar en un nodoInterno en una instancia superior de _insertar_recursivo . El nodoInterno
	 * nuevo creado es guardado en el archivo*/

	Bloque* bloqueNodoInternoActualModificado = archivoNodos.crear_bloque();
	nodoActualInterno.empaquetar(bloqueNodoInternoActualModificado);
	this->archivoNodos.sobreescribir_bloque(bloqueNodoInternoActualModificado,numeroBloqueActual);
	/*se sobreescribe el nodo original en el archivo de bloques*/

	delete bloqueNodoInternoActualModificado;
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
	Bloque* bloqueActual;

	while(nodoActual.es_interno()){

		bloqueActual= archivoNodos.obtener_bloque(primerHijo);
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
