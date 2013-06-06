#include "../src/CapaLogica/Parser/ParserCanciones.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestParserCanciones : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	ParserCanciones parser;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
  }

  // TearDown() is invoked immediately after a test finishes.
  virtual void TearDown() {
  }

  // A helper function that some test uses.

};

TEST_F(TestParserCanciones,General)
{
	std::string na= "../songs/";
	ASSERT_TRUE(parser.crear(na) == RES_OK);
	RegistroCancion regCancion, regCancionNormalizada;
	std::string nombreArchivo = "";

	// Para cada cancion que tengamos...
	while (parser.fin_directorio() == false)
	{
		if (parser.obtener_proxima_cancion(regCancion, regCancionNormalizada, nombreArchivo) == RES_OK)
		{
			// Autores
			for (unsigned short j=0; j < regCancion.get_cantidad_autores(); j++)
			{
				std::string autor = regCancion.get_autor(j);
				ASSERT_TRUE(autor.size() > 1);
			}

			// Atributos de la cancion (NO la letra)
//			for(unsigned short i=1;i<regCancion.contar_parametros();i++)
//			{
//				regCancion.obtener_parametro(i,parametro);
//				ASSERT_TRUE(parametro.size() > 1);
//			}

			// 50 caracteres de la letra
			std::string letra = regCancion.get_letra();
			std::string parteLetra = "";
			for (int i = 0; i < 50; i++)
			{
				parteLetra.push_back(letra[i]);
			}
			ASSERT_TRUE(parteLetra.size() == 50);
		}
	}
}
