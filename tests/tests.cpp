#include "TestNodoInterno.h"
#include "TestRegistroCancion.h"
#include "TestNodoSecuencial.h"
#include "TestHashingExtensible.h"
#include "TestManejadorBloques.h"
#include "TestManejadorRegistrosVariables.h"
#include "TestBloque.h"
#include "TestRegistroVariable.h"
#include "TestHeap.h"
#include "TestNodoSecuencial.h"
#include "TestRegistroClave.h"
#include "TestManejadorArchivoDatosBasicos.h"
#include "TestArbol.h"
#include "TestTabla.h"
#include "TestOrdenamientoExterno.h"
#include "TestIndiceInvertidoPorFrase.h"
#include "TestIndicePorAutor.h"
#include "TestIndicePorTitulo.h"
#include "TestArchivoListas.h"

#include <cassert>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <climits>

using namespace std;
using namespace utilitarios;

void print_test_ok(std::string nombreTest)
{
	std::cout << "OK: " << nombreTest << std::endl;
}

void test_leer_de_archivo()
{
	ParserCanciones parser;
	assert(parser.crear("../songs/") == RES_OK);
	RegistroCancion regCancion;
	std::string nombreArchivo = "";

	// Para cada cancion que tengamos...
	while (parser.fin_directorio() == false)
	{
		if (parser.obtener_proxima_cancion(regCancion, nombreArchivo) == RES_OK)
		{
			// Autores
			for (unsigned short j=0; j < regCancion.get_cantidad_autores(); j++)
			{
				std::string autor = regCancion.get_autor(j);
				assert(autor.size() > 1);
			}

			// Atributos de la cancion (NO la letra)
//			for(unsigned short i=1;i<regCancion.contar_parametros();i++)
//			{
//				regCancion.obtener_parametro(i,parametro);
//				assert(parametro.size() > 1);
//			}

			// 50 caracteres de la letra
			std::string letra = regCancion.get_letra();
			std::string parteLetra = "";
			for (int i = 0; i < 50; i++)
			{
				parteLetra.push_back(letra[i]);
			}
			assert(parteLetra.size() == 50);
		}
	}
	print_test_ok("test_leer_de_archivo");
}

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
	{
		int unaClave;
		vcx.at(i).get_clave(unaClave);
		assert(unaClave == i);
	}

}


void test_input_string(){

//	string entrada;
//	cout<<"ingrese entrada:";
//	cin>>entrada;
//	ofstream archivoSalida("salida.txt");
//	archivoSalida<<entrada;
//	archivoSalida<<'\n';
//	archivoSalida.close();
//
//
//	Normalizador n;
//	n.normalizar("salida.txt","salidaNormalizada.txt");
//
//	ifstream archivoEntrada("salidaNormalizada.txt");
//	char* lineaLeida= new char[64]();
//	archivoEntrada.getline(lineaLeida,64);
//	IMPRIMIR_VARIABLE(lineaLeida);
//	IMPRIMIR_VARIABLE( strlen(lineaLeida) ); FUNCAAA



	string entrada;
	cout<<"ingrese entrada:";
	cin>>entrada;
	Normalizador n;
	string lineaNormalizada;
	lineaNormalizada= n.normalizar_input(entrada);
	IMPRIMIR_VARIABLE(lineaNormalizada);
	int espera;
	cin>>espera;
}


void test_cancion_sabina(){

	RegistroCancion rc;
	ifstream archivo("cancionSabina.txt");
	char buffer[128];
	archivo.getline(buffer,128);
	buffer[strlen(buffer)]= '\n';
	rc.cargar(buffer,128);

}



int main(int argc,char** args)
{
	test_cancion_sabina();
//	test_leer_de_archivo();
	test_clave_numerica();
	test_clave_string();
	test_clavex();
	test_vector_clavex();

	std::vector<Test*> tests;

	tests.push_back(new TestManejadorRegistrosVariables);

	tests.push_back(new TestRegistroCancion);

	tests.push_back(new TestRegistroVariable);

	tests.push_back(new TestManejadorArchivoDatosBasicos);

	tests.push_back(new TestRegistroClave);

	tests.push_back(new TestBloque);
	tests.push_back(new TestManejadorBloques);

	tests.push_back(new TestHashingExtensible);
	tests.push_back(new TestTabla);

	tests.push_back(new TestNodoInterno);
	tests.push_back(new TestNodoSecuencial);

	tests.push_back(new TestHeap);
	tests.push_back(new TestOrdenamientoExterno);

	tests.push_back(new TestArbol);

	tests.push_back(new TestArchivoListas);
	tests.push_back(new TestIndiceInvertidoPorFrase);
//	tests.push_back(new TestIndicePorAutor);
//	tests.push_back(new TestIndicePorTitulo);

	for (unsigned int i = 0; i < tests.size(); i++)
	{
		tests.at(i)->ejecutar();
		delete(tests.at(i));
	}

	cout<<"---FIN TESTS---"<<endl;

	return 0;
}
