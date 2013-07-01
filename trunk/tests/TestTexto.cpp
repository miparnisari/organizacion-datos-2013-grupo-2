#include "../src/CapaLogica/ManejoArchivos/Texto.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

class TestTexto : public testing::Test {

 protected:
	TextoNormalizado texto;

};

TEST_F(TestTexto,Obtener_letras_parseadas)
{
	std::string letra = "(Introducción)\nEste adiós, no maquilla un \"hasta luego\",19998bc 9001,97 hola";
	std::vector<std::string> palabras;
	palabras.push_back("introducción");
	palabras.push_back("este");
	palabras.push_back("adiós");
	palabras.push_back("no");
	palabras.push_back("maquilla");
	palabras.push_back("un");
	palabras.push_back("hasta");
	palabras.push_back("luego");
	palabras.push_back("1999");
	palabras.push_back("8bc");
	palabras.push_back("9001");
	palabras.push_back("97");
	palabras.push_back("hola");

	unsigned int cantidad = texto.parsear(letra);
	assert (cantidad == palabras.size());

	std::string palabra = "";
	unsigned int i = 0;
	while (texto.get_proxima_palabra(palabra) != RES_FIN)
	{
		assert (palabra == palabras[i]);
		i ++;
	}

	assert (i == palabras.size());
}
