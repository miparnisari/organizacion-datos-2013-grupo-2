#include "NodoInterno.h"

NodoInterno::NodoInterno(unsigned int p_minCantidadBytes, unsigned int p_maxCantidadBytes)
	:NodoArbol(TIPO_INTERNO)
{
	maxCantidadBytes = p_maxCantidadBytes;
	minCantidadBytes = p_minCantidadBytes;
	cantidadBytesOcupados = 0;
	vectorHijos.push_back(HIJO_INVALIDO);
	cantidadBytesOcupados+= sizeof(TipoHijo);
	/* Primer hijo, para que cuando agreguemos una clave solo halla que agregar un hijo */
}

NodoInterno::~NodoInterno()
{
}

unsigned short NodoInterno::get_cantidad_claves()
{
	unsigned short retornar= vectorClaves.size();
	return retornar;
}

unsigned short NodoInterno::get_cantidad_hijos()
{
	unsigned short retornar= vectorHijos.size();
	return retornar;
}

bool NodoInterno::esta_vacio()const
{
	return (vectorClaves.size()== 0);
}

bool NodoInterno::hay_overflow()const{

	return (cantidadBytesOcupados > maxCantidadBytes);

}

bool NodoInterno::hay_underflow()const{

	return ( cantidadBytesOcupados < minCantidadBytes );

}




int NodoInterno::get_clave(unsigned short numeroClave,ClaveX& clave){

	if(numeroClave >= vectorClaves.size())
		return RES_ERROR;

	clave= vectorClaves.at(numeroClave);
	return RES_OK;

}


int NodoInterno::get_hijo(TipoHijo& hijo,unsigned short numeroHijo)
{
	if (numeroHijo >= this->get_cantidad_hijos())
		return RES_ERROR;

	hijo = vectorHijos.at(numeroHijo);

	return RES_OK;
}


int NodoInterno::get_clave_mitad(ClaveX& clave){

	if (this->esta_vacio())
		return RES_ERROR;

	const unsigned short CANTIDAD_CLAVES = vectorClaves.size();
	const unsigned short MITAD = (unsigned short)ceil( CANTIDAD_CLAVES/2 );
	this->get_clave(MITAD, clave);

	return RES_OK;

}

int NodoInterno::get_tamanio_ocupado()
{
	return (cantidadBytesOcupados );
}

int NodoInterno::buscar_clave(const ClaveX& clave,unsigned short& posicionClave)
{
	if(this->esta_vacio())
		return RES_ERROR;

	ClaveX unaClave;
	const unsigned short CANTIDAD_CLAVES= this->get_cantidad_claves();
	for(int i=0;i<CANTIDAD_CLAVES;i++){

		this->get_clave(i,unaClave);
		if( (clave)== (unaClave) ){
			posicionClave= i;
			return i;
		}

	}

	return RES_ERROR;


}//


int NodoInterno::get_hijo_izquierdo(TipoHijo& hijo,const ClaveX& clave){

	if(this->esta_vacio())
		return RES_ERROR;

	unsigned short posicionClave;
	if( this->buscar_clave(clave,posicionClave)== RES_ERROR )
		return RES_ERROR;

	return this->get_hijo(hijo,posicionClave);

}

int NodoInterno::get_hijo_derecho(TipoHijo& hijo,const ClaveX& clave){

	if(this->esta_vacio())
		return RES_ERROR;

	unsigned short posicionClave;
	if( this->buscar_clave(clave,posicionClave)== RES_ERROR )
		return RES_ERROR;

	return this->get_hijo(hijo,posicionClave+1);

}





int NodoInterno::remover_clave(unsigned short numeroClave , ClaveX& clave){

	int res= this->get_clave(numeroClave,clave);
	if(res== RES_ERROR)
		return RES_ERROR;

	return this->remover_clave(clave , numeroClave);

}



int NodoInterno::set_hijo_izquierdo(const ClaveX& clave, TipoHijo valor)
{

	unsigned short posicion;
	if( this->buscar_clave(clave,posicion) ==RES_ERROR)
		return RES_ERROR;

	vectorHijos[posicion]= valor;

	return RES_OK;
}

int NodoInterno::set_hijo_derecho(const ClaveX& clave, TipoHijo valor)
{
	unsigned short posicion;
	if( this->buscar_clave(clave,posicion) ==RES_ERROR)
		return RES_ERROR;

	vectorHijos.insert( vectorHijos.begin() + posicion ,valor);

	return RES_OK;
}

int NodoInterno::empaquetar(Bloque* bloque)
{
	/*if (bloque == NULL)
		return RES_ERROR;

	RegistroVariable registro;

	registro.agregar_campo((char*)&tipoNodo,sizeof(tipoNodo));

	unsigned int contadorClaves = 0;
	unsigned int contadorHijos = 1;
	TipoHijo hijo = vectorHijos.at(0);
	registro.agregar_campo((char*)&hijo,sizeof(hijo));
	while (contadorClaves < get_cantidad_claves() && contadorHijos < get_cantidad_hijos())
	{
		ClaveX clave = vectorClaves.at(contadorClaves);
		TipoHijo hijo = vectorHijos.at(contadorHijos);

		char* buffer = new char[clave.get_tamanio_empaquetado()]();
		clave.empaquetar(buffer);
		registro.agregar_campo(buffer,strlen(buffer));
		registro.agregar_campo((char*)&hijo,sizeof(hijo));

		delete[](buffer);
		contadorClaves++;
		contadorHijos++;
	}

	return RES_OK;*/

	if(bloque== NULL)
		return RES_ERROR;

	RegistroVariable registroTipoNodo;
	RegistroVariable registroClaves;
	RegistroVariable registroHijos;

	char ti= TIPO_INTERNO;
	registroTipoNodo.agregar_campo( (char*)&ti , sizeof(tipoNodo) );

	const unsigned short CANTIDAD_CLAVES= vectorClaves.size();
	for(unsigned short i=0;i<CANTIDAD_CLAVES;i++){

		ClaveX unaClave= vectorClaves[i];
		unsigned short tamanioClaveEmpaquetada= unaClave.get_tamanio_empaquetado();
		char* bufferClaveEmpaquetada= new char[tamanioClaveEmpaquetada];
		unaClave.empaquetar(bufferClaveEmpaquetada);
		registroClaves.agregar_campo(bufferClaveEmpaquetada,tamanioClaveEmpaquetada);

		delete[] bufferClaveEmpaquetada;

	}

	const unsigned short CANTIDAD_HIJOS= vectorHijos.size();
	for(unsigned short i=0;i<CANTIDAD_HIJOS;i++){

		TipoHijo unHijo= vectorHijos[i];
		registroHijos.agregar_campo( (char*)&unHijo , sizeof(unHijo) );


	}

	bloque->agregar_registro(&registroTipoNodo);
	bloque->agregar_registro(&registroClaves);
	bloque->agregar_registro(&registroHijos);

	return RES_OK;

}

int NodoInterno::desempaquetar(const Bloque* bloque)
{

	if(bloque== NULL)
		return RES_ERROR;

	this->limpiar();

	RegistroVariable registroTipoNodo,registroClaves,registroHijos;
	bloque->recuperar_registro(&registroTipoNodo,0);
	bloque->recuperar_registro(&registroClaves,1);
	bloque->recuperar_registro(&registroHijos,2);

	registroTipoNodo.recuperar_campo((char*)&tipoNodo , 0);

	const unsigned short CANTIDAD_CLAVES= registroClaves.get_cantidad_campos();
	for(unsigned short i=0;i<CANTIDAD_CLAVES;i++){

		ClaveX unaClave;
		unsigned short tamanioClaveEmpaquetada= registroClaves.get_tamanio_campo(i);
		char* bufferClaveEmpaquetada= new char[tamanioClaveEmpaquetada];
		registroClaves.recuperar_campo(bufferClaveEmpaquetada,i);
		unaClave.desempaquetar(bufferClaveEmpaquetada,tamanioClaveEmpaquetada);
		this->vectorClaves.push_back(unaClave);

		cantidadBytesOcupados+= tamanioClaveEmpaquetada;

		delete[] bufferClaveEmpaquetada;

	}

	const unsigned short CANTIDAD_HIJOS= registroHijos.get_cantidad_campos();
	for(unsigned short i=0;i<CANTIDAD_HIJOS;i++){

		TipoHijo unHijo;
		registroHijos.recuperar_campo( (char*)&unHijo , i );
		vectorHijos.push_back(unHijo);

		cantidadBytesOcupados+= sizeof(unHijo);

	}

	cantidadBytesOcupados+= sizeof(tipoNodo);

	return RES_OK;

}

/*METODOS AGREGADOS POR MARTIN -----------------------------------------------*/

int NodoInterno::insertar_clave(const ClaveX& clave,
		unsigned short& posicionInsercion){

	if( this->buscar_clave(clave,posicionInsercion)!= RES_ERROR )
		return RES_ERROR;

	vectorClaves.push_back(clave);
	std::sort( vectorClaves.begin() , vectorClaves.end() );
	this->buscar_clave(clave , posicionInsercion);
	const unsigned short TAMANIO_EMPAQUETADO_CLAVE= clave.get_tamanio_empaquetado();
	this->cantidadBytesOcupados+= TAMANIO_EMPAQUETADO_CLAVE;

	bool overflow= hay_overflow();
	if(overflow)
		return RES_OVERFLOW;
	if(hay_underflow())
		return RES_UNDERFLOW;

	return RES_OK;


}


int NodoInterno::insertar_hijo(TipoHijo hijo,unsigned short posicion){

	const unsigned short CANTIDAD_HIJOS= vectorHijos.size();

	if(posicion == this->POSICION_FIN_VECTOR_HIJOS || posicion== CANTIDAD_HIJOS){
		vectorHijos.push_back(hijo);
		this->cantidadBytesOcupados+= sizeof(hijo);
		if(hay_overflow())
			return RES_OVERFLOW;
		if(hay_underflow())
			return RES_UNDERFLOW;

		return RES_OK;
	}

	if(posicion> CANTIDAD_HIJOS)
		return RES_ERROR;

	vectorHijos.insert( vectorHijos.begin() + posicion , hijo );
	this->cantidadBytesOcupados+= sizeof(hijo);
	if(hay_overflow())
		return RES_OVERFLOW;
	if(hay_underflow())
		return RES_UNDERFLOW;
	return RES_OK;

}


int NodoInterno::modificar_hijo(TipoHijo valor,unsigned short numeroHijo){

	if(numeroHijo >= vectorHijos.size())
		return RES_ERROR;

	this->vectorHijos[numeroHijo]= valor;
	return RES_OK;

}


int NodoInterno::remover_clave(const ClaveX& clave,unsigned short& posicionEliminacion){

	unsigned short posicionBusqueda;
	int busqueda= this->buscar_clave(clave , posicionBusqueda);

	if(busqueda== RES_ERROR)
		return RES_ERROR;

	this->vectorClaves.erase(vectorClaves.begin()+posicionBusqueda);
	this->cantidadBytesOcupados-= clave.get_tamanio_empaquetado();
	posicionEliminacion= posicionBusqueda;

	if(this->hay_overflow())
		return RES_OVERFLOW;
	if(this->hay_underflow())
		return RES_UNDERFLOW;

	return RES_OK;

}


int NodoInterno::remover_hijo(unsigned short numeroHijo){

	if(numeroHijo >= vectorHijos.size())
		return RES_ERROR;

	this->vectorHijos.erase( vectorHijos.begin() + numeroHijo );
	this->cantidadBytesOcupados-= sizeof(numeroHijo);
	if(this->hay_underflow())
		return RES_UNDERFLOW;
	if(this->hay_overflow())
		return RES_OVERFLOW;

	return RES_OK;

}

void NodoInterno::imprimir()
{
	TipoHijo hijo;
	unsigned short numHijo = 0;
	get_hijo(hijo,numHijo);
	std::cout << hijo << "-";
	for (unsigned int i = 0; i < vectorClaves.size(); i++)
	{
		numHijo ++;
		vectorClaves.at(i).imprimir_dato();
		get_hijo(hijo,numHijo);
		std::cout << " - " << hijo << " - ";
	}
}


void NodoInterno::imprimir_claves()const{

	if(this->esta_vacio())
		cout<<"NODO_VACIO"<<endl;

	const unsigned short CANTIDAD_CLAVES= vectorClaves.size();
	for(unsigned short i=0;i<CANTIDAD_CLAVES;i++){
		vectorClaves.at(i).imprimir_dato();
		cout<<endl;
	}

}


void NodoInterno::imprimir_hijos()const{

	const unsigned short CANTIDAD_HIJOS= vectorHijos.size();
	for(unsigned short i=0;i<CANTIDAD_HIJOS;i++){
		cout<<vectorHijos.at(i);
		cout<<endl;
	}

}


int NodoInterno::insertar_hijo_derecho(const ClaveX& clave,
		TipoHijo valor){


	unsigned short posClave;
	if(this->buscar_clave(clave,posClave)== RES_ERROR)
		return RES_ERROR;

	return this->insertar_hijo(valor,posClave+1);

}


int NodoInterno::insertar_hijo_izquierdo(const ClaveX& clave,
		TipoHijo valor= HIJO_INVALIDO){

	unsigned short posClave;
	if(this->buscar_clave(clave,posClave)== RES_ERROR)
		return RES_ERROR;

	return this->insertar_hijo(valor,posClave);

}

int NodoInterno::modificar_hijo_derecho(const ClaveX& clave,TipoHijo valor){

	unsigned short posClave;
	if(this->buscar_clave(clave,posClave)== RES_ERROR)
		return RES_ERROR;

	return this->modificar_hijo(valor,posClave+1);

}


int NodoInterno::modificar_hijo_izquierdo(const ClaveX& clave,TipoHijo valor){

	unsigned short posClave;
	if(this->buscar_clave(clave,posClave)== RES_ERROR)
		return RES_ERROR;

	return this->modificar_hijo(valor,posClave);

}


void NodoInterno::limpiar(){

	if(vectorClaves.size())
		vectorClaves.erase( vectorClaves.begin() , vectorClaves.end() );

	if(vectorHijos.size())
		vectorHijos.erase( vectorHijos.begin() , vectorHijos.end() );

	this->cantidadBytesOcupados= 0;

}


int NodoInterno::buscar_hijo(TipoHijo valorHijo,unsigned short& posicionHijo){

	const unsigned short CANTIDAD_HIJOS= vectorHijos.size();
	for(unsigned short i=0;i<CANTIDAD_HIJOS;i++){

		TipoHijo unHijo= vectorHijos.at(i);
		if(unHijo == valorHijo){
			posicionHijo= i;
			return RES_OK;
		}

	}

	return RES_ERROR;

}


int NodoInterno::obtener_hijo_siguiente_a(TipoHijo valorHijo,int& hijoSiguiente){

	unsigned short posicionHijo;
	int res= buscar_hijo(valorHijo,posicionHijo);
	if(res== RES_ERROR)
		return RES_ERROR;

	const unsigned short CANTIDAD_HIJOS= this->vectorHijos.size();
	if(posicionHijo== (CANTIDAD_HIJOS-1) )
		hijoSiguiente= -1;/*la posicion de valorHijo es la ULTIMA*/
	else{
		TipoHijo th;
		get_hijo(th,posicionHijo+1);
		hijoSiguiente= (int)th;
	}

	return RES_OK;

}


/*METODOS AGREGADOS POR MARTIN -----------------------------------------------*/


