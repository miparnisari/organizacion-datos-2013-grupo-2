#include "../src/CapaFisica/Bloque.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestBloque : public testing::Test {
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

TEST_F(TestBloque,Agregar_registros)
{
	Bloque b1(2500);
	ASSERT_TRUE( b1.get_cantidad_registros_almacenados()== 0);
	ASSERT_TRUE( b1.esta_vacio() );
	ASSERT_TRUE( b1._calcular_espacio_usable()== b1.get_espacio_libre() );

	const int CANTIDAD_DATOS= 4;
	string datos[CANTIDAD_DATOS]= {"martin","ines","nicolas","ricardo"};
	unsigned int elb1= b1.get_espacio_libre();/*espacio libre de b1. Llevo la cuenta
	de el espacio que va quedando en b1*/

	for(int i=0;i<CANTIDAD_DATOS;i++){
		RegistroVariable rv1;
		rv1.agregar_campo( datos[i].c_str() , datos[i].length() );
		ASSERT_TRUE( b1.agregar_registro(&rv1)==RES_OK );
		ASSERT_TRUE( b1.get_cantidad_registros_almacenados()== (i+1) );
		elb1-= (rv1.get_tamanio() + sizeof(unsigned short) ) ;
		ASSERT_TRUE( b1.get_espacio_libre()== elb1 );

	}
	ASSERT_TRUE(b1.get_cantidad_registros_almacenados()== CANTIDAD_DATOS);

	Bloque b2;
	RegistroVariable rv2;
	unsigned int elb2= b2.get_espacio_libre();
	for(int i=0;i<CANTIDAD_DATOS;i++){
		rv2.agregar_campo( datos[i].c_str() , datos[i].length() );
	}

	elb2-= ( rv2.get_tamanio() + sizeof(unsigned short) );
	/*un registro de varios campos en un bloque se guaradara como :
	 * tamanioRegistro|tamanioCampo1|campo1|tamanioCampo2|... */
	ASSERT_TRUE( b2.agregar_registro(&rv2)== RES_OK );
	ASSERT_TRUE( b2.get_cantidad_registros_almacenados()== 1 );
	ASSERT_TRUE( b2.get_espacio_libre()== elb2 );
	ASSERT_TRUE( b2.esta_vacio() == false);

	ASSERT_TRUE( b2._calcular_espacio_ocupado()== (rv2.get_tamanio()+sizeof(unsigned short)) );
	/*el espacio es administrado correctamente*/

	RegistroVariable rv3;
	for(int i=0;i<CANTIDAD_DATOS;i++,rv3.agregar_campo(datos[i].c_str(),datos[i].length()));
	int tamanioRegistroExportado= rv3.get_tamanio()+sizeof(short);
	Bloque b3( tamanioRegistroExportado + sizeof(int) );
	/*b3 tiene el tamanio JUSTO para guardar rv3*/
	ASSERT_TRUE( b3.agregar_registro(&rv3)== RES_OK );

	ASSERT_TRUE( b3.agregar_registro(&rv2)==RES_INSUFFICIENT_SPACE );
	/*en b3 no hay espacio suficiente para ingresar un registro*/


	Bloque b4;
	RegistroVariable rv4;
	ASSERT_TRUE( b4.agregar_registro(&rv4)==RES_ERROR );
	/*no se puede agregar un registro sin datos*/
	rv4.agregar_campo( datos[0].c_str() , datos[0].length() );
	rv4.limpiar_campos();
	ASSERT_TRUE( b4.agregar_registro(&rv4)== RES_ERROR );
	/*no se puede agregar un registro que fue limpiado a un bloque*/


}

TEST_F(TestBloque,Eliminar_bloque)
{
	Bloque b;
	const unsigned short CANT_DATOS= 4;
	string datos[]= {"aa","bbbb","cccccc","dddddddd"};
	RegistroVariable registros[CANT_DATOS];
	ASSERT_TRUE( !b.fue_eliminado() );
	ASSERT_TRUE( b.obtener_ref_prox_bloque()== RES_BLOQUE_NO_BORRADO );
	ASSERT_TRUE( b.esta_vacio() );

	for(int i=0;i<CANT_DATOS;i++){
		registros[i].agregar_campo(datos[i].c_str() , datos[i].length());
		b.agregar_registro( &registros[i] );
	}
	ASSERT_TRUE( !b.fue_eliminado() );
	ASSERT_TRUE( b.obtener_ref_prox_bloque()== RES_BLOQUE_NO_BORRADO );
	ASSERT_TRUE( !b.esta_vacio() );

	int proximoBloque= 99;
	ASSERT_TRUE( b.actualizar_ref_prox_bloque(proximoBloque)== RES_OK );
	ASSERT_TRUE( b.obtener_ref_prox_bloque() == proximoBloque );

	proximoBloque= 55;
	ASSERT_TRUE( b.actualizar_ref_prox_bloque(proximoBloque)== RES_ERROR );
	ASSERT_TRUE( b.obtener_ref_prox_bloque()== proximoBloque );

	b.agregar_registro( &registros[0] );
	ASSERT_TRUE( !b.fue_eliminado() );

}

TEST_F(TestBloque,Borrar_registros)
{

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
		ASSERT_TRUE( b1.eliminar_registro(0)== RES_OK );
		ASSERT_TRUE( b1.get_cantidad_registros_almacenados()== i );
	}
	ASSERT_TRUE( b1.esta_vacio() );
	ASSERT_TRUE( b1.get_espacio_libre() == elb1 );
	ASSERT_TRUE( b1._calcular_espacio_ocupado()== 0 );
	/*bloque puede llenarse y vaciarse efectivamente*/
	ASSERT_TRUE( b1.eliminar_registro(3000) == RES_OK );
	/*remover un registro de un bloque vacio devuelve ok*/


	for(int i= 0;i<CANTIDAD_DATOS;i++){
		RegistroVariable rv;
		rv.agregar_campo( datos[i].c_str() , datos[i].length() );
		b1.agregar_registro(&rv);
	}
	for(int i=CANTIDAD_DATOS; i<2*CANTIDAD_DATOS; i++)
		ASSERT_TRUE( b1.eliminar_registro(i)== RES_ERROR );
	ASSERT_TRUE( b1.get_cantidad_registros_almacenados()== CANTIDAD_DATOS );
	/*si se busca remover un registro de una posicion mas alla de la ultima
	 * el metodo retorna error y ningun registro es removido del bloque*/

}

TEST_F(TestBloque,Recuperar_registros)
{
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
		ASSERT_TRUE( b1.recuperar_registro(&rv , i) );
		char* campo= new char[16]();
		int tamanioCampo= rv.recuperar_campo(campo,0);
		string s(campo,tamanioCampo);
		delete[] campo;
		ASSERT_TRUE( s == datos[i] );

	}
	/*registros se recuperan correctamente*/
	RegistroVariable rv1;
	ASSERT_TRUE( b1.recuperar_registro(&rv1,CANTIDAD_DATOS)==RES_ERROR );
	/*recuperar un registro en una posicion superior o igual a la cantidad de
	 * registros guardados en bloque retorna error.*/

	ASSERT_TRUE( b1.eliminar_registro(0)==RES_OK );
	/*remuevo martin*/
	RegistroVariable rv;
	ASSERT_TRUE( b1.recuperar_registro(&rv , 0)!=RES_ERROR );
	char* campo= new char[16]();
	int tamanioCampo= rv.recuperar_campo(campo,0);
	string s(campo,tamanioCampo);
	delete[] campo;
	ASSERT_TRUE( s==datos[1] );
	/*en primer registro de bloque se guarda "maine"*/

	b1.eliminar_registro(1);
	/*remuevo "nicolas"*/
	ASSERT_TRUE( b1.get_cantidad_registros_almacenados()== 2 );
	ASSERT_TRUE( b1.recuperar_registro(&rv,1)!=RES_ERROR );
	campo= new char[16]();
	tamanioCampo= rv.recuperar_campo(campo,0);
	string s1(campo,tamanioCampo);
	delete[] campo;
	ASSERT_TRUE( s1==datos[3] );
	/*en el registro de posicion 1 esta "ricardo"*/


	RegistroVariable rv3;
	rv3.agregar_campo( datos[0].c_str() , datos[0].length() );
	b1.agregar_registro(&rv3);
	/*vuelvo a agregar "martin", ahora estara al final*/
	ASSERT_TRUE( b1.get_cantidad_registros_almacenados()== 3 );
	ASSERT_TRUE( b1.recuperar_registro(&rv,2)!=RES_ERROR );
	campo= new char[16]();
	tamanioCampo= rv.recuperar_campo(campo,0);
	string s2(campo,tamanioCampo);
	delete[] campo;
	ASSERT_TRUE( s2==datos[0] );
	/*en la posicion 2 del bloque se almacena el registro "martin"*/


	for(int i=0;i<5;i++)
		ASSERT_TRUE( b1.eliminar_registro(0)==RES_OK );
	ASSERT_TRUE( b1.esta_vacio() );
	ASSERT_TRUE( b1.get_espacio_libre()== elb1 );
	ASSERT_TRUE( b1._calcular_espacio_ocupado()== 0 );
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
	ASSERT_TRUE( b2.recuperar_registro(&rv6 , 0) );
	campo= new char[16]();
	tamanioCampo= rv6.recuperar_campo(campo,1);
	string s3(campo,tamanioCampo);
	ASSERT_TRUE(s3 == datos[1]);
	delete[] campo;
	/*en el campo 1 del registro 0 se guarda "ines"*/

	ASSERT_TRUE( b2.recuperar_registro(&rv6 , 1) );
	campo= new char[16]();
	tamanioCampo= rv6.recuperar_campo(campo,1);
	string s4(campo,tamanioCampo);
	ASSERT_TRUE(s4 == datos[3]);
	/*en el campo 1 del registro 1 se guarda "ricardo"*/

	delete[] campo;

}

TEST_F(TestBloque,Empaquetar_desempaquetar)
{
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

	ASSERT_TRUE( b1.get_cantidad_registros_almacenados()== b2.get_cantidad_registros_almacenados() );
	ASSERT_TRUE( b1.get_espacio_libre()== b2.get_espacio_libre() );
	ASSERT_TRUE( b1._calcular_espacio_ocupado()== b2._calcular_espacio_ocupado() );


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
		ASSERT_TRUE( s1 == s2);

	}/*al desempaquetar b2 es igual a b1*/
}

