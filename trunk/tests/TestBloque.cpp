/*
 * TestBloque.cpp
 *
 *  Created on: May 6, 2013
 *      Author: maine
 */

#include "TestBloque.h"

TestBloque::TestBloque()
	: Test()
{
}

TestBloque::~TestBloque()
{
}

void TestBloque::ejecutar()
{
	test_agregar_registros();
	test_remover_registros();
	test_recuperar_registros();
	test_empaquetar_desempaquetar();
	test_eliminar_bloque();
}

void TestBloque::test_agregar_registros(){

	Bloque b1(2500);
	assert(b1.get_cantidad_registros_almacenados()== 0);
	assert( b1.esta_vacio() );
	assert( b1._calcular_espacio_usable()== b1.get_espacio_libre() );

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

	assert( b2._calcular_espacio_ocupado()== (rv2.get_tamanio()+sizeof(unsigned short)) );
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


void TestBloque::test_eliminar_bloque(){
	Bloque b;
	const unsigned short CANT_DATOS= 4;
	string datos[]= {"aa","bbbb","cccccc","dddddddd"};
	RegistroVariable registros[CANT_DATOS];
	assert( !b.fue_eliminado() );
	assert( b.obtener_ref_prox_bloque()== RES_BLOQUE_NO_BORRADO );
	assert( b.esta_vacio() );

	for(int i=0;i<CANT_DATOS;i++){
		registros[i].agregar_campo(datos[i].c_str() , datos[i].length());
		b.agregar_registro( &registros[i] );
	}
	assert( !b.fue_eliminado() );
	assert( b.obtener_ref_prox_bloque()== RES_BLOQUE_NO_BORRADO );
	assert( !b.esta_vacio() );

	int proximoBloque= 99;
	assert( b.actualizar_ref_prox_bloque(proximoBloque)== RES_OK );
	assert( b.obtener_ref_prox_bloque() == proximoBloque );

	proximoBloque= 55;
	assert( b.actualizar_ref_prox_bloque(proximoBloque)== RES_ERROR );
	assert( b.obtener_ref_prox_bloque()== proximoBloque );

	b.agregar_registro( &registros[0] );
	assert( !b.fue_eliminado() );


	print_test_ok("test_eliminar_bloque");
}


void TestBloque::test_remover_registros(){

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
		assert( b1.eliminar_registro(0)== RES_OK );
		assert( b1.get_cantidad_registros_almacenados()== i );
	}
	assert( b1.esta_vacio() );
	assert( b1.get_espacio_libre() == elb1 );
	assert( b1._calcular_espacio_ocupado()== 0 );
	/*bloque puede llenarse y vaciarse efectivamente*/
	assert( b1.eliminar_registro(3000) == RES_OK );
	/*remover un registro de un bloque vacio devuelve ok*/


	for(int i= 0;i<CANTIDAD_DATOS;i++){
		RegistroVariable rv;
		rv.agregar_campo( datos[i].c_str() , datos[i].length() );
		b1.agregar_registro(&rv);
	}
	for(int i=CANTIDAD_DATOS; i<2*CANTIDAD_DATOS; i++)
		assert( b1.eliminar_registro(i)== RES_ERROR );
	assert( b1.get_cantidad_registros_almacenados()== CANTIDAD_DATOS );
	/*si se busca remover un registro de una posicion mas alla de la ultima
	 * el metodo retorna error y ningun registro es removido del bloque*/

	print_test_ok("test_remover_registros_bloque");


}/*FUNCIONA! . REGISTROS SE REMUEVEN DEL BLOQUE EFECTIVAMENTE . */


void TestBloque::test_recuperar_registros(){

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

	assert( b1.eliminar_registro(0)==RES_OK );
	/*remuevo martin*/
	RegistroVariable rv;
	assert( b1.recuperar_registro(&rv , 0)!=RES_ERROR );
	char* campo= new char[16]();
	int tamanioCampo= rv.recuperar_campo(campo,0);
	string s(campo,tamanioCampo);
	delete[] campo;
	assert( s==datos[1] );
	/*en primer registro de bloque se guarda "maine"*/

	b1.eliminar_registro(1);
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
		assert( b1.eliminar_registro(0)==RES_OK );
	assert( b1.esta_vacio() );
	assert( b1.get_espacio_libre()== elb1 );
	assert( b1._calcular_espacio_ocupado()== 0 );
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


void TestBloque::test_empaquetar_desempaquetar(){

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
	assert( b1._calcular_espacio_ocupado()== b2._calcular_espacio_ocupado() );


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

