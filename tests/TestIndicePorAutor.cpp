#include "TestIndicePorAutor.h"
#define ARCHIVO_ARBOL	"IndicePorAutor.dat"
#define	ARCHIVO_LISTAS	"ListasPorAutor.dat"

TestIndicePorAutor::TestIndicePorAutor()
{

}

TestIndicePorAutor::~TestIndicePorAutor()
{

}

void TestIndicePorAutor::ejecutar()
{
    test_indice_autor_crear_indice();
    test_indice_autor_agregar_cancion();
    test_indice_autor_devolver_canciones_por_autor();
    test_indice_autor_borrar_indice();
    test_indice_autor_agregar_muchas_canciones();
    test_indice_autor_devolver_muchas_canciones_por_autor();
}

void TestIndicePorAutor::test_indice_autor_crear_indice()
{
    IndiceInvertidoPorAutor indice;
    ArchivoListas listas;
    ArbolBMas arbol;
    indice.crear_indice("");
    //Vemos si se creo el archivo de listas
    assert(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
    //Vemos si se creo el arbol
    assert(arbol.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);

    this->eliminar_archivos();
    print_test_ok("test_indice_autor_crear_indice");
}

void TestIndicePorAutor::test_indice_autor_agregar_cancion()
{
    IndiceInvertidoPorAutor indice;
    ArchivoListas listas;
    ArbolBMas arbol;
    RegistroCancion cancion;
    RegistroVariable lista;
    RegistroClave reg_autor;
    ClaveX clave, clave_aux;
    int id, ref_lista;
    indice.crear_indice("");
    indice.abrir_indice("");
    this->crear_reg_cancion("Pink Floyd", cancion);
    indice.agregar_cancion(cancion, 23);
    //Abro los archivos del indice y veo si se crearon los registros correspondientes
    assert(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
    assert(arbol.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
    //Veo si se guardo una lista con el IDcancion 23
    assert(listas.get_cantidad_listas() == 1);
    assert(listas.devolver(&lista,0) == RES_OK);
    assert(lista.get_cantidad_campos() == 1);
    //Recupero el ID que guarda la lista y este deberia ser 23
    lista.recuperar_campo((char*)&id,0);
    assert(id == 23);

    //Veo si se guardo el registro del autor en el arbol
    clave.set_clave("Pink Floyd");
    reg_autor.set_clave(clave);
    //Busco el registro en el arbol
    assert(arbol.buscar(reg_autor) == RES_OK);
    clave_aux.set_clave("Pink Floyd");
    assert(reg_autor.get_clave() == clave_aux);
    assert(reg_autor.get_cantidad_campos() == 1);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
    reg_autor.recuperar_campo(((char*)&ref_lista),0);
    assert(ref_lista == 0);

    this->eliminar_archivos();
    print_test_ok("test_indice_autor_agregar_cancion");
}

void TestIndicePorAutor::test_indice_autor_devolver_canciones_por_autor()
{
	IndiceInvertidoPorAutor indice;
    ArchivoListas listas;
    ArbolBMas arbol;
    RegistroCancion cancion;
    RegistroVariable lista;
    RegistroClave reg_autor;
    ClaveX clave;
    int id;
    indice.crear_indice("");
    indice.abrir_indice("");
    this->crear_reg_cancion("Pink Floyd", cancion);
	indice.agregar_cancion(cancion, 23);
	//Veo si se crearon los archivos del indice
	assert(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
	assert(arbol.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);

	//Le pido al indice que me devuelva la lista de canciones de Pink Floyd
	assert(indice.buscar_autor("Pink Floyd", lista) == RES_OK);
	//Veo que solo tenga un campo
	assert(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	assert(id == 23);
    this->eliminar_archivos();
    print_test_ok("test_indice_autor_devolver_canciones_por_autor");
}

void TestIndicePorAutor::test_indice_autor_borrar_indice()
{
	IndiceInvertidoPorAutor indice;
	ArbolBMas arbol;
	ArchivoListas listas;
	indice.crear_indice("");
	//Veo si se crearon los archivos del indice
	assert(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
	assert(arbol.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
	//Elimino el indice
	indice.borrar_indice();
	//Veo si se eliminaron los archivos del indice
	assert(listas.abrir("",ARCHIVO_LISTAS) != RES_OK);
	assert(arbol.abrir(ARCHIVO_ARBOL, "rb+") != RES_OK);

    this->eliminar_archivos();
    print_test_ok("test_indice_autor_borrar_indice");
}

void TestIndicePorAutor::test_indice_autor_agregar_muchas_canciones()
{
	IndiceInvertidoPorAutor indice;
    ArchivoListas listas;
    ArbolBMas arbol;
    RegistroCancion cancion;
    RegistroVariable lista1, lista2;
    RegistroClave reg_autor;
    ClaveX clave;
    int ref_lista;
    indice.crear_indice("");
    indice.abrir_indice("");

    //Agrego tres canciones con el mismo autor y uno distinto
    this->crear_reg_cancion("Pink Floyd", cancion);
    indice.agregar_cancion(cancion, 23);
    this->crear_reg_cancion("Pink Floyd", cancion);
	indice.agregar_cancion(cancion, 24);
    this->crear_reg_cancion("Queen", cancion);
    indice.agregar_cancion(cancion, 25);
    this->crear_reg_cancion("Pink Floyd", cancion);
    indice.agregar_cancion(cancion, 26);

   	//Abro los archivos del indice y veo si se crearon los registros correspondientes
    assert(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
    assert(arbol.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
    //Veo si se guardaron 2 listas, ya que solo tenemos 2 autores
    assert(listas.get_cantidad_listas() == 2);
    assert(listas.devolver(&lista1,0) == RES_OK);
    assert(listas.devolver(&lista2,0) == RES_OK);
    //La suma de las dos listas deberia dar la cantidad de registros almacenados
    assert((lista1.get_cantidad_campos()+lista2.get_cantidad_campos()) == 4);

	//Veo si se guardo el registro del autor Pink Floyd en el arbol
    clave.set_clave("Pink Floyd");
    reg_autor.set_clave(clave);
    //Busco el registro en el arbol
    assert(arbol.buscar(reg_autor) == RES_OK);
    clave.set_clave("Pink Floyd");
    assert(reg_autor.get_clave() == clave);
    assert(reg_autor.get_cantidad_campos() == 1);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
	reg_autor.recuperar_campo(((char*)&ref_lista),0);
	//Veo que guarde 3 canciones
	assert(listas.devolver(&lista1,ref_lista) == RES_OK);
	assert(lista1.get_cantidad_campos() == 3);

    //Veo si se guardo el registro del autor Queen en el arbol
    clave.set_clave("Queen");
    reg_autor.set_clave(clave);
    //Busco el registro en el arbol
    assert(arbol.buscar(reg_autor) == RES_OK);
    clave.set_clave("Queen");
    assert(reg_autor.get_clave() == clave);
    assert(reg_autor.get_cantidad_campos() == 1);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
    reg_autor.recuperar_campo(((char*)&ref_lista),0);
    //Veo que guarde 1 cancion
    assert(listas.devolver(&lista2,ref_lista) == RES_OK);
    assert(lista2.get_cantidad_campos() == 1);

    this->eliminar_archivos();
    print_test_ok("test_aindice_autor_gregar_muchas_canciones");
}

void TestIndicePorAutor::test_indice_autor_devolver_muchas_canciones_por_autor()
{
	IndiceInvertidoPorAutor indice;
    ArchivoListas listas;
    ArbolBMas arbol;
    RegistroCancion cancion;
    RegistroVariable lista1, lista2;
    RegistroClave reg_autor;
    ClaveX clave;
    int id;
    indice.crear_indice("");
    indice.abrir_indice("");

    //Agrego tres canciones con el mismo autor y uno distinto
    this->crear_reg_cancion("Pink Floyd", cancion);
    indice.agregar_cancion(cancion, 23);
    this->crear_reg_cancion("Pink Floyd", cancion);
	indice.agregar_cancion(cancion, 24);
    this->crear_reg_cancion("Queen", cancion);
    indice.agregar_cancion(cancion, 25);
    this->crear_reg_cancion("Pink Floyd", cancion);
    indice.agregar_cancion(cancion, 26);

    //Abro los archivos del indice y veo si se crearon los registros correspondientes
    assert(listas.abrir("",ARCHIVO_LISTAS) == RES_OK);
    assert(arbol.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);

    //Veo que me devuelvan las listas por cada autor con los IDcanciones correspondientes
    assert(indice.buscar_autor("Pink Floyd", lista1));
    assert(lista1.get_cantidad_campos() == 3);
    lista1.recuperar_campo((char*)&id, 0);
    assert(id == 23);
    lista1.recuperar_campo((char*)&id, 1);
    assert(id == 24);
    lista1.recuperar_campo((char*)&id, 2);
    assert(id == 26);

    assert(indice.buscar_autor("Queen", lista2));
    assert(lista2.get_cantidad_campos() == 3);
    lista2.recuperar_campo((char*)&id, 0);
    assert(id == 25);

    this->eliminar_archivos();
    print_test_ok("test_indice_autor_devolver_muchas_canciones_por_autor");
}

void TestIndicePorAutor::eliminar_archivos()
{
    //Elimina los archivos que se usan en cada archivo
    ManejadorArchivos manejador;
    ArbolBMas arbol;
    manejador.eliminar_archivo(ARCHIVO_LISTAS);
    arbol.eliminar(ARCHIVO_ARBOL);
}

void TestIndicePorAutor::crear_reg_cancion(const char *autor, RegistroCancion &reg)
{
    //Crea un reg cancion con el autor que nos pasan por parametro
	std::string cancion = autor+"-2013-The wall-Ingles-Pink Floyd";
    reg.cargar(cancion.c_str(), cancion.length());
}
