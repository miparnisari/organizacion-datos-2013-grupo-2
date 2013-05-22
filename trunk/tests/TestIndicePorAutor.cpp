#include "TestIndicePorAutor.h"

TestIndicePorAutor::TestIndicePorAutor()
{

}

TestIndicePorAutor::~TestIndicePorAutor()
{

}

void TestIndicePorAutor::ejecutar()
{
    test_crear_indice();
    test_agregar_cancion();
    test_devolver_canciones_por_autor();
    test_borrar_indice();
    test_agregar_muchas_canciones();
    test_devolver_muchas_canciones_por_autor();
}

void TestIndicePorAutor::test_crear_indice()
{
    IndiceInvertidoPorAutor indice;
    ArchivoListas listas;
    ArbolBMas arbol;
    indice.crear_indice("");
    //Vemos si se creo el archivo de listas
    assert(listas.abrir("","ListasPorAutor.dat") == RES_OK);
    //Vemos si se creo el arbol
    assert(arbol.abrir("IndicePorAutor.dat", "rb+") == RES_OK);

    this->eliminar_archivos();
    print_test_ok("test_crear_indice");
}

void TestIndicePorAutor::test_agregar_cancion()
{
    IndiceInvertidoPorAutor indice;
    ArchivoListas listas;
    ArbolBMas arbol;
    RegistroCancion cancion;
    RegistroVariable lista;
    RegistroClave reg_autor;
    ClaveX clave;
    int id, ref_lista;
    indice.crear_indice("");
    indice.abrir_indice("");
    this->crear_reg_cancion("Pink Floyd", cancion);
    indice.agregar_cancion(cancion, 23);
    //Abro los archivos del indice y veo si se crearon los registros correspondientes
    assert(listas.abrir("","ListasPorAutor.dat") == RES_OK);
    assert(arbol.abrir("IndicePorAutor.dat", "rb+") == RES_OK);
    //Veo si se guardo una lista con el IDcancion 23
    assert(listas.get_cantidad_listas() == 1);
    assert(listas.devolver(&lista,0) == RES_OK);
    assert(lista.get_cantidad_campos() == 1);
    //Recupero el ID que guarda la lista y este deberia ser 23
    lista.recuperar_campo((char*)&id,0);
    assert(id == 23);

    //Veo si se guardo el registro del autor en el arbol
   /*** assert(arbol.buscar(reg_autor) == RES_OK)   //deberia obtener el reg del autor*****/
    clave.set_clave("Pink Floyd");
    assert(reg_autor.get_clave() == clave);
    assert(reg_autor.get_cantidad_campos() == 1);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
    reg_autor.recuperar_campo((char*)&ref_lista),0);
    assert(ref_lista == 0);

    this->eliminar_archivos();
    print_test_ok("test_agregar_cancion");
}

void TestIndicePorAutor::test_devolver_canciones_por_autor()
{
    this->eliminar_archivos();
    print_test_ok("test_devolver_canciones_por_autor");
}

void TestIndicePorAutor::test_borrar_indice()
{
    this->eliminar_archivos();
    print_test_ok("test_borrar_indice");
}

void TestIndicePorAutor::test_agregar_muchas_canciones()
{
    this->eliminar_archivos();
    print_test_ok("test_agregar_muchas_canciones");
}

void TestIndicePorAutor::test_devolver_muchas_canciones_por_autor()
{
    this->eliminar_archivos();
    print_test_ok("test_devolver_muchas_canciones_por_autor");
}

void TestIndicePorAutor::eliminar_archivos()
{
    //Elimina los archivos que se usan en cada archivo
    ManejadorArchivos manejador;
    manejador.eliminar_archivo("ListasPorAutor.dat");
    manejador.eliminar_archivo("IndicePorAutor.dat");
}

void TestIndicePorAutor::crear_reg_cancion(char* autor, RegistroCancion &reg)
{
    //Crea un reg cancion con el autor que nos pasan por parametro
    char *cancion = strcpy(autor,"-2013-The wall-Ingles-Pink Floyd");
    reg.cargar(cancion, strlen(cancion));
}
