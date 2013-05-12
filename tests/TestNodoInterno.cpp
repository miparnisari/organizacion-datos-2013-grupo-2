/*
 * TestNodoInterno.cpp
 *
 *  Created on: May 4, 2013
 *      Author: maine
 */

#include "TestNodoInterno.h"

TestNodoInterno::TestNodoInterno()
	:Test()
{
}

TestNodoInterno::~TestNodoInterno()
{
}

void TestNodoInterno::ejecutar()
{
	crear_nodo_con_demasiadas_claves_falla();
	cout<<"OK: test_nodo_interno"<<endl;

}


void TestNodoInterno::crear_nodo_con_demasiadas_claves_falla()
{
	NodoInterno nodoI(0,16);
	ClaveX clave1, clave2, clave3, clave4;
	clave1.set_clave("aca");
	clave2.set_clave("alla");
	clave3.set_clave("abba");
	clave4.set_clave("clave que no entra");

	unsigned short pos= 0;
	assert(nodoI.insertar_clave(clave1,pos)== RES_OK && pos==0);
	assert(nodoI.insertar_clave(clave2,pos)== RES_OK && pos==1);
	assert(nodoI.insertar_clave(clave3,pos)== RES_OK && pos== 0);
	assert(nodoI.insertar_clave(clave4,pos)== RES_OVERFLOW && pos==3);
	assert(nodoI.get_cantidad_claves() == 4);

	assert(nodoI.remover_clave(clave1,pos)== RES_OVERFLOW);
	//al remover la clave mas pequenia, el nodo sigue en estado overflow


}


void TestNodoInterno::empaquetar_en_bloque_chico_falla()
{
	NodoInterno nodoI(0,11);
	ClaveX clave1, clave2, clave3;
	clave1.set_clave("aca");
	clave2.set_clave("alla");
	clave3.set_clave("abba");

//	nodoI.agregar_clave(&clave1);
//	nodoI.agregar_clave(&clave2);
//	nodoI.agregar_clave(&clave3);

	nodoI.set_hijo_izquierdo(clave1,1);
	nodoI.set_hijo_derecho(clave1,2);

	nodoI.set_hijo_izquierdo(clave2,3);
	nodoI.set_hijo_derecho(clave2,4);

	nodoI.set_hijo_izquierdo(clave3,5);
	nodoI.set_hijo_derecho(clave3,6);

	int tamanioOcupado = sizeof(char)*11 + sizeof(short)*4;
	assert(nodoI.get_tamanio_ocupado() == tamanioOcupado);

	int tamBloque = 5;
	assert(nodoI.get_tamanio_ocupado() > tamBloque);
	ManejadorBloques manejadorBloques;
	manejadorBloques.crear_archivo("testnodos.dat",tamBloque);
	Bloque* bloque = manejadorBloques.crear_bloque();

	assert(nodoI.empaquetar(bloque) == RES_ERROR);

	delete(bloque);
	manejadorBloques.cerrar_archivo();
}

void TestNodoInterno::empaquetar_en_bloque_da_ok()
{
	NodoInterno nodoI(0,11);
	ClaveX clave1, clave2, clave3;
	clave1.set_clave("aca");
	clave2.set_clave("alla");
	clave3.set_clave("abba");

//	nodoI.agregar_clave(&clave1);
//	nodoI.agregar_clave(&clave2);
//	nodoI.agregar_clave(&clave3);

	nodoI.set_hijo_izquierdo(clave1,1);
	nodoI.set_hijo_derecho(clave1,2);

	nodoI.set_hijo_izquierdo(clave2,3);
	nodoI.set_hijo_derecho(clave2,4);

	nodoI.set_hijo_izquierdo(clave3,5);
	nodoI.set_hijo_derecho(clave3,6);

	int tamanioOcupado = sizeof(char)*11 + sizeof(short)*4;
	assert(nodoI.get_tamanio_ocupado() == tamanioOcupado);

	int tamBloque = tamanioOcupado + 5;
	ManejadorBloques manejadorBloques;
	manejadorBloques.crear_archivo("testnodos.dat",tamBloque);
	Bloque* bloque = manejadorBloques.crear_bloque();

	// Escribo el nodo
	assert(nodoI.empaquetar(bloque) == RES_OK);
	assert(manejadorBloques.abrir_archivo("testnodos.dat","wb+") == RES_OK);
	assert(manejadorBloques.escribir_bloque(bloque) == RES_OK);

	// Leo el nodo
	Bloque* bloqueLeido = manejadorBloques.obtener_bloque(0);
	assert(bloqueLeido != NULL);

	NodoInterno nodoLeido(0,11);
	nodoLeido.desempaquetar(bloqueLeido);

	delete(bloqueLeido);
	delete(bloque);
	manejadorBloques.cerrar_archivo();
}
