#include "../src/CapaLogica/ArbolBMas/ArbolBMas.h"
#include "../src/CapaLogica/ArbolBMas/IterArbolBMas.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestArbol : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	ArbolBMas arbol;
	typedef NodoInterno::TipoHijo TipoHijo;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
  }

  virtual void TearDown() {
	  ASSERT_TRUE(arbol.cerrar() == RES_OK);
	  ASSERT_TRUE(arbol.eliminar("arbol.dat") == RES_OK);
  }

  // A helper function that some test uses.

};

TEST_F(TestArbol,Insertar_registro)
{
	ASSERT_TRUE (arbol.crear("arbol.dat",BLOQUE_TAM_DEFAULT) == RES_OK);
	ASSERT_TRUE (arbol.abrir("arbol.dat","rb+") == RES_OK);

	RegistroClave reg;
	ClaveX clave;
	clave.set_clave("u2 123");
	reg.set_clave(clave);

	std::string campo = "ID CANCION = 3";
	reg.agregar_campo(campo.c_str(),campo.size());

	ASSERT_TRUE (arbol.agregar(reg) == RES_OK);

	RegistroClave regLeido;
	regLeido.set_clave(clave);

	// Buscamos "u2 123" en el arbol
	unsigned int numBloque = -1;
	ASSERT_TRUE (arbol._buscar(regLeido, numBloque) != RES_RECORD_DOESNT_EXIST);
	ASSERT_TRUE (numBloque == 1);
	ASSERT_TRUE (regLeido.get_clave() == clave);

	char* buffer = new char[regLeido.get_tamanio_campo(1) + 1]();
	buffer[regLeido.get_tamanio_campo(1)] = '\0';
	regLeido.recuperar_campo(buffer,1);

	ASSERT_TRUE (strcmp(buffer,campo.c_str()) == 0);

	delete[] buffer;
}


TEST_F(TestArbol,Abrir_cerrar)
{

	ASSERT_TRUE (arbol.crear("arbol.dat",BLOQUE_TAM_DEFAULT) == RES_OK);
	ASSERT_TRUE (arbol.abrir("arbol.dat","rb+") == RES_OK);
	//ASSERT_TRUE (arbol.get_cant_minima_nodo() == sizeof(TipoHijo));
	ASSERT_TRUE (arbol.get_cant_minima_nodo() == 1);
	//FIXME cambiado 25/5/13

	ASSERT_TRUE (arbol.get_cant_maxima_nodo() == (int)(BLOQUE_TAM_DEFAULT * ArbolBMas::FACTOR_CARGA/100));
}


TEST_F(TestArbol,Insertar_pocos_registros)
{
	ASSERT_TRUE( arbol.crear("arbol.dat",BLOQUE_TAM_DEFAULT)==RES_OK );
	ASSERT_TRUE(arbol.abrir("arbol.dat","rb+") == RES_OK);

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
	ASSERT_TRUE(ni.hay_overflow());
	/*cargue a ni con claves del 0 al 5 y con hijos del 0 al 5*/

	{

		ClaveX claveBuscar;
		claveBuscar.set_clave( (int)-1 );
		RegistroClave registroBuscar;
		registroBuscar.set_clave(claveBuscar);
		TipoHijo hijoBuscar;

		arbol._hallar_hijo_correspondiente(&registroBuscar,&ni,hijoBuscar);
		ASSERT_TRUE( hijoBuscar== 0 );
		//el hijo devuelto es el mas a la izquierda del nodo interno

		for(unsigned short i=0;i<CANTIDAD_HIJOS;i++){

			ClaveX claveBuscar;
			claveBuscar.set_clave( (int)i );
			RegistroClave registroBuscar;
			registroBuscar.set_clave(claveBuscar);
			TipoHijo hijoBuscar;

			arbol._hallar_hijo_correspondiente(&registroBuscar,&ni,hijoBuscar);
			ASSERT_TRUE( hijoBuscar >= (int)i );

		}

	}


	ClaveX clavePromocionada;
	TipoHijo bloquePromocionado;
	arbol._split_interno(&ni,&clavePromocionada,bloquePromocionado);
//	cout<<"clave promocionada: ";clavePromocionada.imprimir_dato();cout<<endl;
//	cout<<"bloque promocionado: "<<bloquePromocionado<<endl;
}


TEST_F(TestArbol,Split_raiz)
{
	unsigned int tamanioBloque= 64;
	ASSERT_TRUE( arbol.crear("arbol.dat",tamanioBloque)== RES_OK );
	ASSERT_TRUE( arbol.abrir("arbol.dat","rb+")== RES_OK );

	RegistroClave rc;
	ClaveX c;

	for(int i=0;i<8;i++){
		string clave= "aaa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<2;i++){
		string clave= "aaCa";
		clave[3]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<3;i++){

		string clave= "baa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<10;i++){

		string clave= "caa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );

	}

	cout<<"ARBOL RESULTANTE: "<<endl;
	arbol.imprimir();
}

TEST_F(TestArbol,Buscar)
{
	unsigned int tamanioBloque= 100;
	ASSERT_TRUE( arbol.crear("arbol.dat",tamanioBloque)== RES_OK );
	ASSERT_TRUE( arbol.abrir("arbol.dat","rb+")== RES_OK );


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
		ASSERT_TRUE( arbol.agregar(reg)== RES_OK );
	}

	for (int i = 0; i < 100; i++)
	{
		ClaveX c;
		RegistroClave reg;

		string clave= "artista  ";
		clave[8]= 65+i;
		c.set_clave(clave);
		reg.set_clave(c);

		ASSERT_TRUE(arbol.buscar(reg) != RES_RECORD_DOESNT_EXIST);
		ASSERT_TRUE(arbol.buscar(reg) >= 0);
		char* buffer = new char[reg.get_tamanio_campo(1) + 1]();
		buffer[reg.get_tamanio_campo(1)] = '\0';

		reg.recuperar_campo(buffer,1);

		std::string campo = "campo  ";
		campo[6] = 65+i;


		ASSERT_TRUE(strcmp(buffer,campo.c_str()) == 0);
		delete[] buffer;
	}

}

TEST_F(TestArbol,Buscar_secuencial)
{
	unsigned int tamanioBloque= 100;
	ASSERT_TRUE( arbol.crear("arbol.dat",tamanioBloque)== RES_OK );
	ASSERT_TRUE( arbol.abrir("arbol.dat","rb+")== RES_OK );

	// Agregamos desde "arjona0" hasta "arjona9"
	for(int i=0;i<10;i++){
		RegistroClave reg;
		ClaveX c;
		string clave= "arjona ";
		clave[6]= 48+i;
		c.set_clave(clave);
		reg.set_clave(c);
		ASSERT_TRUE( arbol.agregar(reg)== RES_OK );
	}

	// Agregamos desde "abba" hasta "abba9"
	for(int i=0;i<10;i++){
		RegistroClave reg;
		ClaveX c;

		string clave= "abba ";
		clave[4]= 48+i;
		c.set_clave(clave);
		reg.set_clave(c);

		ASSERT_TRUE( arbol.agregar(reg)== RES_OK );
	}

	// Agregamos desde "alan johnson0" hasta "alan johnson9"
	for(int i=0;i<6;i++){
		RegistroClave reg;
		ClaveX c;

		string clave= "alan johnson ";
		clave[12]= 48+i;
		c.set_clave(clave);
		reg.set_clave(c);

		ASSERT_TRUE( arbol.agregar(reg)== RES_OK );
	}

	RegistroClave reg;
	ClaveX c;

	string clave= "alan johnson ";
	clave[12]= 48+6;
	c.set_clave(clave);
	reg.set_clave(c);

	ASSERT_TRUE( arbol.agregar(reg)== RES_OK );



	arbol.imprimir();

	// ASSERT_TRUE: Buscamos las claves por rango
	RegistroClave actual;
	IterArbolBMas unIterador(arbol);

	ClaveX claveA, claveABBA, claveArjona, claveAlan, claveB;
	claveA.set_clave("a");
	claveABBA.set_clave("abba0");
	claveArjona.set_clave("arjona0");
	claveAlan.set_clave("alan johnson0");
	claveB.set_clave("b");

	std::cout << "Buscando >= a: " << endl;
	unIterador.comienzo(">=",claveA);
	while (unIterador.leer_siguiente(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << "\nBuscando >= abba0:" << endl;
	unIterador.comienzo(">=",claveABBA);
	while (unIterador.leer_siguiente(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << "\nBuscando >= alan johnson0: " << std::endl;
	unIterador.comienzo(">=",claveAlan);
	while (unIterador.leer_siguiente(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << "\nBuscando >= arjona0:" << std::endl;
	unIterador.comienzo(">=",claveArjona);
	while (unIterador.leer_siguiente(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << std::endl;

	std::cout << "\nBuscando >= b:" << std::endl;
	unIterador.comienzo(">=",claveB);
	while (unIterador.leer_siguiente(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << "\nBuscando > arjona0:" << endl;
	unIterador.comienzo(">",claveArjona);
	while (unIterador.leer_siguiente(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << "\nBuscando > alan johnson0:" << endl;
	unIterador.comienzo(">",claveAlan);
	while (unIterador.leer_siguiente(actual) != RES_FIN)
	{
		actual.get_clave().imprimir_dato(); cout<<", ";
	}

	std::cout << std::endl;

	arbol.imprimir();
}

TEST_F(TestArbol,Eliminar_raiz)
{
	unsigned int tamanioBloque= 64;
	ASSERT_TRUE( arbol.crear("arbol.dat",tamanioBloque)== RES_OK );
	ASSERT_TRUE( arbol.abrir("arbol.dat","rb+")== RES_OK );

	RegistroClave rc;
		ClaveX c;

		for(int i=0;i<3;i++){
			string clave= "aaa";
			clave[2]= 65+i;
			c.set_clave(clave);
			rc.set_clave(c);
			ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
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
}

TEST_F(TestArbol,Eliminar_raiz_sin_underflow)
{
	unsigned int tamanioBloque= 64;
	ASSERT_TRUE( arbol.crear("arbol.dat",tamanioBloque)== RES_OK );
	ASSERT_TRUE( arbol.abrir("arbol.dat","rb+")== RES_OK );

	RegistroClave rc;
	ClaveX c;

	for(int i=0;i<8;i++){
		string clave= "aaa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<2;i++){
		string clave= "aaCa";
		clave[3]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<3;i++){

		string clave= "baa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<10;i++){

		string clave= "caa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );

	}

	cout<<"ARBOL RESULTANTE: "<<endl;

	RegistroClave registroEliminar;
	ClaveX claveEliminar;
	claveEliminar.set_clave("aaD");
	registroEliminar.set_clave(claveEliminar);
	ASSERT_TRUE( arbol.quitar(registroEliminar)== RES_OK );

	arbol.imprimir();
}

TEST_F(TestArbol,Eliminar_con_merge_secuenciales)
{
	unsigned int tamanioBloque= 64;
	ASSERT_TRUE( arbol.crear("arbol.dat",tamanioBloque)== RES_OK );
	ASSERT_TRUE( arbol.abrir("arbol.dat","rb+")== RES_OK );

	RegistroClave rc;
	ClaveX c;

	for(int i=0;i<8;i++){
		string clave= "aaa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<2;i++){
		string clave= "aaCa";
		clave[3]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<3;i++){

		string clave= "baa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<10;i++){

		string clave= "caa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );

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
		/*merge de secuencial no ultimo hijo*/

		cout<<"imprimiendo arbol con merge sin ultimo hijo: ----------------------"<<endl;
		arbol.imprimir();


		/*funciona hasta aqui*/

		ce.set_clave("baB");
		re.set_clave(ce);
		arbol.quitar(re);
		ce.set_clave("baC");
		re.set_clave(ce);
		arbol.quitar(re);
		ce.set_clave("caB");
		re.set_clave(ce);
		arbol.quitar(re);
		ce.set_clave("caC");
		re.set_clave(ce);
		arbol.quitar(re);
		ce.set_clave("caD");
		re.set_clave(ce);
		arbol.quitar(re);

		cout<<"imprimiendo arbol con merge con ultimo hijo: ----------------------"<<endl;
		arbol.imprimir();

	}

}

TEST_F(TestArbol,Eliminar_con_balanceo_secuenciales)
{
	unsigned int tamanioBloque= 64;
	ASSERT_TRUE( arbol.crear("arbol.dat",tamanioBloque)== RES_OK );
	ASSERT_TRUE( arbol.abrir("arbol.dat","rb+")== RES_OK );

	RegistroClave rc;
	ClaveX c;

	for(int i=0;i<8;i++){
		string clave= "aaa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<2;i++){
		string clave= "aaCa";
		clave[3]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<3;i++){

		string clave= "baa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<10;i++){

		string clave= "caa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );

	}

	cout<<"imprimiendo arbol original: ----------------------"<<endl;
	arbol.imprimir();

	ClaveX ce;
	RegistroClave re;
	ce.set_clave("aaA");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("aaB");
	re.set_clave(ce);
	arbol.quitar(re);


	cout<<"imprimiendo arbol con balanceo sin ultimo hijo: ----------------------"<<endl;
	arbol.imprimir();

	/*funciona hasta aqui*/

	ce.set_clave("aaD");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("aaE");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("aaF");
	re.set_clave(ce);
	arbol.quitar(re);

	//otro porque el anterior paso de una
	ce.set_clave("caH");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caI");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caJ");
	re.set_clave(ce);
	arbol.quitar(re);

	cout<<"imprimiendo arbol con balanceo con ultimo hijo: ----------------------"<<endl;
	arbol.imprimir();
}

TEST_F(TestArbol,Eliminar_con_merge_internos)
{
	unsigned int tamanioBloque= 64;
	ASSERT_TRUE( arbol.crear("arbol.dat",tamanioBloque)== RES_OK );
	ASSERT_TRUE( arbol.abrir("arbol.dat","rb+")== RES_OK );

	RegistroClave rc;
	ClaveX c;

	for(int i=0;i<8;i++){
		string clave= "aaa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<2;i++){
		string clave= "aaCa";
		clave[3]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<3;i++){

		string clave= "baa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<10;i++){

		string clave= "caa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );

	}

	cout<<"imprimiendo arbol original: ----------------------"<<endl;
	arbol.imprimir();

	ClaveX ce;
	RegistroClave re;
	ce.set_clave("baB");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("baC");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caB");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caC");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caD");
	re.set_clave(ce);
	arbol.quitar(re);


	ce.set_clave("aaG");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("aaH");
	re.set_clave(ce);
	arbol.quitar(re);


	cout<<"imprimiendo arbol justo antes de merge de internos: ----------------------"<<endl;
	arbol.imprimir();

	ce.set_clave("baA");
	re.set_clave(ce);
	arbol.quitar(re);


	cout<<"imprimiendo arbol con merge interno sin ultimo hijo: ----------------------"<<endl;
	arbol.imprimir();
}

TEST_F(TestArbol,Eliminar_con_merge_interno_en_ultimo)
{
	unsigned int tamanioBloque= 64;
	ASSERT_TRUE( arbol.crear("arbol.dat",tamanioBloque)== RES_OK );
	ASSERT_TRUE( arbol.abrir("arbol.dat","rb+")== RES_OK );

	RegistroClave rc;
	ClaveX c;

	for(int i=0;i<8;i++){
		string clave= "aaa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<2;i++){
		string clave= "aaCa";
		clave[3]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<3;i++){

		string clave= "baa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<10;i++){

		string clave= "caa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );

	}

	cout<<"imprimiendo arbol original: ----------------------"<<endl;
	arbol.imprimir();

	ClaveX ce;
	RegistroClave re;
	ce.set_clave("baB");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("baC");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caB");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caC");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caD");
	re.set_clave(ce);
	arbol.quitar(re);


	ce.set_clave("aaG");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("aaH");
	re.set_clave(ce);
	arbol.quitar(re);


	ce.set_clave("caE");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caF");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caH");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caJ");
	re.set_clave(ce);
	arbol.quitar(re);

	cout<<"imprimiendo arbol justo antes de merge de internos: ----------------------"<<endl;
	arbol.imprimir();

	ce.set_clave("caG");
	re.set_clave(ce);
	arbol.quitar(re);


	cout<<"imprimiendo arbol con merge interno en el ultimo: ----------------------"<<endl;
	arbol.imprimir();
}

TEST_F(TestArbol,Eliminar_con_balanceo_interno_en_ultimo)
{
	unsigned int tamanioBloque= 64;
	ASSERT_TRUE( arbol.crear("arbol.dat",tamanioBloque)== RES_OK );
	ASSERT_TRUE( arbol.abrir("arbol.dat","rb+")== RES_OK );

	RegistroClave rc;
	ClaveX c;

	for(int i=0;i<8;i++){
		string clave= "aaa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<2;i++){
		string clave= "aaCa";
		clave[3]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<3;i++){

		string clave= "baa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<10;i++){

		string clave= "caa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );

	}

	cout<<"imprimiendo arbol original: ----------------------"<<endl;
	arbol.imprimir();

	ClaveX ce;
	RegistroClave re;
	ce.set_clave("aaA");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("aaB");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("aaC");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("aaD");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("aaE");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("aaCA");
	re.set_clave(ce);
	arbol.quitar(re);


	cout<<"imprimiendo arbol justo antes de balancear: ----------------------"<<endl;
	arbol.imprimir();

	ce.set_clave("aaCB");
	re.set_clave(ce);
	arbol.quitar(re);

	cout<<"imprimiendo arbol despues de balancear: ----------------------"<<endl;
	arbol.imprimir();
}

TEST_F(TestArbol,Eliminar_con_balanceo_interno_en_ultimo_nodo)
{
	unsigned int tamanioBloque= 64;
	ASSERT_TRUE( arbol.crear("arbol.dat",tamanioBloque)== RES_OK );
	ASSERT_TRUE( arbol.abrir("arbol.dat","rb+")== RES_OK );

	RegistroClave rc;
	ClaveX c;

	for(int i=0;i<8;i++){
		string clave= "aaa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<2;i++){
		string clave= "aaCa";
		clave[3]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<3;i++){

		string clave= "baa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<10;i++){

		string clave= "caa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );

	}

	cout<<"imprimiendo arbol original: ----------------------"<<endl;
	arbol.imprimir();

	ClaveX ce;
	RegistroClave re;
	ce.set_clave("caE");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caF");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caI");
	re.set_clave(ce);
	arbol.quitar(re);
	ce.set_clave("caH");
	re.set_clave(ce);
	arbol.quitar(re);


	cout<<"imprimiendo arbol justo antes de balancear: ----------------------"<<endl;
	arbol.imprimir();

	ce.set_clave("caJ");
	re.set_clave(ce);
	arbol.quitar(re);


	cout<<"imprimiendo arbol despues de balancear: ----------------------"<<endl;
	arbol.imprimir();
}

TEST_F(TestArbol,Eliminar_arbol_completamente)
{
	unsigned int tamanioBloque= 64;
	ASSERT_TRUE( arbol.crear("arbol.dat",tamanioBloque)== RES_OK );
	ASSERT_TRUE( arbol.abrir("arbol.dat","rb+")== RES_OK );

	RegistroClave rc;
	ClaveX c;

	for(int i=0;i<8;i++){
		string clave= "aaa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<2;i++){
		string clave= "aaCa";
		clave[3]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<3;i++){

		string clave= "baa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );
	}

	for(int i=0;i<10;i++){

		string clave= "caa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.agregar(rc)== RES_OK );

	}

	cout<<"imprimiendo arbol original: ----------------------"<<endl;
	arbol.imprimir();

	for(int i=0;i<8;i++){
		string clave= "aaa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.quitar(rc)== RES_OK );
	}

	cout<<"imprimiendo arbol etapa 1: ----------------------"<<endl;
	arbol.imprimir();

	for(int i=0;i<2;i++){
		string clave= "aaCa";
		clave[3]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.quitar(rc)== RES_OK );
	}

	cout<<"imprimiendo arbol etapa 2: ----------------------"<<endl;
	arbol.imprimir();

	for(int i=0;i<3;i++){

		string clave= "baa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.quitar(rc)== RES_OK );
	}

	cout<<"imprimiendo arbol etapa 3: ----------------------"<<endl;
	arbol.imprimir();

	for(int i=0;i<10;i++){

		string clave= "caa";
		clave[2]= 65+i;
		c.set_clave(clave);
		rc.set_clave(c);
		ASSERT_TRUE( arbol.quitar(rc)== RES_OK );

	}

	cout<<"imprimiendo arbol borrado: ----------------------"<<endl;
	arbol.imprimir();
}
