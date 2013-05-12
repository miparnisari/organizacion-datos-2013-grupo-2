#include "TestNodoInterno.h"
#include "TestNodoSecuencial.h"
#include "TestHashingExtensible.h"
#include "TestManejadorBloques.h"
#include "TestManejadorRegistrosVariables.h"
#include "TestBloque.h"
#include "TestRegistroVariable.h"
#include "TestHeap.h"
#include "TestNodoSecuencial.h"

#include <cassert>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <climits>

#define IMPRIMIR_VARIABLE(var)\
	std::cout<<#var<<" = "<<var<<'\n';

using namespace std;
using namespace utilitarios;

void print_test_ok(std::string nombreTest)
{
	std::cout << "OK: " << nombreTest << std::endl;
}

void test_leer_de_archivo()
{
	ParserCanciones parser;
	assert(parser.crear("../songs") == RES_OK);
	RegistroCancion regCancion;
	std::string parametro = "xxxxx";
		
	// Para cada cancion que tengamos...
	while (! parser.finDirectorio())
	{
		if (parser.getNextCancion(regCancion) == RES_OK)
		{
			std::cout << "-----------CANCION-----------" << std::endl;
			
			// Imprimo autores
			for (unsigned short j=0; j < regCancion.get_cantidad_autores(); j++)
			{
				std::string autor = regCancion.get_autor(j);
				IMPRIMIR_VARIABLE(autor);
			}
			
			// Imprimo los atributos de la cancion (NO la letra)
			for(unsigned short i=1;i<regCancion.contar_parametros();i++)
			{
				regCancion.obtener_parametro(i,parametro);
				IMPRIMIR_VARIABLE(parametro);
			}

			// Imprimo una partecita de la letra
			std::string letra = regCancion.get_letra();
			for (int i = 0; i < 50; i++)
			{
				std::cout << letra[i];
			}
			std::cout << "..." << std::endl;
		}
	}
	print_test_ok("test_leer_de_archivo");
}

void test_registro_cancion()throw(){

	std::cout<<"----------test_registro_cancion-------------"<<std::endl;


	char linea2[]= "autor1;autor2;autor3-titulo-idiomainvalido\nlalalalaalallaallalalalalalal\n";
	IMPRIMIR_VARIABLE(linea2);
	RegistroCancion rc2;

	assert( rc2.cargar(linea2,strlen(linea2))==RES_ERROR );


	char linea3[]= "autor1;autor2;autor3-titulo-english\nlalalalaalallaallalalalalalal\n";
	RegistroCancion rc3;
	assert( rc3.cargar(linea3,strlen(linea3))== RES_OK );
	assert(rc3.contar_parametros()== 3);

	cout<<endl<<endl;

	string parametro= "xxxxx";
	for(unsigned short i=0;i<4;i++){
		rc3.obtener_parametro(i,parametro);
		IMPRIMIR_VARIABLE(parametro);
	}



	char linea4[]= "arjona-Martin-spanish\nesta es una letra de prueba\ncon varias lineas\nconcatenadas a re loco\n";
	RegistroCancion rc4;
	assert( rc4.cargar( linea4 , strlen(linea4) )== RES_OK );
	assert( rc4.get_cantidad_autores()== 1 );
	assert( rc4.get_anio()== Anio::ANIO_DEFAULT );
	assert( rc4.get_idioma().compare("spanish")== 0 );
	IMPRIMIR_VARIABLE( rc4.get_letra() );


	char linea5[]= "arjona-Martin-spanish";
	RegistroCancion rc5;
	assert( rc5.cargar(linea5,strlen(linea5))== RES_ERROR );

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
	IMPRIMIR_VARIABLE(rc8.get_titulo());


	char linea9[]="the beatles;arjona-2012-la vida es bella-english\nesta es\nuna letra\nmuy larga\n";
	RegistroCancion rc9;
	assert( rc9.cargar(linea9,strlen(linea9))== RES_OK );
	assert( rc9.get_cantidad_autores()== 2 );
	IMPRIMIR_VARIABLE(rc9.get_autor(0));
	assert(rc9.contar_parametros()== 4);



	print_test_ok("test_registro_cancion");
}/*funcionaaa*/

void test_clave_string(){
	ClaveString clave1;
	ClaveString clave2;

	clave1 = "ab";
	clave2 = "as";
	assert(clave1 < clave2);

	clave1 = "asd012";
	clave2 = "asd010";
	assert(clave1 > clave2);

	clave1 = "01";
	clave2 = "02";
	assert(clave1 < clave2);

	clave1 = "ho co es to bi";
	clave1.remover_espacios();
	clave2 = "hocoestobi";
	assert(clave1 == clave2);

	ClaveString cs7("mi nombre");
	ClaveString cs8("mia me llamo");
	assert(cs8 < cs7);

	const int TAMANIO_CADENA = 16;
	char* cadena = new char[TAMANIO_CADENA]();
	clave1.empaquetar(cadena);
	clave2.desempaquetar(cadena);
	assert(clave1== clave2);

	clave2.desempaquetar(cadena ,10 );
	assert(clave1== clave2);
	
	delete[] cadena;

	print_test_ok("test_clave_string");

}/*funcionaaa*/

bool mayor_a(Clave* a,Clave* b){
	return ( (*a)>(*b) );
}

void test_clave_numerica(){

	ClaveNumerica cn1(1),cn2(2),cn3(1);

	assert(cn1 <= cn2);
	assert(cn2 >= cn1);
	assert(cn1 <= cn3 && cn1>= cn3 && cn1== cn3);

	ClaveString cs1("hola como estas"),cs2("holaaa como estas");
	assert(cs1 >= cs2);
	assert(cs2 <= cs1 );
	assert( cs1 > cs2);
	assert(cs2 < cs1);

	ClaveString cs3;
	const int TAMANIO_BUFFER= 64;
	char* buffer= new char[TAMANIO_BUFFER]();
	cs1.empaquetar(buffer);
	cs3.desempaquetar(buffer);
	assert(cs1 <= cs3 && cs1 >= cs3 && cs1 == cs3);
	delete[] buffer;

	string s1= "mi nombre es martin";
	ClaveString cs4;
	cs4.set_dato(s1);
	buffer= new char[TAMANIO_BUFFER]();
	assert( cs4.empaquetar(buffer) == (signed)s1.length() );
	assert( strlen(buffer)== s1.length() );
	ClaveString cs5;
	int s1Largo= cs4.empaquetar(buffer);
	cs5.desempaquetar(buffer,s1Largo);
	assert(cs4 == cs5);
	delete[] buffer;

	ClaveNumerica cn4(1);
	ClaveNumerica cn5(10);
	assert( mayor_a(&cn5 , &cn4) );
	assert( !mayor_a(&cn4,&cn5) );

	print_test_ok("test_clave");

}/*funcionaaa*/

void test_clavex(){

	ClaveX cx1,cx2;
	string s= "hola mi nombre es martin";
	int n= 45;
	assert( cx1==cx2 );

	cx1.set_clave(s);
	assert( cx1!=cx2 );
	cx1.set_clave(n);
	assert( cx1!=cx2 );


	cx1.set_clave(n);
	cx2.set_clave(n);
	assert( cx1==cx2 );
	cx1.set_clave(s);
	cx2.set_clave(s);
	assert( cx1==cx2 );

	cx1.set_clave(s);
	cx2.set_clave(n);
	assert( cx2!=cx1  );
	/*el statement anterior retornara falso dado q se establecio que cx1 guarda
	 * la clave s y cx2 guarda la clave n que son diferentes en TIPO*/

	ClaveX cx3;
	char empaquetado[64];
	int tamanioEmpaquetado= cx1.empaquetar(empaquetado);
	cx3.desempaquetar(empaquetado , tamanioEmpaquetado);
	assert( cx3==cx1 );

	cx1.set_clave(n);
	tamanioEmpaquetado = cx1.empaquetar(empaquetado);
	cx3.desempaquetar(empaquetado , tamanioEmpaquetado);
	assert( cx1==cx3 );

	ClaveX cx4;
	cx4= cx1;
	assert( cx4==cx1 );
	cx1.set_clave(s);
	cx4= cx1;
	assert( cx4==cx1 );

	ClaveX cx5(cx1);
	assert( cx1==cx5 );
	cx1.set_clave(n);
	ClaveX cx6(cx1);
	assert(cx1==cx6);

	cx1.set_clave(1);
	cx2.set_clave(2);
	assert( cx2>= cx1 && cx1<= cx2 );
	cx2.set_clave(1);
	assert( cx2<= cx1 && cx2>= cx1 );

	print_test_ok("test_clavex");


}


void test_vector_clavex(){

	vector<ClaveX> vcx;

	for(int i=0;i<3;i++){
		ClaveX cx;
		cx.set_clave(i);
		vcx.push_back(cx);
	}

	for(int i=0;i<3;i++)
		vcx[i].imprimir_dato();

}

int main(int argc,char** args)
{
//	test_leer_de_archivo();
	test_clave_numerica();
	test_clave_string();
	test_registro_cancion(); //TODO poner asserts en vez de couts
	test_clavex();

	std::vector<Test*> tests;
	tests.push_back(new TestRegistroVariable);
	tests.push_back(new TestBloque);
	tests.push_back(new TestManejadorRegistrosVariables);
	tests.push_back(new TestManejadorBloques);
//	tests.push_back(new TestNodoSecuencial);
	tests.push_back(new TestNodoInterno);
//	tests.push_back(new TestHashingExtensible);

	for (unsigned int i = 0; i < tests.size(); i++)
	{
		tests.at(i)->ejecutar();
		delete(tests.at(i));
	}

/*	TestHeap test6;
	test6.ejecutar();

	TestNodoSecuencial test7;
	test7.ejecutar();*/

	//test_vector_clavex();

	cout<<"fin tests!!!"<<endl;

	return 0;
}
