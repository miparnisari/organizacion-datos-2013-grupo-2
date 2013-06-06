#include "../src/CapaLogica/HashingExtensible/HashingExtensible.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestHashingExtensible : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	HashingExtensible hash1;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	  ASSERT_TRUE (hash1.crear_archivo("HashingDePrueba") == RES_OK);
	  ASSERT_TRUE (hash1.abrir_archivo("HashingDePrueba") == RES_OK);
  }

  virtual void TearDown() {
	  ASSERT_TRUE (hash1.cerrar_archivo() == RES_OK);
	  ASSERT_TRUE (hash1.eliminar_archivo() == RES_OK);
  }

  // A helper function that some test uses.
  int crear_registro_y_agregar(HashingExtensible & hash1, std::string campo, ClaveX clave)
  {
      RegistroClave reg;
      reg.set_clave(clave);
      reg.agregar_campo(campo.c_str(), campo.size());
      return hash1.agregar(reg);
  }

  // A helper function that some test uses.
  void recuperar_dato_registro(char** campoRecuperado, ClaveX& clave, HashingExtensible & hash1)
  {
      RegistroClave reg;
      hash1.devolver(clave,&reg);
      *campoRecuperado = new char[reg.get_tamanio_campo(1) +1]();
      (*campoRecuperado) [reg.get_tamanio_campo(1)] = '\0';
      reg.recuperar_campo(*campoRecuperado,1);
  }

};

#define DIRECCIONTABLA "HashingDePruebaTabla.dat" //Ya sabemos como se va a llamar el archivo

using namespace std;
using namespace utilitarios;

TEST_F(TestHashingExtensible,Guardar_recuperar_ints)
{
	// Guardo en el disco
	HashingExtensible indiceSecundarioTitulo;
	ASSERT_TRUE(indiceSecundarioTitulo.crear_archivo("hash_autor_id") == RES_OK);
	ASSERT_TRUE(indiceSecundarioTitulo.abrir_archivo("hash_autor_id") == RES_OK);

	RegistroClave regClave;
	ClaveX claveTitulo;
	claveTitulo.set_clave("orgadedatos");
	int idCancion = 156;
	// Registro = "orgadedatos" + "10"
	regClave.set_clave(claveTitulo);
	regClave.agregar_campo((char*)&idCancion,sizeof(idCancion));
	ASSERT_TRUE(indiceSecundarioTitulo.agregar(regClave) == RES_OK);
	indiceSecundarioTitulo.cerrar_archivo();
	// Consulto del disco
	indiceSecundarioTitulo.abrir_archivo("hash_autor_id");
	ClaveX claveConsulta;
	claveConsulta.set_clave("orgadedatos");

	RegistroClave regRecuperado;
	ASSERT_TRUE (indiceSecundarioTitulo.devolver(claveConsulta,&regRecuperado) == RES_OK);

	// Recupero la clave
	ClaveX clave = regRecuperado.get_clave();

	// Recupero el dato
	int intrecuperado;
	ASSERT_TRUE(regRecuperado.recuperar_campo((char*)(&intrecuperado),1) != RES_ERROR);
	ASSERT_TRUE (regRecuperado.get_cantidad_campos() == 2);
	ASSERT_TRUE (regRecuperado.get_tamanio_campo(0) == 11 + 1);
	ASSERT_TRUE (regRecuperado.get_tamanio_campo(1) == 4);

	indiceSecundarioTitulo.cerrar_archivo();
	indiceSecundarioTitulo.eliminar_archivo();
}

TEST_F(TestHashingExtensible,Modificar_valor)
{
	RegistroClave regClave;
	ClaveX clave;
	clave.set_clave("clavecita");
	string campo = "uncampo";
	string comprobacion_clave;

	this->crear_registro_y_agregar(hash1, campo, clave);

	//cambio el campo de la clave
	regClave.set_clave(clave);
	campo = "otroCampo";
	regClave.agregar_campo(campo.c_str(), campo.size());

	//modifico el registro
	ASSERT_TRUE(hash1.modificar(regClave) == RES_OK);

	ASSERT_TRUE(hash1.devolver(clave, &regClave) == RES_OK);

	//verifico que no haya cambios en el numero de clave
	clave.get_clave(comprobacion_clave);
	ASSERT_TRUE(comprobacion_clave == "clavecita");

	//chequeo que se cambio el dato del campo
	char *dato = NULL;
	this->recuperar_dato_registro(&dato, clave, hash1);
	ASSERT_TRUE (strlen(dato) == 9);
	ASSERT_TRUE (strcmp(dato,campo.c_str()) == 0);
	delete[] dato;


	//veo que si entro una clave que no existe, no se modifica ningun valor, ni se agregan datos.
	clave.set_clave(32);
	campo = "otracosaaa";
	regClave.set_clave(clave);
	regClave.agregar_campo(campo.c_str(), campo.size());

	ASSERT_TRUE(hash1.modificar(regClave) == RES_RECORD_DOESNT_EXIST);

	ASSERT_TRUE(hash1.devolver(clave, &regClave) == RES_RECORD_DOESNT_EXIST);

}

TEST_F(TestHashingExtensible,Eliminar_registro)
{
    RegistroClave reg;
    ClaveX clave;
    clave.set_clave("esta es una clave");
    std::string campo = "Hola";

    this->crear_registro_y_agregar(hash1, campo, clave);
    ASSERT_TRUE (hash1.eliminar(clave) == RES_OK);
    ASSERT_TRUE (hash1.devolver(clave, &reg) == RES_RECORD_DOESNT_EXIST);
}

TEST_F(TestHashingExtensible,Agregar_devolver_registro)
{
    RegistroClave reg;
    int claveNum;
    ClaveX clave, clave2;

    ManejadorBloques manejador;
    Bloque* bloq = NULL;
    RegistroVariable regVar;

    //Creo un nuevo registro "23" (clave) + "Hola" (dato)
    clave.set_clave(23);

    std::string campo = "Hola";
    ASSERT_TRUE(this->crear_registro_y_agregar(hash1, campo, clave) == RES_OK);

    ASSERT_TRUE(hash1.devolver(clave, &reg) == RES_OK);

    ASSERT_TRUE(reg.get_cantidad_campos() == 2);

    //Pruebo si saco el registro correcto
    clave2 = reg.get_clave();
    clave2.get_clave(claveNum);
    ASSERT_TRUE(claveNum == 23);

    //Pruebo si se guardo el dato correcto
    char *dato = NULL;
    this->recuperar_dato_registro(&dato, clave, hash1);
    ASSERT_TRUE (strlen(dato) == 4);
    ASSERT_TRUE (strcmp(dato,campo.c_str()) == 0);

    delete[] dato;
    delete bloq;
}
TEST_F(TestHashingExtensible,Crear_cerrar_abrir)
{
    ClaveX clave;
    char* campoRecuperado = NULL;
    clave.set_clave(23);
    std::string campo = "Hola";

    this->crear_registro_y_agregar(hash1, campo, clave);


	HashingExtensible hash2;
	ASSERT_TRUE( hash2.crear_archivo("HashingDePrueba") == RES_FILE_EXISTS);

    this->recuperar_dato_registro(&campoRecuperado, clave, hash1);

    ASSERT_TRUE (strcmp(campoRecuperado,campo.c_str()) == 0);

    delete[] campoRecuperado;
}

TEST_F(TestHashingExtensible,Agregar_varios_registros_y_devolver)
{
    RegistroClave reg;
    int claveNum = -1;
    char *dato = NULL;
    std::string campo;
    ClaveX clave, clave2;
    //Creo varios registro
    clave.set_clave(23);
    campo = "Hola";

    this->crear_registro_y_agregar(hash1, campo, clave);

    clave.set_clave(32);
    campo = "Chau";
    this->crear_registro_y_agregar(hash1, campo, clave);

    clave.set_clave(72);
    campo = "Hola mundo";
    this->crear_registro_y_agregar(hash1, campo, clave);

    clave.set_clave(55);
    campo = "Hola chau";
    this->crear_registro_y_agregar(hash1, campo, clave);

    //Pruebo a ver si encuentra el registro en el hash
    clave.set_clave(72);
    ASSERT_TRUE(hash1.devolver(clave, &reg) == RES_OK);

    //Pruebo si saco el registro correcto
    clave2 = reg.get_clave();
    clave2.get_clave(claveNum);
    ASSERT_TRUE(claveNum == 72);

    //Pruebo si se guardo el dato correcto
    this->recuperar_dato_registro(&dato, clave, hash1);
    campo = "Hola mundo";
    ASSERT_TRUE (strcmp(dato,campo.c_str()) == 0);
    delete[] dato;
}

TEST_F(TestHashingExtensible,Duplicar_tabla)
{
  /**Desbloquear cuando tengamos la tabla y ver cuantos registros necesitamos para duplicar la tabla*/
	ClaveX clave;
	int i;
	std::string campo = "Dato de relleno ..............";

	Tabla tabla;
	tabla.crear(DIRECCIONTABLA);
	ASSERT_TRUE(1 == tabla.get_tamanio()); //Vemos si la tabla comienza con un elemento
	for(i=0; i<24;i++){ //Agregamos cantidad de elementos hasta que se llene un bloque
		clave.set_clave(i);
		ASSERT_TRUE(this->crear_registro_y_agregar(hash1, campo, clave) == RES_OK);
	}
	// Agregamos el elemento que provoca que la tabla se duplique
	clave.set_clave(25);
	ASSERT_TRUE(this->crear_registro_y_agregar(hash1, campo, clave) == RES_OK);
	ASSERT_TRUE(2 == tabla.get_tamanio()); //Vemos si se duplico la tabla
	ASSERT_TRUE(tabla.obtener_valor(1) != tabla.obtener_valor(0)); //vemos si los bloques son distintos de cada elemento de la tabla
}

TEST_F(TestHashingExtensible,Dividir_tabla)
{
   /**Desbloquear cuando tengamos la tabla y ver cuantos registros necesitamos para duplicar la tabla*/
	ClaveX clave;
	int i;
	std::string campo = "Dato de relleno ..............";

	Tabla tabla;
	tabla.crear(DIRECCIONTABLA);
	ASSERT_TRUE(1 == tabla.get_tamanio()); //Vemos si la tabla comienza con un elemento
	for(i=0; i<24;i++){ //Agregamos cantidad de elementos hasta que se llene un bloque
		clave.set_clave(i);
		ASSERT_TRUE(this->crear_registro_y_agregar(hash1, campo, clave) == RES_OK);
		ASSERT_TRUE(this->crear_registro_y_agregar(hash1, campo, clave) == RES_RECORD_EXISTS);
	}
	// Agregamos el elemento que provoca que la tabla se duplique
	clave.set_clave(25);
	ASSERT_TRUE(this->crear_registro_y_agregar(hash1, campo, clave) == RES_OK);
	ASSERT_TRUE(2 == tabla.get_tamanio());

	// Elimino todos los registros del bloque 0
	for (int i = 1; i < 26; i+=2)
	{
		clave.set_clave(i);
		ASSERT_TRUE(hash1.eliminar(clave) == RES_OK);
		ASSERT_TRUE(hash1.eliminar(clave) == RES_RECORD_DOESNT_EXIST);
	}

	ASSERT_TRUE(1 == tabla.get_tamanio()); //Vemos si se dividio la tabla

}
