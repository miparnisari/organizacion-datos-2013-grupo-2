/*
 * TestCodigo.cpp
 *
 *  Created on: Jun 8, 2013
 *      Author: martin
 */
#include "../src/CapaLogica/Compresor/Codigo.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestCodigo : public testing::Test {
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

TEST_F(TestCodigo,Copiar_codigos)
{

	typedef Codigo::TipoIdentificador TipoIdentificador;
	Codigo c1(4);
	Codigo c2(4);
	ASSERT_TRUE( c1==c2 && c1>=c2 && c1<=c2 );
	Codigo c3(5);
	ASSERT_TRUE( c3>c2 && c2<c3 );

	Codigo c4(c3);
	ASSERT_TRUE( c4==c3 && c4>=c3 && c4<=c3 );
	TipoIdentificador ti= c4.get_identificador();
	ASSERT_TRUE( c4==ti && c4<=ti && c4>=ti );

	ti=99;
	ASSERT_TRUE( c4<ti );
	ti=1;
	ASSERT_TRUE( c4>ti );




}
