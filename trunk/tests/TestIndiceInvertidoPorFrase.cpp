#include "../src/CapaLogica/Indices/IndiceInvertido.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

#define NOMBRE_INDICE 			"IndicePorFrase"
#define ARCHIVO_ARBOL			"IndicePorFraseVocabulario.dat"
#define	 ARCHIVO_LISTAS			"ListasInvertidas"
#define	 ARCHIVO_LISTAS_POS		"ListasPosiciones"
#define ARCHIVO_TERMINOS		"IndicePorFraseTerminos.dat"
#define ARCHIVO_TEMPORAL		"archivo_terminos_canciones"

// To use a test fixture, derive a class from testing::Test.
class TestIndiceInvertidoPorFrase : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	IndiceInvertido indice;
	ManejadorRegistrosVariables terminos;
	ArchivoListas listasInvertidas, listasPos;
    ArbolBMas vocabulario;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	ASSERT_TRUE(indice.crear_indice("", NOMBRE_INDICE) == RES_OK);
	ASSERT_TRUE(indice.abrir_indice("", NOMBRE_INDICE) == RES_OK);
	ASSERT_TRUE(listasInvertidas.abrir("",ARCHIVO_LISTAS) == RES_OK);
	ASSERT_TRUE(listasPos.abrir("",ARCHIVO_LISTAS_POS) == RES_OK);
	ASSERT_TRUE(vocabulario.abrir(ARCHIVO_ARBOL, "rb+") == RES_OK);
	ASSERT_TRUE(terminos.abrir_archivo(ARCHIVO_TERMINOS) == RES_OK);
  }

  virtual void TearDown() {
	ASSERT_TRUE(indice.cerrar_indice() == RES_OK);
	ASSERT_TRUE(vocabulario.cerrar() == RES_OK);
	ASSERT_TRUE(indice.borrar_indice() == RES_OK);
  }

  // A helper function
  // Crea un reg cancion con la letra que nos pasan por parametro
  void crear_reg_cancion(std::string letra, RegistroCancion &reg)
  {
  	std::string cancion = "Bersuit-2013-Un pacto para vivir-spanish\n"+letra+'\n';
  	ASSERT_TRUE(reg.cargar(cancion.c_str(), cancion.length()) == RES_OK);
  }

};


TEST_F(TestIndiceInvertidoPorFrase,Agregar_cancion)
{
    RegistroCancion cancion;
    RegistroVariable lista, lista1, lista2, termino;
    RegistroClave listaCan1, listaCan2;
    RegistroClave reg_termino;
    ClaveX clave, clave_aux;
    char* campo = new char[100]();
    int ref_lista, pos;

    this->crear_reg_cancion("casa lago", cancion);
    ASSERT_TRUE (indice.agregar_texto(cancion.get_letra(), 23) == RES_OK);

    //Veo archivo de terminos

    //Veo que guarde los dos terminos de la letra
    ASSERT_TRUE(terminos.get_cantidad_registros_ocupados() == 2);
    //Veo que guarde como primer termino casa
    ASSERT_TRUE(terminos.get_registro_ocupado(&termino, 0) >= 0);

    termino.recuperar_campo(campo, 0);
    ASSERT_TRUE(strcmp(campo, "casa") == 0);
    //Veo que guarde como segundo termino lago
    ASSERT_TRUE(terminos.get_registro_ocupado(&termino, 1) >= 0);
    memset(campo,0,100);
    termino.recuperar_campo(campo, 0);
    ASSERT_TRUE(strcmp(campo, "lago") == 0);



    ASSERT_TRUE(listasPos.get_cantidad_listas() == 2);




    //Veo listas invertidas

    //Veo si se guardo 2 lista invertida ya que solo tenemos 2 terminos que se guardan en el indice
    ASSERT_TRUE(listasInvertidas.get_cantidad_listas() == 2);
    //Veo que cada lista guarde solo una lista que tiene como clave el IDcancion 23
    ASSERT_TRUE(listasInvertidas.devolver(&lista1,0) >= 0);
    ASSERT_TRUE(lista1.get_cantidad_campos() == 1);
    //Obtengo la primera cancion
    lista1.recuperar_campo(campo, 0);
    listaCan1.desempaquetar(campo);
    clave = listaCan1.get_clave();
    //Veo que guarde el IDcan 23
    clave_aux.set_clave(23);
    ASSERT_TRUE(clave == clave_aux);
    //Recupero  la referencia a la lista de posiciones y esta deberia ser la pos 0 del archivo de listas
    listaCan1.recuperar_campo(((char*)&ref_lista),1);
    ASSERT_TRUE(ref_lista == 0);
    //Hago lo mismo para la otra lista
    //Veo que cada lista guarde solo una lista que tiene como clave el IDcancion 23
     ASSERT_TRUE(listasInvertidas.devolver(&lista2,1) >= 0);
     ASSERT_TRUE(lista2.get_cantidad_campos() == 1);
     //Obtengo la primera cancion
     lista2.recuperar_campo(campo, 0);
     listaCan2.desempaquetar(campo);
     listaCan2.set_clave(clave);
     //Veo que guarde el IDcan 23
     clave_aux.set_clave(23);
     ASSERT_TRUE(clave == clave_aux);
     ref_lista = -1;
     //Recupero  la referencia a la lista de posiciones y esta deberia ser la pos 1 del archivo de listas
     listaCan2.recuperar_campo(((char*)&ref_lista),1);
      ASSERT_TRUE(ref_lista == 1);

     //Veo listas de posiciones

    //Veo si se guardo bien las lista de posiciones
     ASSERT_TRUE(listasPos.get_cantidad_listas() == 2);
    //La primera lista seria del primer termino entonces debe guardar la pos 0
    ASSERT_TRUE(listasPos.devolver(&lista,0) >= 0);
    ASSERT_TRUE(lista.get_cantidad_campos() == 1);
    //Recupero el ID que guarda la lista y este deberia ser 23
    lista.recuperar_campo((char*)&pos,0);
    ASSERT_TRUE(pos == 0);
    //La segunda lista seria del segundo termino entonces debe guardar la pos 1
    ASSERT_TRUE(listasPos.devolver(&lista,1) >= 0);
    ASSERT_TRUE(lista.get_cantidad_campos() == 1);
    //Recupero el ID que guarda la lista y este deberia ser 23
    lista.recuperar_campo((char*)&pos,0);
    ASSERT_TRUE(pos == 1);

    //Veo el vocabulario

    //Veo si se guardo los terminos en el arbol
    //Veo el primer termino de la letra
    clave.set_clave("casa");
    reg_termino.set_clave(clave);
    //Busco el registro en el arbol
    ASSERT_TRUE(vocabulario.buscar(reg_termino) == RES_OK);
    clave_aux.set_clave("casa");
    ASSERT_TRUE(reg_termino.get_clave() == clave_aux);
    ASSERT_TRUE(reg_termino.get_cantidad_campos() == 3);
    //Recupero  la referencia a la lista y esta deberia ser la pos 0 del archivo de listas
    reg_termino.recuperar_campo(((char*)&ref_lista),2);
    ASSERT_TRUE(ref_lista == 0);
    //Veo el segundo termino de la letra
    clave.set_clave("lago");
    reg_termino.set_clave(clave);
    //Busco el registro en el arbol
    ASSERT_TRUE(vocabulario.buscar(reg_termino) == RES_OK);
    clave_aux.set_clave("lago");
    ASSERT_TRUE(reg_termino.get_clave() == clave_aux);
    ASSERT_TRUE(reg_termino.get_cantidad_campos() == 3);
    //Recupero  la referencia a la lista y esta deberia ser la pos 1 del archivo de listas
    reg_termino.recuperar_campo(((char*)&ref_lista),2);
    ASSERT_TRUE(ref_lista == 1);

    ASSERT_TRUE(indice.borrar_indice() == RES_OK);
    delete[] campo;
}

TEST_F(TestIndiceInvertidoPorFrase,Devolver_canciones_con_1termino_1cancion)
{
	RegistroCancion cancion;
	RegistroVariable lista, termino;
	RegistroClave listaCan;
	RegistroClave reg_termino;
	ClaveX clave, clave_aux;
	int id;

	this->crear_reg_cancion("la casa del lago", cancion);
	indice.agregar_texto(cancion.get_letra(),23);
	//Abro los archivos del indice y veo si se crearon los registros correspondientes

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "lago"
	ASSERT_TRUE(indice.buscar_frase("lago", lista) == RES_OK);
	//Veo que solo tenga un campo
	ASSERT_TRUE(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	ASSERT_TRUE(id == 23);
	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "vaca" y deberia tirar NO_EXISTE
	int res = indice.buscar_frase("vaca", lista);
	ASSERT_TRUE(indice.buscar_frase("vaca", lista) == RES_RECORD_DOESNT_EXIST);
}

TEST_F(TestIndiceInvertidoPorFrase,Devolver_canciones_con_1termino_varias_canciones)
{
	RegistroCancion cancion;
	RegistroVariable lista, termino;
	RegistroClave listaCan;
	RegistroClave reg_termino;
	ClaveX clave, clave_aux;
	int id;

	this->crear_reg_cancion("la casa del tipo grande", cancion);
	indice.agregar_texto(cancion.get_letra(), 23);
	cancion.limpiar_campos();
	this->crear_reg_cancion("casa del lago", cancion);
	indice.agregar_texto(cancion.get_letra(), 24);
	cancion.limpiar_campos();
	this->crear_reg_cancion("una casa del", cancion);
	indice.agregar_texto(cancion.get_letra(), 25);

	//Abro los archivos del indice y veo si se crearon los registros correspondientes

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "lago"
	ASSERT_TRUE(indice.buscar_frase("lago", lista) == RES_OK);
	//Veo que solo tenga un campo
	ASSERT_TRUE(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	ASSERT_TRUE(id == 24);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "casa"
	lista.limpiar_campos();
	ASSERT_TRUE(indice.buscar_frase("casa del", lista) == RES_OK);
	//Veo que solo tenga un campo
	int res = lista.get_cantidad_campos();
	lista.recuperar_campo((char*)&id, 0);
	ASSERT_TRUE(lista.get_cantidad_campos() == 3);
	//recupero la primera cancion
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	ASSERT_TRUE(id == 23);
//	//recupero la segunda cancion
	lista.recuperar_campo((char*)&id, 1);
//	//Veo que sea el id correcto
	ASSERT_TRUE(id == 24);
	lista.recuperar_campo((char*)&id, 2);
//	//Veo que sea el id correcto
	ASSERT_TRUE(id == 25);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "vaca" y deberia tirar NO_EXISTE
	ASSERT_TRUE(indice.buscar_frase("vaca", lista) == RES_RECORD_DOESNT_EXIST);
}


TEST_F(TestIndiceInvertidoPorFrase,Devolver_canciones_con_una_cancion)
{
	RegistroCancion cancion;
	RegistroVariable lista, termino;
	RegistroClave listaCan;
	RegistroClave reg_termino;
	ClaveX clave, clave_aux;
	int id;

	this->crear_reg_cancion("La casa del lago", cancion);
	indice.agregar_texto(cancion.get_letra(), 23);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "la casa del lago"
	ASSERT_TRUE(indice.buscar_frase("la casa del lago", lista) == RES_OK);
	//Veo que solo tenga un campo
	ASSERT_TRUE(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	ASSERT_TRUE(id == 23);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "vaca" y deberia tirar NO_EXISTE
	ASSERT_TRUE(indice.buscar_frase("vaca loca", lista) == RES_RECORD_DOESNT_EXIST);

}

TEST_F(TestIndiceInvertidoPorFrase,Devolver_canciones_con_varias_canciones)
{
	RegistroCancion cancion;
	RegistroVariable lista, termino;
	RegistroClave listaCan;
	RegistroClave reg_termino;
	ClaveX clave, clave_aux;
	int id;

	this->crear_reg_cancion("La casa", cancion);
	indice.agregar_texto(cancion.get_letra(), 23);
	this->crear_reg_cancion("La casa del lago", cancion);
	indice.agregar_texto(cancion.get_letra(), 24);
	this->crear_reg_cancion("La granja", cancion);
	indice.agregar_texto(cancion.get_letra(), 25);

	indice.agregar_texto(cancion.get_letra(), 23);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "la casa del lago"
	ASSERT_TRUE(indice.buscar_frase("la casa del lago", lista) == RES_OK);
	//Veo que solo tenga un campo
	ASSERT_TRUE(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	ASSERT_TRUE(id == 24);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "la casa del lago"
	ASSERT_TRUE(indice.buscar_frase("la casa", lista) == RES_OK);
	//Veo que solo tenga dos campo
	ASSERT_TRUE(lista.get_cantidad_campos() == 2);
	//Veo la primera cancion
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	ASSERT_TRUE(id == 23);
	//Veo la segunda cancion
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	ASSERT_TRUE(id == 24);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "vaca" y deberia tirar NO_EXISTE
	ASSERT_TRUE(indice.buscar_frase("vaca loca", lista) == RES_RECORD_DOESNT_EXIST);

}

TEST_F(TestIndiceInvertidoPorFrase,Devolver_canciones_con_una_cancion_terminos_repetidos)
{
	RegistroCancion cancion;
	RegistroVariable lista, termino;
	RegistroClave listaCan;
	RegistroClave reg_termino;
	ClaveX clave, clave_aux;
	int id;

	this->crear_reg_cancion("La casa junto a la casa oscura", cancion);
	indice.agregar_texto(cancion.get_letra(), 23);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "la casa del lago"
	ASSERT_TRUE(indice.buscar_frase("la casa oscura", lista) == RES_OK);
	//Veo que solo tenga un campo
	ASSERT_TRUE(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	ASSERT_TRUE(id == 23);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "vaca" y deberia tirar NO_EXISTE
	ASSERT_TRUE(indice.buscar_frase("vaca loca", lista) == RES_RECORD_DOESNT_EXIST);

}

TEST_F(TestIndiceInvertidoPorFrase,Devolver_canciones_con_varias_canciones_terminos_repetidos)
{
	RegistroCancion cancion;
	RegistroVariable lista, termino;
	RegistroClave listaCan;
	RegistroClave reg_termino;
	ClaveX clave, clave_aux;
	int id;


	this->crear_reg_cancion("La casa junto a la casa oscura", cancion);
	indice.agregar_texto(cancion.get_letra(), 23);
	this->crear_reg_cancion("La casa del lago del sur", cancion);
	indice.agregar_texto(cancion.get_letra(), 24);
	this->crear_reg_cancion("La granja del lago", cancion);
	indice.agregar_texto(cancion.get_letra(), 25);

	indice.agregar_texto(cancion.get_letra(), 23);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "la casa del lago"
	ASSERT_TRUE(indice.buscar_frase("la casa del lago", lista) == RES_OK);
	//Veo que solo tenga un campo
	ASSERT_TRUE(lista.get_cantidad_campos() == 1);
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	ASSERT_TRUE(id == 24);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "la casa del lago"
	ASSERT_TRUE(indice.buscar_frase("la casa", lista) == RES_OK);
	//Veo que solo tenga dos campo
	ASSERT_TRUE(lista.get_cantidad_campos() == 2);
	//Veo la primera cancion
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	ASSERT_TRUE(id == 23);
	//Veo la segunda cancion
	lista.recuperar_campo((char*)&id, 0);
	//Veo que sea el id correcto
	ASSERT_TRUE(id == 24);

	//Le pido al indice que me devuelva la lista de canciones que guarda la frase "vaca" y deberia tirar NO_EXISTE
	ASSERT_TRUE(indice.buscar_frase("vaca loca", lista) == RES_RECORD_DOESNT_EXIST);

}
