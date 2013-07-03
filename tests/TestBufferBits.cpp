/*
 * TestBufferBits.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: martin
 */
#include "../src/CapaLogica/Compresor/BufferBits.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"
#include "../src/Constantes.h"


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

};


TEST_F(TestBufferBits,Get_bit){

	const unsigned short TAMANIO_BITSET= 8;
	BufferBits<TAMANIO_BITSET> bb;

	for(unsigned short i=0;i<TAMANIO_BITSET;i++){

		if(i%2)
			bb.agregar_bit(true);
		else
			bb.agregar_bit(false);

	}
	ASSERT_EQ(bb.to_string() , "01010101");
	ASSERT_TRUE(bb.get_indice_buffer()== TAMANIO_BITSET);

	for(unsigned short i=0;i<TAMANIO_BITSET;i++){

		bool bit;
		bb.get_bit(i,bit);
		if(i%2)
			ASSERT_TRUE(bit);
		else
			ASSERT_TRUE(!bit);

	}

	bitset<8> bs;
	bs.reset();
	bs.set(0);

	for(int i=0;i<TAMANIO_BITSET;i++){

		if(i%2)
			bb.agregar_bit(true);
		else
			bb.agregar_bit(false);
		bool bit;
		ASSERT_TRUE( bb.get_bit(i,bit)!= RES_ERROR);

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


TEST_F(TestBufferBits,To_string)
{

	const unsigned short TBS= 10;
	BufferBits<TBS> bb;

	const unsigned short CANTIDAD_DUPLAS= 3;
	for(unsigned short i=0;i<CANTIDAD_DUPLAS;i++){
		bb.agregar_bit(true);
		bb.agregar_bit(false);
	}
	string bbString= bb.to_string();
	string bbStringCompleto= bb.to_string_bitset();

	for(unsigned short i=0;i<(int)(CANTIDAD_DUPLAS*2);i+=2){
		ASSERT_TRUE(bbString.at(i)== '1');
		ASSERT_TRUE(bbString.at(i+1)== '0');

	}
}


TEST_F(TestBufferBits,Contar_bytes)
{
	const unsigned short TAMANIO_BITSET=32;
	BufferBits<TAMANIO_BITSET> bb;

	ASSERT_TRUE(bb.get_cantidad_bytes()== 0);
	const unsigned short CANTIDAD_BITS= 17;
	for (unsigned short i=0;i<CANTIDAD_BITS;i++)
		bb.agregar_bit(true);

	ASSERT_TRUE(bb.get_cantidad_bytes()== 2);
}


TEST_F(TestBufferBits,To_char){

	BufferBits<8> bb;
	bb.agregar_bit(1);
	for(unsigned short i=0;i<7;i++)
		bb.agregar_bit(0);

	ASSERT_TRUE( bb.to_char()== 128 );

	bb.flush();
	for(unsigned short i=0;i<7;i++)
		bb.agregar_bit(0);
	bb.agregar_bit(1);

	ASSERT_TRUE(bb.to_char()== 1);

	bb.flush();
	bb.agregar_bits("00001000");

	ASSERT_TRUE(bb.to_char()== 8);

}


TEST_F(TestBufferBits,Quitar_bit){

	const unsigned short TBS=32;
	BufferBits<TBS> bb;
	bb.agregar_bits("1000100100000000000000000000000");

	bb.quitar_bit(4);
	Byte c;
	bb.get_byte(0,c);
	ASSERT_EQ( (int)c , 130);

	bb.quitar_bit(0);
	bb.get_byte(0,c);
	ASSERT_EQ( (int)c , 4);

	bb.flush();
	bb.agregar_bits("00000000000111010000000000");
	bb.quitar_bit(11);
	bb.quitar_bit(11);
	bb.quitar_bit(11);
	bb.get_byte(1,c);
	ASSERT_EQ( (int)c , 8);

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

	const unsigned short  TBS= 32;
	BufferBits<TBS> bb;

	for(unsigned short i=0;i<8;i++)
		bb.agregar_bit(1);
	bb.agregar_bit(1);
	for(unsigned short i=9;i<16;i++)
		bb.agregar_bit(0);

	ASSERT_TRUE(bb.get_cantidad_bytes()== 2);

	ASSERT_EQ(bb.to_string() , "1111111110000000");

	Byte c;
	bb.get_byte(0,c);
	ASSERT_TRUE( (int)c == 255 );

	bb.get_byte(1,c);
	ASSERT_TRUE( (int)c == 128 );

	bb.agregar_bits("00000100");
	ASSERT_TRUE( bb.get_cantidad_bytes()== 3 );
	bb.get_byte(2,c);
	ASSERT_TRUE( (int)c== 4 );

	bb.agregar_bits("00000111");
	bb.get_byte(3,c);
	bb.get_byte(3,c);
	ASSERT_TRUE( (int)c== 7 );
	ASSERT_TRUE( bb.get_cantidad_bytes()==4 );
	ASSERT_TRUE( bb.agregar_bit(true)== RES_ERROR );


	bb.flush();
	bool valores[]= {true,false,false,false,false,false,false,false};//128
	bb.agregar_bits(valores,8);
	bb.get_byte(0,c);
	ASSERT_TRUE( (int)c== 128 );

}


TEST_F(TestBufferBits,Pop_byte){

	const unsigned short TBS= 32;
	BufferBits<TBS> bb;
	bb.agregar_bits("1111111100001000");
	Byte c;

	bb.pop_byte(c);
	ASSERT_EQ((int)c,255);

	bb.pop_byte(c);
	ASSERT_EQ((int)c,8);

}


TEST_F(TestBufferBits,Test_dump){

	const unsigned short TAMANIO_BITSET= 64;
	BufferBits<TAMANIO_BITSET> bb;

	bb.agregar_bits( "01000001010000110100010001001111000" );


	ASSERT_TRUE( bb.get_indice_buffer() == 35 );
	ASSERT_TRUE( bb.get_cantidad_bytes()== 4 );

	char arreglo[4];
	bb.dump(arreglo);

	char resultado[4] = {'A','C','D'};


	for(unsigned short i=0;i<3;i++)
	{
		ASSERT_EQ( arreglo[i],resultado[i] );
	}

}


TEST_F(TestBufferBits,Test_dump_y_completar){

	const unsigned short TBS= 64;
	BufferBits<TBS> bb;

	bb.agregar_bits( "01000001010000110100010001001111010001" );
	char* arreglo= new char[ bb.get_cantidad_bytes()+1 ];

	ASSERT_TRUE( bb.get_indice_buffer() == 38 );

	char resultado [] = {'A','C','D', 'O'};

	bb.dump_y_completar(arreglo);

	for(unsigned short i=0;i<4;i++)
	{
		ASSERT_EQ(arreglo[i], resultado[i]);
	}

	delete[] arreglo;

}


TEST_F(TestBufferBits , Agregar_bits_char){

	BufferBits<32> bb;

	char c1= (char)2;
	char c2 = (char)1;
//	char c3 = (char)7;

	bb.agregar_bits(c1);
	bb.agregar_bits(c2);
	ASSERT_TRUE(bb.to_string() == "0000001000000001");

	//si lo lleno
	bb.agregar_bits(c2);
	bb.agregar_bits(c2);
	ASSERT_TRUE(bb.agregar_bits(c2)==RES_ERROR);
	ASSERT_TRUE(bb.to_string() == "00000010000000010000000100000001");

}

TEST_F(TestBufferBits , Agregar_bits_char_con_bits_iniciales){

	BufferBits<32> bb;

	bool bits[2];
	bits[0]= true;
	bits[1]= false;
	bb.agregar_bits(bits,2);

	char c1= (char)2;
	bb.agregar_bits(c1);

	ASSERT_TRUE(bb.to_string() == "1000000010");

	c1= (char)7;
	bb.agregar_bits(c1);
	ASSERT_TRUE(bb.to_string() == "100000001000000111");
}


TEST_F(TestBufferBits , To_long){

	BufferBits<128> bb;
	const unsigned char CANTIDAD_BYTES= 4;

	for(char c=1;c<CANTIDAD_BYTES;c++){
		bb.agregar_bits( (char)0 );
	}

	bb.agregar_bits( (char)1 );

	unsigned long valorLong;
	bb.get_long(0,valorLong);
	ASSERT_TRUE( valorLong == 1 );

	bb.agregar_bits( (char)0 );
	bb.agregar_bits( (char)0 );
	bb.agregar_bits( (char)1 );
	bb.agregar_bits( (char)1 );
	bb.get_long(1,valorLong);
	ASSERT_TRUE( valorLong == 257 );

	bb.agregar_bits( (char)3 );
	bb.agregar_bits( (char)0 );
	bb.agregar_bits( (char)0 );
	bb.agregar_bits( (char)0 );
	bb.get_long(2,valorLong);
	ASSERT_TRUE( valorLong == 50331648 );

}


TEST_F(TestBufferBits,Get_primer_valor_numerico){


	BufferBits<BITS_LONG> bb;
	bb.agregar_bits( (Byte)255 );//11111111

	unsigned long valor= 0;
	ASSERT_EQ(bb.to_string(), "11111111");

	unsigned long resultados[] = {1,3,7,15,31,63,127,255};

	for( unsigned short i=1;i<=8;i++ )
	{
		bb.get_primer_valor_numerico(i,valor);
		ASSERT_EQ(valor,resultados[i-1]);
	}

	bb.flush();
	bb.agregar_bits( (Byte)128 );
	bb.agregar_bits( (Byte)0 );

	unsigned long resultados2[] = {128,256,512,1024,2048,4096,8192,16384,32768};

	for( unsigned short i=8;i<=16;i++ ){

		bb.get_primer_valor_numerico(i,valor);
		ASSERT_EQ(valor,resultados2[i-8]);

	}

}


TEST_F(TestBufferBits,Test_stress){

	const TamanioBitset TAMANIO_BITSET= 4096*8;
	BufferBits<TAMANIO_BITSET> bb;

	for(TamanioBitset i=0;i<TAMANIO_BITSET;i++){
		if(i%2)
			bb.agregar_bit(true);
		else
			bb.agregar_bit(false);
	}

	string s = bb.to_string();
	ASSERT_TRUE( s.length()== TAMANIO_BITSET );

	for(TamanioBitset i=0;i<TAMANIO_BITSET;i++){
		bool bit;
		bb.get_bit(i,bit);
		if(i%2)
			ASSERT_TRUE(bit);
		else
			ASSERT_TRUE(!bit);
	}

}


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
	Byte c;
	bb.get_byte(0,c);

}
