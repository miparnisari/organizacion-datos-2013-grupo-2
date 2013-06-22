/*
 * TestContexto.cpp
 *
 *  Created on: Jun 21, 2013
 *      Author: nico
 */


#include "../src/CapaLogica/Compresor/Contextos.h"
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

  	Contextos unContexto;
  	string s = "A";
  	ModeloProbabilistico* unModelo = new ModeloProbabilistico(TAMANIO_ALFABETO);
  	ModeloProbabilistico* modeloAux;
  	unModelo->incrementar_frecuencia(VALOR_DEL_ESCAPE);

  	ASSERT_EQ(unContexto.incrementar_frecuencia(65, s), RES_ERROR);
  	ASSERT_EQ(unContexto.devolver_modelo(s,&modeloAux), RES_ERROR);

  	ASSERT_EQ(unContexto.agregar_modelo(s, unModelo), RES_OK);

  	ASSERT_EQ(unContexto.devolver_modelo(s,&modeloAux), RES_OK);
  	ASSERT_EQ(modeloAux->calcular_low_count(VALOR_DEL_ESCAPE), 0);
  	ASSERT_EQ(modeloAux->calcular_high_count(VALOR_DEL_ESCAPE), 1);

  	ASSERT_EQ(unContexto.incrementar_frecuencia(65, s), RES_OK);
  	ASSERT_EQ(unContexto.devolver_modelo(s,&modeloAux), RES_OK);
  	ASSERT_EQ(modeloAux->calcular_low_count(65), 0);
  	ASSERT_EQ(modeloAux->calcular_high_count(65), 0.5);

  }

TEST_F(TestContexto, AgregarDosModelosAContexto){

	Contextos unContexto;
	string s1 = "A";
	string s2 = "B";
	ModeloProbabilistico* modelo1 = new ModeloProbabilistico(TAMANIO_ALFABETO);
	ModeloProbabilistico* modelo2 = new ModeloProbabilistico(TAMANIO_ALFABETO);
	ModeloProbabilistico* modeloAux;
	modelo1->incrementar_frecuencia(VALOR_DEL_ESCAPE);
	modelo2->incrementar_frecuencia(VALOR_DEL_ESCAPE);


	ASSERT_EQ(unContexto.agregar_modelo(s1, modelo1), RES_OK);
	ASSERT_EQ(unContexto.agregar_modelo(s2, modelo2), RES_OK);

	ASSERT_EQ(unContexto.incrementar_frecuencia(65, s2), RES_OK);


	ASSERT_EQ(unContexto.devolver_modelo(s1,&modeloAux), RES_OK);
	ASSERT_EQ(modeloAux->calcular_low_count(VALOR_DEL_ESCAPE), 0);
	ASSERT_EQ(modeloAux->calcular_high_count(VALOR_DEL_ESCAPE), 1);

	ASSERT_EQ(unContexto.devolver_modelo(s2,&modeloAux), RES_OK);
	ASSERT_EQ(modeloAux->calcular_low_count(VALOR_DEL_ESCAPE), 0.5);
	ASSERT_EQ(modeloAux->calcular_high_count(VALOR_DEL_ESCAPE), 1);

	ASSERT_EQ(modeloAux->calcular_low_count(65), 0);
	ASSERT_EQ(modeloAux->calcular_high_count(65), 0.5);

}

TEST_F(TestContexto, AgregarDosModelosIguales){
	Contextos unContexto;
	string s = "A";
	ModeloProbabilistico* modelo1 = new ModeloProbabilistico(TAMANIO_ALFABETO);
	ModeloProbabilistico* modelo2 = new ModeloProbabilistico(TAMANIO_ALFABETO);
	modelo1->incrementar_frecuencia(VALOR_DEL_ESCAPE);
	modelo2->incrementar_frecuencia(VALOR_DEL_ESCAPE);

	ASSERT_EQ(unContexto.agregar_modelo(s, NULL), RES_ERROR);
	ASSERT_EQ(unContexto.agregar_modelo(s, modelo1), RES_OK);
	ASSERT_EQ(unContexto.agregar_modelo(s,modelo1), RES_ERROR);
	ASSERT_EQ(unContexto.agregar_modelo(s, modelo2), RES_ERROR);

}
