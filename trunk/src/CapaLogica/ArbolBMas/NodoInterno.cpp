#include "NodoInterno.h"

NodoInterno::NodoInterno(unsigned int p_minCantidadBytesClaves, unsigned int p_maxCantidadBytesClaves)
	:Nodo('I')
{
//	tamanio = tamanioNodo;
//	espacioLibre = tamanio - sizeof(espacioLibre);
	unsigned int minCantidadBytesClaves = p_minCantidadBytesClaves;
	unsigned int maxCantidadBytesClaves = p_maxCantidadBytesClaves;
	unsigned int cantidadBytesOcupadosClaves = 0;
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


ClaveX* NodoInterno::get_clave(unsigned short numeroClave)
{
	if (esta_vacio())
		return NULL;
	if (numeroClave >= this->get_cantidad_claves())
		return NULL;

	return &vectorClaves.at(numeroClave);
}


int NodoInterno::get_hijo(unsigned short& hijo,unsigned short numeroHijo)
{
	if (esta_vacio())
		return RES_ERROR;
	if (numeroHijo >= this->get_cantidad_hijos())
		return RES_ERROR;

	hijo = vectorHijos.at(numeroHijo);

	return RES_OK;
}


ClaveX* NodoInterno::get_clave_mitad()
{
	if (this->esta_vacio())
		return NULL;

	const unsigned short CANTIDAD_CLAVES = vectorClaves.size();
	const unsigned short MITAD = (unsigned short)(CANTIDAD_CLAVES/2);

	return this->get_clave(MITAD);

}

int NodoInterno::get_tamanio_ocupado()
{
	return (cantidadBytesOcupadosClaves + vectorHijos.size()*sizeof(unsigned short));
}

int NodoInterno::buscar_clave(const ClaveX* clave,unsigned short& posicionClave)
{

	const unsigned short CANTIDAD_CLAVES= this->get_cantidad_claves();
	for(int i=0;i<CANTIDAD_CLAVES;i++){

		ClaveX* unaClave= this->get_clave(i);
		if( (*clave)== (*unaClave) ){
			posicionClave= i;
			return i;
		}

	}

	return RES_ERROR;


}//FIXME Para que sirve este metodo!? Es lo mismo que get_clave(pos)...


int NodoInterno::get_hijo_izquierdo(unsigned short& hijo, ClaveX* clave){

	if(this->esta_vacio())
		return RES_ERROR;

	unsigned short posicionClave;
	if( this->buscar_clave(clave,posicionClave)== RES_ERROR )
		return RES_ERROR;

	return this->get_hijo(hijo,posicionClave);

}

int NodoInterno::get_hijo_derecho(unsigned short& hijo, ClaveX* clave){

	if(this->esta_vacio())
		return RES_ERROR;

	unsigned short posicionClave;
	if( this->buscar_clave(clave,posicionClave)== RES_ERROR )
		return RES_ERROR;

	return this->get_hijo(hijo,posicionClave+1);

}


int NodoInterno::remover_clave(const ClaveX* clave,unsigned short numeroClave){

	if(this->get_clave(numeroClave)== NULL)
		return RES_ERROR;

	return this->remover_clave(clave);

}


int NodoInterno::remover_clave(const ClaveX* clave){

	unsigned short posicionClave;
	if (this->buscar_clave(clave,posicionClave)== RES_ERROR)
		return RES_ERROR;

	vectorClaves.erase(vectorClaves.begin() + posicionClave);

	return posicionClave;

}


int NodoInterno::_insertar_clave(ClaveX* claveInsertar,ClaveX& claveMitad ,
		unsigned short& posicionInsercion){

	if(claveInsertar== NULL)
		return RES_ERROR;
	unsigned short espacioDisponible= maxCantidadBytesClaves - cantidadBytesOcupadosClaves;
	bool overflow = ( espacioDisponible < claveInsertar->get_tamanio_empaquetado() );

	vectorClaves.push_back( (*claveInsertar) );
	std::sort( vectorClaves.begin() , vectorClaves.end() );

	if(overflow){

		unsigned short mitad= ceil(vectorClaves.size() / 2.0);
		IteradorVectorClaves it;
		for(int i=0;i<mitad;i++, it++);
		claveMitad= vectorClaves.at(mitad);
		vectorClaves.erase(it);

	}

	this->buscar_clave(claveInsertar , posicionInsercion);

	if(overflow)
		return RES_OVERFLOW;

	return RES_OK;



}


int NodoInterno::agregar_clave(ClaveX* clave)
{
	if (clave == NULL)
		return RES_ERROR;
	if (clave->get_tamanio_empaquetado() + cantidadBytesOcupadosClaves > maxCantidadBytesClaves )
		return RES_OVERFLOW;
	unsigned short var;
	if (buscar_clave(clave, var) == RES_ERROR)
		return RES_RECORD_EXISTS;

	vectorClaves.push_back(*clave);
	std::sort(vectorClaves.begin(), vectorClaves.end());


	set_hijo_izquierdo(clave,HIJO_INVALIDO);
	set_hijo_derecho(clave,HIJO_INVALIDO);

	cantidadBytesOcupadosClaves += clave->get_tamanio_empaquetado();
	return RES_OK;
}

int NodoInterno::set_hijo_izquierdo(ClaveX* clave, unsigned short valor)
{
	//TODO
	return RES_OK;
}

int NodoInterno::set_hijo_derecho(ClaveX* clave, unsigned short valor)
{
	//TODO
	return RES_OK;
}

int NodoInterno::empaquetar(Bloque* bloque)
{
	if (bloque == NULL)
		return RES_ERROR;

	RegistroVariable registro;

	registro.agregar_campo((char*)&tipoNodo,sizeof(tipoNodo));

	unsigned int contadorClaves = 0;
	unsigned int contadorHijos = 1;
	unsigned short hijo = vectorHijos.at(0);
	registro.agregar_campo((char*)&hijo,sizeof(hijo));
	while (contadorClaves < get_cantidad_claves() && contadorHijos < get_cantidad_hijos())
	{
		ClaveX clave = vectorClaves.at(contadorClaves);
		unsigned short hijo = vectorHijos.at(contadorHijos);

		char* buffer = new char[clave.get_tamanio_empaquetado()]();
		clave.empaquetar(buffer);
		registro.agregar_campo(buffer,strlen(buffer));
		registro.agregar_campo((char*)&hijo,sizeof(hijo));

		delete[](buffer);
		contadorClaves++;
		contadorHijos++;
	}

	return RES_OK;
}

int NodoInterno::desempaquetar(const Bloque* bloque)
{
	if (bloque == NULL)
		return RES_ERROR;

	return RES_OK;
}
