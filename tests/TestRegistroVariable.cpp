#include "../src/CapaFisica/RegistroVariable.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestRegistroVariable : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	RegistroVariable rv;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

  // A helper function that some test uses.

};

TEST_F(TestRegistroVariable,Agregar_campos)
{
	std::string campos[3]= {"martin", "mateo", "hector"};
	int tamanioCampos = 6+5+6;
	int cantidadCampos = 3;

	for (int i = 0; i < cantidadCampos; i++)
	{
		rv.agregar_campo(campos[i].c_str(),strlen(campos[i].c_str()));
		assert(rv.get_cantidad_campos() == i+1);
	}

	assert(rv.get_cantidad_campos() == cantidadCampos);
	assert(rv.get_tamanio_campo(0) == 6);
	assert(rv.get_tamanio_campo(1) == 5);
	assert(rv.get_tamanio_campo(2) == 6);
	assert(rv.get_tamanio() == tamanioCampos + cantidadCampos * sizeof(unsigned short));

	{
		RegistroVariable rv1;

		const unsigned short CANTIDAD_CAMPOS= 3;
		string campoAcumulado= "d";
		for(unsigned short i=0;i<CANTIDAD_CAMPOS;i++){
			campoAcumulado+= "aa";
			rv1.agregar_campo(campoAcumulado.c_str() , campoAcumulado.length());
		}

		RegistroVariable rv2(rv1);
		assert( rv1.get_cantidad_campos()== rv2.get_cantidad_campos() &&
				rv1.get_tamanio()== rv2.get_tamanio() );
		for(unsigned short i=0;i<CANTIDAD_CAMPOS;i++){
			assert(rv1.get_tamanio_campo(i)== rv2.get_tamanio_campo(i));
		}


	}
}

TEST_F(TestRegistroVariable,Eliminar)
{
	std::string unicoCampo = "unico campo";
	int tamanioCampo = unicoCampo.size();

	rv.agregar_campo(unicoCampo.c_str(),strlen(unicoCampo.c_str()));
	assert(rv.get_cantidad_campos() == 1);
	assert(rv.get_tamanio() == tamanioCampo + sizeof(unsigned short));
	assert(rv.get_tamanio_campo(0) == tamanioCampo);

	unsigned short tamanioAntesDeBorrar = rv.get_tamanio();

	assert(rv.eliminar() == RES_OK);
	assert(rv.get_cantidad_campos() == 0);
	assert(rv.get_tamanio() == tamanioAntesDeBorrar);
	assert(rv.get_tamanio_campo(0) == RES_ERROR);
}

TEST_F(TestRegistroVariable,Empaquetar_desempaquetar)
{
	std::string campos[]= {"martin", "mateo", "hector"};
	unsigned short int cantidadCampos = 3;
	std::string mostrar = "martin|mateo|hector|";

	for (int i = 0; i < cantidadCampos; i++)
	{
		rv.agregar_campo(campos[i].c_str(),strlen(campos[i].c_str()));
		assert(rv.get_cantidad_campos() == i+1);
	}

	assert(rv.mostrar() == mostrar);

	char buffer[64];
	rv.empaquetar(buffer);
	RegistroVariable rv2;

	assert(rv2.desempaquetar(buffer) == RES_OK);
	assert(rv2.get_tamanio() == rv.get_tamanio() );
	assert(rv2.mostrar() == mostrar);
	assert(rv.fue_eliminado()== false );
}

TEST_F(TestRegistroVariable,Recuperar_campos)
{
	std::string campos[]= {"martin", "mateo", "hector"};

	rv.agregar_campo(campos[0].c_str(),strlen(campos[0].c_str()));
	rv.agregar_campo(campos[1].c_str(),strlen(campos[1].c_str()));
	rv.agregar_campo(campos[2].c_str(),strlen(campos[2].c_str()));

	char* campoRecuperado;
	for (unsigned short int i = 0; i < rv.get_cantidad_campos(); i ++)
	{
		campoRecuperado = new char[rv.get_tamanio_campo(i) +1]();
		rv.recuperar_campo(campoRecuperado,i);
		const char* copia = campoRecuperado;
		const char* campo = campos[i].c_str();
		assert (strcmp(copia,campo) == 0);
		delete[] campoRecuperado;
	}
}
