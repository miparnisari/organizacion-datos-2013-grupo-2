#include "../src/CapaLogica/HashingExtensible/HashingExtensible.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestManejadorBloques : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	ManejadorBloques manejador;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	  ASSERT_TRUE(manejador.crear_archivo("manejador_bloques.dat",BLOQUE_TAM_DEFAULT) == RES_OK);
	  ASSERT_TRUE(manejador.abrir_archivo("manejador_bloques.dat","rb+") == RES_OK);
	  ASSERT_TRUE(manejador.get_cantidad_bloques() == 0);
	  //ASSERT_TRUE(manejador.get_primer_bloque_libre() == -1);
	  ASSERT_TRUE(manejador.get_tamanio_bloque() == BLOQUE_TAM_DEFAULT);

	ASSERT_TRUE(manejador.obtener_bloque(0) == NULL);
	ASSERT_TRUE(manejador.obtener_bloque(1) == NULL);
	Bloque unBloque;
	ASSERT_TRUE(manejador.sobreescribir_bloque(&unBloque,0) == RES_ERROR);
	ASSERT_TRUE(manejador.sobreescribir_bloque(&unBloque,1) == RES_ERROR);
  }

  virtual void TearDown() {
	  ASSERT_TRUE(manejador.cerrar_archivo() == RES_OK);
	  ASSERT_TRUE(manejador.eliminar_archivo("manejador_bloques.dat") == RES_OK);
	  ASSERT_TRUE(manejador.eliminar_archivo("manejador_bloques.dat") == RES_FILE_DOESNT_EXIST);
  }

};


TEST_F(TestManejadorBloques,Guardar_recuperar_int)
{
	// ARRANGE
	Bloque* bloque = manejador.crear_bloque();
	int numeroEscrito = 4097;
    RegistroClave regEscrito;
    ClaveX clave;
    clave.set_clave(1);
    regEscrito.set_clave(clave);
    ASSERT_TRUE (regEscrito.agregar_campo((char*)&numeroEscrito,sizeof(numeroEscrito)) == RES_OK);
    ASSERT_TRUE (bloque->agregar_registro(&regEscrito) == RES_OK);


    // ACT -> Escribo en el archivo
    ASSERT_TRUE (manejador.escribir_bloque(bloque) == RES_OK);

    // ASSERT_TRUE -> Leo del archivo
    RegistroClave regLeido;
    Bloque* bloqueLeido = manejador.obtener_bloque(0);
    bloqueLeido->recuperar_registro(&regLeido,0);
    int numeroLeido;
    regLeido.recuperar_campo((char*)&numeroLeido,1);

    ASSERT_TRUE(numeroLeido == numeroEscrito);

    delete bloque;
    delete bloqueLeido;
}

TEST_F(TestManejadorBloques,Escribir_bloque)
{
	// Agrego el primer bloque del archivo
	Bloque* bloque = manejador.crear_bloque();
	RegistroVariable registro;
	std::string campo  = "hola como te va?";
	registro.agregar_campo(campo.c_str(),campo.size());
	bloque->agregar_registro(&registro);
	ASSERT_TRUE(manejador.escribir_bloque(bloque) == 0);
//	ASSERT_TRUE(manejador.get_primer_bloque_libre() == -1);
	ASSERT_TRUE(manejador.get_cantidad_bloques() == 1);
	ASSERT_TRUE(manejador.obtener_bloque(1) == NULL);
	Bloque* bloqueLeido = manejador.obtener_bloque(0);
	ASSERT_TRUE(bloqueLeido != NULL);
	RegistroVariable registroLeido;
	ASSERT_TRUE(bloqueLeido->recuperar_registro(&registroLeido,0) != RES_ERROR);
	char* buffer = new char[5000]();
	ASSERT_TRUE(registroLeido.recuperar_campo(buffer,0) != RES_ERROR);
	ASSERT_TRUE(std::string(buffer) == campo);
	delete[] buffer;
	delete(bloqueLeido);

	// Sobreescribo lo que escribi antes
	Bloque bloque2;
	RegistroVariable registro2;
	campo  = "muy bien gracias. 123_-";
	registro2.agregar_campo(campo.c_str(),campo.size());
	bloque2.agregar_registro(&registro2);
	ASSERT_TRUE(manejador.sobreescribir_bloque(&bloque2,0) == RES_OK);
	bloqueLeido = manejador.obtener_bloque(0);
	ASSERT_TRUE(bloqueLeido != NULL);
	ASSERT_TRUE(bloqueLeido->recuperar_registro(&registroLeido,0) != RES_ERROR);
	buffer = new char[5000]();
	ASSERT_TRUE(registroLeido.recuperar_campo(buffer,0) != RES_ERROR);
	ASSERT_TRUE(std::string(buffer) == campo);
	delete[] buffer;
//	ASSERT_TRUE(manejador.get_primer_bloque_libre() == -1);
	ASSERT_TRUE(manejador.get_cantidad_bloques() == 1);
	delete(bloqueLeido);

	// No se puede escribir un bloque vacio
	Bloque* bloqueNulo = NULL;
	ASSERT_TRUE(manejador.escribir_bloque(bloqueNulo) == RES_ERROR);
//	ASSERT_TRUE(manejador.get_primer_bloque_libre() == -1);
	ASSERT_TRUE(manejador.get_cantidad_bloques() == 1);

	// Libero el unico bloque, por ende queda marcado como "Libre".
	Bloque bloqueVacio;
	ASSERT_TRUE(manejador.sobreescribir_bloque(&bloqueVacio,0) == RES_OK);
//	ASSERT_TRUE(manejador.get_primer_bloque_libre() == 0);
	ASSERT_TRUE(manejador.get_cantidad_bloques() == 1);
	Bloque* res = manejador.obtener_bloque(0);
	ASSERT_TRUE(res->fue_eliminado());
	ASSERT_TRUE(res->obtener_ref_prox_bloque()==-1);
	delete(res);

	// Escribo en el primer bloque (que estaba liberado)
	ASSERT_TRUE(manejador.escribir_bloque(bloque) == 0);
//	ASSERT_TRUE(manejador.get_primer_bloque_libre() == -1);
	ASSERT_TRUE(manejador.get_cantidad_bloques() == 1);

	// Agrego un bloque mas
	ASSERT_TRUE(manejador.escribir_bloque(&bloque2) == 1);
//	ASSERT_TRUE(manejador.get_primer_bloque_libre() == -1);
	ASSERT_TRUE(manejador.get_cantidad_bloques() == 2);

	//Libero el primer bloque que agregue
//	ASSERT_TRUE(manejador.sobreescribir_bloque(&bloqueVacio,0) == RES_OK);
//	ASSERT_TRUE(manejador.get_primer_bloque_libre() == 0);
//	ASSERT_TRUE(manejador.get_cantidad_bloques() == 2);
//	res = manejador.obtener_bloque(0);
//	ASSERT_TRUE(res->fue_eliminado());
//	ASSERT_TRUE(res->obtener_ref_prox_bloque()==-1);
//	delete(res);

	delete(bloque);
}

TEST_F(TestManejadorBloques,Escribir_masivo)
{
	// Agrego 1000 bloques, todos con el mismo registro
	Bloque bloque(BLOQUE_TAM_DEFAULT);
	RegistroVariable registro;
	std::string campo  = "123456789 abcdefghijklmnopqrstuvwxyz ,.-_¬|@·~½¬{[]}";
	registro.agregar_campo(campo.c_str(),campo.size());
	bloque.agregar_registro(&registro);
	for (int i = 0; i < 1000; i++)
	{
		int num_bloque_escrito = manejador.escribir_bloque(&bloque);
		ASSERT_TRUE(num_bloque_escrito == i);
	}

	// Borro dos bloques, primero el 5...
	Bloque bloqueVacio(BLOQUE_TAM_DEFAULT);
	ASSERT_TRUE(manejador.sobreescribir_bloque(&bloqueVacio,5) == RES_OK);
//	ASSERT_TRUE(manejador.get_primer_bloque_libre() == 5);
	Bloque* res = manejador.obtener_bloque(5);
	ASSERT_TRUE(res->fue_eliminado());
	ASSERT_TRUE(res->obtener_ref_prox_bloque()==-1);
	delete(res);

	// ...y luego el 50
	ASSERT_TRUE(manejador.sobreescribir_bloque(&bloqueVacio,50) == RES_OK);
//	ASSERT_TRUE(manejador.get_primer_bloque_libre() == 50);
	res = manejador.obtener_bloque(50);
	ASSERT_TRUE(res->fue_eliminado());
	ASSERT_TRUE(res->obtener_ref_prox_bloque()==5);
	delete(res);

	//Intento escribir un bloque como usado que no es el tope de la pila de libres
	ASSERT_TRUE(manejador.sobreescribir_bloque(&bloque,5) == RES_ERROR);

	//Añado un bloque nuevo (debera guardarse en la posicion 50)
	ASSERT_TRUE(manejador.escribir_bloque(&bloque) == 50);

}
