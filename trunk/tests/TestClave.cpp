#include "../src/CapaLogica/ManejoArchivos/Clave.h"
#include "../src/CapaLogica/ManejoArchivos/ClaveX.h"
#include "../src/CapaLogica/ManejoArchivos/ClaveString.h"
#include "../src/CapaLogica/ManejoArchivos/ClaveNumerica.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestClave : public testing::Test {
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
  bool mayor_a(Clave* a,Clave* b){
  	return ( (*a)>(*b) );
  }

};

TEST_F(TestClave,String)
{
	ClaveString clave1;
	ClaveString clave2;

	clave1 = "ab";
	clave2 = "as";
	ASSERT_TRUE(clave1 < clave2);

	clave1 = "asd012";
	clave2 = "asd010";
	ASSERT_TRUE(clave1 > clave2);

	clave1 = "01";
	clave2 = "02";
	ASSERT_TRUE(clave1 < clave2);

	clave1 = "ho co es to bi";
	clave1.remover_espacios();
	clave2 = "hocoestobi";
	ASSERT_TRUE(clave1 == clave2);

	ClaveString cs7("mi nombre");
	ClaveString cs8("mia me llamo");
	ASSERT_TRUE(cs8 < cs7);

	const int TAMANIO_CADENA = 16;
	char* cadena = new char[TAMANIO_CADENA]();
	clave1.empaquetar(cadena);
	clave2.desempaquetar(cadena);
	ASSERT_TRUE(clave1== clave2);

	clave2.desempaquetar(cadena ,10 );
	ASSERT_TRUE(clave1== clave2);

	delete[] cadena;
}

TEST_F(TestClave,Numerica)
{
	ClaveNumerica cn1(1),cn2(2),cn3(1);

	ASSERT_TRUE(cn1 <= cn2);
	ASSERT_TRUE(cn2 >= cn1);
	ASSERT_TRUE(cn1 <= cn3 && cn1>= cn3 && cn1== cn3);

	ClaveString cs1("hola como estas"),cs2("holaaa como estas");
	ASSERT_TRUE(cs1 >= cs2);
	ASSERT_TRUE(cs2 <= cs1 );
	ASSERT_TRUE( cs1 > cs2);
	ASSERT_TRUE(cs2 < cs1);

	ClaveString cs3;
	const int TAMANIO_BUFFER= 64;
	char* buffer= new char[TAMANIO_BUFFER]();
	cs1.empaquetar(buffer);
	cs3.desempaquetar(buffer);
	ASSERT_TRUE(cs1 <= cs3 && cs1 >= cs3 && cs1 == cs3);
	delete[] buffer;

	string s1= "mi nombre es martin";
	ClaveString cs4;
	cs4.set_dato(s1);
	buffer= new char[TAMANIO_BUFFER]();
	ASSERT_TRUE( cs4.empaquetar(buffer) == (signed)s1.length() );
	ASSERT_TRUE( strlen(buffer)== s1.length() );
	ClaveString cs5;
	int s1Largo= cs4.empaquetar(buffer);
	cs5.desempaquetar(buffer,s1Largo);
	ASSERT_TRUE(cs4 == cs5);
	delete[] buffer;

	ClaveNumerica cn4(1);
	ClaveNumerica cn5(10);
	ASSERT_TRUE( mayor_a(&cn5 , &cn4) );
	ASSERT_TRUE( !mayor_a(&cn4,&cn5) );

}

TEST_F(TestClave,ClaveX)
{
	ClaveX cx1,cx2;
	string s= "hola mi nombre es martin";
	int n= 45;
	ASSERT_TRUE( cx1==cx2 );

	cx1.set_clave(s);
	ASSERT_TRUE( cx1!=cx2 );
	cx1.set_clave(n);
	ASSERT_TRUE( cx1!=cx2 );


	cx1.set_clave(n);
	cx2.set_clave(n);
	ASSERT_TRUE( cx1==cx2 );
	cx1.set_clave(s);
	cx2.set_clave(s);
	ASSERT_TRUE( cx1==cx2 );

	cx1.set_clave(s);
	cx2.set_clave(n);
	ASSERT_TRUE( cx2!=cx1  );
	/*el statement anterior retornara falso dado q se establecio que cx1 guarda
	 * la clave s y cx2 guarda la clave n que son diferentes en TIPO*/

	ClaveX cx3;
	char empaquetado[64];
	int tamanioEmpaquetado= cx1.empaquetar(empaquetado);
	cx3.desempaquetar(empaquetado , tamanioEmpaquetado);
	ASSERT_TRUE( cx3==cx1 );

	cx1.set_clave(n);
	tamanioEmpaquetado = cx1.empaquetar(empaquetado);
	cx3.desempaquetar(empaquetado , tamanioEmpaquetado);
	ASSERT_TRUE( cx1==cx3 );

	ClaveX cx4;
	cx4= cx1;
	ASSERT_TRUE( cx4==cx1 );
	cx1.set_clave(s);
	cx4= cx1;
	ASSERT_TRUE( cx4==cx1 );

	ClaveX cx5(cx1);
	ASSERT_TRUE( cx1==cx5 );
	cx1.set_clave(n);
	ClaveX cx6(cx1);
	ASSERT_TRUE(cx1==cx6);

	cx1.set_clave(1);
	cx2.set_clave(2);
	ASSERT_TRUE( cx2>= cx1 && cx1<= cx2 );
	cx2.set_clave(1);
	ASSERT_TRUE( cx2<= cx1 && cx2>= cx1 );
}

TEST_F(TestClave,Vector_claveX)
{
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
		ASSERT_TRUE(unaClave == i);
	}

}
