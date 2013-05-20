#define DIRECCION "HashingDePrueba.dat"
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
	test_hashing_guardar_y_leer_int();
    test_crear_hashing();
    test_eliminar_registro();
	test_agregar_y_devolver_registro();
	test_crear_hashing_cerrarlo_y_abrirlo();
	test_agregar_varios_registros_y_devolver();
	test_agregar_reg_y_duplicar_tabla();
	test_eliminar_reg_y_dividir_tabla();
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
    assert (hash1.crear(DIRECCION) == RES_OK);
    assert (hash1.eliminar() == RES_OK);

    print_test_ok("crear_hashing");
}

void TestHashingExtensible::test_eliminar_registro()
{
    HashingExtensible hash1;
    assert( hash1.crear(DIRECCION) == RES_OK);
    RegistroClave reg;
    ClaveX clave;
    clave.set_clave(23);
    std::string campo = "Hola";
    this->crear_registro_y_agregar(&hash1, campo, clave);

    printf("paso el agregar");


    hash1.eliminar(clave);
    assert(hash1.devolver(clave, &reg) == NO_EXISTE);

    hash1.eliminar();
    print_test_ok("test_eliminar_registro");
}

void TestHashingExtensible::test_agregar_y_devolver_registro()
{
	HashingExtensible hash1;
	assert( hash1.crear(DIRECCION) == RES_OK);
    RegistroClave reg;
    int claveNum;
    char *dato = NULL;
    ClaveX clave, clave2;
    //Creo un nuevo registro
    clave.set_clave(23);
    std::string campo = "Hola";
    this->crear_registro_y_agregar(&hash1, campo, clave);

    //Pruebo a ver si encuentra el registro en el hash
    assert(hash1.devolver(clave, &reg) == RES_OK);

    //Pruebo si saco el registro correcto
    clave2 = reg.get_clave();
    clave2.get_clave(claveNum);
    assert(claveNum == 23);

    //Pruebo si se guardo el dato correcto
    this->recuperar_dato_registro(dato, clave, hash1);
    assert (strcmp(dato,campo.c_str()) == 0);
    delete[] dato;

    hash1.eliminar();
    print_test_ok("test_agregar_y_devolver_registro");
}

void TestHashingExtensible::test_crear_hashing_cerrarlo_y_abrirlo()
{
	HashingExtensible hash1;
	assert( hash1.crear(DIRECCION) == RES_OK);
    ClaveX clave;
    char* campoRecuperado = NULL;
    clave.set_clave(23);
    std::string campo = "Hola";
    this->crear_registro_y_agregar(&hash1, campo, clave);
    hash1.~HashingExtensible();

	HashingExtensible hash2;
	assert( hash2.crear(DIRECCION) == RES_OK);
    this->recuperar_dato_registro(campoRecuperado, clave, hash2);

    assert (strcmp(campoRecuperado,campo.c_str()) == 0);
    delete[] campoRecuperado;

    hash1.eliminar();
    print_test_ok("test_crear_hashing_cerrarlo_y_abrirlo");
}

void TestHashingExtensible::test_agregar_varios_registros_y_devolver()
{
	HashingExtensible hash1;
	assert( hash1.crear(DIRECCION) == RES_OK);
    RegistroClave reg;
    int claveNum;
    char *dato = NULL;
    std::string campo;
    ClaveX clave, clave2;
    //Creo varios registro
    clave.set_clave(23);
    campo = "Hola";
    this->crear_registro_y_agregar(&hash1, campo, clave);

    clave.set_clave(32);
    campo = "Chau";
    this->crear_registro_y_agregar(&hash1, campo, clave);

    clave.set_clave(72);
    campo = "Hola mundo";
    this->crear_registro_y_agregar(&hash1, campo, clave);

    clave.set_clave(55);
    campo = "Hola chau";
    this->crear_registro_y_agregar(&hash1, campo, clave);

    //Pruebo a ver si encuentra el registro en el hash
    clave.set_clave(72);
    assert(hash1.devolver(clave, &reg) == RES_OK);

    //Pruebo si saco el registro correcto
    clave2 = reg.get_clave();
    clave2.get_clave(claveNum);
    assert(claveNum == 72);

    //Pruebo si se guardo el dato correcto
    this->recuperar_dato_registro(dato, clave, hash1);
    campo = "Hola mundo";
    assert (strcmp(dato,campo.c_str()) == 0);
    delete[] dato;

    hash1.eliminar();
    print_test_ok("test_agregar_varios_registros_y_devolver");
}

void TestHashingExtensible::test_agregar_reg_y_duplicar_tabla()
{   /**Desbloquear cuando tengamos la tabla y ver cuantos registros necesitamos para duplicar la tabla*/
ClaveX clave;
	int i;
	std::string campo = "Dato de relleno ..............";
	HashingExtensible hash1;
	assert( hash1.crear(DIRECCION) == RES_OK);
	Tabla tabla;
	tabla.crear(DIRECCIONTABLA);
	assert(1 == tabla.get_tamanio()); //Vemos si la tabla comienza con un elemento
	for(i=0; i<3000;i++){ //Agregamos cantidad de elementos hasta que se llene el bloque mas un elemento
        clave.set_clave(i);
        this->crear_registro_y_agregar(&hash1, campo, clave);
	}
	assert(2 == tabla.get_tamanio()); //Vemos si se duplico la tabla
	assert(tabla.obtener_valor(1) != tabla.obtener_valor(0)); //vemos si los bloques son distintos de cada elemento de la tabla
    print_test_ok("test_agregar_reg_y_duplicar_tabla");
}

void TestHashingExtensible::test_eliminar_reg_y_dividir_tabla()
{   /**Desbloquear cuando tengamos la tabla y ver cuantos registros necesitamos para duplicar la tabla*/
	ClaveX clave;
	int i;
	std::string campo = "Dato de relleno ..............";
	HashingExtensible hash1;
	assert( hash1.crear(DIRECCION) == RES_OK);
	Tabla tabla;
	tabla.crear(DIRECCIONTABLA);
	assert(1 == tabla.get_tamanio()); //Vemos si la tabla comienza con un elemento
	for(i=0; i<3000;i++){ //Agregamos cantidad de elementos hasta que se llene el bloque mas un elemento
        clave.set_clave(i);
        this->crear_registro_y_agregar(&hash1, campo, clave);
	}
	assert(2 == tabla.get_tamanio()); //Vemos si se duplico la tabla
	hash1.eliminar(clave); //Elimino el ultimo registro
	assert(1 == tabla.get_tamanio()); //Vemos si se dividio la tabla
    print_test_ok("test_eliminar_reg_y_dividir_tabla");
}

void TestHashingExtensible::crear_registro_y_agregar(HashingExtensible *hash1, std::string campo, ClaveX clave)
{
    RegistroClave reg;
    reg.agregar_campo(campo.c_str(), strlen(campo.c_str()));
    reg.set_clave(clave);
    hash1->agregar(reg);
}

void TestHashingExtensible::recuperar_dato_registro(char *campoRecuperado, ClaveX clave, HashingExtensible hash1)
{
    RegistroClave reg;
    hash1.devolver(clave,&reg);
    campoRecuperado = new char[reg.get_tamanio_campo(1) +1]();
    reg.recuperar_campo(campoRecuperado,1);
}
