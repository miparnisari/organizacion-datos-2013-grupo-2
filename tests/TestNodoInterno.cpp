#include "../src/CapaLogica/ArbolBMas/NodoInterno.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestNodoInterno : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	typedef NodoInterno::TipoHijo TipoHijo;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
  }

  // TearDown() is invoked immediately after a test finishes.
  virtual void TearDown() {
  }

  // A helper function that some test uses.

};

TEST_F(TestNodoInterno,Insertar_hijos)
{
	srand(time(NULL));

	{
		const unsigned short LIMITE_RANDOM= 32;

		const unsigned int MAX_CANT_BYTES= 32;
		const unsigned int MIN_CANT_BYTES= 16;
		NodoInterno ni(MIN_CANT_BYTES,MAX_CANT_BYTES);
		const unsigned short CANTIDAD_HIJOS= 5;
		TipoHijo hijos[CANTIDAD_HIJOS];
		for(unsigned short i=0;i<CANTIDAD_HIJOS;i++)
			hijos[i]= rand()%LIMITE_RANDOM;

		const unsigned int TAMANIO_INICIAL= ni.get_tamanio_ocupado();
		ASSERT_TRUE( ni.insertar_hijo(99,2)== RES_ERROR );
		ASSERT_TRUE( ni.get_tamanio_ocupado()== (signed)TAMANIO_INICIAL);
		for(unsigned short i=0;i<2;i++)
			ASSERT_TRUE( ni.insertar_hijo(hijos[i],i+1)==RES_UNDERFLOW );
		ASSERT_TRUE( ni.insertar_hijo(hijos[2],3)== RES_UNDERFLOW );

		ASSERT_TRUE( ni.insertar_hijo(hijos[3],0)== RES_OK );

		NodoInterno ni2;
		for(unsigned short i=0;i<CANTIDAD_HIJOS;i++)
			ni2.insertar_hijo(hijos[i],0);

		for(unsigned short i=0;i<CANTIDAD_HIJOS;i++){

			unsigned short elHijo= CANTIDAD_HIJOS-1-i;
			TipoHijo unHijo;
			ni2.get_hijo(unHijo,i);
			ASSERT_TRUE( unHijo== hijos[elHijo] );

		}


	}

	{

		NodoInterno ni;
		const unsigned short CANTIDAD_CLAVES_INICIAL= ni.get_cantidad_claves();
		const unsigned short CANTIDAD_HIJOS_INICIAL= ni.get_cantidad_hijos();

		const unsigned short CANTIDAD_DATOS= 5;
		string datos[CANTIDAD_DATOS]={"martin","mateo","chindasvinto","farinelo",
		"extraterrestre"};
		ClaveX claves[CANTIDAD_DATOS];
		for(unsigned short i=0;i<CANTIDAD_DATOS;i++)
			claves[i].set_clave(datos[i]);

		TipoHijo hijos[CANTIDAD_DATOS+1]= {1, 5 , 20 , 21 , 34,86};
		const unsigned short CANTIDAD_HIJOS= CANTIDAD_DATOS+1;

		TipoHijo unHijo;
		ASSERT_TRUE( ni.get_hijo(unHijo,0) == RES_OK && unHijo== HIJO_INVALIDO );
		ASSERT_TRUE( ni.get_hijo(unHijo,1)== RES_ERROR );
		ASSERT_TRUE( ni.insertar_hijo_derecho(claves[0], hijos[0])== RES_ERROR );
		ASSERT_TRUE( ni.get_cantidad_claves()== CANTIDAD_CLAVES_INICIAL );
		ASSERT_TRUE( ni.get_cantidad_hijos()== CANTIDAD_HIJOS_INICIAL );
		ASSERT_TRUE( ni.modificar_hijo(hijos[0],0) == RES_OK );
		ASSERT_TRUE( ni.modificar_hijo(hijos[0],1) == RES_ERROR );


		for(unsigned short i=0;i<CANTIDAD_DATOS;i++){
			unsigned short us;
			ASSERT_TRUE( ni.insertar_clave(claves[i],us)==RES_OK );
			ASSERT_TRUE( ni.insertar_hijo(HIJO_INVALIDO)== RES_OK );

		}

		for(unsigned short i=0;i<CANTIDAD_DATOS;i++)
			ASSERT_TRUE( ni.modificar_hijo_derecho(claves[i],hijos[i+1])== RES_OK );
		for(unsigned short i=0;i<CANTIDAD_DATOS;i++){
			TipoHijo unHijo;
			ASSERT_TRUE( ni.get_hijo_derecho(unHijo , claves[i])== RES_OK );
			ASSERT_TRUE( unHijo== hijos[i+1] );
		}
		ASSERT_TRUE( ni.get_cantidad_claves()== CANTIDAD_DATOS &&
				ni.get_cantidad_hijos()== CANTIDAD_HIJOS );

		for(unsigned int i=0;i<CANTIDAD_DATOS;i++){
			ClaveX unaClave;
			ni.remover_clave(0,unaClave);
			ni.remover_hijo(0);
		}

		for(unsigned short i=0;i<CANTIDAD_DATOS;i++){
			unsigned short us;
			ASSERT_TRUE( ni.insertar_clave(claves[i],us)==RES_OK );
			ASSERT_TRUE( ni.insertar_hijo_derecho(claves[i]) == RES_OK );

		}

		for(unsigned short i=0;i<CANTIDAD_DATOS;i++)
			ASSERT_TRUE( ni.modificar_hijo_derecho(claves[i],hijos[i+1])== RES_OK );
		for(unsigned short i=0;i<CANTIDAD_DATOS;i++){
			TipoHijo unHijo;
			ASSERT_TRUE( ni.get_hijo_derecho(unHijo , claves[i])== RES_OK );
			ASSERT_TRUE( unHijo== hijos[i+1] );
		}
		ASSERT_TRUE( ni.get_cantidad_claves()== CANTIDAD_DATOS &&
				ni.get_cantidad_hijos()== CANTIDAD_HIJOS );

		NodoInterno ni2;
		for(unsigned short i=0;i<CANTIDAD_DATOS;i++){
			unsigned short us;
			ni2.insertar_clave(claves[i],us);
			ni2.insertar_hijo(hijos[i+1]);
		}

		for(unsigned short i=0;i<CANTIDAD_DATOS;i++){
			ni2.modificar_hijo_izquierdo(claves[i],hijos[i]);
			TipoHijo unHijo;
			ni2.get_hijo_izquierdo(unHijo,claves[i]);
			ASSERT_TRUE( unHijo== hijos[i] );
		}


	}

}

TEST_F(TestNodoInterno,Crear_nodo_con_demasiadas_claves_falla)
{
	NodoInterno nodoI(0,16);
	ClaveX clave1, clave2, clave3, clave4;
	clave1.set_clave("aca");
	clave2.set_clave("alla");
	clave3.set_clave("abba");
	clave4.set_clave("clave que no entra");

	unsigned short pos= 0;
	ASSERT_TRUE(nodoI.insertar_clave(clave1,pos)== RES_OK && pos==0);
	ASSERT_TRUE(nodoI.insertar_clave(clave2,pos)== RES_OK && pos==1);
	ASSERT_TRUE(nodoI.insertar_clave(clave3,pos)== RES_OVERFLOW && pos== 0);
	ASSERT_TRUE(nodoI.insertar_clave(clave4,pos)== RES_OVERFLOW && pos==3);
	ASSERT_TRUE(nodoI.get_cantidad_claves() == 4);

	ASSERT_TRUE(nodoI.remover_clave(clave1,pos)== RES_OVERFLOW);
	//al remover la clave mas pequenia, el nodo sigue en estado overflow

	{
		const unsigned short CANTIDAD_CLAVES= 5;
		ClaveX claves[CANTIDAD_CLAVES];
		for(unsigned short i=0;i<CANTIDAD_CLAVES;i++)
			claves[i].set_clave((int)(2*i));


		NodoInterno ni;
		const unsigned int TAMANIO_INICIAL= ni.get_tamanio_ocupado();

		for(unsigned short i=0;i<CANTIDAD_CLAVES;i++){
			unsigned short ocurrenciaInsercion= 0;
			ASSERT_TRUE( ni.insertar_clave(claves[i],ocurrenciaInsercion)== RES_OK );
			ASSERT_TRUE( ocurrenciaInsercion== i );
			ASSERT_TRUE( ni.insertar_clave(claves[i],ocurrenciaInsercion)== RES_ERROR );
			ASSERT_TRUE( ni.get_tamanio_ocupado()== (signed)TAMANIO_INICIAL +
					claves[i].get_tamanio_empaquetado() * (i+1) );
		}

		unsigned short ocurrenciaInsercion= 0;
		ClaveX cx;
		cx.set_clave(1);
		ASSERT_TRUE( ni.insertar_clave(cx,ocurrenciaInsercion)== RES_OK );
		ASSERT_TRUE( ocurrenciaInsercion== 1 );

		cx.set_clave(3);
		ASSERT_TRUE( ni.insertar_clave(cx,ocurrenciaInsercion)== RES_OK );
		ASSERT_TRUE( ocurrenciaInsercion== 3 );

	}//las claves se insertan en el orden apropiado

	{

		const unsigned short CANTIDAD_CLAVES= 5;
		ClaveX claves[CANTIDAD_CLAVES];
		for(unsigned short i=0;i<CANTIDAD_CLAVES;i++)
			claves[i].set_clave((int)(2*i));
		const unsigned short TAMANIO_EMPAQUETADO_CLAVE=
				claves[0].get_tamanio_empaquetado();

		const unsigned int MAX_BYTES_NODO_INTERNO= 16;
		NodoInterno ni(0,MAX_BYTES_NODO_INTERNO);
		const unsigned short TAMANIO_INICIAL= ni.get_tamanio_ocupado();

		srand(time(NULL));
		const unsigned int LIMITE_RANDOM= 20;
		NodoInterno::TipoHijo hijos[CANTIDAD_CLAVES];
		for(unsigned short i=0;i<CANTIDAD_CLAVES;i++)
			hijos[i]= rand()%LIMITE_RANDOM;

		unsigned short ocurrenciaInsercion= 0;
		ni.insertar_clave(claves[0],ocurrenciaInsercion);
		ni.insertar_clave(claves[1],ocurrenciaInsercion);
		ASSERT_TRUE( ni.get_tamanio_ocupado()== TAMANIO_INICIAL +
				TAMANIO_EMPAQUETADO_CLAVE*2 );
		ni.insertar_hijo(hijos[0],1);



	}


	{
		NodoInterno ni;
		typedef NodoInterno::TipoHijo TipoHijo;
		string claveAcumulada= "dato";
		const unsigned short CANTIDAD_DATOS= 5;
		const unsigned short CANTIDAD_HIJOS= CANTIDAD_DATOS+1;
		ni.modificar_hijo(0,0);

		for(unsigned short i=0;i<CANTIDAD_DATOS;i++){

			unsigned short ocurrenciaInsercion;
			ClaveX unaClave;
			unaClave.set_clave(claveAcumulada);
			ASSERT_TRUE( ni.insertar_clave(unaClave,ocurrenciaInsercion)== RES_OK );
			claveAcumulada+= (char)(67+i);
			ASSERT_TRUE( ni.insertar_hijo(i+1,i+1)== RES_OK );

		}ASSERT_TRUE(ni.get_cantidad_claves()== CANTIDAD_DATOS && ni.get_cantidad_hijos()== CANTIDAD_HIJOS);

		//ni.imprimir();

		string datoClaveMitad= "datoCD";
		ClaveX claveMitad;
		claveMitad.set_clave(datoClaveMitad);
		ClaveX claveMitadNodo;
		ni.get_clave_mitad(claveMitadNodo);
		ASSERT_TRUE( claveMitad== claveMitadNodo );

		unsigned short numeroClaveMitad;
		ni.buscar_clave(claveMitad,numeroClaveMitad);
		vector<TipoHijo> hijosMover;
		vector<ClaveX> clavesMover;
		for(unsigned short i=numeroClaveMitad;i<CANTIDAD_DATOS;i++){

			ClaveX unaClave;
			ASSERT_TRUE( ni.remover_clave(numeroClaveMitad,unaClave)== RES_OK );
			clavesMover.push_back(unaClave);
			TipoHijo unHijo;
			ni.get_hijo(unHijo,numeroClaveMitad+1);
			hijosMover.push_back(unHijo);
			ASSERT_TRUE( ni.remover_hijo(numeroClaveMitad+1)== RES_OK );


		}/*pruebo remover la clave de la mitad y los hijos de la misma*/

		NodoInterno nodoNuevo;
		nodoNuevo.limpiar();
		nodoNuevo.insertar_hijo(hijosMover.at(0));

		for(unsigned short i=0; i< (CANTIDAD_DATOS-numeroClaveMitad); i++){

			unsigned short ocurrenciaInsercion;
			nodoNuevo.insertar_clave(clavesMover[i],ocurrenciaInsercion);
			nodoNuevo.insertar_hijo(hijosMover[i+1]);

		}

	}


}

TEST_F(TestNodoInterno,Empaquetar)
{
	srand(time(NULL));
/*
	{

		NodoInterno ni,ni2;
		NodoArbol na(TIPO_HOJA);
		Bloque* b= new Bloque;

		const unsigned short CANTIDAD_CLAVES= 5;
		string datos[CANTIDAD_CLAVES]= {"ante","paraiso","demoledor","extraterrestre","invasion"};
		ClaveX claves[CANTIDAD_CLAVES];
		for(unsigned short i=0;i<CANTIDAD_CLAVES;i++)
			claves[i].set_clave(datos[i]);

		const unsigned short CANTIDAD_HIJOS= CANTIDAD_CLAVES+1;
		TipoHijo hijos[CANTIDAD_HIJOS];
		for(unsigned short i=0;i<CANTIDAD_HIJOS;i++ , hijos[i]=rand()%32);
		ASSERT_TRUE( ni.modificar_hijo(hijos[0],0)== RES_OK ) ;


		for(unsigned short i=0;i<CANTIDAD_CLAVES;i++){
			unsigned short posicionOcurrencia= 0;
			ni.insertar_clave(claves[i],posicionOcurrencia);
			ASSERT_TRUE(ni.get_cantidad_claves()== i+1);
			ASSERT_TRUE( ni.insertar_hijo_derecho(claves[i],hijos[i+1])== RES_OK );
		}

		ASSERT_TRUE( ni.empaquetar(b)==RES_OK );
		delete b;


	}//empaquetar y desempaquetar un nodo correctamente
*/


	NodoInterno ni;
	NodoInterno ni2;

	const unsigned short CANTIDAD_DATOS= 5;
	string datoAcumulado= "d";
	for(int i=0;i<CANTIDAD_DATOS;i++){

		datoAcumulado+= "a";
		string dato= datoAcumulado;
		ClaveX clave;
		clave.set_clave(dato);
		unsigned short ocurrencia;
		ni.insertar_clave(clave,ocurrencia);
		ASSERT_TRUE( ni.insertar_hijo_derecho(clave,(TipoHijo)ocurrencia) == RES_OK);

	}

	Bloque b;
	ASSERT_TRUE( ni.empaquetar(&b)== RES_OK );
	ASSERT_TRUE( ni2.desempaquetar(&b)== RES_OK );

	ASSERT_TRUE( ni2.get_cantidad_claves() == ni.get_cantidad_claves() );
	ASSERT_TRUE( ni2.get_cantidad_hijos()== ni.get_cantidad_hijos() );

	for(unsigned short i=0;i<CANTIDAD_DATOS;i++){
		ClaveX c1,c2;
		ni.get_clave(i,c1);
		ni2.get_clave(i,c2);
		ASSERT_TRUE( c1==c2 );

		TipoHijo h1,h2;
		ni.get_hijo_derecho(h1,c1);
		ni2.get_hijo_derecho(h2,c2);
		ASSERT_TRUE( h1==h2 );
		ni.get_hijo(h1,i+1);
		ni2.get_hijo(h2,i+1);
		ASSERT_TRUE(h1==h2);
	}

	NodoArbol na(TIPO_HOJA);
	ASSERT_TRUE( na.desempaquetar(&b)== RES_OK );
	ASSERT_TRUE( na.es_interno() );
}
