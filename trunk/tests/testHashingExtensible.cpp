#include "CapaLogica/HashingExtensible/HashingExtensible.h"
#include "CapaFisica/ManejadorArchivos.h"
#include "CapaFisica/RegistroVariable.h"

#include <cassert>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <climits>

#define IMPRIMIR_VARIABLE(var)\
	std::cout<<#var<<" = "<<var<<'\n';

#define DIRECCION "HashingDePrueba.txt"

/***Revisar todo una vez que tengamos el registro clave correcto*/

using namespace std;
using namespace utilitarios;

void print_test_ok1(std::string nombreTest)
{
	std::cout << "OK: " << nombreTest << std::endl;
}

void eliminar_archivo(std::string direccion)
{
    ManejadorArchivos manejador;
    manejador.eliminar_archivo(direccion);
}

void crear_hashing()
{
    //Probamos que se cree la tabla y el primer bloque de registros cuando se cree el hashing
    HashingExtensible hash1("",DIRECCION);
    ManejadorBloques manejador_bloques;
    manejador_bloques.abrir_archivo(DIRECCION, "rb");
    assert (manejador_bloques.get_cantidad_bloques() == 2);
    //Ya que el primer bloque es la tabla y el segundo es el primer bloque de registros

    print_test_ok1("crear_hashing");
    eliminar_archivo(DIRECCION);
    hash1.~HashingExtensible();
    manejador_bloques.~ManejadorArchivos();
}

void eliminar_registro()
{
    //Probamos eliminar un registro y ver si nos devuelve algo
    HashingExtensible hash1("",DIRECCION);
    RegistroClave reg, reg2;
    ClaveX clave; //esta clave debe ser la del reg***********************
    //*************bueno aca deberia crear el registro con la clave y agregarla al hash**************
    //el reg debe tener clave 23
    hash1.agregar(reg);
    hash1.eliminar(clave);
    assert(hash1.devolver(clave, &reg2) == NO_EXISTE);

    print_test_ok1("eliminar_registro");
    eliminar_archivo(DIRECCION);
    hash1.~HashingExtensible();
    reg.~RegistroVariable();
    reg2.~RegistroVariable();
    clave.~ClaveX();
}

void agregar_y_devolver_registro()
{
    //probamos que se agregue y se devuelva correctamente los datos
    HashingExtensible hash1("",DIRECCION);
    RegistroClave reg, reg2;
    ClaveX clave; //esta clave debe ser la del reg***********************
    //*************bueno aca deberia crear el registro con la clave y agregarla al hash**************
    //el reg debe tener clave 23 y dato "texto"
    hash1.agregar(reg);
    assert(hash1.devolver(clave, &reg2) == RES_OK);
   /* assert(reg2.clave() == 23);
    assert(strcmp(re2.dato(), "texto"));

    *****desbloquear cuando tenga el registro correspondiente*/

    print_test_ok1("agregar_y_devolver_registro");
    eliminar_archivo(DIRECCION);
    hash1.~HashingExtensible();
    reg.~RegistroVariable();
    reg2.~RegistroVariable();
    clave.~ClaveX();
}

void crear_hashing_cerrarlo_y_abrirlo()
{
    //Probamos de guardar un registro en un archivo y ver si al abrirlo sigue estando
    HashingExtensible hash1("",DIRECCION);
    RegistroClave reg, reg2;
    ClaveX clave; //esta clave debe ser la del reg*****************
    char* campoRecuperado;
    const char* copia;
    std::string campo = "Hola";
    reg.agregar_campo(campo.c_str(), strlen(campo.c_str()));
    /**En realidad deberia estar el registro que corresponda con la clave 23*/
    hash1.agregar(reg);
    hash1.~HashingExtensible();

    HashingExtensible hash2("",DIRECCION);
    hash2.devolver(clave,&reg2);
    campoRecuperado = new char[reg2.get_tamanio_campo(0) +1]();
    reg2.recuperar_campo(campoRecuperado,0);

    copia = campoRecuperado;
    assert (strcmp(copia,campo.c_str()) == 0);
    delete campoRecuperado;

    print_test_ok1("crear_hashing_cerrarlo_y_abrirlo");

    eliminar_archivo(DIRECCION);
    hash2.~HashingExtensible();
    reg.~RegistroVariable();
    reg2.~RegistroVariable();
    clave.~ClaveX();
}

void agregar_reg_y_duplicar_tabla()
{
    //Probamos llenando el primer bloque y vemos si se duplica la tabla, si se crean solo dos bloques
    print_test_ok1("agregar_reg_y_duplicar_tabla");
}
