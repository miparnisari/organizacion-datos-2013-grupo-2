#define DIRECCION "HashingDePrueba.txt"

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

void TestHashingExtensible::eliminar_archivo(std::string direccion)
{
    ManejadorArchivos manejador;
    manejador.eliminar_archivo(direccion);
}

void TestHashingExtensible::test_crear_hashing()
{
    //Probamos que se cree la tabla y el primer bloque de registros cuando se cree el hashing
    HashingExtensible hash1("",DIRECCION);
    ManejadorBloques manejador_bloques;
    manejador_bloques.abrir_archivo(DIRECCION, "rb");
    assert (manejador_bloques.get_cantidad_bloques() == 2);
    //Ya que el primer bloque es la tabla y el segundo es el primer bloque de registros

    print_test_ok("crear_hashing");
    eliminar_archivo(DIRECCION);
    hash1.~HashingExtensible();
    manejador_bloques.~ManejadorArchivos();
}

void TestHashingExtensible::test_eliminar_registro()
{
    //Probamos eliminar un registro y ver si nos devuelve algo
    HashingExtensible hash1("",DIRECCION);
    RegistroClave reg;
    ClaveX clave;
    clave.set_clave(23);
    std::string campo = "Hola";
    crear_registro_y_agregar(&hash1, campo, clave);

    hash1.eliminar(clave);
    assert(hash1.devolver(clave, &reg) == NO_EXISTE);

    print_test_ok("test_eliminar_registro");
    eliminar_archivo(DIRECCION);
    hash1.~HashingExtensible();
    reg.~RegistroVariable();
    clave.~ClaveX();
}

void TestHashingExtensible::test_agregar_y_devolver_registro()
{
    //Probamos que se agregue y se devuelva correctamente los datos
    HashingExtensible hash1("",DIRECCION);
    RegistroClave reg;
    int claveNum;
    char *dato;
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
    eliminar_archivo(DIRECCION);
    hash1.~HashingExtensible();
    reg.~RegistroVariable();
    clave.~ClaveX();
}

void TestHashingExtensible::test_crear_hashing_cerrarlo_y_abrirlo()
{
    //Probamos de guardar un registro en un archivo y ver si al abrirlo sigue estando
    HashingExtensible hash1("",DIRECCION);
    ClaveX clave;
    char* campoRecuperado;
    clave.set_clave(23);
    std::string campo = "Hola";
    crear_registro_y_agregar(&hash1, campo, clave);
    hash1.~HashingExtensible();

    HashingExtensible hash2("",DIRECCION);
    recuperar_dato_registro(campoRecuperado, clave, hash2);

    assert (strcmp(campoRecuperado,campo.c_str()) == 0);
    delete[] campoRecuperado;

    print_test_ok("test_crear_hashing_cerrarlo_y_abrirlo");

    eliminar_archivo(DIRECCION);
    hash2.~HashingExtensible();
    clave.~ClaveX();
}

void TestHashingExtensible::test_agregar_reg_y_duplicar_tabla()
{
    //Probamos llenando el primer bloque y vemos si se duplica la tabla, si se crean solo dos bloques
	// TODO
    print_test_ok("test_agregar_reg_y_duplicar_tabla");
}

void TestHashingExtensible::test_agregar_varios_registros_y_devolver()
{
    //Probamos que se agregue y se devuelva correctamente los datos
    HashingExtensible hash1("",DIRECCION);
    RegistroClave reg;
    int claveNum;
    char *dato;
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
    eliminar_archivo(DIRECCION);
    hash1.~HashingExtensible();
    reg.~RegistroVariable();
    clave.~ClaveX();
}

void TestHashingExtensible::ejecutar()
{
	test_agregar_reg_y_duplicar_tabla();
	test_eliminar_registro();
	test_crear_hashing_cerrarlo_y_abrirlo();
	test_agregar_y_devolver_registro();
	test_crear_hashing();
	test_agregar_varios_registros_y_devolver();
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
    campoRecuperado = new char[reg.get_tamanio_campo(0) +1]();
    reg.recuperar_campo(campoRecuperado,0);
}
