/*
 * TestManejadorBloques.cpp
 *
 *  Created on: May 5, 2013
 *      Author: maine
 */

#include "TestManejadorBloques.h"

TestManejadorBloques::TestManejadorBloques() {

}

TestManejadorBloques::~TestManejadorBloques() {
}

void TestManejadorBloques::ejecutar()
{
	test_manejador_bloques_crear();
	test_manejador_bloques_escribir_bloques();
	test_manejador_bloques_masivo();
}

void TestManejadorBloques::test_manejador_bloques_crear()
{
	ManejadorBloques manejador;

	assert (manejador.crear_archivo("manejadorbloques.dat", BLOQUE_TAM_DEFAULT) == RES_OK);

	assert (manejador.abrir_archivo("manejadorbloques.dat","rb") == RES_OK);

	assert(manejador.get_cantidad_bloques() == 0);
	assert(manejador.get_primer_bloque_libre() == -1);
	assert(manejador.get_tamanio_bloque() == BLOQUE_TAM_DEFAULT);

	assert (manejador.cerrar_archivo() == RES_OK);

	print_test_ok("test_manejador_bloques_crear");

}

void TestManejadorBloques::test_manejador_bloques_escribir_bloques()
{
	ManejadorBloques manejador;
	assert (manejador.crear_archivo("manejadorbloques.dat", BLOQUE_TAM_DEFAULT) == RES_OK);
	assert (manejador.abrir_archivo("manejadorbloques.dat","rb+") == RES_OK);

	// El archivo no posee ningun bloque
	assert(manejador.get_primer_bloque_libre() == -1);
	assert(manejador.get_cantidad_bloques() == 0);
	Bloque unBloque;
	assert(manejador.obtener_bloque(0) == NULL);
	assert(manejador.obtener_bloque(1) == NULL);
	assert(manejador.sobreescribir_bloque(&unBloque,0) == RES_ERROR);
	assert(manejador.sobreescribir_bloque(&unBloque,1) == RES_ERROR);

	// Agrego el primer bloque del archivo
	Bloque* bloque = manejador.crear_bloque();
	RegistroVariable registro;
	std::string campo  = "hola como te va?";
	registro.agregar_campo(campo.c_str(),campo.size());
	bloque->agregar_registro(&registro);
	assert(manejador.escribir_bloque(bloque) == 0);
	assert(manejador.get_primer_bloque_libre() == -1);
	assert(manejador.get_cantidad_bloques() == 1);
	assert(manejador.obtener_bloque(1) == NULL);
	Bloque* bloqueLeido = manejador.obtener_bloque(0);
	assert(bloqueLeido != NULL);
	RegistroVariable registroLeido;
	assert(bloqueLeido->recuperar_registro(&registroLeido,0) != RES_ERROR);
	char* buffer = new char[5000]();
	assert(registroLeido.recuperar_campo(buffer,0) != RES_ERROR);
	assert(std::string(buffer) == campo);
	delete[] buffer;
	delete(bloqueLeido);

	// Sobreescribo lo que escribi antes
	Bloque bloque2;
	RegistroVariable registro2;
	campo  = "muy bien gracias. 123_-";
	registro2.agregar_campo(campo.c_str(),campo.size());
	bloque2.agregar_registro(&registro2);
	assert(manejador.sobreescribir_bloque(&bloque2,0) == RES_OK);
	bloqueLeido = manejador.obtener_bloque(0);
	assert(bloqueLeido != NULL);
	assert(bloqueLeido->recuperar_registro(&registroLeido,0) != RES_ERROR);
	buffer = new char[5000]();
	assert(registroLeido.recuperar_campo(buffer,0) != RES_ERROR);
	assert(std::string(buffer) == campo);
	delete[] buffer;
	assert(manejador.get_primer_bloque_libre() == -1);
	assert(manejador.get_cantidad_bloques() == 1);
	delete(bloqueLeido);

	// No se puede escribir un bloque vacio
	Bloque* bloqueNulo = NULL;
	assert(manejador.escribir_bloque(bloqueNulo) == RES_ERROR);
	assert(manejador.get_primer_bloque_libre() == -1);
	assert(manejador.get_cantidad_bloques() == 1);

	// Libero el unico bloque, por ende queda marcado como "Libre".
	Bloque bloqueVacio;
	assert(manejador.sobreescribir_bloque(&bloqueVacio,0) == RES_OK);
	assert(manejador.get_primer_bloque_libre() == 0);
	assert(manejador.get_cantidad_bloques() == 1);
	Bloque* res = manejador.obtener_bloque(0);
	assert(res->fue_eliminado());
	assert(res->obtener_ref_prox_bloque()==-1);
	delete(res);

	// Escribo en el primer bloque (que estaba liberado)
	assert(manejador.escribir_bloque(bloque) == 0);
	assert(manejador.get_primer_bloque_libre() == -1);
	assert(manejador.get_cantidad_bloques() == 1);

	// Agrego un bloque mas
	assert(manejador.escribir_bloque(&bloque2) == 1);
	assert(manejador.get_primer_bloque_libre() == -1);
	assert(manejador.get_cantidad_bloques() == 2);

	//Libero el primer bloque que agregue
	assert(manejador.sobreescribir_bloque(&bloqueVacio,0) == RES_OK);
	assert(manejador.get_primer_bloque_libre() == 0);
	assert(manejador.get_cantidad_bloques() == 2);
	res = manejador.obtener_bloque(0);
	assert(res->fue_eliminado());
	assert(res->obtener_ref_prox_bloque()==-1);
	delete(res);

	delete(bloque);

	assert (manejador.cerrar_archivo() == RES_OK);

	print_test_ok("test_manejador_bloques_escribir_bloques");
}

void TestManejadorBloques::test_manejador_bloques_masivo()
{
	unsigned int tamBloque = 100;
	ManejadorBloques manejador;
	assert (manejador.crear_archivo("bloquesmasivo.dat",tamBloque) == RES_OK);
	assert (manejador.abrir_archivo("bloquesmasivo.dat","rb+") == RES_OK);

	// Agrego 1000 bloques, todos con el mismo registro
	Bloque bloque(tamBloque);
	RegistroVariable registro;
	std::string campo  = "123456789 abcdefghijklmnopqrstuvwxyz ,.-_¬|@·~½¬{[]}";
	registro.agregar_campo(campo.c_str(),campo.size());
	bloque.agregar_registro(&registro);
	for (int i = 0; i < 1000; i++)
	{
		int num_bloque_escrito = manejador.escribir_bloque(&bloque);
		assert(num_bloque_escrito == i);
	}

	// Borro dos bloques, primero el 5...
	Bloque bloqueVacio(tamBloque);
	assert(manejador.sobreescribir_bloque(&bloqueVacio,5) == RES_OK);
	assert(manejador.get_primer_bloque_libre() == 5);
	Bloque* res = manejador.obtener_bloque(5);
	assert(res->fue_eliminado());
	assert(res->obtener_ref_prox_bloque()==-1);
	delete(res);

	// ...y luego el 50
	assert(manejador.sobreescribir_bloque(&bloqueVacio,50) == RES_OK);
	assert(manejador.get_primer_bloque_libre() == 50);
	res = manejador.obtener_bloque(50);
	assert(res->fue_eliminado());
	assert(res->obtener_ref_prox_bloque()==5);
	delete(res);

	//Intento escribir un bloque como usado que no es el tope de la pila de libres
	assert(manejador.sobreescribir_bloque(&bloque,5) == RES_ERROR);

	//Añado un bloque nuevo (debera guardarse en la posicion 50)
	assert(manejador.escribir_bloque(&bloque) == 50);

	assert(manejador.cerrar_archivo() == RES_OK);

	print_test_ok("test_manejador_bloques_masivo");

}
