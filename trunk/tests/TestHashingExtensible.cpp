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

void TestHashingExtensible::test_crear_hashing()
{
    HashingExtensible hash1("",DIRECCION);
    ManejadorBloques manejador_bloques;
    manejador_bloques.abrir_archivo(DIRECCION, "rb");
    assert (manejador_bloques.get_cantidad_bloques() == 0);
    //Ya que el primer bloque es la tabla y el segundo es el primer bloque de registros

    print_test_ok("crear_hashing");
    hash1.eliminar_hashing_extensible();
    hash1.~HashingExtensible();
}

void TestHashingExtensible::test_eliminar_registro()
{
    HashingExtensible hash1("",DIRECCION);
    RegistroClave reg;
    ClaveX clave;
    clave.set_clave(23);
    std::string campo = "Hola";
    crear_registro_y_agregar(&hash1, campo, clave);

    hash1.eliminar(clave);
    assert(hash1.devolver(clave, &reg) == NO_EXISTE);

    print_test_ok("test_eliminar_registro");
    hash1.eliminar_hashing_extensible();
    hash1.~HashingExtensible();
    reg.~RegistroVariable();
    clave.~ClaveX();
}

void TestHashingExtensible::test_agregar_y_devolver_registro()
{
    HashingExtensible hash1("",DIRECCION);
    RegistroClave reg;
    int claveNum;
    char *dato = NULL;
    ClaveX clave, clave2;
    //Creo un nuevo registro
    clave.set_clave(23);
    std::string campo = "Hola";
    crear_registro_y_agregar(&hash1, campo, clave);

    //Pruebo a ver si encuentra el registro en el hash
    assert(hash1.devolver(clave, &reg) == RES_OK);

    //Pruebo si saco el registro correcto
    reg.get_clave(clave2);
    clave2.get_clave(claveNum);
    assert(claveNum == 23);

    //Pruebo si se guardo el dato correcto
    recuperar_dato_registro(dato, clave, hash1);
    assert (strcmp(dato,campo.c_str()) == 0);
    delete[] dato;

    print_test_ok("test_agregar_y_devolver_registro");
    hash1.eliminar_hashing_extensible();
    hash1.~HashingExtensible();
    reg.~RegistroVariable();
    clave.~ClaveX();
}

void TestHashingExtensible::test_crear_hashing_cerrarlo_y_abrirlo()
{
    HashingExtensible hash1("",DIRECCION);
    ClaveX clave;
    char* campoRecuperado = NULL;
    clave.set_clave(23);
    std::string campo = "Hola";
    crear_registro_y_agregar(&hash1, campo, clave);
    hash1.~HashingExtensible();

    HashingExtensible hash2("",DIRECCION);
    recuperar_dato_registro(campoRecuperado, clave, hash2);

    assert (strcmp(campoRecuperado,campo.c_str()) == 0);
    delete[] campoRecuperado;

    print_test_ok("test_crear_hashing_cerrarlo_y_abrirlo");

    hash1.eliminar_hashing_extensible();
    hash2.~HashingExtensible();
    clave.~ClaveX();
}

void TestHashingExtensible::test_agregar_varios_registros_y_devolver()
{
    HashingExtensible hash1("",DIRECCION);
    RegistroClave reg;
    int claveNum;
    char *dato = NULL;
    std::string campo;
    ClaveX clave, clave2;
    //Creo varios registro
    clave.set_clave(23);
    campo = "Hola";
    crear_registro_y_agregar(&hash1, campo, clave);

    clave.set_clave(32);
    campo = "Chau";
    crear_registro_y_agregar(&hash1, campo, clave);

    clave.set_clave(72);
    campo = "Hola mundo";
    crear_registro_y_agregar(&hash1, campo, clave);

    clave.set_clave(55);
    campo = "Hola chau";
    crear_registro_y_agregar(&hash1, campo, clave);

    //Pruebo a ver si encuentra el registro en el hash
    clave.set_clave(72);
    assert(hash1.devolver(clave, &reg) == RES_OK);

    //Pruebo si saco el registro correcto
    reg.get_clave(clave2);
    clave2.get_clave(claveNum);
    assert(claveNum == 72);

    //Pruebo si se guardo el dato correcto
    recuperar_dato_registro(dato, clave, hash1);
    campo = "Hola mundo";
    assert (strcmp(dato,campo.c_str()) == 0);
    delete[] dato;

    print_test_ok("test_agregar_varios_registros_y_devolver");
    hash1.eliminar_hashing_extensible();
    hash1.~HashingExtensible();
    reg.~RegistroVariable();
    clave.~ClaveX();
}

void TestHashingExtensible::test_agregar_reg_y_duplicar_tabla()
{   /**Desbloquear cuando tengamos la tabla y ver cuantos registros necesitamos para duplicar la tabla*/
/*	ClaveX clave;
	int i;
	std::string campo = "Dato de relleno ..............";
	HashingExtensible hash1("",DIRECCION);
	Tabla tabla(DIRECCIONTABLA);
	assert(1 == tabla.get_tamanio()); //Vemos si la tabla comienza con un elemento
	for(i=0; i<3000;i++){ //Agregamos cantidad de elementos hasta que se llene el bloque mas un elemento
        clave.set_clave(i);
        crear_registro_y_agregar(&hash1, campo, clave);
	}
	assert(2 == tabla.get_tamanio()); //Vemos si se duplico la tabla
	assert(tabla.obtener_valor(1) != tabla.obtener_valor(0)); //vemos si los bloques son distintos de cada elemento de la tabla
    print_test_ok("test_agregar_reg_y_duplicar_tabla");*/
}

void TestHashingExtensible::test_eliminar_reg_y_dividir_tabla()
{   /**Desbloquear cuando tengamos la tabla y ver cuantos registros necesitamos para duplicar la tabla*/
	/*ClaveX clave;
	int i;
	std::string campo = "Dato de relleno ..............";
	HashingExtensible hash1("",DIRECCION);
	Tabla tabla(DIRECCIONTABLA);
	assert(1 == tabla.get_tamanio()); //Vemos si la tabla comienza con un elemento
	for(i=0; i<3000;i++){ //Agregamos cantidad de elementos hasta que se llene el bloque mas un elemento
        clave.set_clave(i);
        crear_registro_y_agregar(&hash1, campo, clave);
	}
	assert(2 == tabla.get_tamanio()); //Vemos si se duplico la tabla
	hash1.eliminar(clave); //Elimino el ultimo registro
	assert(1 == tabla.get_tamanio()); //Vemos si se dividio la tabla
    print_test_ok("test_eliminar_reg_y_dividir_tabla");*/
}

void TestHashingExtensible::ejecutar()
{
    test_crear_hashing();
   /* test_eliminar_registro();
	test_agregar_y_devolver_registro();
	test_crear_hashing_cerrarlo_y_abrirlo();
	test_agregar_varios_registros_y_devolver();
	test_agregar_reg_y_duplicar_tabla();
	test_eliminar_reg_y_dividir_tabla();*/
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
