/*
 * TestArbol.cpp
 *
 *  Created on: May 16, 2013
 *      Author: martin
 */

#include "TestArbol.h"

typedef NodoInterno::TipoHijo TipoHijo;

void TestArbol::ejecutar(){
	test_arbol_abrir_no_existente();
	test_arbol_abrir_cerrar();
	test_insertar_pocos_registros();
	test_arbol_insertar_un_registro();
	test_split_raiz();

	test_arbol_buscar();
	test_arbol_buscar_secuencial();
	test_eliminar_sin_underflow();
	test_eliminar_con_merge_secuenciales();
}

void TestArbol::test_arbol_insertar_un_registro()
{
	ArbolBMas arbol;
	assert (arbol.crear("arbol.dat",BLOQUE_TAM_DEFAULT) == RES_OK);
	assert (arbol.abrir("arbol.dat","rb+") == RES_OK);

	RegistroClave reg;
	ClaveX clave;
	clave.set_clave("u2 123");
	reg.set_clave(clave);

	std::string campo = "ID CANCION = 3";
	reg.agregar_campo(campo.c_str(),campo.size());

	assert (arbol.agregar(reg) == RES_OK);
	assert (arbol.cerrar() == RES_OK);

	assert (arbol.abrir("arbol.dat","rb+") == RES_OK);

	RegistroClave regLeido;
	regLeido.set_clave(clave);

	// Buscamos "u2 123" en el arbol
	unsigned int numBloque = -1;
	assert (arbol._buscar(regLeido, numBloque) != RES_RECORD_DOESNT_EXIST);
	assert (numBloque == 1);
	assert (regLeido.get_clave() == clave);

	char* buffer = new char[regLeido.get_tamanio_campo(1) + 1]();
	buffer[regLeido.get_tamanio_campo(1)] = '\0';
	regLeido.recuperar_campo(buffer,1);

	assert (strcmp(buffer,campo.c_str()) == 0);

	delete[] buffer;

	assert(arbol.cerrar() == RES_OK);

	print_test_ok("test_arbol_insertar_un_registro");
}

void TestArbol::test_arbol_abrir_no_existente()
{
	ArbolBMas arbol;
	assert ( arbol.abrir("nombreinvalido","rb+") == RES_INVALID_FILENAME);
	assert ( arbol.abrir("estearbolnoexiste.dat","rb+") == RES_ERROR);


	print_test_ok("test_arbol_abrir_no_existente");
}

void TestArbol::test_arbol_abrir_cerrar()
{
	ArbolBMas arbol;
	assert (arbol.crear("unArbol.dat",BLOQUE_TAM_DEFAULT) == RES_OK);
	assert (arbol.abrir("unArbol.dat","rb+") == RES_OK);
	assert (arbol.get_cant_minima_nodo() == sizeof(TipoHijo));

	assert (arbol.get_cant_maxima_nodo() == (int)(BLOQUE_TAM_DEFAULT * ArbolBMas::FACTOR_CARGA/100));
	assert (arbol.cerrar() == RES_OK);

	FILE* handler = fopen("unArbol.dat","rb+");
	fseek(handler,0,SEEK_END);
	int tamanio = ftell(handler);
	assert (tamanio == 3*sizeof(int)+2*BLOQUE_TAM_DEFAULT);

	assert (arbol.eliminar("unArbol.dat") == RES_OK);

	fclose(handler);

	print_test_ok("test_arbol_abrir_cerrar");
}


void TestArbol::test_insertar_pocos_registros(){

	ArbolBMas abm;
	string nombreArchivoArbol= "testArbol.dat";
	assert( abm.crear(nombreArchivoArbol,BLOQUE_TAM_DEFAULT)==RES_OK );
	assert(abm.abrir("testArbol.dat","rb+") == RES_OK);

	const unsigned int MAXIMA_CANTIDAD_BYTES_NODO_INTERNO= 45;
	NodoInterno ni(0,MAXIMA_CANTIDAD_BYTES_NODO_INTERNO);
	ni.limpiar();
	const unsigned short CANTIDAD_CLAVES= 5;
	const unsigned short CANTIDAD_HIJOS= CANTIDAD_CLAVES+1;
	ni.insertar_hijo(0);


	for(unsigned short i=0;i<CANTIDAD_CLAVES;i++){
		ClaveX unaClave;
		unaClave.set_clave( (int)i );
		unsigned short ocurrenciaInsercion;
		ni.insertar_clave(unaClave,ocurrenciaInsercion);
		ni.insertar_hijo( (TipoHijo)(i+1) );
	}
	assert(ni.hay_overflow());
	/*cargue a ni con claves del 0 al 5 y con hijos del 0 al 5*/

	{

		ClaveX claveBuscar;
		claveBuscar.set_clave( (int)-1 );
		RegistroClave registroBuscar;
		registroBuscar.set_clave(claveBuscar);
		TipoHijo hijoBuscar;

		abm._hallar_hijo_correspondiente(&registroBuscar,&ni,hijoBuscar);
		assert( hijoBuscar== 0 );
		//el hijo devuelto es el mas a la izquierda del nodo interno

		for(unsigned short i=0;i<CANTIDAD_HIJOS;i++){

			ClaveX claveBuscar;
			claveBuscar.set_clave( (int)i );
			RegistroClave registroBuscar;
			registroBuscar.set_clave(claveBuscar);
			TipoHijo hijoBuscar;

			abm._hallar_hijo_correspondiente(&registroBuscar,&ni,hijoBuscar);
			assert( hijoBuscar >= (int)i );

		}

	}



	ClaveX clavePromocionada;
	TipoHijo bloquePromocionado;
	abm._split_interno(&ni,&clavePromocionada,bloquePromocionado);
//	cout<<"clave promocionada: ";clavePromocionada.imprimir_dato();cout<<endl;
//	cout<<"bloque promocionado: "<<bloquePromocionado<<endl;


	assert(abm.cerrar() == RES_OK);

	assert(abm.eliminar(nombreArchivoArbol)== RES_OK);

	print_test_ok("test_insertar_pocos_registros");

}


void TestArbol::test_split_raiz(){

	ArbolBMas arbol;
	string nombreArchivo= "arbolSplit.dat";
	unsigned int tamanioBloque= 64;
	assert( arbol.crear(nombreArchivo,tamanioBloque)== RES_OK );
	assert( arbol.abrir(nombreArchivo,"rb+")== RES_OK );

	RegistroClave rc;
	ClaveX c;

	for(int i=0;i<8;i++){
		string clave= "aaa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		assert( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<2;i++){
		string clave= "aaCa";
		clave[3]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		assert( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<3;i++){

		string clave= "baa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		assert( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<10;i++){

		string clave= "caa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		assert( arbol.agregar(rc)== RES_OK );

	}

	cout<<"ARBOL RESULTANTE: "<<endl;
	arbol.imprimir();

	assert(arbol.cerrar() == RES_OK);

	print_test_ok("test_split_raiz");

}

void TestArbol::test_arbol_buscar()
{
	ArbolBMas arbol;
	string nombreArchivo= "arbolSplit_buscar.dat";
	unsigned int tamanioBloque= 100;
	assert( arbol.crear(nombreArchivo,tamanioBloque)== RES_OK );
	assert( arbol.abrir(nombreArchivo,"rb+")== RES_OK );


	for(int i=0;i<100;i++){
		RegistroClave reg;
		ClaveX c;

		string clave= "artista  ";
		clave[8]= 65+i;
		c.set_clave(clave);
		reg.set_clave(c);

		std::string campo = "campo  ";
		campo[6] = 65+i;
		reg.agregar_campo(campo.c_str(),campo.size());
		assert( arbol.agregar(reg)== RES_OK );
	}

	arbol.cerrar();
	arbol.abrir(nombreArchivo,"rb+");

	for (int i = 0; i < 100; i++)
	{
		ClaveX c;
		RegistroClave reg;

		string clave= "artista  ";
		clave[8]= 65+i;
		c.set_clave(clave);
		reg.set_clave(c);

		assert(arbol.buscar(reg) != RES_RECORD_DOESNT_EXIST);
		assert(arbol.buscar(reg) >= 0);
		char* buffer = new char[reg.get_tamanio_campo(1) + 1]();
		buffer[reg.get_tamanio_campo(1)] = '\0';

		reg.recuperar_campo(buffer,1);

		std::string campo = "campo  ";
		campo[6] = 65+i;


		assert(strcmp(buffer,campo.c_str()) == 0);
		delete[] buffer;
	}

	arbol.cerrar();
}

void TestArbol::test_arbol_buscar_secuencial()
{
	ArbolBMas arbol;
	string nombreArchivo = "arbolSplit_buscar.dat";
	unsigned int tamanioBloque= 100;
	assert( arbol.crear(nombreArchivo,tamanioBloque)== RES_OK );
	assert( arbol.abrir(nombreArchivo,"rb+")== RES_OK );

	// Agregamos desde "arjona0" hasta "arjona9"
	for(int i=0;i<10;i++){
		RegistroClave reg;
		ClaveX c;
		string clave= "arjona ";
		clave[6]= 48+i;
		c.set_clave(clave);
		reg.set_clave(c);
		assert( arbol.agregar(reg)== RES_OK );
	}

	// Agregamos desde "abba" hasta "abba9"
	for(int i=0;i<10;i++){
		RegistroClave reg;
		ClaveX c;

		string clave= "abba ";
		clave[4]= 48+i;
		c.set_clave(clave);
		reg.set_clave(c);

		assert( arbol.agregar(reg)== RES_OK );
	}

	// Agregamos desde "alan johnson0" hasta "alan johnson9"
	for(int i=0;i<6;i++){
		RegistroClave reg;
		ClaveX c;

		string clave= "alan johnson ";
		clave[12]= 48+i;
		c.set_clave(clave);
		reg.set_clave(c);

		assert( arbol.agregar(reg)== RES_OK );
	}

	RegistroClave reg;
	ClaveX c;

	string clave= "alan johnson ";
	clave[12]= 48+6;
	c.set_clave(clave);
	reg.set_clave(c);

	assert( arbol.agregar(reg)== RES_OK );



	arbol.imprimir();

	arbol.cerrar();
	arbol.abrir(nombreArchivo,"rb+");

	// ASSERT: Buscamos las claves por rango
	RegistroClave actual;
	IterArbolBMas unIterador(arbol);


	ClaveX claveA, claveABBA, claveArjona, claveAlan, claveB;
	claveA.set_clave("a");
	claveABBA.set_clave("abba0");
	claveArjona.set_clave("arjona0");
	claveAlan.set_clave("alan johnson0");
	claveB.set_clave("b");

	std::cout << "Buscando >= a: " << endl;
	unIterador.start(">=",claveA);
	while (unIterador.readNext(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << "\nBuscando >= abba0:" << endl;
	unIterador.start(">=",claveABBA);
	while (unIterador.readNext(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << "\nBuscando >= alan johnson0: " << std::endl;
	unIterador.start(">=",claveAlan);
	while (unIterador.readNext(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << "\nBuscando >= arjona0:" << std::endl;
	unIterador.start(">=",claveArjona);
	while (unIterador.readNext(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << std::endl;

	std::cout << "\nBuscando >= b:" << std::endl;
	unIterador.start(">=",claveB);
	while (unIterador.readNext(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << "\nBuscando > arjona0:" << endl;
	unIterador.start(">",claveArjona);
	while (unIterador.readNext(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << "\nBuscando > alan johnson0:" << endl;
	unIterador.start(">",claveAlan);
	while (unIterador.readNext(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << std::endl;


	arbol.cerrar();
	arbol.abrir(nombreArchivo,"rb+");
	arbol.imprimir();
	arbol.cerrar();

	print_test_ok("test_arbol_buscar_secuencial");
}


void TestArbol::test_eliminar_sin_underflow(){

	cout<<"inicio test_eliminar_sin_underflow -------------------------"<<endl;

	{
		ArbolBMas arbol;
		string nombreArchivo= "arbolSoloRaizEliminar.dat";
		unsigned int tamanioBloque= 64;
		assert( arbol.crear(nombreArchivo,tamanioBloque)== RES_OK );
		assert( arbol.abrir(nombreArchivo,"rb+")== RES_OK );

		RegistroClave rc;
			ClaveX c;

			for(int i=0;i<3;i++){
				string clave= "aaa";
				clave[2]= 65+i;
				c.set_clave(clave);
				rc.set_clave(c);
				assert( arbol.agregar(rc)== RES_OK );
			}

		for(unsigned short i= 0;i<3;i++){
			string clave= "aaa";
			clave[2]= 65+i;
			ClaveX claveEliminar;
			claveEliminar.set_clave(clave);
			RegistroClave registroEliminar;
			registroEliminar.set_clave(claveEliminar);
			arbol.quitar(registroEliminar);
		}

		cout<<"imprimiendo arbol con registros solo en raiz eliminados : "<<endl;
		arbol.imprimir();
		arbol.cerrar();


	}


	{
	ArbolBMas arbol;
	string nombreArchivo= "arbolSinUnderflow.dat";
	unsigned int tamanioBloque= 64;
	assert( arbol.crear(nombreArchivo,tamanioBloque)== RES_OK );
	assert( arbol.abrir(nombreArchivo,"rb+")== RES_OK );

	RegistroClave rc;
	ClaveX c;

	for(int i=0;i<8;i++){
		string clave= "aaa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		assert( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<2;i++){
		string clave= "aaCa";
		clave[3]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		assert( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<3;i++){

		string clave= "baa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		assert( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<10;i++){

		string clave= "caa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		assert( arbol.agregar(rc)== RES_OK );

	}

	cout<<"ARBOL RESULTANTE: "<<endl;

	RegistroClave registroEliminar;
	ClaveX claveEliminar;
	claveEliminar.set_clave("aaD");
	registroEliminar.set_clave(claveEliminar);
	assert( arbol.quitar(registroEliminar)== RES_OK );

	arbol.imprimir();
	assert(arbol.cerrar() == RES_OK);
	}



	print_test_ok("test_eliminar_sin_underflow");

}


void TestArbol::test_eliminar_con_merge_secuenciales(){

	{
		ArbolBMas arbol;
		string nombreArchivo= "arbolConUnderflowMergeSecuenciales.dat";
		unsigned int tamanioBloque= 64;
		assert( arbol.crear(nombreArchivo,tamanioBloque)== RES_OK );
		assert( arbol.abrir(nombreArchivo,"rb+")== RES_OK );

		RegistroClave rc;
		ClaveX c;

		for(int i=0;i<8;i++){
			string clave= "aaa";
			clave[2]= 65+i;
			c.set_clave(clave);
			rc.set_clave(c);
			assert( arbol.agregar(rc)== RES_OK );
		}

		for(int i=0;i<2;i++){
			string clave= "aaCa";
			clave[3]= 65+i;
			c.set_clave(clave);
			rc.set_clave(c);
			assert( arbol.agregar(rc)== RES_OK );
		}

		for(int i=0;i<3;i++){

			string clave= "baa";
			clave[2]= 65+i;
			c.set_clave(clave);
			rc.set_clave(c);
			assert( arbol.agregar(rc)== RES_OK );
		}

		for(int i=0;i<10;i++){

			string clave= "caa";
			clave[2]= 65+i;
			c.set_clave(clave);
			rc.set_clave(c);
			assert( arbol.agregar(rc)== RES_OK );

		}

		cout<<"ARBOL RESULTANTE: "<<endl;


		{
			ClaveX ce;
			RegistroClave re;
			ce.set_clave("aaC");
			re.set_clave(ce);
			arbol.quitar(re);
			ce.set_clave("aaCA");
			re.set_clave(ce);
			arbol.quitar(re);
			ce.set_clave("aaA");
			re.set_clave(ce);
			arbol.quitar(re);
			ce.set_clave("aaB");
			re.set_clave(ce);
			arbol.quitar(re);
		}


		cout<<"imprimiendo arbol con merge de secuenciales "<<endl;
		arbol.imprimir();
		assert(arbol.cerrar() == RES_OK);





	}



		print_test_ok("test_eliminar_con_merge_secuenciales");

}

