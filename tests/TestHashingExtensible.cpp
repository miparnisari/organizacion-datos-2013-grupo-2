#define DIRECCION "HashingDePrueba"
#define DIRECCIONTABLA "HashingDePruebaTabla.dat" //Ya sabemos como se va a llamar el archivo

#include "TestHashingExtensible.h"

using namespace std;
using namespace utilitarios;

TestHashingExtensible::TestHashingExtensible()
        : Test()
{

}
TestHashingExtensible::~TestHashingExtensible()
{

}

void TestHashingExtensible::ejecutar()
{
//	test_guardar_y_recuperar_ints();
    test_hashing_guardar_y_leer_int();
    test_crear_hashing();
    test_eliminar_registro();
    test_agregar_y_devolver_registro();
    test_crear_hashing_cerrarlo_y_abrirlo();
    test_agregar_varios_registros_y_devolver();
    test_agregar_reg_y_duplicar_tabla();
    test_eliminar_reg_y_dividir_tabla();
}

void TestHashingExtensible::test_guardar_y_recuperar_ints()
{
	// Guardo en el disco
	HashingExtensible indiceSecundarioTitulo;
	indiceSecundarioTitulo.crear_archivo("hash_autor_id");
	indiceSecundarioTitulo.abrir_archivo("hash_autor_id");

	RegistroClave regClave;
	ClaveX claveTitulo;
	claveTitulo.set_clave("orgadedatos");
	int idCancion = 10;

	// Registro = "somewhere only we know" + "10"
	regClave.set_clave(claveTitulo);
	regClave.agregar_campo((char*)&idCancion,sizeof(idCancion));

	assert(indiceSecundarioTitulo.agregar(regClave) == RES_OK);
	indiceSecundarioTitulo.cerrar_archivo();

	// Consulto del disco
	indiceSecundarioTitulo.abrir_archivo("hash_autor_id");
	ClaveX claveConsulta;
	claveConsulta.set_clave("orgadedatos");

	RegistroClave regRecuperado;
	assert (indiceSecundarioTitulo.devolver(claveConsulta,&regRecuperado) == RES_OK);

	assert (regRecuperado.get_cantidad_campos() == 2);
	assert (regRecuperado.get_tamanio_campo(0) == 12);
	assert (regRecuperado.get_tamanio_campo(1) == 4);

	indiceSecundarioTitulo.cerrar_archivo();

	print_test_ok("test_guardar_y_recuperar_ints");
}

void TestHashingExtensible::test_hashing_guardar_y_leer_int()
{
	// ARRANGE
	ManejadorBloques manejador;
	manejador.crear_archivo("prueba_int.dat",100);
	manejador.abrir_archivo("prueba_int.dat","rb+");
	Bloque* bloque = manejador.crear_bloque();
	int numeroEscrito = 156;
    RegistroVariable regEscrito;
    assert (regEscrito.agregar_campo((char*)&numeroEscrito,sizeof(numeroEscrito)) == RES_OK);
    assert (bloque->agregar_registro(&regEscrito) == RES_OK);


    // ACT -> Escribo en el archivo
    assert (manejador.escribir_bloque(bloque) == RES_OK);
    manejador.cerrar_archivo();


    // ASSERT -> Leo del archivo
    manejador.abrir_archivo("prueba_int.dat","rb+");
    RegistroVariable regLeido;
    Bloque* bloqueLeido = manejador.obtener_bloque(0);
    bloqueLeido->recuperar_registro(&regLeido,0);
    int numeroLeido;
    regLeido.recuperar_campo((char*)&numeroLeido,0);

    assert(numeroLeido == numeroEscrito);

    manejador.cerrar_archivo();
    delete bloque;
    delete bloqueLeido;

    print_test_ok("test_hashing_guardar_y_leer_int");

}

void TestHashingExtensible::test_crear_hashing()
{
    HashingExtensible hash1;
    if (hash1.abrir_archivo(DIRECCION) == RES_OK){
    	assert (hash1.cerrar_archivo() == RES_OK);
    	assert (hash1.eliminar_archivo() == RES_OK);
    }

    assert (hash1.cerrar_archivo() == RES_OK);
    assert (hash1.crear_archivo(DIRECCION) == RES_OK);

    print_test_ok("crear_hashing");
}

void TestHashingExtensible::test_eliminar_registro()
{
    HashingExtensible hash1;

    if (hash1.abrir_archivo(DIRECCION) == RES_OK){
    	assert (hash1.cerrar_archivo() == RES_OK);
    	assert (hash1.eliminar_archivo() == RES_OK);
    }

    assert (hash1.cerrar_archivo() == RES_OK);
    assert( hash1.crear_archivo(DIRECCION) == RES_OK);
    RegistroClave reg;
    ClaveX clave;
    clave.set_clave("esta es una clave");
    std::string campo = "Hola";

    assert (hash1.abrir_archivo(DIRECCION) == RES_OK);
    this->crear_registro_y_agregar(hash1, campo, clave);
    assert (hash1.eliminar(clave) == RES_OK);
    assert(hash1.devolver(clave, &reg) == RES_RECORD_DOESNT_EXIST);
    assert (hash1.cerrar_archivo() == RES_OK);

    print_test_ok("test_eliminar_registro");
}

void TestHashingExtensible::test_agregar_y_devolver_registro()
{
    HashingExtensible hash1;

    if (hash1.abrir_archivo(DIRECCION) == RES_OK){
    	assert (hash1.cerrar_archivo() == RES_OK);
    	assert (hash1.eliminar_archivo() == RES_OK);
    }

    assert (hash1.cerrar_archivo() == RES_OK);
    assert( hash1.crear_archivo(DIRECCION) == RES_OK);
    RegistroClave reg;
    int claveNum;
    ClaveX clave, clave2;

    ManejadorBloques manejador;
    Bloque* bloq = NULL;
    RegistroVariable regVar;
    int numeroLeido;

    //Creo un nuevo registro "23" (clave) + "Hola" (dato)
    clave.set_clave(23);

    std::string campo = "Hola";
    assert (hash1.abrir_archivo(DIRECCION) == RES_OK);
    assert(this->crear_registro_y_agregar(hash1, campo, clave) == RES_OK);
    assert (hash1.cerrar_archivo() == RES_OK);

    manejador.abrir_archivo("HashingDePrueba.dat", "rb+");
    bloq = manejador.obtener_bloque(0);
    assert (bloq != NULL);
    assert (bloq -> get_cantidad_registros_almacenados() == 2);
    bloq->recuperar_registro(&regVar,0);
    assert(regVar.get_cantidad_campos() == 1);
    regVar.recuperar_campo((char*)&numeroLeido,0);

    assert(numeroLeido == 1);

    bloq->recuperar_registro(&reg,1);
    clave = reg.get_clave();

    clave.get_clave(numeroLeido);
    assert(numeroLeido == 23);
    manejador.cerrar_archivo();
    //Pruebo a ver si encuentra el registro en el hash
    assert (hash1.abrir_archivo(DIRECCION) == RES_OK);
    assert(hash1.devolver(clave, &reg) == RES_OK);

    assert(reg.get_cantidad_campos() == 2);

    //Pruebo si saco el registro correcto
    clave2 = reg.get_clave();
    clave2.get_clave(claveNum);
    assert(claveNum == 23);

    //Pruebo si se guardo el dato correcto
    char *dato = NULL;
    this->recuperar_dato_registro(&dato, clave, hash1);
    assert (strlen(dato) == 4);
    assert (strcmp(dato,campo.c_str()) == 0);
    delete[] dato;

    assert (hash1.cerrar_archivo() == RES_OK);

    delete bloq;

    print_test_ok("test_agregar_y_devolver_registro");
}

void TestHashingExtensible::test_crear_hashing_cerrarlo_y_abrirlo()
{

	HashingExtensible hash1;

	if (hash1.abrir_archivo(DIRECCION) == RES_OK){
		assert (hash1.cerrar_archivo() == RES_OK);
		assert(hash1.eliminar_archivo() == RES_OK);
	}
	assert (hash1.cerrar_archivo() == RES_OK);
	assert( hash1.crear_archivo(DIRECCION) == RES_OK);
    ClaveX clave;
    char* campoRecuperado = NULL;
    clave.set_clave(23);
    std::string campo = "Hola";
    hash1.abrir_archivo(DIRECCION);
    this->crear_registro_y_agregar(hash1, campo, clave);


	HashingExtensible hash2;
	assert( hash2.crear_archivo(DIRECCION) == RES_FILE_EXISTS);

    this->recuperar_dato_registro(&campoRecuperado, clave, hash1);
    hash1.cerrar_archivo();
    assert (strcmp(campoRecuperado,campo.c_str()) == 0);

    delete[] campoRecuperado;

    print_test_ok("test_crear_hashing_cerrarlo_y_abrirlo");
}

void TestHashingExtensible::test_agregar_varios_registros_y_devolver()
{

	HashingExtensible hash1;

	if (hash1.abrir_archivo(DIRECCION) == RES_OK){
		assert (hash1.cerrar_archivo() == RES_OK);
		assert (hash1.eliminar_archivo() == RES_OK);
	}
	assert (hash1.cerrar_archivo() == RES_OK);
	assert( hash1.crear_archivo(DIRECCION) == RES_OK);
    RegistroClave reg;
    int claveNum = -1;
    char *dato = NULL;
    std::string campo;
    ClaveX clave, clave2;
    //Creo varios registro
    clave.set_clave(23);
    campo = "Hola";
    hash1.abrir_archivo(DIRECCION);
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
    assert(hash1.devolver(clave, &reg) == RES_OK);

    //Pruebo si saco el registro correcto
    clave2 = reg.get_clave();
    clave2.get_clave(claveNum);
    assert(claveNum == 72);

    //Pruebo si se guardo el dato correcto
    this->recuperar_dato_registro(&dato, clave, hash1);
    campo = "Hola mundo";
    assert (strcmp(dato,campo.c_str()) == 0);
    delete[] dato;

    hash1.cerrar_archivo();
    print_test_ok("test_agregar_varios_registros_y_devolver");
}

void TestHashingExtensible::test_agregar_reg_y_duplicar_tabla()
{   /**Desbloquear cuando tengamos la tabla y ver cuantos registros necesitamos para duplicar la tabla*/
	ClaveX clave;
	int i;
	std::string campo = "Dato de relleno ..............";
	HashingExtensible hash1;

	if (hash1.abrir_archivo(DIRECCION) == RES_OK){
		assert (hash1.cerrar_archivo() == RES_OK);
		hash1.eliminar_archivo();
	}
	assert (hash1.cerrar_archivo() == RES_OK);
	assert (hash1.crear_archivo(DIRECCION) == RES_OK);
	hash1.abrir_archivo(DIRECCION);
	Tabla tabla;
	tabla.crear(DIRECCIONTABLA);
	assert(1 == tabla.get_tamanio()); //Vemos si la tabla comienza con un elemento
	for(i=0; i<24;i++){ //Agregamos cantidad de elementos hasta que se llene un bloque
		clave.set_clave(i);
		assert(this->crear_registro_y_agregar(hash1, campo, clave) == RES_OK);
	}
	// Agregamos el elemento que provoca que la tabla se duplique
	clave.set_clave(25);
	assert(this->crear_registro_y_agregar(hash1, campo, clave) == RES_OK);
	assert(2 == tabla.get_tamanio()); //Vemos si se duplico la tabla
	assert(tabla.obtener_valor(1) != tabla.obtener_valor(0)); //vemos si los bloques son distintos de cada elemento de la tabla

	hash1.cerrar_archivo();
	print_test_ok("test_agregar_reg_y_duplicar_tabla");
}

void TestHashingExtensible::test_eliminar_reg_y_dividir_tabla()
{   /**Desbloquear cuando tengamos la tabla y ver cuantos registros necesitamos para duplicar la tabla*/
	ClaveX clave;
	int i;
	std::string campo = "Dato de relleno ..............";
	HashingExtensible hash1;

	if (hash1.abrir_archivo(DIRECCION) == RES_OK){
		assert (hash1.cerrar_archivo() == RES_OK);
		assert (hash1.eliminar_archivo() == RES_OK);
	}
	assert (hash1.cerrar_archivo() == RES_OK);
	assert (hash1.crear_archivo(DIRECCION) == RES_OK);
	hash1.abrir_archivo(DIRECCION);
	Tabla tabla;
	tabla.crear(DIRECCIONTABLA);
	assert(1 == tabla.get_tamanio()); //Vemos si la tabla comienza con un elemento
	for(i=0; i<24;i++){ //Agregamos cantidad de elementos hasta que se llene un bloque
		clave.set_clave(i);
		assert(this->crear_registro_y_agregar(hash1, campo, clave) == RES_OK);
		assert(this->crear_registro_y_agregar(hash1, campo, clave) == RES_RECORD_EXISTS);
	}
	// Agregamos el elemento que provoca que la tabla se duplique
	clave.set_clave(25);
	assert(this->crear_registro_y_agregar(hash1, campo, clave) == RES_OK);
	assert(2 == tabla.get_tamanio());

	// Elimino todos los registros del bloque 0
	for (int i = 1; i < 26; i+=2)
	{
		clave.set_clave(i);
		assert(hash1.eliminar(clave) == RES_OK);
		assert(hash1.eliminar(clave) == RES_RECORD_DOESNT_EXIST);
	}

	hash1.cerrar_archivo();
	assert(1 == tabla.get_tamanio()); //Vemos si se dividio la tabla

    print_test_ok("test_eliminar_reg_y_dividir_tabla");
}

int TestHashingExtensible::crear_registro_y_agregar(HashingExtensible & hash1, std::string campo, ClaveX clave)
{
    RegistroClave reg;
    reg.set_clave(clave);
    reg.agregar_campo(campo.c_str(), campo.size());
    return hash1.agregar(reg);
}

void TestHashingExtensible::recuperar_dato_registro(char** campoRecuperado, ClaveX& clave, HashingExtensible & hash1)
{
    RegistroClave reg;
    hash1.devolver(clave,&reg);
    *campoRecuperado = new char[reg.get_tamanio_campo(1) +1]();
    (*campoRecuperado) [reg.get_tamanio_campo(1)] = '\0';
    reg.recuperar_campo(*campoRecuperado,1);
}
