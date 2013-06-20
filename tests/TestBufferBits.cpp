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

//	for(unsigned short i=0;i<TBS;i++){
//
//		if(i%2)
//			bb.agregar_bit(true);
//		else
//			bb.agregar_bit(false);
//
//	}
//	IMPRIMIR_MY_VARIABLE(bb.to_string());
//	ASSERT_TRUE(bb.get_indice_buffer()== TBS);
//
//	for(unsigned short i=0;i<TBS;i++){
//
//		bool bit;
//		bb.get_bit(i,bit);
//		IMPRIMIR_MY_VARIABLE(bit);
//		if(i%2)
//			ASSERT_TRUE(bit);
//		else
//			ASSERT_TRUE(!bit);
//
//	}

	bitset<8> bs;
	bs.reset();
	bs.set(0);

	for(int i=0;i<TBS;i++){

		if(i%2)
			bb.agregar_bit(true);
		else
			bb.agregar_bit(false);
		IMPRIMIR_MY_VARIABLE(bb.to_string_bitset());
		bool bit;
		ASSERT_TRUE( bb.get_bit(i,bit)!= RES_ERROR);
		IMPRIMIR_MY_VARIABLE(bit);

	}

	bb.flush();
	bb.agregar_bit(1);
	bb.agregar_bit(0);
	bb.agregar_bit(0);
	bb.agregar_bit(0);
	bb.agregar_bit(0);
	bb.agregar_bit(0);
	bb.agregar_bit(0);
	bb.agregar_bit(0);

	bool bit;
	bb.get_bit(0,bit);
	ASSERT_TRUE(bit);


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


TEST_F(TestBufferBits,Quitar_bit){

	imprimir_test_buffer_bits("Quitar_bit");

	const unsigned short TBS=32;
	BufferBits<TBS> bb;
	bb.agregar_bits("1000100100000000000000000000000");

	bb.quitar_bit(4);
	unsigned char c;
	bb.get_byte(0,c);
	IMPRIMIR_MY_VARIABLE( (int)c );

	bb.quitar_bit(0);
	bb.get_byte(0,c);
	IMPRIMIR_MY_VARIABLE( (int)c );

	bb.flush();
	bb.agregar_bits("00000000000111010000000000");
	bb.quitar_bit(11);
	bb.quitar_bit(11);
	bb.quitar_bit(11);
	bb.get_byte(1,c);
	IMPRIMIR_MY_VARIABLE( (int)c);

	bb.flush();
	bb.agregar_bits("10101010");
	bb.quitar_bit(1);
	bb.quitar_bit(1);
	bb.quitar_bit(1);
	ASSERT_TRUE( bb.get_indice_buffer()<8 );

	bb.flush();



}


TEST_F(TestBufferBits,Pop_bit){

	BufferBits<8> bb;
	bb.agregar_bits( "00000001" );

	for(unsigned short i=0;i<7;i++){
		bool bit;
		bb.pop_bit(bit);
		ASSERT_TRUE(bit== false);
	}

	bool bit;
	bb.pop_bit(bit);
	ASSERT_TRUE(bit==true);

}


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


	unsigned char c;
	bb.get_byte(0,c);
	IMPRIMIR_MY_VARIABLE( (int)c );
	ASSERT_TRUE( (int)c == 255 );

	bb.get_byte(1,c);
	IMPRIMIR_MY_VARIABLE( (int)c );
	ASSERT_TRUE( (int)c == 128 );

	bb.agregar_bits("00000100");
	ASSERT_TRUE( bb.get_cantidad_bytes()== 3 );
	bb.get_byte(2,c);
	IMPRIMIR_MY_VARIABLE( (int)c );
	ASSERT_TRUE( (int)c== 4 );

	bb.agregar_bits("00000111");
	bb.get_byte(3,c);
	IMPRIMIR_MY_VARIABLE( (int)c );
	bb.get_byte(3,c);
	ASSERT_TRUE( (int)c== 7 );
	ASSERT_TRUE( bb.get_cantidad_bytes()==4 );
	ASSERT_TRUE( bb.agregar_bit(true)== RES_ERROR );


	bb.flush();
	bool valores[]= {true,false,false,false,false,false,false,false};//128
	bb.agregar_bits(valores,8);
	bb.get_byte(0,c);
	IMPRIMIR_MY_VARIABLE( (int)c );


}//


TEST_F(TestBufferBits,Pop_byte){

	imprimir_test_buffer_bits("Pop_byte");

	const unsigned short TBS= 32;
	BufferBits<TBS> bb;
	bb.agregar_bits("1111111100001000");
	unsigned char c;

	bb.pop_byte(c);
	IMPRIMIR_MY_VARIABLE((int)c);


	bb.pop_byte(c);
	IMPRIMIR_MY_VARIABLE((int)c);


}


TEST_F(TestBufferBits,Test_dump){

	imprimir_test_buffer_bits("Test_dump");
	const unsigned short TBS= 64;
	BufferBits<TBS> bb;

	bb.agregar_bits( "01000001010000110100010001001111000" );
	char arreglo[4];

	IMPRIMIR_MY_VARIABLE( bb.get_indice_buffer() );
	ASSERT_TRUE( bb.get_cantidad_bytes()== 4 );

	int cbe= bb.dump(arreglo);
	IMPRIMIR_MY_VARIABLE(cbe);
	for(unsigned short i=0;i<3;i++)
		IMPRIMIR_MY_VARIABLE( arreglo[i] );

}


TEST_F(TestBufferBits,Test_dump_y_completar){

	imprimir_test_buffer_bits("Test_dump_y_completar");
	const unsigned short TBS= 64;
	BufferBits<TBS> bb;

	bb.agregar_bits( "01000001010000110100010001001111010001" );
	char* arreglo= new char[ bb.get_cantidad_bytes()+1 ];

	IMPRIMIR_MY_VARIABLE( bb.get_indice_buffer() );

	int cbe= bb.dump_y_completar(arreglo);
	IMPRIMIR_MY_VARIABLE(cbe);
	for(unsigned short i=0;i<4;i++)
		IMPRIMIR_MY_VARIABLE( arreglo[i] );

	delete[] arreglo;

}


TEST_F(TestBufferBits , Agregar_bits_char){

	BufferBits<32> bb;

	char c1= (char)2;
	char c2 = (char)1;

	bb.agregar_bits(c1);
	bb.agregar_bits(c2);
	IMPRIMIR_MY_VARIABLE( bb.to_string() );

	//si lo lleno
	bb.agregar_bits(c2);
	bb.agregar_bits(c2);
	ASSERT_TRUE(bb.agregar_bits(c2)==RES_ERROR);

}

TEST_F(TestBufferBits , Agregar_bits_char_con_bits_iniciales){

	BufferBits<32> bb;

	bool bits[2];
	bb.agregar_bits(bits,2);

	char c1= (char)2;
	bb.agregar_bits(c1);

	IMPRIMIR_MY_VARIABLE( bb.to_string() );
}



//TEST_F(TestBufferBits,Test_stress){
//
//	imprimir_test_buffer_bits("Test_stress");
//
//	const TamanioBitset TBS= 4096*8;
//	BufferBits<TBS> bb;
//
//	for(TamanioBitset i=0;i<TBS;i++){
//		if(i%2)
//			bb.agregar_bit(true);
//		else
//			bb.agregar_bit(false);
//	}
//
//	string s= bb.to_string();
//	IMPRIMIR_MY_VARIABLE(s);
//	ASSERT_TRUE( s.length()== TBS );
//
//	for(TamanioBitset i=0;i<TBS;i++){
//		bool bit;
//		bb.get_bit(i,bit);
//		if(i%2)
//			ASSERT_TRUE(bit);
//		else
//			ASSERT_TRUE(!bit);
//	}
//
//}


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
	unsigned char c;
	bb.get_byte(0,c);



}
