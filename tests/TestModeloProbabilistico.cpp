/*
 * TestModeloProbabilistico.cpp
 *
 *  Created on: Jun 23, 2013
 *      Author: maine
 */

#include "../src/CapaLogica/Compresor/ModeloProbabilistico.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

class TestModeloProbabilistico : public testing::Test {

 protected:
	ModeloProbabilistico* modelo;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {

  }

  // TearDown() is invoked immediately after a test finishes.
  virtual void TearDown() {
	  delete modelo;
  }

};

TEST_F(TestModeloProbabilistico,CopiarModelo)
{
	ModeloProbabilistico unModelo (TAMANIO_ALFABETO);
	std::vector<unsigned short> vector;
	vector.push_back('A');
	vector.push_back('B');
	unModelo.inicializar_frecuencias_en_1(vector);

	modelo = new ModeloProbabilistico(unModelo);

	ASSERT_EQ(modelo->get_tamanio_alfabeto(),TAMANIO_ALFABETO);

	for (Uint i = 0; i < TAMANIO_ALFABETO; i++ )
	{
		if (i == 'A' || i == 'B')
		{
			ASSERT_EQ(modelo->get_frecuencia(i),1);
		}
		else
		{
			ASSERT_EQ(modelo->get_frecuencia(i),0);
		}
	}

	unModelo.incrementar_frecuencia('A');
	ASSERT_EQ(unModelo.get_frecuencia('A'),2);
	ASSERT_EQ(modelo->get_frecuencia('A'),1);
}

