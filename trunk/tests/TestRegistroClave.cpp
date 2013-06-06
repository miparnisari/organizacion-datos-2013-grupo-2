#include "../src/CapaLogica/ManejoArchivos/RegistroClave.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestRegistroClave : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	RegistroClave registroClave;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

  // A helper function that some test uses.

};

TEST_F(TestRegistroClave,Guardar_leer)
{
	ClaveX claveescrita;
	claveescrita.set_clave("una artista");
	std::string campo = "madonna";

	registroClave.set_clave(claveescrita);
	registroClave.agregar_campo(campo.c_str(), campo.size());

	assert(registroClave.get_cantidad_campos() == 2);
	assert(registroClave.get_clave() == claveescrita);
	assert(registroClave.get_tamanio_campo(1) != RES_ERROR);
	assert((unsigned)registroClave.get_tamanio_campo(1) == campo.size());

	for (unsigned int i = 1; i < registroClave.get_cantidad_campos(); i++)
	{
		char* buffer = new char[registroClave.get_tamanio_campo(i)+1]();
		buffer[registroClave.get_tamanio_campo(i)] = '\0';
		registroClave.recuperar_campo(buffer,i);

		if (i == 0)
			assert(strcmp(buffer,"una artista") == 0);
		else
			assert(strcmp(buffer,"madonna") == 0);

		delete[] buffer;
	}

}

TEST_F(TestRegistroClave,Empaquetar_desempaquetar)
{
	ClaveX clave;
	clave.set_clave("una clave 123");
	std::string campo = "un campo 456";

	registroClave.set_clave(clave);
	registroClave.agregar_campo(campo.c_str(), campo.size());

	char* buffer = new char[registroClave.get_tamanio_empaquetado() +1]();
	buffer[registroClave.get_tamanio_empaquetado()] = '\0';

	assert(registroClave.empaquetar(buffer) == RES_OK);

	RegistroClave registroDesempaquetado;
	registroDesempaquetado.desempaquetar(buffer);

	// Recupero la clave
	assert (registroDesempaquetado.get_clave() == clave);

	// Recupero el campo
	char* campoRecuperado = new char[registroDesempaquetado.get_tamanio_campo(1) +1]();
	campoRecuperado[registroDesempaquetado.get_tamanio_campo(1)] = '\0';

	registroDesempaquetado.recuperar_campo(campoRecuperado,1);

	assert( strcmp(campoRecuperado,campo.c_str()) == 0);

	delete[] buffer;
	delete[] campoRecuperado;
}
