/*
 * TestNodoInterno.cpp
 *
 *  Created on: May 4, 2013
 *      Author: maine
 */

#include "TestNodoInterno.h"
#include "../src/CapaLogica/ArbolBMas/NodoInterno.h"
#include "../src/CapaFisica/ManejadorBloques.h"
#include "../src/CapaFisica/Bloque.h"

TestNodoInterno::TestNodoInterno() {
}

void TestNodoInterno::crear_nodo_con_demasiadas_claves_falla()
{
	NodoInterno nodoI(0,11);
	ClaveX clave1, clave2, clave3, clave4;
	clave1.set_clave("aca");
	clave2.set_clave("alla");
	clave3.set_clave("abba");
	clave4.set_clave("clave que no entra");

	assert(nodoI.agregar_clave(&clave1) == RES_OK);
	assert(nodoI.agregar_clave(&clave2) == RES_OK);
	assert(nodoI.agregar_clave(&clave3) == RES_OK);
	assert(nodoI.agregar_clave(&clave4) == RES_ERROR);

	assert(nodoI.set_hijo_izquierdo(&clave1,1) == RES_OK);
	assert(nodoI.set_hijo_derecho(&clave1,2) == RES_OK);

	assert(nodoI.set_hijo_izquierdo(&clave2,3) == RES_OK);
	assert(nodoI.set_hijo_derecho(&clave2,4) == RES_OK);

	assert(nodoI.set_hijo_izquierdo(&clave3,5) == RES_OK);
	assert(nodoI.set_hijo_derecho(&clave3,6) == RES_OK);

	assert(nodoI.set_hijo_izquierdo(&clave4,5) == RES_ERROR);
	assert(nodoI.set_hijo_derecho(&clave4,6) == RES_ERROR);

	unsigned int tamanioOcupado = sizeof(char)*11 + sizeof(short)*4;
	assert(nodoI.get_tamanio_ocupado() == tamanioOcupado);
}

TestNodoInterno::~TestNodoInterno()
{
}

void TestNodoInterno::empaquetar_en_bloque_chico_falla()
{
	NodoInterno nodoI(0,11);
	ClaveX clave1, clave2, clave3;
	clave1.set_clave("aca");
	clave2.set_clave("alla");
	clave3.set_clave("abba");

	nodoI.agregar_clave(&clave1);
	nodoI.agregar_clave(&clave2);
	nodoI.agregar_clave(&clave3);

	nodoI.set_hijo_izquierdo(&clave1,1);
	nodoI.set_hijo_derecho(&clave1,2);

	nodoI.set_hijo_izquierdo(&clave2,3);
	nodoI.set_hijo_derecho(&clave2,4);

	nodoI.set_hijo_izquierdo(&clave3,5);
	nodoI.set_hijo_derecho(&clave3,6);

	unsigned int tamanioOcupado = sizeof(char)*11 + sizeof(short)*4;
	assert(nodoI.get_tamanio_ocupado() == tamanioOcupado);

	unsigned int tamBloque = 5;
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

	nodoI.agregar_clave(&clave1);
	nodoI.agregar_clave(&clave2);
	nodoI.agregar_clave(&clave3);

	nodoI.set_hijo_izquierdo(&clave1,1);
	nodoI.set_hijo_derecho(&clave1,2);

	nodoI.set_hijo_izquierdo(&clave2,3);
	nodoI.set_hijo_derecho(&clave2,4);

	nodoI.set_hijo_izquierdo(&clave3,5);
	nodoI.set_hijo_derecho(&clave3,6);

	unsigned int tamanioOcupado = sizeof(char)*11 + sizeof(short)*4;
	assert(nodoI.get_tamanio_ocupado() == tamanioOcupado);

	unsigned int tamBloque = tamanioOcupado + 5;
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

void TestNodoInterno::ejecutar()
{
	crear_nodo_con_demasiadas_claves_falla();
	empaquetar_en_bloque_chico_falla();
	empaquetar_en_bloque_da_ok();

}



