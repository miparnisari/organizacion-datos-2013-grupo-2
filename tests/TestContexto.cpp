/*
 * TestContexto.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: nico
 */


#include "../src/CapaLogica/Compresor/Contexto.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"


using namespace std;
// To use a test fixture, derive a class from testing::Test.
class TestContexto : public testing::Test {
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


TEST_F(TestContexto, AgregarModeloAContexto){

  	Contexto unContexto;
  	string s = "A";
  	ModeloProbabilistico* unModelo;

  	ASSERT_EQ(unContexto.incrementar_frecuencia(65, s), RES_ERROR);
  	ASSERT_EQ(unContexto.devolver_modelo(s,&unModelo), RES_ERROR);

  	unContexto.agregar_modelo(s);

  	ASSERT_EQ(unContexto.devolver_modelo(s,&unModelo), RES_OK);
  	ASSERT_EQ(unModelo->calcular_low_count(VALOR_DEL_ESCAPE), 0);
  	ASSERT_EQ(unModelo->calcular_high_count(VALOR_DEL_ESCAPE), 1);

  	ASSERT_EQ(unContexto.incrementar_frecuencia(65, s), RES_OK);
  	ASSERT_EQ(unContexto.devolver_modelo(s,&unModelo), RES_OK);
  	ASSERT_EQ(unModelo->calcular_low_count(65), 0);
  	ASSERT_EQ(unModelo->calcular_high_count(65), 0.5);




  }
