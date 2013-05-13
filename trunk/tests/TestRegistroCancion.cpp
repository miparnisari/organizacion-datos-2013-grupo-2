/*
 * TestRegistroCancion.cpp
 *
 *  Created on: May 13, 2013
 *      Author: maine
 */

#include "TestRegistroCancion.h"

TestRegistroCancion::TestRegistroCancion() {

}

TestRegistroCancion::~TestRegistroCancion() {
}

void TestRegistroCancion::ejecutar()
{
	prueba();
}

void TestRegistroCancion::prueba()
{
	RegistroCancion rc2;
	char linea2[]= "autor1;autor2;autor3-titulo-idiomainvalido\nletrasssssssss\n";
	assert( rc2.cargar(linea2,strlen(linea2)) == RES_ERROR );

	RegistroCancion rc3;
	char linea3[]= "autor1;autor2;autor3-titulo-english\nletrasssssssssssss\n";
	assert( rc3.cargar(linea3,strlen(linea3))== RES_OK );
	assert( rc3.contar_parametros()== 3);

	string parametro;
	rc3.obtener_parametro(0,parametro);
	assert(parametro == "autor1;autor2;autor3");

	rc3.obtener_parametro(1,parametro);
	assert(parametro == "titulo");

	rc3.obtener_parametro(2,parametro);
	assert(parametro == "english");

	assert(rc3.obtener_parametro(3,parametro) == false);

	char linea4[]= "arjona-Martin-spanish\nesta es una letra de prueba\ncon varias lineas\nconcatenadas a re loco\n";
	RegistroCancion rc4;
	assert( rc4.cargar( linea4 , strlen(linea4) )== RES_OK );
	assert( rc4.get_cantidad_autores()== 1 );
	assert( rc4.get_anio()== Anio::ANIO_DEFAULT );
	assert( rc4.get_idioma().compare("spanish")== 0 );
	assert( rc4.get_letra() == "esta es una letra de prueba\ncon varias lineas\nconcatenadas a re loco\n");

	char linea5[]= "arjona-Martin-spanish";
	RegistroCancion rc5;
	assert( rc5.cargar(linea5,strlen(linea5)) == RES_ERROR );

	char linea6[]= "asd-sda-asdpaoisd-aspojdad-asd\n";
	char linea7[]= "asd-dsa\n";
	RegistroCancion rc6;
	assert( rc6.cargar(linea6,strlen(linea6))== RES_ERROR );
	rc6.limpiar_buffer();
	assert(rc6.cargar(linea7,strlen(linea7))== RES_ERROR  );


	char linea8[]= "arjona-2012-la vida es bella-english\nesta es una letra\nde varias\nlineas\n";
	RegistroCancion rc8;
	assert( rc8.cargar(linea8,strlen(linea8))== RES_OK );
	assert( rc8.contar_parametros()== 4 );
	assert( rc8.get_titulo() == "la vida es bella");

	char linea9[]="the beatles;arjona-2012-la vida es bella-english\nesta es\nuna letra\nmuy larga\n";
	RegistroCancion rc9;
	assert( rc9.cargar(linea9,strlen(linea9))== RES_OK );
	assert( rc9.get_cantidad_autores()== 2 );
	assert( rc9.get_autor(0) == "the beatles");
	assert( rc9.contar_parametros()== 4);

	print_test_ok("test_registro_cancion");
}/*funcionaaa*/
