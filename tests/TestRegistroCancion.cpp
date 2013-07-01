#include "../src/CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "../src/CapaFisica/ManejadorRegistrosVariables.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestRegistroCancion : public testing::Test {
 protected:
	// Declares the variables your tests want to use.

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

  // A helper function that some test uses.

};

TEST_F(TestRegistroCancion,Parametros)
{
	RegistroCancion rc2;
	char linea2[]= "autor1;autor2;autor3-titulo-idiomainvalido\nletrasssssssss\n";
	ASSERT_TRUE( rc2.cargar(linea2,strlen(linea2)) == RES_ERROR );

	RegistroCancion rc3;
	char linea3[]= "autor1;autor2;autor3-titulo-english\nletrasssssssssssss\n";
	ASSERT_TRUE( rc3.cargar(linea3,strlen(linea3))== RES_OK );
	ASSERT_TRUE( rc3.get_cantidad_parametros()== 3);

//	string parametro;
//	rc3.obtener_parametro(0,parametro);
//	ASSERT_TRUE(parametro == "autor1;autor2;autor3");
//
//	rc3.obtener_parametro(1,parametro);
//	ASSERT_TRUE(parametro == "titulo");
//
//	rc3.obtener_parametro(2,parametro);
//	ASSERT_TRUE(parametro == "english");
//
//	ASSERT_TRUE(rc3.obtener_parametro(3,parametro) == false);

	char linea4[]= "arjona-Martin-spanish\nesta es una letra de prueba\ncon varias lineas\nconcatenadas a re loco\n";
	RegistroCancion rc4;
	ASSERT_TRUE( rc4.cargar( linea4 , strlen(linea4) )== RES_OK );
	ASSERT_TRUE( rc4.get_cantidad_autores()== 1 );
	ASSERT_TRUE( rc4.get_anio()== Anio::ANIO_DEFAULT );
	ASSERT_TRUE( rc4.get_idioma().compare("spanish")== 0 );
	ASSERT_TRUE( rc4.get_letra() == "esta es una letra de prueba\ncon varias lineas\nconcatenadas a re loco");

	char linea5[]= "arjona-Martin-spanish";
	RegistroCancion rc5;
	ASSERT_TRUE( rc5.cargar(linea5,strlen(linea5)) == RES_ERROR );

	char linea6[]= "asd-sda-asdpaoisd-aspojdad-asd\n";
	char linea7[]= "asd-dsa\n";
	RegistroCancion rc6;
	ASSERT_TRUE( rc6.cargar(linea6,strlen(linea6))== RES_ERROR );
	rc6.limpiar_buffer();
	ASSERT_TRUE(rc6.cargar(linea7,strlen(linea7))== RES_ERROR  );


	char linea8[]= "arjona-2012-la vida es bella-english\nesta es una letra\nde varias\nlineas\n";
	RegistroCancion rc8;
	ASSERT_TRUE( rc8.cargar(linea8,strlen(linea8))== RES_OK );
	ASSERT_TRUE( rc8.get_cantidad_parametros()== 4 );
	ASSERT_TRUE( rc8.get_titulo() == "la vida es bella");

	char linea9[]="the beatles;arjona-2012-la vida es bella-english\nesta es\nuna letra\nmuy larga\n";
	RegistroCancion rc9;
	ASSERT_TRUE( rc9.cargar(linea9,strlen(linea9))== RES_OK );
	ASSERT_TRUE( rc9.get_cantidad_autores()== 2 );
	ASSERT_TRUE( rc9.get_autor(0) == "the beatles");
	ASSERT_TRUE( rc9.get_cantidad_parametros()== 4);

	{
		char buffer[64];
		rc9.empaquetar(buffer);
		RegistroCancion rc10;
		rc10.desempaquetar(buffer);
		ASSERT_TRUE( rc9.get_cantidad_autores() == rc10.get_cantidad_autores() );
		ASSERT_TRUE( rc9.get_anio()== rc10.get_anio() );
		ASSERT_TRUE( rc9.get_autor(1)== rc10.get_autor(1) );
		ASSERT_TRUE( rc9.get_idioma()== rc10.get_idioma() );

	}
}

TEST_F(TestRegistroCancion,Guardar_en_archivo_registros_variables)
{
	ManejadorRegistrosVariables mrv;
	string nombreArchivo("guardar_en_archivo_registros_variables.dat");
	mrv.eliminar_archivo(nombreArchivo);
	mrv.crear_archivo(nombreArchivo);

	{
		char linea9[]="the beatles;arjona-2012-la vida es bella-english\nesta es\nuna letra\nmuy larga\n";
		RegistroCancion rc9;
		ASSERT_TRUE( rc9.cargar(linea9,strlen(linea9))== RES_OK );
		ASSERT_TRUE( rc9.get_cantidad_autores()== 2 );
		ASSERT_TRUE( rc9.get_autor(0) == "the beatles");
		ASSERT_TRUE( rc9.get_cantidad_parametros()== 4);

		mrv.agregar_registro( &rc9 );
		RegistroCancion rc10;
		mrv.get_registro( &rc10,0 );
	}

	mrv.eliminar_archivo(nombreArchivo);
}

