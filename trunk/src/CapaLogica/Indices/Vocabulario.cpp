/*
 * Vocabulario.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: maine
 */

#include "Vocabulario.h"

Vocabulario::Vocabulario() {

}

Vocabulario::~Vocabulario() {
}

int Vocabulario::crear(std::string nombre_archivo)
{
	fileName = nombre_archivo;
	int res = arbol.crear(fileName,1000);
	return res;
}
int Vocabulario::eliminar ()
{
	int res = arbol.eliminar(fileName);
	return res;
}

int Vocabulario::abrir(std::string nombre_archivo)
{
	fileName = nombre_archivo;
	int res = arbol.abrir(fileName,"rb+");
	return res;

}
int Vocabulario::cerrar ()
{
	int res = arbol.cerrar();
	return res;
}

int Vocabulario::agregar_termino (const Termino_t & termino, const IDtermino_t id_termino, const ReferenciaListaInvertida_t ref_lista_inv)
{
	ClaveX clave;
	clave.set_clave(termino);
	RegistroClave registro;
	registro.set_clave(clave);

	int resultadoBusqueda = arbol.buscar(registro);
	if (resultadoBusqueda == RES_OK )
	{
		return RES_RECORD_EXISTS;
	}
	else if (resultadoBusqueda == RES_RECORD_DOESNT_EXIST)
	{
		registro.agregar_campo((char*)& id_termino,sizeof(id_termino));
		registro.agregar_campo((char*)& ref_lista_inv,sizeof(ref_lista_inv));

		int res = arbol.agregar(registro);
		return res;
	}

	return RES_ERROR;
}

int Vocabulario::buscar_termino
		(const Termino_t & termino,
		IDtermino_t & id_termino,
		ReferenciaListaInvertida_t & referencia)
{
	ClaveX clave;
	clave.set_clave(termino);
	RegistroClave registro;
	registro.set_clave(clave);

	int resultadoBusqueda = arbol.buscar(registro);
	if (resultadoBusqueda == RES_OK )
	{
		registro.recuperar_campo((char*)& id_termino,1);
		registro.recuperar_campo((char*)& referencia,2);
		return RES_OK;
	}
	else if (resultadoBusqueda == RES_RECORD_DOESNT_EXIST)
	{
		return RES_RECORD_DOESNT_EXIST;
	}

	return RES_ERROR;
}

int Vocabulario:: actualizar_referencia_lista_invertida
		(const Termino_t & termino,
		const ReferenciaListaInvertida_t ref_nueva)
{
	IDtermino_t id_termino;
	ReferenciaListaInvertida_t ref_vieja;

	int res_busqueda = buscar_termino (termino, id_termino, ref_vieja);
	if (res_busqueda == RES_RECORD_DOESNT_EXIST)
	{
		return RES_RECORD_DOESNT_EXIST;
	}
	else if (res_busqueda == RES_OK)
	{
		RegistroClave registro;
		ClaveX clave;
		clave.set_clave(termino);
		registro.set_clave(clave);
		arbol.quitar(registro);

		registro.agregar_campo((char*)& id_termino,sizeof(id_termino));
		registro.agregar_campo((char*)& ref_nueva,sizeof(ref_nueva));

		return arbol.agregar(registro);
	}

	return RES_ERROR;
}

int Vocabulario::eliminar_termino (const Termino_t & termino)
{
	ClaveX clave;
	clave.set_clave(termino);
	RegistroClave registro;
	registro.set_clave(clave);

	int res = arbol.quitar(registro);
	return res;
}
