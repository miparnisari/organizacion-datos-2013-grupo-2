/*
 * TestIntervalo.cpp
 *
 *  Created on: Jun 15, 2013
 *      Author: maine
 */

#include "../src/CapaLogica/Compresor/Intervalo.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestIntervalo : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	Intervalo intervalo;
  virtual void SetUp() {
	  unsigned long long rango_inicial = 4294967296;
	  ASSERT_EQ(intervalo.get_rango(), rango_inicial );
  }

  virtual void TearDown() {
  }

};

TEST_F(TestIntervalo,Crear)
{
	BitsPiso bitspiso = intervalo.get_piso();
	BitsTecho bitstecho = intervalo.get_techo();

	std::string stringCeros, stringUnos;
	for (unsigned int i = 0; i < PRECISION; i++) {
		stringCeros.push_back('0');
		stringUnos.push_back('1');
	}

	// El piso empieza con ceros y el techo con unos
	ASSERT_EQ(bitspiso.to_string(),stringCeros);
	ASSERT_EQ(bitstecho.to_string(),stringUnos);

	ASSERT_EQ(intervalo.hay_overflow(), false);
	ASSERT_EQ(intervalo.hay_underflow(), false);
}

TEST_F(TestIntervalo, Normalizar_con_overflow)
{
	intervalo.set_piso("00000000000000000000000000000000");
	intervalo.set_techo("01111111111111111111111111111111");

	// Se produce un overflow
	Byte cOverflow,cUnderflow;
	std::vector<bool> resultado = intervalo.normalizar(	cOverflow,cUnderflow);
	ASSERT_EQ(resultado.size(),1);
	ASSERT_EQ(resultado[0],0);

	std::bitset<PRECISION> piso_despues_de_normalizar (std::string("00000000000000000000000000000000"));
	std::bitset<PRECISION> techo_despues_de_normalizar (std::string("11111111111111111111111111111111"));
	ASSERT_EQ(intervalo.get_piso(), piso_despues_de_normalizar);
	ASSERT_EQ(intervalo.get_techo(), techo_despues_de_normalizar);
}

TEST_F(TestIntervalo, Normalizar_con_overflow_y_underflow)
{
	intervalo.set_piso("10010100000000000000000000000000");
	intervalo.set_techo("10101111111111111111111111111111");

	// Se produce un overflow de "10"
	Byte cOverflow,cUnderflow;
	std::vector<bool> resultado = intervalo.normalizar(	 cOverflow,cUnderflow);
	ASSERT_EQ(resultado.size(),2);
	ASSERT_EQ(resultado[0],1);
	ASSERT_EQ(resultado[1],0);

	std::bitset<PRECISION> piso_despues_de_normalizar (std::string("00100000000000000000000000000000"));
	std::bitset<PRECISION> techo_despues_de_normalizar (std::string("11111111111111111111111111111111"));
	ASSERT_EQ(intervalo.get_piso(), piso_despues_de_normalizar);
	ASSERT_EQ(intervalo.get_techo(), techo_despues_de_normalizar);
}

TEST_F(TestIntervalo, Normalizar_con_underflow)
{
	intervalo.set_piso("01100100000000000000000000000000");
	intervalo.set_techo("10000111111111111111111111111111");

	ASSERT_EQ(intervalo.hay_underflow(),true);

	Byte cOverflow,cUnderflow;
	std::vector<bool> resultado = intervalo.normalizar(	 cOverflow,cUnderflow);
	ASSERT_EQ(resultado.size(),0);

	// Quedan iguales porque no se normalizo nada
	std::bitset<PRECISION> piso_despues_de_normalizar (std::string("00010000000000000000000000000000"));
	std::bitset<PRECISION> techo_despues_de_normalizar (std::string("10011111111111111111111111111111"));
	ASSERT_EQ(intervalo.get_piso(), piso_despues_de_normalizar);
	ASSERT_EQ(intervalo.get_techo(), techo_despues_de_normalizar);
}
