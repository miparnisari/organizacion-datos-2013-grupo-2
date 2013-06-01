/*
 * TestIndiceInvertidoPorFrase.cpp
 *
 *  Created on: 25/05/2013
 *      Author: nadia
 */

#include "TestIndiceInvertidoPorFrase.h"

#define NOMBRE_INDICE 			"IndicePorFrase"
#define ARCHIVO_ARBOL			"IndicePorFraseVocabulario.dat"
#define	 ARCHIVO_LISTAS			"ListasInvertidas"
#define	 ARCHIVO_LISTAS_POS		"ListasPosiciones"
#define ARCHIVO_TERMINOS		"IndicePorFraseTerminos.dat"
#define ARCHIVO_TEMPORAL		"archivo_terminos_canciones"

TestIndiceInvertidoPorFrase::TestIndiceInvertidoPorFrase()
{

}

TestIndiceInvertidoPorFrase::~TestIndiceInvertidoPorFrase()
{

}

void TestIndiceInvertidoPorFrase::ejecutar()
{
    test_indice_por_frase_crear_indice();
    test_indice_por_frase_agregar_cancion();
    test_indice_por_frase_devolver_canciones_con_1termino_1cancion();
    test_indice_por_frase_devolver_canciones_con_1termino_varias_canciones();
    test_indice_por_frase_devolver_canciones_con_una_cancion();
    test_indice_por_frase_devolver_canciones_con_varias_canciones();
    test_indice_por_frase_devolver_canciones_con_una_cancion_terminos_repetidos();
    test_indice_por_frase_devolver_canciones_con_varias_canciones_terminos_repetidos();
    test_indice_por_frase_borrar_indice();
}

void TestIndiceInvertidoPorFrase::test_indice_por_frase_crear_indice()
{
	IndiceInvertido indice;
	ManejadorRegistrosVariables terminos;
    ArchivoListas listasInvertidas, listasPos;
    ArbolBMas vocabulario;
    assert(indice.crear_indice("", NOMBRE_INDICE) == RES_OK);
    //Vemos si se creo el archivo de listas invertidas
    assert(listasInvertidas.abrir("",ARCHIVO_LISTAS) == RES_OK);
    //Vemos si se creo el archivo de listas invertidas
    assert(listasPos.abrir("",ARCHIVO_LISTAS_POS) == RES_OK);
    //Vemos si se creo el arbol
    assert(vocabulario.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
    //Vemos si se creo el archivo de terminos
    assert(terminos.abrir_archivo(ARCHIVO_TERMINOS) == RES_OK);

    this->eliminar_archivos();
    print_test_ok("test_indice_por_frase_crear_indice");
}

void TestIndiceInvertidoPorFrase::test_indice_por_frase_agregar_cancion()
{
	IndiceInvertido indice;
	ManejadorRegistrosVariables terminos;
	ArchivoListas listasInvertidas, listasPos;
	ArbolBMas vocabulario;
    RegistroCancion cancion;
    RegistroVariable lista, termino;
    RegistroClave listaCan;
    RegistroClave reg_termino;
    ClaveX clave, clave_aux;
    char* campo = new char[100];
    int ref_lista, pos;


    indice.crear_indice("", NOMBRE_INDICE);
//    indice.abrir_indice("", NOMBRE_INDICE);
    this->crear_reg_cancion("casa lago", cancion);
    assert (indice.agregar_texto(cancion.get_letra(), 23) == RES_OK);
    indice.cerrar_indice();
    //Abro los archivos del indice y veo si se crearon los registros correspondientes
    //Vemos si se creo el archivo de listas invertidas
    assert(listasInvertidas.abrir("",ARCHIVO_LISTAS) == RES_OK);
    //Vemos si se creo el archivo de listas invertidas
    assert(listasPos.abrir("",ARCHIVO_LISTAS_POS) == RES_OK);
    //Vemos si se creo el arbol
    assert(vocabulario.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
    //Vemos si se creo el archivo de terminos
    assert(terminos.abrir_archivo(ARCHIVO_TERMINOS) == RES_OK);

    //Veo archivo de terminos

    //Veo que guarde los dos terminos de la letra
    assert(terminos.get_cantidad_registros_ocupados() == 2);
    //Veo que guarde como primer termino casa
    assert(terminos.get_registro_ocupado(&termino, 0) == RES_OK);
    termino.recuperar_campo(campo, 0);
    assert(strcmp(campo, "casa"));
    //Veo que guarde como segundo termino lago
    assert(terminos.get_registro_ocupado(&termino, 0) == RES_OK);
    termino.recuperar_campo(campo, 0);
    assert(strcmp(campo, "lago"));

    //Veo listas invertidas

    //Veo si se guardo 2 lista invertida ya que solo tenemos 2 terminos que se guardan en el indice
    assert(listasInvertidas.get_cantidad_listas() == 2);
    //Veo que cada lista guarde solo una lista que tiene como clave el IDcancion 23
    assert(listasInvertidas.devolver(&lista,0) == RES_OK);
    assert(lista.get_cantidad_campos() == 1);
    //Obtengo la primera cancion
    lista.recuperar_campo(campo, 0);
    listaCan.desempaquetar(campo);
    listaCan.set_clave(clave);
    //Veo que guarde el IDcan 23
    clave_aux.set_clave(23);
    assert(clave == clave_aux);
    //Recupero  la referencia a la lista de posiciones y esta deberia ser la pos 0 del archivo de listas
    listaCan.recuperar_campo(((char*)&ref_lista),1);
    assert(ref_lista == 0);
    //Hago lo mismo para la otra lista
    //Veo que cada lista guarde solo una lista que tiene como clave el IDcancion 23
     assert(listasInvertidas.devolver(&lista,1) == RES_OK);
     assert(lista.get_cantidad_campos() == 1);
     //Obtengo la primera cancion
     lista.recuperar_campo(campo, 0);
     listaCan.desempaquetar(campo);
     listaCan.set_clave(clave);
     //Veo que guarde el IDcan 23
     clave_aux.set_clave(23);
     assert(clave == clave_aux);
     //Recupero  la referencia a la lista de posiciones y esta deberia ser la pos 1 del archivo de listas
     listaCan.recuperar_campo(((char*)&ref_lista),1);
      assert(ref_lista == 1);

     //Veo listas de posiciones

    //Veo si se guardo bien las lista de posiciones
     assert(listasPos.get_cantidad_listas() == 2);
    //La primera lista seria del primer termino entonces debe guardar la pos 0
    assert(listasPos.devolver(&lista,0) == RES_OK);
    assert(lista.get_cantidad_campos() == 1);
    //Recupero el ID que guarda la lista y este deberia ser 23
    lista.recuperar_campo((char*)&pos,0);
    assert(pos == 0);
    //La segunda lista seria del segundo termino entonces debe guardar la pos 1
    assert(listasPos.devolver(&lista,1) == RES_OK);
    assert(lista.get_cantidad_campos() == 1);
    //Recupero el ID que guarda la lista y este deberia ser 23
    lista.recuperar_campo((char*)&pos,0);
    assert(pos == 1);

    //Veo el vocabulario

    //Veo si se guardo los terminos en el arbol
    //Veo el primer termino de la letra
    clave.set_clave("casa");
    reg_termino.set_clave(clave);
    //Busco el registro en el arbol
    assert(vocabulario.buscar(reg_termino) == RES_OK);
    clave_aux.set_clave("casa");
    assert(reg_termino.get_clave() == clave_aux);
    assert(reg_termino.get_cantidad_campos() == 3);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
    reg_termino.recuperar_campo(((char*)&ref_lista),0);
    assert(ref_lista == 0);
    //Veo el segundo termino de la letra
    clave.set_clave("lago");
    reg_termino.set_clave(clave);
    //Busco el registro en el arbol
    assert(vocabulario.buscar(reg_termino) == RES_OK);
    clave_aux.set_clave("lago");
    assert(reg_termino.get_clave() == clave_aux);
    assert(reg_termino.get_cantidad_campos() == 3);
    //Recupero  la referencia a la lista y esta deberia ser la pos 1 del archivo de listas
    reg_termino.recuperar_campo(((char*)&ref_lista),0);
    assert(ref_lista == 1);

    this->eliminar_archivos();
    delete[] campo;
    print_test_ok("test_indice_por_frase_agregar_cancion");
}

void TestIndiceInvertidoPorFrase::test_indice_por_frase_devolver_canciones_con_1termino_1cancion()
{
	IndiceInvertido indice;
	ManejadorRegistrosVariables terminos;
	ArchivoListas listasInvertidas, listasPos;
	ArbolBMas vocabulario;
	RegistroCancion cancion;
	RegistroVariable lista, termino;
	RegistroClave listaCan;
	RegistroClave reg_termino;
	ClaveX clave, clave_aux;
	int id;
	indice.crear_indice("", NOMBRE_INDICE);
	indice.abrir_indice("", NOMBRE_INDICE);
	this->crear_reg_cancion("La casa del lago", cancion);
	indice.agregar_texto(cancion.get_letra(),23);
	//Abro los archivos del indice y veo si se crearon los registros correspondientes
	//Vemos si se creo el archivo de listas invertidas
	assert(listasInvertidas.abrir("",ARCHIVO_LISTAS) == RES_OK);
	//Vemos si se creo el archivo de listas invertidas
	assert(listasPos.abrir("",ARCHIVO_LISTAS_POS) == RES_OK);
	//Vemos si se creo el arbol
	assert(vocabulario.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
	//Vemos si se creo el archivo de terminos
	assert(terminos.abrir_archivo(ARCHIVO_TERMINOS) == RES_OK);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "lago"
	assert(indice.buscar_frase("lago", lista) == RES_OK);
	//Veo que solo tenga un campo
	assert(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	assert(id == 23);
	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "vaca" y deberia tirar NO_EXISTE
	assert(indice.buscar_frase("vaca", lista) == NO_EXISTE);

    this->eliminar_archivos();
    print_test_ok("test_indice_por_frase_devolver_canciones_con_1termino_1cancion");
}

void TestIndiceInvertidoPorFrase::test_indice_por_frase_devolver_canciones_con_1termino_varias_canciones()
{
	IndiceInvertido indice;
	ManejadorRegistrosVariables terminos;
	ArchivoListas listasInvertidas, listasPos;
	ArbolBMas vocabulario;
	RegistroCancion cancion;
	RegistroVariable lista, termino;
	RegistroClave listaCan;
	RegistroClave reg_termino;
	ClaveX clave, clave_aux;
	int id;
	indice.crear_indice("", NOMBRE_INDICE);
	indice.abrir_indice("", NOMBRE_INDICE);

	this->crear_reg_cancion("La casa", cancion);
	indice.agregar_texto(cancion.get_letra(), 23);
	this->crear_reg_cancion("La casa del lago", cancion);
	indice.agregar_texto(cancion.get_letra(), 24);
	this->crear_reg_cancion("La granja", cancion);
	indice.agregar_texto(cancion.get_letra(), 25);

	//Abro los archivos del indice y veo si se crearon los registros correspondientes
	//Vemos si se creo el archivo de listas invertidas
	assert(listasInvertidas.abrir("",ARCHIVO_LISTAS) == RES_OK);
	//Vemos si se creo el archivo de listas invertidas
	assert(listasPos.abrir("",ARCHIVO_LISTAS_POS) == RES_OK);
	//Vemos si se creo el arbol
	assert(vocabulario.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
	//Vemos si se creo el archivo de terminos
	assert(terminos.abrir_archivo(ARCHIVO_TERMINOS) == RES_OK);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "lago"
	assert(indice.buscar_frase("lago", lista) == RES_OK);
	//Veo que solo tenga un campo
	assert(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	assert(id == 23);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "casa"
	assert(indice.buscar_frase("casa", lista) == RES_OK);
	//Veo que solo tenga un campo
	assert(lista.get_cantidad_campos() == 2);
	//recupero la primera cancion
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	assert(id == 23);
	//recupero la segunda cancion
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	assert(id == 24);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "vaca" y deberia tirar NO_EXISTE
	assert(indice.buscar_frase("vaca", lista) == NO_EXISTE);

    this->eliminar_archivos();
    print_test_ok("test_indice_por_frase_devolver_canciones_con_1termino_varias_canciones");
}

void TestIndiceInvertidoPorFrase::test_indice_por_frase_devolver_canciones_con_una_cancion()
{
	IndiceInvertido indice;
	ManejadorRegistrosVariables terminos;
	ArchivoListas listasInvertidas, listasPos;
	ArbolBMas vocabulario;
	RegistroCancion cancion;
	RegistroVariable lista, termino;
	RegistroClave listaCan;
	RegistroClave reg_termino;
	ClaveX clave, clave_aux;
	int id;
	indice.crear_indice("", NOMBRE_INDICE);
	indice.abrir_indice("", NOMBRE_INDICE);
	this->crear_reg_cancion("La casa del lago", cancion);
	indice.agregar_texto(cancion.get_letra(), 23);
	//Abro los archivos del indice y veo si se crearon los registros correspondientes
	//Vemos si se creo el archivo de listas invertidas
	assert(listasInvertidas.abrir("",ARCHIVO_LISTAS) == RES_OK);
	//Vemos si se creo el archivo de listas invertidas
	assert(listasPos.abrir("",ARCHIVO_LISTAS_POS) == RES_OK);
	//Vemos si se creo el arbol
	assert(vocabulario.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
	//Vemos si se creo el archivo de terminos
	assert(terminos.abrir_archivo(ARCHIVO_TERMINOS) == RES_OK);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "la casa del lago"
	assert(indice.buscar_frase("la casa del lago", lista) == RES_OK);
	//Veo que solo tenga un campo
	assert(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	assert(id == 23);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "vaca" y deberia tirar NO_EXISTE
	assert(indice.buscar_frase("vaca loca", lista) == NO_EXISTE);

    this->eliminar_archivos();
    print_test_ok("test_indice_por_frase_devolver_canciones_con_una_cancion");
}

void TestIndiceInvertidoPorFrase::test_indice_por_frase_devolver_canciones_con_varias_canciones()
{
	IndiceInvertido indice;
	ManejadorRegistrosVariables terminos;
	ArchivoListas listasInvertidas, listasPos;
	ArbolBMas vocabulario;
	RegistroCancion cancion;
	RegistroVariable lista, termino;
	RegistroClave listaCan;
	RegistroClave reg_termino;
	ClaveX clave, clave_aux;
	int id;
	indice.crear_indice("", NOMBRE_INDICE);
	indice.abrir_indice("", NOMBRE_INDICE);

	this->crear_reg_cancion("La casa", cancion);
	indice.agregar_texto(cancion.get_letra(), 23);
	this->crear_reg_cancion("La casa del lago", cancion);
	indice.agregar_texto(cancion.get_letra(), 24);
	this->crear_reg_cancion("La granja", cancion);
	indice.agregar_texto(cancion.get_letra(), 25);

	indice.agregar_texto(cancion.get_letra(), 23);
	//Abro los archivos del indice y veo si se crearon los registros correspondientes
	//Vemos si se creo el archivo de listas invertidas
	assert(listasInvertidas.abrir("",ARCHIVO_LISTAS) == RES_OK);
	//Vemos si se creo el archivo de listas invertidas
	assert(listasPos.abrir("",ARCHIVO_LISTAS_POS) == RES_OK);
	//Vemos si se creo el arbol
	assert(vocabulario.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
	//Vemos si se creo el archivo de terminos
	assert(terminos.abrir_archivo(ARCHIVO_TERMINOS) == RES_OK);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "la casa del lago"
	assert(indice.buscar_frase("la casa del lago", lista) == RES_OK);
	//Veo que solo tenga un campo
	assert(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	assert(id == 24);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "la casa del lago"
	assert(indice.buscar_frase("la casa", lista) == RES_OK);
	//Veo que solo tenga dos campo
	assert(lista.get_cantidad_campos() == 2);
	//Veo la primera cancion
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	assert(id == 23);
	//Veo la segunda cancion
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	assert(id == 24);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "vaca" y deberia tirar NO_EXISTE
	assert(indice.buscar_frase("vaca loca", lista) == NO_EXISTE);

    this->eliminar_archivos();
    print_test_ok("test_indice_por_frase_devolver_canciones_con_varias_canciones");
}



void TestIndiceInvertidoPorFrase::test_indice_por_frase_devolver_canciones_con_una_cancion_terminos_repetidos()
{
		IndiceInvertido indice;
		ManejadorRegistrosVariables terminos;
		ArchivoListas listasInvertidas, listasPos;
		ArbolBMas vocabulario;
		RegistroCancion cancion;
		RegistroVariable lista, termino;
		RegistroClave listaCan;
		RegistroClave reg_termino;
		ClaveX clave, clave_aux;
		int id;
		indice.crear_indice("", NOMBRE_INDICE);
		indice.abrir_indice("", NOMBRE_INDICE);
		this->crear_reg_cancion("La casa junto a la casa oscura", cancion);
		indice.agregar_texto(cancion.get_letra(), 23);
		//Abro los archivos del indice y veo si se crearon los registros correspondientes
		//Vemos si se creo el archivo de listas invertidas
		assert(listasInvertidas.abrir("",ARCHIVO_LISTAS) == RES_OK);
		//Vemos si se creo el archivo de listas invertidas
		assert(listasPos.abrir("",ARCHIVO_LISTAS_POS) == RES_OK);
		//Vemos si se creo el arbol
		assert(vocabulario.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
		//Vemos si se creo el archivo de terminos
		assert(terminos.abrir_archivo(ARCHIVO_TERMINOS) == RES_OK);

		//Le pido al indice que me devuelva la lista de canciones que guarda la frase "la casa del lago"
		assert(indice.buscar_frase("la casa oscura", lista) == RES_OK);
		//Veo que solo tenga un campo
		assert(lista.get_cantidad_campos() == 1);
		lista.recuperar_campo((char*)&id, 0);
		//Veo que sea el id correcto
		assert(id == 23);

		//Le pido al indice que me devuelva la lista de canciones que guarda la frase "vaca" y deberia tirar NO_EXISTE
		assert(indice.buscar_frase("vaca loca", lista) == NO_EXISTE);

	    this->eliminar_archivos();
	    print_test_ok("test_indice_por_frase_devolver_canciones_con_una_cancion_terminos_repetidos");
}

void TestIndiceInvertidoPorFrase::test_indice_por_frase_devolver_canciones_con_varias_canciones_terminos_repetidos()
{
	IndiceInvertido indice;
	ManejadorRegistrosVariables terminos;
	ArchivoListas listasInvertidas, listasPos;
	ArbolBMas vocabulario;
	RegistroCancion cancion;
	RegistroVariable lista, termino;
	RegistroClave listaCan;
	RegistroClave reg_termino;
	ClaveX clave, clave_aux;
	int id;
	indice.crear_indice("", NOMBRE_INDICE);
	indice.abrir_indice("", NOMBRE_INDICE);

	this->crear_reg_cancion("La casa junto a la casa oscura", cancion);
	indice.agregar_texto(cancion.get_letra(), 23);
	this->crear_reg_cancion("La casa del lago del sur", cancion);
	indice.agregar_texto(cancion.get_letra(), 24);
	this->crear_reg_cancion("La granja del lago", cancion);
	indice.agregar_texto(cancion.get_letra(), 25);

	indice.agregar_texto(cancion.get_letra(), 23);
	//Abro los archivos del indice y veo si se crearon los registros correspondientes
	//Vemos si se creo el archivo de listas invertidas
	assert(listasInvertidas.abrir("",ARCHIVO_LISTAS) == RES_OK);
	//Vemos si se creo el archivo de listas invertidas
	assert(listasPos.abrir("",ARCHIVO_LISTAS_POS) == RES_OK);
	//Vemos si se creo el arbol
	assert(vocabulario.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
	//Vemos si se creo el archivo de terminos
	assert(terminos.abrir_archivo(ARCHIVO_TERMINOS) == RES_OK);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "la casa del lago"
	assert(indice.buscar_frase("la casa del lago", lista) == RES_OK);
	//Veo que solo tenga un campo
	assert(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	assert(id == 24);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "la casa del lago"
	assert(indice.buscar_frase("la casa", lista) == RES_OK);
	//Veo que solo tenga dos campo
	assert(lista.get_cantidad_campos() == 2);
	//Veo la primera cancion
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	assert(id == 23);
	//Veo la segunda cancion
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	assert(id == 24);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "vaca" y deberia tirar NO_EXISTE
	assert(indice.buscar_frase("vaca loca", lista) == NO_EXISTE);

    this->eliminar_archivos();
    print_test_ok("test_indice_por_frase_devolver_canciones_con_varias_canciones_terminos_repetidos");
}

void TestIndiceInvertidoPorFrase::test_indice_por_frase_borrar_indice()
{
	IndiceInvertido indice;
	ManejadorRegistrosVariables terminos;
	ArchivoListas listasInvertidas, listasPos;
    ArbolBMas vocabulario;
    indice.crear_indice("", NOMBRE_INDICE);
    //Vemos si se creo el archivo de listas invertidas
    assert(listasInvertidas.abrir("",ARCHIVO_LISTAS) == RES_OK);
    //Vemos si se creo el archivo de listas invertidas
    assert(listasPos.abrir("",ARCHIVO_LISTAS_POS) == RES_OK);
    //Vemos si se creo el arbol
    assert(vocabulario.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
    //Vemos si se creo el archivo de terminos
    assert(terminos.abrir_archivo(ARCHIVO_TERMINOS) == RES_OK);

    indice.borrar_indice();
    //Vemos si se haya eliminado el archivo de listas invertidas
    assert(listasInvertidas.abrir("",ARCHIVO_LISTAS) != RES_OK);
    //Vemos si se haya eliminado el archivo de listas invertidas
    assert(listasPos.abrir("",ARCHIVO_LISTAS_POS) != RES_OK);
    //Vemos si se haya eliminado el arbol
    assert(vocabulario.abrir(ARCHIVO_ARBOL, "rb+") != RES_OK);
    //Vemos si se haya eliminado el archivo de terminos
    assert(terminos.abrir_archivo(ARCHIVO_TERMINOS) != RES_OK);

    this->eliminar_archivos();
    print_test_ok("test_indice_por_frase_borrar_indice");
}

void TestIndiceInvertidoPorFrase::eliminar_archivos()
{
    //Elimina los archivos que se usan en cada archivo
    ManejadorArchivos manejador;
    ArbolBMas arbol;
    manejador.eliminar_archivo(ARCHIVO_LISTAS);
    arbol.eliminar(ARCHIVO_ARBOL);
}

void TestIndiceInvertidoPorFrase::crear_reg_cancion(std::string letra, RegistroCancion &reg)
{
    //Crea un reg cancion con el autor que nos pasan por parametro
	std::string cancion = "Bersuit-2013-Un pacto para vivir-Castellano-"+letra;
    reg.cargar(cancion.c_str(), cancion.length());
}


