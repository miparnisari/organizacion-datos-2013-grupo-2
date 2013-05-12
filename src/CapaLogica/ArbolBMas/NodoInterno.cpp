#include "NodoInterno.h"

NodoInterno::NodoInterno(unsigned int p_minCantidadBytes, unsigned int p_maxCantidadBytes)
	:NodoArbol(TIPO_INTERNO)
{
	maxCantidadBytes = p_maxCantidadBytes;
	minCantidadBytes = p_minCantidadBytes;
	cantidadBytesOcupados = 0;
	vectorHijos.push_back(HIJO_INVALIDO);
	/* Primer hijo, para que cuando agreguemos una clave solo halla que agregar un hijo */
}

NodoInterno::~NodoInterno()
{
}

unsigned short NodoInterno::get_cantidad_claves()
{
	return vectorClaves.size();
}

unsigned short NodoInterno::get_cantidad_hijos()
{
	return vectorHijos.size();
}

bool NodoInterno::esta_vacio()
{
	return (vectorClaves.size()== 0);
}

bool NodoInterno::hay_overflow(){

	return (cantidadBytesOcupados > maxCantidadBytes);

}

bool NodoInterno::hay_underflow(){

	return ( cantidadBytesOcupados < minCantidadBytes );

}

int NodoInterno::_insertar_si_overflow(ClaveX* claveInsertar,unsigned short& posicionInsertar ,
		unsigned short& posicionClavePromocionar){

	const unsigned short CANTIDAD_CLAVES= vectorClaves.size();
	bool posicionHallada= false;
	posicionInsertar= CANTIDAD_CLAVES;

	for(unsigned short i= 0; i<CANTIDAD_CLAVES && !posicionHallada ; i++){

		ClaveX unaClave= vectorClaves.at(i);
		if(unaClave > (*claveInsertar)){
			posicionInsertar= i;
			posicionHallada= true;
		}

	}

	//FIXME
	return RES_OK;


}


int NodoInterno::get_clave(unsigned short numeroClave,ClaveX& clave){

	if(numeroClave >= vectorClaves.size())
		return RES_ERROR;

	clave= vectorClaves.at(numeroClave);
	return RES_OK;

}


int NodoInterno::get_hijo(TipoHijo& hijo,unsigned short numeroHijo)
{
	if (esta_vacio())
		return RES_ERROR;
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
	return (cantidadBytesOcupados + vectorHijos.size()*sizeof(TipoHijo));
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


}//FIXME Para que sirve este metodo!? Es lo mismo que get_clave(pos)...


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


int NodoInterno::remover_clave(const ClaveX& clave){

	unsigned short posicionClave;
	if (this->buscar_clave(clave,posicionClave)== RES_ERROR)
		return RES_ERROR;

	vectorClaves.erase(vectorClaves.begin() + posicionClave);

	return posicionClave;

}

/*
int NodoInterno::_insertar_clave(const ClaveX& claveInsertar,ClaveX& claveMitad ,
		unsigned short& posicionInsercion){

	unsigned short espacioDisponible = maxCantidadBytes - cantidadBytesOcupados;
	bool overflow = ( espacioDisponible < claveInsertar.get_tamanio_empaquetado() );

	vectorClaves.push_back( claveInsertar );
	std::sort( vectorClaves.begin() , vectorClaves.end() );

	if(overflow){
		unsigned short mitad= ceil(vectorClaves.size() / 2.0);
		vectorClaves.erase(vectorClaves.begin()+mitad);

	}

	this->buscar_clave(claveInsertar , posicionInsercion);

	if(overflow)
		return RES_OVERFLOW;

	return RES_OK;



}
*/

int NodoInterno::agregar_clave(const ClaveX& clave)
{
	unsigned short var;
	if (buscar_clave(clave, var) != RES_ERROR)
		return RES_RECORD_EXISTS;

	vectorClaves.push_back(clave);
	std::sort(vectorClaves.begin(), vectorClaves.end());


	set_hijo_derecho(clave,HIJO_INVALIDO);


	return RES_OK;
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

	RegistroVariable registroTipoNodo, registroClaves, registroHijos;

	registroTipoNodo.agregar_campo( (char*)&tipoNodo , sizeof(tipoNodo) );

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

	if(posicion == this->POSICION_FIN_VECTOR_HIJOS){
		vectorHijos.push_back(hijo);
		this->cantidadBytesOcupados+= sizeof(hijo);
		if(hay_overflow())
			return RES_OVERFLOW;
		else
			return RES_OK;
	}

	if(posicion>= vectorClaves.size())
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


/*METODOS AGREGADOS POR MARTIN -----------------------------------------------*/


