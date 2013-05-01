#include "../src/CapaFisica/ManejadorArchivosTexto.h"
#include "../src/CapaFisica/ManejadorRegistrosVariables.h"
#include "../src/CapaFisica/Bloque.h"
#include "../src/CapaFisica/ManejadorBloques.h"

#include "../src/CapaLogica/ManejoArchivos/RegistroCancion.h"
#include "../src/CapaLogica/Parser/ParserCanciones.h"
#include "../src/CapaLogica/ManejoArchivos/ClaveString.h"
#include "../src/CapaLogica/ManejoArchivos/ClaveNumerica.h"

#include "../src/Constantes.h"

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
	parser.create("../songs");
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

void test_create_file(){
	ofstream file("../new.txt");
	assert(file.is_open() == true);

	file.close();
	assert(file.is_open() == false);

	print_test_ok("test_create_file");

}/*un nombre de archivo puede incluir un path!*/

void test_manejador_archivos_texto()
{

	std::cout<<"------test_manejador_archivos_texto---------"<<std::endl;
	ManejadorArchivosTexto mat;
	assert(mat.agregar_linea("noexiste.txt","lalal") == RES_FILE_DOESNT_EXIST);
	
	mat.crear_archivo("nuevo.txt");
	mat.agregar_linea("nuevo.txt","hola como estas");
	mat.agregar_linea("nuevo.txt","mas lineas y mas lineas, muy muy largas");
	mat.agregar_linea("nuevo.txt","sigo agregando lineas!!");
	
	assert( mat.mostrar_archivo("nuevo.txt") == RES_OK );

	assert( mat.hallar_lineas("nuevo.txt","lineas") ); //FIXME No se que devuelve esto
	assert( mat.hallar_lineas("nuevo.txt", "muy muy") ); //FIXME No se que devuelve esto

	assert( mat.eliminar_archivo("nuevo.txt") == RES_OK );
	print_test_ok("test_manejador_archivos_texto");
}



void test_agregar_campos_registro_variable()
{
	std::string campos[3]= {"martin", "mateo", "hector"};
	int tamanioCampos = 6+5+6;
	int cantidadCampos = 3;

	RegistroVariable rv;

	for (int i = 0; i < cantidadCampos; i++)
	{
		rv.agregar_campo(campos[i].c_str(),strlen(campos[i].c_str()));
		assert(rv.get_cantidad_campos() == i+1);
	}

	assert(rv.get_cantidad_campos() == cantidadCampos);
	assert(rv.get_tamanio_campo(0) == 6);
	assert(rv.get_tamanio_campo(1) == 5);
	assert(rv.get_tamanio_campo(2) == 6);
	assert(rv.get_tamanio() == tamanioCampos + cantidadCampos * sizeof(unsigned short));

	print_test_ok("test_agregar_campos_registro_variable");
}

void test_eliminar_registro_variable()
{
	std::string unicoCampo = "unico campo";
	int tamanioCampo = unicoCampo.size();

	RegistroVariable rv;
	rv.agregar_campo(unicoCampo.c_str(),strlen(unicoCampo.c_str()));
	assert(rv.get_cantidad_campos() == 1);
	assert(rv.get_tamanio() == tamanioCampo + sizeof(unsigned short));
	assert(rv.get_tamanio_campo(0) == tamanioCampo);

	unsigned short tamanioAntesDeBorrar = rv.get_tamanio();

	assert(rv.eliminar() == RES_OK);
	assert(rv.get_cantidad_campos() == 0);
	assert(rv.get_tamanio() == tamanioAntesDeBorrar);
	assert(rv.get_tamanio_campo(0) == RES_ERROR);

	print_test_ok("test_eliminar_registro_variable");
}

void test_empaquetar_desempaquetar_registro_variable()
{
	std::string campos[]= {"martin", "mateo", "hector"};
	unsigned short int cantidadCampos = 3;
	std::string mostrar = "martin|mateo|hector|";

	RegistroVariable rv;

	for (int i = 0; i < cantidadCampos; i++)
	{
		rv.agregar_campo(campos[i].c_str(),strlen(campos[i].c_str()));
		assert(rv.get_cantidad_campos() == i+1);
	}

	assert(rv.mostrar() == mostrar);

	char buffer[64];
	rv.empaquetar(buffer);
	RegistroVariable rv2;

	assert(rv2.desempaquetar(buffer) == RES_OK);
	assert(rv2.get_tamanio() == rv.get_tamanio() );
	assert(rv2.mostrar() == mostrar);
	assert(rv.fue_eliminado()== false );

	print_test_ok("test_empaquetar_desempaquetar_registro_variable");
}


void test_recuperar_campos_registro_variable()
{
	std::string campos[]= {"martin", "mateo", "hector"};

	RegistroVariable rv3;
	rv3.agregar_campo(campos[0].c_str(),strlen(campos[0].c_str()));
	rv3.agregar_campo(campos[1].c_str(),strlen(campos[1].c_str()));
	rv3.agregar_campo(campos[2].c_str(),strlen(campos[2].c_str()));

	char* campoRecuperado;
	for (unsigned short int i = 0; i < rv3.get_cantidad_campos(); i ++)
	{
		campoRecuperado = new char[rv3.get_tamanio_campo(i) +1]();
		rv3.recuperar_campo(campoRecuperado,i);
		const char* copia = campoRecuperado;
		const char* campo = campos[i].c_str();
		assert (strcmp(copia,campo) == 0);
		delete[] campoRecuperado;
	}

	print_test_ok("test_recuperar_campos_registro_variable");
}


void test_manejador_registros_variables()
{

	ManejadorRegistrosVariables mrv;
	assert( mrv.abrir_archivo("nombreArchInexistente")== RES_ERROR );
	assert( mrv.get_cantidad_registros()== RES_ERROR );

	assert( mrv.eliminar_archivo("tmrv1.dat")== RES_FILE_DOESNT_EXIST );
	assert( mrv.crear_archivo("tmrv1.dat")== RES_OK );
	assert( mrv.get_cantidad_registros()== 0 );

	const int CANT_DATOS= 3;
	string datos[]= {"martin","mateo","hector"};
	RegistroVariable rv;
	rv.agregar_campo(datos[0].c_str() , datos[0].length());
	assert( mrv.agregar_registro(&rv)!= RES_ERROR );

	rv.limpiar_buffer();
	rv.agregar_campo(datos[1].c_str() , datos[1].length());
	mrv.agregar_registro(&rv);

	rv.limpiar_buffer();
	rv.agregar_campo(datos[2].c_str() , datos[2].length());
	mrv.agregar_registro(&rv);

	assert( mrv.get_cantidad_registros()== 3 );

	RegistroVariable rv2;
	assert( mrv.get_registro(&rv2,3)== RES_ERROR );
	assert( mrv.get_registro(&rv2,0)== sizeof(ManejadorRegistrosVariables::Header) );
	assert( rv2.get_tamanio()== datos[0].length()+sizeof(unsigned short) );
	char campo[16];
	int tamanioCampo= rv2.recuperar_campo(campo,0);
	string s2(campo,tamanioCampo);
	assert( s2== datos[0] );

	rv2.limpiar_buffer();
	unsigned short offsetRegistro2= sizeof(ManejadorRegistrosVariables::Header)+
			10+9;
	assert( mrv.get_registro(&rv2,2)==offsetRegistro2 );
	assert( rv2.get_tamanio()== datos[2].length()+sizeof(unsigned short) );
	tamanioCampo= rv2.recuperar_campo(campo,0);
	string s3(campo,tamanioCampo);
	assert( s3== datos[2] );

	RegistroVariable rv3;
	for(int i=0;i<CANT_DATOS;i++)
		rv3.agregar_campo(datos[i].c_str() , datos[i].length());
	mrv.agregar_registro(&rv3);
	rv3.limpiar_buffer();
	mrv.get_registro(&rv3,3);
	for(int i=0;i<CANT_DATOS;i++){
		char campo[16];
		int tamanioCampo= rv3.recuperar_campo(campo,i);
		string s(campo,tamanioCampo);
		assert( s==datos[i] );
	}
	/*es posible recuperar un registro de varios campos */
	assert(mrv.get_cantidad_registros()== 4);


	long tamanioArchivo= mrv.get_tamanio_archivo();
	assert( mrv.eliminar_registro(5)== RES_ERROR );
	assert( mrv.eliminar_registro(0)!= RES_ERROR );
	assert( mrv.get_tamanio_archivo()== tamanioArchivo );
	assert( mrv.get_cantidad_registros_ocupados()== 3 );
	assert( mrv.get_cantidad_registros()== 4 );
	/*al eliminar un registro disminuye la cantidad de registros ocupados y
	 * aumenta la cantidad de registros libres, la cantidad de registros totales
	 * permanece constante. El tamanio del archivo no disminuye dado que quedan los
	 * registros liberados en el archivo*/
	assert( mrv.get_registro(&rv2 , 0)== RES_ERROR );


	rv2.limpiar_buffer();
	assert( mrv.get_registro(&rv2,1)!= RES_ERROR );
	assert( rv2.get_tamanio()== datos[1].length()+sizeof(unsigned short) );
	tamanioCampo= rv2.recuperar_campo(campo,0);
	string s4(campo,tamanioCampo);
	assert( s4== datos[1] );

	rv2.limpiar_buffer();
	assert( mrv.get_registro(&rv2,2)!= RES_ERROR );
	assert( rv2.get_tamanio()== datos[2].length()+sizeof(unsigned short) );
	tamanioCampo= rv2.recuperar_campo(campo,0);
	string s5(campo,tamanioCampo);
	assert( s5== datos[2] );

	assert( mrv.eliminar_archivo("tmrv1.dat")== RES_OK);


	print_test_ok("test_manejador_registros_variables");


}/*funciona */


void test_manejador_registros_variables_recuperar_espacio_libre(){

	ManejadorRegistrosVariables mrv;
	string nombreArchivo= "tmrvrel.dat";
	mrv.eliminar_archivo(nombreArchivo);
	mrv.crear_archivo(nombreArchivo);
	assert( mrv.get_cantidad_registros_ocupados()== 0 );

	const int CANT_DATOS= 4;
	string datos[]={ "aaaa","bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb","ccccc","dddddddddddd"};
	RegistroVariable registros[CANT_DATOS];
	for(int i=0;i<CANT_DATOS;i++){
		registros[i].agregar_campo( datos[i].c_str() , datos[i].length() );
		mrv.agregar_registro(&registros[i]);
	}
	assert( mrv.get_cantidad_registros_ocupados()== CANT_DATOS );
	long tamanioInicial= mrv.get_tamanio_archivo();


	assert( mrv.eliminar_registro(1)!= RES_ERROR );
	IMPRIMIR_VARIABLE( registros[1].get_tamanio_empaquetado() );
	assert( mrv.agregar_registro(&registros[0])!= RES_ERROR );

	{
		RegistroVariable rv;
		assert( mrv.get_registro(&rv,1)== RES_ERROR );
		assert( mrv.get_registro(&rv,2)!= RES_ERROR );
		char campo[64];
		int tamanioCampo= rv.recuperar_campo(campo,0);
		string s(campo,tamanioCampo);
		assert( s==datos[0] );
	}

	{
		RegistroVariable rv;
		assert( mrv.get_registro(&rv,3)!= RES_ERROR );
		char campo[64];
		int tamanioCampo= rv.recuperar_campo(campo,0);
		string s(campo,tamanioCampo);
		assert( s==datos[2] );
	}

	{
		RegistroVariable rv;
		assert( mrv.get_registro(&rv,4)!= RES_ERROR );
		char campo[64];
		int tamanioCampo= rv.recuperar_campo(campo,0);
		string s(campo,tamanioCampo);
		assert( s==datos[3] );
	}

	assert( mrv.agregar_registro(&registros[0])!= RES_ERROR );

	{
		RegistroVariable rv;
		assert( mrv.get_registro(&rv,1)== RES_ERROR );
		assert( mrv.get_registro(&rv,2)!= RES_ERROR );
		char campo[64];
		int tamanioCampo= rv.recuperar_campo(campo,0);
		string s(campo,tamanioCampo);
		assert( s==datos[0] );
	}

	assert( tamanioInicial== mrv.get_tamanio_archivo() );


	assert( mrv.eliminar_registro( mrv.get_cantidad_registros()-1 )!=RES_ERROR );
	mrv.agregar_registro( &registros[1] );
	assert( mrv.get_tamanio_archivo() > tamanioInicial );



	ManejadorRegistrosVariables mrv2;
	string nombreArchivo2= "tmrvre2.dat";
	mrv2.eliminar_archivo(nombreArchivo2);
	mrv2.crear_archivo(nombreArchivo2);
	assert( mrv2.get_cantidad_registros_ocupados()== 0 );

	string datos2[]={ "aaaaaa","bbbbbbbbbbbbbbbb","cccccc","dddddddddddddddd" };
	RegistroVariable registros2[CANT_DATOS];
	long offsetAppend= mrv2.get_tamanio_archivo();
	long offsetsAppend[CANT_DATOS];
	for(int i=0;i<CANT_DATOS;i++){
		offsetsAppend[i]= offsetAppend;
		registros2[i].agregar_campo( datos2[i].c_str() , datos2[i].length() );
		assert( mrv2.agregar_registro(&registros2[i])== offsetAppend );
		offsetAppend+= registros2[i].get_tamanio_empaquetado();
	}
	assert( mrv2.get_cantidad_registros_ocupados()== CANT_DATOS );

	assert( mrv2.eliminar_registro(1)== offsetsAppend[1] );
	assert( mrv2.eliminar_registro(3)== offsetsAppend[3] );

	assert( mrv2.agregar_registro(&registros2[2]) > offsetsAppend[3] );
	/*verifico que al agregar un nuevo registro el mismo se agregue cerca de donde
	 * se removio el dato2[3]*/
	assert( mrv2.eliminar_registro( 1 )== RES_ERROR );

	assert( mrv2.agregar_registro(&registros2[0]) < offsetsAppend[2] );

	offsetAppend= mrv2.get_tamanio_archivo();
	assert( mrv2.agregar_registro(&registros2[0])== offsetAppend  );

	print_test_ok("test_manejador_registros_variables_recuperar_espacio_libre");


}/*funcionaaaaa
TODO mas tests que verifiquen que el espacio libre es recuperado sin afectar la
integridad del archivo*/


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


void test_agregar_registros_bloque(){

	Bloque b1(2500);
	assert(b1.get_cantidad_registros_almacenados()== 0);
	assert( b1.esta_vacio() );
	assert( b1.calcular_espacio_usable()== b1.get_espacio_libre() );

	const int CANTIDAD_DATOS= 4;
	string datos[CANTIDAD_DATOS]= {"martin","ines","nicolas","ricardo"};
	unsigned int elb1= b1.get_espacio_libre();/*espacio libre de b1. Llevo la cuenta
	de el espacio que va quedando en b1*/

	for(int i=0;i<CANTIDAD_DATOS;i++){
		RegistroVariable rv1;
		rv1.agregar_campo( datos[i].c_str() , datos[i].length() );
		assert( b1.agregar_registro(&rv1)==RES_OK );
		assert( b1.get_cantidad_registros_almacenados()== (i+1) );
		elb1-= (rv1.get_tamanio() + sizeof(unsigned short) ) ;
		assert( b1.get_espacio_libre()== elb1 );

	}
	assert(b1.get_cantidad_registros_almacenados()== CANTIDAD_DATOS);

	Bloque b2;
	RegistroVariable rv2;
	unsigned int elb2= b2.get_espacio_libre();
	for(int i=0;i<CANTIDAD_DATOS;i++){
		rv2.agregar_campo( datos[i].c_str() , datos[i].length() );
	}

	elb2-= ( rv2.get_tamanio() + sizeof(unsigned short) );
	/*un registro de varios campos en un bloque se guaradara como :
	 * tamanioRegistro|tamanioCampo1|campo1|tamanioCampo2|... */
	assert( b2.agregar_registro(&rv2)== RES_OK );
	assert( b2.get_cantidad_registros_almacenados()== 1 );
	assert( b2.get_espacio_libre()== elb2 );
	assert( b2.esta_vacio() == false);

	assert( b2.calcular_espacio_ocupado()== (rv2.get_tamanio()+sizeof(unsigned short)) );
	/*el espacio es administrado correctamente*/

	RegistroVariable rv3;
	for(int i=0;i<CANTIDAD_DATOS;i++,rv3.agregar_campo(datos[i].c_str(),datos[i].length()));
	int tamanioRegistroExportado= rv3.get_tamanio()+sizeof(short);
	Bloque b3( tamanioRegistroExportado + sizeof(int) );
	/*b3 tiene el tamanio JUSTO para guardar rv3*/
	assert( b3.agregar_registro(&rv3)== RES_OK );

	assert( b3.agregar_registro(&rv2)==RES_INSUFFICIENT_SPACE );
	/*en b3 no hay espacio suficiente para ingresar un registro*/


	Bloque b4;
	RegistroVariable rv4;
	assert( b4.agregar_registro(&rv4)==RES_ERROR );
	/*no se puede agregar un registro sin datos*/
	rv4.agregar_campo( datos[0].c_str() , datos[0].length() );
	rv4.limpiar_buffer();
	assert( b4.agregar_registro(&rv4)== RES_ERROR );
	/*no se puede agregar un registro que fue limpiado a un bloque*/

	print_test_ok("test_agregar_registros_bloque");


}/*FUNCIONA! .LA INSERSION DE REGISTROS (APPEND) Y CALCULOS DE ESPACIO LIBRE FUNCIONAN
CORRECTAMENTE . VALE PARA CUALQUIER TAMANIO DE BLOQUE. EN UN BLOQUE LLENO NO SE PUEDEN
AGREGAR REGISTROS. */


void test_remover_registros_bloque(){

	const int CANTIDAD_DATOS= 4;
	string datos[CANTIDAD_DATOS]= {"martin","ines","nicolas","ricardo"};

	Bloque b1;
	unsigned int elb1= b1.get_espacio_libre();
	for(int i= 0;i<CANTIDAD_DATOS;i++){
		RegistroVariable rv;
		rv.agregar_campo( datos[i].c_str() , datos[i].length() );
		b1.agregar_registro(&rv);
	}
	for(int i= (CANTIDAD_DATOS-1);i>=0 ;i-- ){
		assert( b1.remover_registro(0)== RES_OK );
		assert( b1.get_cantidad_registros_almacenados()== i );
	}
	assert( b1.esta_vacio() );
	assert( b1.get_espacio_libre() == elb1 );
	assert( b1.calcular_espacio_ocupado()== 0 );
	/*bloque puede llenarse y vaciarse efectivamente*/
	assert( b1.remover_registro(3000) == RES_OK );
	/*remover un registro de un bloque vacio devuelve ok*/


	for(int i= 0;i<CANTIDAD_DATOS;i++){
		RegistroVariable rv;
		rv.agregar_campo( datos[i].c_str() , datos[i].length() );
		b1.agregar_registro(&rv);
	}
	for(int i=CANTIDAD_DATOS; i<2*CANTIDAD_DATOS; i++)
		assert( b1.remover_registro(i)== RES_ERROR );
	assert( b1.get_cantidad_registros_almacenados()== CANTIDAD_DATOS );
	/*si se busca remover un registro de una posicion mas alla de la ultima
	 * el metodo retorna error y ningun registro es removido del bloque*/

	print_test_ok("test_remover_registros_bloque");


}/*FUNCIONA! . REGISTROS SE REMUEVEN DEL BLOQUE EFECTIVAMENTE . */


void test_recuperar_registros_bloque(){

	const int CANTIDAD_DATOS= 4;
	string datos[CANTIDAD_DATOS]= {"martin","ines","nicolas","ricardo"};

	Bloque b1;
	unsigned int elb1= b1.get_espacio_libre();
	for(int i=0;i<CANTIDAD_DATOS;i++){
		RegistroVariable rv;
		rv.agregar_campo( datos[i].c_str() , datos[i].length() );
		b1.agregar_registro(&rv);
	}

	for(int i=0;i<CANTIDAD_DATOS;i++){

		RegistroVariable rv;
		assert( b1.recuperar_registro(&rv , i) );
		char* campo= new char[16]();
		int tamanioCampo= rv.recuperar_campo(campo,0);
		string s(campo,tamanioCampo);
		delete[] campo;
		assert( s == datos[i] );

	}
	/*registros se recuperan correctamente*/
	RegistroVariable rv1;
	assert( b1.recuperar_registro(&rv1,CANTIDAD_DATOS)==RES_ERROR );
	/*recuperar un registro en una posicion superior o igual a la cantidad de
	 * registros guardados en bloque retorna error.*/

	assert( b1.remover_registro(0)==RES_OK );
	/*remuevo martin*/
	RegistroVariable rv;
	assert( b1.recuperar_registro(&rv , 0)!=RES_ERROR );
	char* campo= new char[16]();
	int tamanioCampo= rv.recuperar_campo(campo,0);
	string s(campo,tamanioCampo);
	delete[] campo;
	assert( s==datos[1] );
	/*en primer registro de bloque se guarda "maine"*/

	b1.remover_registro(1);
	/*remuevo "nicolas"*/
	assert( b1.get_cantidad_registros_almacenados()== 2 );
	assert( b1.recuperar_registro(&rv,1)!=RES_ERROR );
	campo= new char[16]();
	tamanioCampo= rv.recuperar_campo(campo,0);
	string s1(campo,tamanioCampo);
	delete[] campo;
	assert( s1==datos[3] );
	/*en el registro de posicion 1 esta "ricardo"*/


	RegistroVariable rv3;
	rv3.agregar_campo( datos[0].c_str() , datos[0].length() );
	b1.agregar_registro(&rv3);
	/*vuelvo a agregar "martin", ahora estara al final*/
	assert( b1.get_cantidad_registros_almacenados()== 3 );
	assert( b1.recuperar_registro(&rv,2)!=RES_ERROR );
	campo= new char[16]();
	tamanioCampo= rv.recuperar_campo(campo,0);
	string s2(campo,tamanioCampo);
	delete[] campo;
	assert( s2==datos[0] );
	/*en la posicion 2 del bloque se almacena el registro "martin"*/


	for(int i=0;i<5;i++)
		assert( b1.remover_registro(0)==RES_OK );
	assert( b1.esta_vacio() );
	assert( b1.get_espacio_libre()== elb1 );
	assert( b1.calcular_espacio_ocupado()== 0 );
	/*despues de agregar y remover varios registros, el espacio libre se recupera*/



	Bloque b2;
	RegistroVariable rv5,rv4;
	rv4.agregar_campo( datos[0].c_str() , datos[0].length() );
	rv4.agregar_campo( datos[1].c_str() , datos[1].length() );
	rv5.agregar_campo( datos[2].c_str() , datos[2].length() );
	rv5.agregar_campo( datos[3].c_str() , datos[3].length() );
	b2.agregar_registro(&rv4);
	b2.agregar_registro(&rv5);

	RegistroVariable rv6;
	assert( b2.recuperar_registro(&rv6 , 0) );
	campo= new char[16]();
	tamanioCampo= rv6.recuperar_campo(campo,1);
	string s3(campo,tamanioCampo);
	assert(s3 == datos[1]);
	delete[] campo;
	/*en el campo 1 del registro 0 se guarda "ines"*/

	assert( b2.recuperar_registro(&rv6 , 1) );
	campo= new char[16]();
	tamanioCampo= rv6.recuperar_campo(campo,1);
	string s4(campo,tamanioCampo);
	assert(s4 == datos[3]);
	/*en el campo 1 del registro 1 se guarda "ricardo"*/

	delete[] campo;
	print_test_ok("test_recuperar_registros_bloque");


}/*FUNCIONA. LOS REGISTROS SE RECUPERAN DE FORMA CORRECTA Y SUS CAMPOS PUEDEN
LEERSE CORRECTAMENTE */


void test_empaquetar_desempaquetar_bloque(){

	const int CANTIDAD_DATOS= 4;
	string datos[CANTIDAD_DATOS]= {"martin","ines","nicolas","ricardo"};

	Bloque b1;
	for(int i=0;i<CANTIDAD_DATOS;i++){
		RegistroVariable rv;
		rv.agregar_campo( datos[i].c_str() , datos[i].length() );
		b1.agregar_registro(&rv);
	}
	Bloque b2;
	char* empaquetador = new char[b1.get_tamanio_bloque()+100]();
	b1.empaquetar(empaquetador);
	b2.desempaquetar(empaquetador);
	delete[] empaquetador;
	/*el empaquetamiento y desempaquetamiento resultara a pesar que el tamanio del buffer
	 * empaquetador sea mas grande que el tamnio del bloque a empaquetar.*/

	assert( b1.get_cantidad_registros_almacenados()== b2.get_cantidad_registros_almacenados() );
	assert( b1.get_espacio_libre()== b2.get_espacio_libre() );
	assert( b1.calcular_espacio_ocupado()== b2.calcular_espacio_ocupado() );


	for(int i=0;i<CANTIDAD_DATOS;i++){

		RegistroVariable rv1,rv2;
		b1.recuperar_registro(&rv1,i);
		b2.recuperar_registro(&rv2,i);
		char* campo1= new char[16];
		char* campo2= new char[16];
		int tamanioCampo1= rv1.recuperar_campo(campo1,0);
		int tamanioCampo2= rv2.recuperar_campo(campo2,0);
		string s1(campo1,tamanioCampo1);
		string s2(campo2,tamanioCampo2);
		delete[] campo1;
		delete[] campo2;
		assert( s1 == s2);

	}/*al desempaquetar b2 es igual a b1*/


	print_test_ok("test_empaquetar_desempaquetar_bloque");
}

void test_manejador_bloques_crear()
{
	unsigned int minRegsPorBloque = 0;
	unsigned int maxRegsPorBloque = 100;
	ManejadorBloques manejador(BLOQUE_TAM_DEFAULT, minRegsPorBloque, maxRegsPorBloque);

	assert (manejador.crear_archivo("manejadorbloques.dat") == RES_OK);

	assert (manejador.abrir_archivo("manejadorbloques.dat","rb") == RES_OK);

	assert(manejador.get_cantidad_bloques() == 0);
	assert(manejador.get_primer_bloque_libre() == -1);
	assert(manejador.get_min_regs_bloque() == minRegsPorBloque);
	assert(manejador.get_max_regs_bloque() == maxRegsPorBloque);
	assert(manejador.get_tamanio_bloque() == BLOQUE_TAM_DEFAULT);

	assert (manejador.cerrar_archivo() == RES_OK);

	print_test_ok("test_crear_archivo_bloques");

}

void test_manejador_bloques_escribir_bloques()
{
	unsigned int minRegsPorBloque = 0;
	unsigned int maxRegsPorBloque = 100;
	ManejadorBloques manejador(BLOQUE_TAM_DEFAULT, minRegsPorBloque, maxRegsPorBloque);
	assert (manejador.crear_archivo("manejadorbloques.dat") == RES_OK);
	assert (manejador.abrir_archivo("manejadorbloques.dat","rb+") == RES_OK);

	Bloque bloque;
	RegistroVariable registro;
	std::string campo  = "hola como te va? bien gracias.";
	registro.agregar_campo(campo.c_str(),campo.size());
	bloque.agregar_registro(&registro);

	// Agrego el primer bloque del archivo
	assert(manejador.escribir_bloque("manejadorbloques.dat",&bloque) == RES_OK);
	assert(manejador.get_primer_bloque_libre() == -1);
	assert(manejador.get_cantidad_bloques() == 1);

	// No se puede escribir un bloque vacio
	Bloque bloqueVacio;
	assert(manejador.escribir_bloque("manejadorbloques.dat", &bloqueVacio) == RES_OK);
	assert(manejador.get_primer_bloque_libre() == -1);
	assert(manejador.get_cantidad_bloques() == 1);

	// Libero el unico bloque, por ende queda marcado como "Libre"
	assert(manejador.sobreescribir_bloque("manejadorbloques.dat",&bloqueVacio,0) == RES_OK);
	assert(manejador.get_primer_bloque_libre() == 0);
	assert(manejador.get_cantidad_bloques() == 1);

	// Escribo en el primer bloque (que estaba liberado)
	assert(manejador.escribir_bloque("manejadorbloques.dat",&bloque) == RES_OK);
	assert(manejador.get_primer_bloque_libre() == -1);
	assert(manejador.get_cantidad_bloques() == 1);

	// Agrego un bloque mas
	assert(manejador.escribir_bloque("manejadorbloques.dat",&bloque) == RES_OK);
	assert(manejador.get_primer_bloque_libre() == -1);
	assert(manejador.get_cantidad_bloques() == 2);


	assert (manejador.cerrar_archivo() == RES_OK);

	print_test_ok("test_manejador_bloques_escribir_bloques");
}

void test_manejador_bloques_sobreescribir_bloques()
{

	print_test_ok("test_agregar_bloques");
}

void test_manejador_bloques_eliminar()
{

	print_test_ok("test_manejador_bloques_eliminar");

}

int main(int argc,char** args)
{
//	test_leer_de_archivo(); TODO des-comentar
	test_eliminar_registro_variable();
	test_agregar_campos_registro_variable();
	test_empaquetar_desempaquetar_registro_variable();
	test_recuperar_campos_registro_variable();
	test_clave_string();
//	test_registro_cancion(); TODO poner asserts en vez de cout
	test_agregar_registros_bloque();
	test_remover_registros_bloque();
	test_recuperar_registros_bloque();
	test_empaquetar_desempaquetar_bloque();
	test_manejador_registros_variables();
	test_manejador_registros_variables_recuperar_espacio_libre();
	test_manejador_bloques_crear();
	test_manejador_bloques_escribir_bloques();

	return RES_OK;
}
