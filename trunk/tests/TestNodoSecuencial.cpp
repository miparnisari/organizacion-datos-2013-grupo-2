#include "../src/CapaLogica/ArbolBMas/NodoSecuencial.h"
#include "../src/CapaLogica/ArbolBMas/IterNodoSecuencial.h"
#include "../src/CapaFisica/ManejadorBloques.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestNodoSecuencial : public testing::Test {
 protected:
	// Declares the variables your tests want to use.

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

TEST_F(TestNodoSecuencial,Crear)
{
	NodoSecuencial* nodo = new NodoSecuencial(100,1000);
	assert(nodo->get_bytes_ocupados() == sizeof(int) + sizeof(char));
	assert(nodo->get_proximo_nodo() == -1);
	assert(nodo->get_cantidad_registros() == 0);
	assert(nodo->esta_vacio() == true);
	delete(nodo);
}

TEST_F(TestNodoSecuencial,Insertar_simple)
{
	NodoSecuencial* nodo = new NodoSecuencial(5,100);
	std::vector <RegistroClave> regsOverflow;
	RegistroClave registro;
	RegistroClave* registroCopia = NULL;

	ClaveX clave;
	clave.set_clave("una clave");
	std::string campo = "un campo";

	registro.set_clave(clave);
	registro.agregar_campo(campo.c_str(), campo.size());

	assert(registro.get_tamanio_empaquetado() == 24);

	assert(nodo->buscar(clave,&registroCopia) < 0);
	assert(registroCopia == NULL);

	assert(nodo->insertar(registro,regsOverflow) == RES_OK);

	assert(nodo->buscar(clave,&registroCopia) == 0);
	assert(registroCopia != NULL);

	assert(registroCopia->get_tamanio() == registro.get_tamanio());

	char* buffer = new char[registroCopia->get_tamanio_campo(1) +1]();
	registroCopia->recuperar_campo(buffer,1);
	buffer[registroCopia->get_tamanio_campo(1)] = '\0';
	assert (strcmp(buffer,campo.c_str()) == 0);

	delete[] buffer;
	delete(nodo);
	delete(registroCopia);
}


TEST_F(TestNodoSecuencial,Empaquetar_desempaquetar)
{
	// SET UP
	ManejadorBloques manejador;
	manejador.crear_archivo("bloques_nodos_secuenciales.dat", 4096);
	manejador.abrir_archivo("bloques_nodos_secuenciales.dat","wb+");

	Bloque* bloque = manejador.crear_bloque();
	assert(bloque != NULL);
	NodoSecuencial* nodo = new NodoSecuencial(100,4096);

	std::string campoEscrito = "un campo";
	RegistroClave registro;
	ClaveX claveEscrita;
	claveEscrita.set_clave("una clave");
	registro.set_clave(claveEscrita);
	registro.agregar_campo(campoEscrito.c_str(),campoEscrito.size());


	std::vector<RegistroClave> regsOverflow;

	// Inserto "una clave | un campo"
	assert(nodo->insertar(registro,regsOverflow) == RES_OK);
	assert(nodo->get_cantidad_registros() == 1);
	assert(nodo->get_proximo_nodo() == -1);
	assert(nodo->buscar(claveEscrita,NULL) == 0);
	assert(regsOverflow.empty());

	// Empaqueto el nodo y lo guardo en el archivo
	assert(nodo->empaquetar(bloque) == RES_OK);
	assert(manejador.escribir_bloque(bloque) == 0); // Se escribe en el bloque 0
	assert(manejador.cerrar_archivo() == RES_OK);


	manejador.abrir_archivo("bloques_nodos_secuenciales.dat","rb+");

	// Levanto el nodo
	NodoSecuencial* nodoLeido = new NodoSecuencial(100,4096);
	Bloque* bloqueLeido = manejador.obtener_bloque(0);

	// Lo desempaqueto
	assert(nodoLeido->desempaquetar(bloqueLeido) == RES_OK);
	assert(nodoLeido->get_cantidad_registros()==1);
	assert(nodoLeido->get_proximo_nodo() == -1);


	// Comprobaciones pertinentes
	RegistroClave* registroLeido = NULL;
	ClaveX claveLeida;
	assert(nodoLeido->buscar(claveEscrita,&registroLeido) == 0);
	assert(registroLeido != NULL);
	assert(registroLeido->get_clave() == claveEscrita);

	char* campoLeido = new char[registroLeido->get_tamanio_campo(1)+1];
	assert(registroLeido->recuperar_campo(campoLeido,1) != RES_ERROR);
	campoLeido[registroLeido->get_tamanio_campo(1)] = '\0';
	assert(strcmp(campoLeido,campoEscrito.c_str()) == 0);

	delete[] campoLeido;
	delete(nodo);
	delete(bloque);
	delete(nodoLeido);
	delete(bloqueLeido);
	delete(registroLeido);

	manejador.cerrar_archivo();
}

TEST_F(TestNodoSecuencial,Insertar_eliminar)
{
	// Set up
	NodoSecuencial* nodo = new NodoSecuencial(11,100);
	RegistroClave registro;
	ClaveX clave;
	std::vector<RegistroClave> regsUnderflow;
	std::vector<RegistroClave> regsOverflow;

	// Pruebo eliminar una clave vacia -> no debe andar
	assert(nodo->eliminar(clave,regsUnderflow) != RES_OK);
	assert(regsUnderflow.empty() == true);
	// Pruebo eliminar una clave que no existe -> no debe andar
	clave.set_clave("una clave");
	assert(nodo->eliminar(clave,regsUnderflow) == RES_RECORD_DOESNT_EXIST);
	assert(regsUnderflow.empty() == true);

	// Agrego una clave (no se produce overflow)
	// intento insertarla de nuevo (no se puede)
	// y la borro (se produce underflow!)
	registro.set_clave(clave);
	std::string campo = "un campo";
	registro.agregar_campo(campo.c_str(), campo.size());
	assert(nodo->insertar(registro,regsOverflow) == RES_OK);
	assert(regsOverflow.empty() == true);
	assert(nodo->insertar(registro,regsOverflow) == RES_RECORD_EXISTS);
	assert(regsOverflow.empty() == true);

	assert(nodo->eliminar(clave,regsUnderflow) == RES_UNDERFLOW);
	assert(regsUnderflow.empty() == true);
	assert(nodo->get_cantidad_registros() == 0);


	// Agrego dos claves (no se produce overflow)
	// y borro una (se produce underflow!)
	RegistroClave reg1, reg2;
	ClaveX clave1, clave2;
	std::string campo1, campo2;
	clave1.set_clave("aa");
	clave2.set_clave("bb");
	campo1 = "1";
	campo2 = "2";
	reg1.agregar_campo(campo1.c_str(), campo1.size());
	reg2.agregar_campo(campo2.c_str(), campo2.size());
	reg1.set_clave(clave1);
	reg2.set_clave(clave2);

	// El nodo tiene "una clave|un campo"
	assert(nodo->insertar(reg2,regsOverflow) == RES_OK);
	// El nodo tiene "una clave|un campo" y "aa|2"
	assert(regsOverflow.empty() == true);
	assert(nodo->insertar(reg1,regsOverflow) == RES_OK);
	assert(regsOverflow.empty() == true);

	//assert(nodo->get_bytes_ocupados() == 20);

	// Los registros deben estar ordenados dentro del nodo
	RegistroClave* regleido1 = NULL;
	RegistroClave* regleido2 = NULL;
	assert(nodo->buscar(clave1,&regleido1) == 0);
	assert(regleido1 != NULL);
	assert(nodo->buscar(clave2,&regleido2) == 1);
	assert(regleido2 != NULL);

	// Elimino una clave
	//assert(nodo->get_bytes_ocupados() == 20);
	assert(nodo->eliminar(clave1,regsUnderflow) == RES_UNDERFLOW);
	assert(nodo->get_cantidad_registros() == 0);
	assert(regsUnderflow.empty() == false);
	assert(regsUnderflow.size() == 1);

	assert (regsUnderflow.at(0).get_clave() == clave2);

	assert(nodo->get_cantidad_registros() == 0);


	delete(nodo);
	delete(regleido1);
	delete(regleido2);
}


TEST_F(TestNodoSecuencial,Overflow_masivo)
{
	std::vector<RegistroClave> regsOverflow;
	std::vector<RegistroClave> registros;
	std::vector<ClaveX> claves;
	std::string campo = "1";

	int valorClave = 0;
	for (; valorClave<1000 ; valorClave++)
	{
		ClaveX clave;
		clave.set_clave(valorClave);
		claves.push_back(clave);

		RegistroClave registro;
		registro.set_clave(clave);
		registro.agregar_campo(campo.c_str(), campo.size());
		registros.push_back(registro);
	}

	int tamReg = registros.at(0).get_tamanio_empaquetado();
	int tamRegTotal = tamReg * 1000;

	NodoSecuencial* nodo = new NodoSecuencial(5,tamRegTotal+sizeof(int)+sizeof(char));
	for (int i = 0; i<1000;i++)
	{
		assert(nodo->insertar(registros.at(i),regsOverflow) == RES_OK);
		assert(regsOverflow.empty() == true);
	}

	RegistroClave regOverflow;
	ClaveX clave;
	clave.set_clave(valorClave+1);
	regOverflow.set_clave(clave);
	regOverflow.agregar_campo(campo.c_str(), campo.size());

	// El nodo tenia 1000 claves y estaba lleno.
	// Al querer insertarle una clave mas, se produce overflow.
	// Esa clave se inserta, y se devuelven los registros en overflow
	// de tal forma que el nodo izquierdo siempre tenga un registro mas
	// que el derecho
	assert(nodo->get_cantidad_registros()==1000);
	assert(nodo->insertar(regOverflow,regsOverflow) == RES_OVERFLOW);
	assert(regsOverflow.empty() == false);
	assert(regsOverflow.size() == 500);
	assert(nodo->get_cantidad_registros()==501);

	delete(nodo);
}

TEST_F(TestNodoSecuencial,Overflow_impar)
{
	ClaveX clave1, clave2, clave3;
	clave1.set_clave("a");
	clave2.set_clave("b");
	clave3.set_clave("c");

	std::string campo = "un campo";

	RegistroClave reg1, reg2, reg3;
	reg1.set_clave(clave1); reg1.agregar_campo(campo.c_str(),campo.size());
	reg2.set_clave(clave2); reg2.agregar_campo(campo.c_str(),campo.size());
	reg3.set_clave(clave3); reg3.agregar_campo(campo.c_str(),campo.size());

	// El nodo soportara hasta 2 registros
	NodoSecuencial* nodo = new NodoSecuencial(0,2*reg1.get_tamanio_empaquetado());


	// Meto el primero, no falla
	std::vector <RegistroClave> regsOverflow;
	assert (nodo->insertar(reg1,regsOverflow) == RES_OK);
	assert (regsOverflow.empty() == true);

	// Meto el segundo, no falla
	assert (nodo->insertar(reg2,regsOverflow) == RES_OK);
	assert (regsOverflow.empty() == true);

	// Meto el tercero -> overflow!
	assert (nodo->insertar(reg3,regsOverflow) == RES_OVERFLOW);
	assert (nodo->get_cantidad_registros() == 2);
	assert (regsOverflow.size() == 1);
	assert (regsOverflow[0] == reg3);


	delete(nodo);
}

TEST_F(TestNodoSecuencial,Overflow_par)
{
	ClaveX clave1, clave2;
	clave1.set_clave("a");
	clave2.set_clave("b");

	std::string campo = "un campo";

	RegistroClave reg1, reg2;
	reg1.set_clave(clave1); reg1.agregar_campo(campo.c_str(),campo.size());
	reg2.set_clave(clave2); reg2.agregar_campo(campo.c_str(),campo.size());

	assert (reg1.get_tamanio_empaquetado() == reg2.get_tamanio_empaquetado());

	// El nodo soportara hasta 1 registro
	NodoSecuencial* nodo = new NodoSecuencial(0,1*reg1.get_tamanio_empaquetado());

	// Meto el primero, no falla
	std::vector <RegistroClave> regsOverflow;
	assert (nodo->insertar(reg1,regsOverflow) == RES_OK);
	assert (regsOverflow.empty() == true);

	// Meto el segundo -> overflow!
	assert (nodo->insertar(reg2,regsOverflow) == RES_OVERFLOW);
	assert (nodo->get_cantidad_registros() == 1);
	assert (regsOverflow.size() == 1);
	assert (regsOverflow[0] == reg2);

	delete(nodo);
}

TEST_F(TestNodoSecuencial,Guardar_comparar)
{
	NodoSecuencial ns(0,4096);
	RegistroClave rc1,rc2;
	ClaveX c1,c2;
	c1.set_clave(99);
	c2.set_clave(100);
	rc1.set_clave(c1);
	rc2.set_clave(c2);
	vector<RegistroClave> v;
	ns.insertar(rc1,v);
	ns.insertar(rc2,v);
	Bloque b;

	ns.empaquetar(&b);
	NodoSecuencial ns2(0,4096);
	ns2.desempaquetar(&b);
	RegistroClave *rc3= NULL,*rc4= NULL;
	ns2.buscar(c1,&rc3);
	ns2.buscar(c2,&rc4);

	assert( (*rc3)<(*rc4) );

	delete rc3;
	delete rc4;
}
