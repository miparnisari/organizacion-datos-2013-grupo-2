/*
 * TestBufferBits.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: martin
 */
#include "../src/CapaLogica/Compresor/BufferBits.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"
#include "../src/Constantes.h"


void imprimir_test_buffer_bits(string s){

	cout<<">TestBufferBits: "+s<<endl;

}


// To use a test fixture, derive a class from testing::Test.
class TestBufferBits : public testing::Test {
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


TEST_F(TestBufferBits,Get_bit){

	imprimir_test_buffer_bits("Get_bit");

	const unsigned short TBS= 8;
	BufferBits<TBS> bb;

	for(unsigned short i=0;i<TBS;i++){

		if(i%2)
			bb.agregar_bit(true);
		else
			bb.agregar_bit(false);

	}
	IMPRIMIR_MY_VARIABLE(bb.to_string());
	ASSERT_TRUE(bb.get_indice_buffer()== TBS);

	for(unsigned short i=0;i<TBS;i++){

		bool bit;
		bb.get_bit(i,bit);
		if(i%2)
			ASSERT_TRUE(bit);
		else
			ASSERT_TRUE(!bit);

	}

}


TEST_F(TestBufferBits,To_string){

	imprimir_test_buffer_bits("To_string");

	const unsigned short TBS= 10;
	BufferBits<TBS> bb;


	const unsigned short CANTIDAD_DUPLAS= 3;
	for(unsigned short i=0;i<CANTIDAD_DUPLAS;i++){
		bb.agregar_bit(true);
		bb.agregar_bit(false);
	}
	string bbString= bb.to_string();
	string bbStringCompleto= bb.to_string_bitset();


	IMPRIMIR_MY_VARIABLE(bbString);
	IMPRIMIR_MY_VARIABLE(bbStringCompleto);
	for(unsigned short i=0;i<(int)(CANTIDAD_DUPLAS*2);i+=2){
		ASSERT_TRUE(bbString.at(i)== '1');
		ASSERT_TRUE(bbString.at(i+1)== '0');

	}




}


TEST_F(TestBufferBits,Contar_bytes){

	imprimir_test_buffer_bits("Contar_bytes");

	const unsigned short TBS=32;
	BufferBits<TBS> bb;

	ASSERT_TRUE(bb.get_cantidad_bytes()== 0);
	const unsigned short CANTIDAD_BITS= 17;
	for(unsigned short i=0;i<CANTIDAD_BITS;i++)
		bb.agregar_bit(true);

	ASSERT_TRUE(bb.get_cantidad_bytes()== 2);
	IMPRIMIR_MY_VARIABLE(bb.to_string());


}




TEST_F(TestBufferBits,To_char){

	imprimir_test_buffer_bits("To_char");

	BufferBits<8> bb;
	bb.agregar_bit(1);
	for(unsigned short i=0;i<7;i++)
		bb.agregar_bit(0);

	IMPRIMIR_MY_VARIABLE(bb.to_char());
	ASSERT_TRUE( bb.to_char()== 128 );

	bb.flush();
	for(unsigned short i=0;i<7;i++)
		bb.agregar_bit(0);
	bb.agregar_bit(1);
	IMPRIMIR_MY_VARIABLE(bb.to_char());
	ASSERT_TRUE(bb.to_char()== 1);

	bb.flush();
	bb.agregar_bits("00001000");
	IMPRIMIR_MY_VARIABLE(bb.to_char());
	ASSERT_TRUE(bb.to_char()== 8);



}//


TEST_F(TestBufferBits,Get_byte){

	imprimir_test_buffer_bits("Get_byte");

	const unsigned short  TBS= 32;
	BufferBits<TBS> bb;

	for(unsigned short i=0;i<8;i++)
		bb.agregar_bit(1);
	bb.agregar_bit(1);
	for(unsigned short i=9;i<16;i++)
		bb.agregar_bit(0);

	ASSERT_TRUE(bb.get_cantidad_bytes()== 2);


	IMPRIMIR_MY_VARIABLE(bb.to_string());


	unsigned char c= bb.get_byte(0);
	IMPRIMIR_MY_VARIABLE( (int)c );
	ASSERT_TRUE( (int)c == 255 );

	c= bb.get_byte(1);
	IMPRIMIR_MY_VARIABLE( (int)c );
	ASSERT_TRUE( (int)c == 128 );

	bb.agregar_bits("00000100");
	ASSERT_TRUE( bb.get_cantidad_bytes()== 3 );
	IMPRIMIR_MY_VARIABLE( (int)bb.get_byte(2) );
	ASSERT_TRUE( (int)bb.get_byte(2)== 4 );

	bb.agregar_bits("00000111");
	IMPRIMIR_MY_VARIABLE( (int)bb.get_byte(3) );
	ASSERT_TRUE( (int)bb.get_byte(3)== 7 );


	bb.flush();
	bool valores[]= {true,false,false,false,false,false,false,false};//128
	bb.agregar_bits(valores,8);
	IMPRIMIR_MY_VARIABLE( (int)bb.get_byte(0) );


}//FIXME


TEST_F(TestBufferBits,Buffer_bits)
{
	TamanioBitset const TBS= 5;
	BufferBits<TBS> bb;

	bb.reset();
	bb.set();
	bb.agregar_bit(true);
	bool valor;
	bb.get_bit(0,valor);
	string s= bb.to_string();

	bb.get_indice_buffer();
	bb.get_cantidad_bytes();
	bb.get_byte(0);



}
