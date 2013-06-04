/*
 * TestClave.cpp
 *
 *  Created on: Jun 4, 2013
 *      Author: maine
 */

#include "TestClave.h"

TestClave::TestClave() {

}

TestClave::~TestClave() {
}

void TestClave::ejecutar()
{
	test_clave_string();
	test_clave_numerica();
	test_vector_clavex();
	test_clavex();
}

void TestClave::test_clave_string(){
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

}

bool TestClave::mayor_a(Clave* a,Clave* b){
	return ( (*a)>(*b) );
}

void TestClave::test_clave_numerica(){

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

void TestClave::test_clavex(){

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


void TestClave::test_vector_clavex(){

	vector<ClaveX> vcx;

	for(int i=0;i<3;i++){
		ClaveX cx;
		cx.set_clave(i);
		vcx.push_back(cx);
	}

	for(int i=0;i<3;i++)
	{
		int unaClave;
		vcx.at(i).get_clave(unaClave);
		assert(unaClave == i);
	}

}
