/*
 * TestNodoInterno.cpp
 *
 *  Created on: May 4, 2013
 *      Author: maine
 */

#include "TestNodoInterno.h"

TestNodoInterno::TestNodoInterno()
	:Test()
{
}

TestNodoInterno::~TestNodoInterno()
{
}

void TestNodoInterno::ejecutar()
{
	crear_nodo_con_demasiadas_claves_falla();
	insertar_hijos();
	todo_tipo_empaquetado();

	cout<<"OK: test_nodo_interno"<<endl;

}


void TestNodoInterno::insertar_hijos(){

	typedef NodoInterno::TipoHijo TipoHijo;
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
		assert( ni.insertar_hijo(99,2)== RES_ERROR );
		assert( ni.get_tamanio_ocupado()== (signed)TAMANIO_INICIAL);
		for(unsigned short i=0;i<2;i++)
			assert( ni.insertar_hijo(hijos[i],i+1)==RES_UNDERFLOW );
		assert( ni.insertar_hijo(hijos[2],3)== RES_OK );

		assert( ni.insertar_hijo(hijos[3],0)== RES_OK );

		NodoInterno ni2;
		for(unsigned short i=0;i<CANTIDAD_HIJOS;i++)
			ni2.insertar_hijo(hijos[i],0);

		for(unsigned short i=0;i<CANTIDAD_HIJOS;i++){

			unsigned short elHijo= CANTIDAD_HIJOS-1-i;
			TipoHijo unHijo;
			ni2.get_hijo(unHijo,i);
			assert( unHijo== hijos[elHijo] );

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
		assert( ni.get_hijo(unHijo,0) == RES_OK && unHijo== HIJO_INVALIDO );
		assert( ni.get_hijo(unHijo,1)== RES_ERROR );
		assert( ni.insertar_hijo_derecho(claves[0], hijos[0])== RES_ERROR );
		assert( ni.get_cantidad_claves()== CANTIDAD_CLAVES_INICIAL );
		assert( ni.get_cantidad_hijos()== CANTIDAD_HIJOS_INICIAL );
		assert( ni.modificar_hijo(hijos[0],0) == RES_OK );
		assert( ni.modificar_hijo(hijos[0],1) == RES_ERROR );


		for(unsigned short i=0;i<CANTIDAD_DATOS;i++){
			unsigned short us;
			assert( ni.insertar_clave(claves[i],us)==RES_OK );
			assert( ni.insertar_hijo(HIJO_INVALIDO)== RES_OK );

		}

		for(unsigned short i=0;i<CANTIDAD_DATOS;i++)
			assert( ni.modificar_hijo_derecho(claves[i],hijos[i+1])== RES_OK );
		for(unsigned short i=0;i<CANTIDAD_DATOS;i++){
			TipoHijo unHijo;
			assert( ni.get_hijo_derecho(unHijo , claves[i])== RES_OK );
			assert( unHijo== hijos[i+1] );
		}
		assert( ni.get_cantidad_claves()== CANTIDAD_DATOS &&
				ni.get_cantidad_hijos()== CANTIDAD_HIJOS );

		for(unsigned int i=0;i<CANTIDAD_DATOS;i++){
			ClaveX unaClave;
			ni.remover_clave(0,unaClave);
			ni.remover_hijo(0);
		}

		for(unsigned short i=0;i<CANTIDAD_DATOS;i++){
			unsigned short us;
			assert( ni.insertar_clave(claves[i],us)==RES_OK );
			assert( ni.insertar_hijo_derecho(claves[i]) == RES_OK );

		}

		for(unsigned short i=0;i<CANTIDAD_DATOS;i++)
			assert( ni.modificar_hijo_derecho(claves[i],hijos[i+1])== RES_OK );
		for(unsigned short i=0;i<CANTIDAD_DATOS;i++){
			TipoHijo unHijo;
			assert( ni.get_hijo_derecho(unHijo , claves[i])== RES_OK );
			assert( unHijo== hijos[i+1] );
		}
		assert( ni.get_cantidad_claves()== CANTIDAD_DATOS &&
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
			assert( unHijo== hijos[i] );
		}


	}

}


void TestNodoInterno::crear_nodo_con_demasiadas_claves_falla()
{
	NodoInterno nodoI(0,16);
	ClaveX clave1, clave2, clave3, clave4;
	clave1.set_clave("aca");
	clave2.set_clave("alla");
	clave3.set_clave("abba");
	clave4.set_clave("clave que no entra");

	unsigned short pos= 0;
	assert(nodoI.insertar_clave(clave1,pos)== RES_OK && pos==0);
	assert(nodoI.insertar_clave(clave2,pos)== RES_OK && pos==1);
	assert(nodoI.insertar_clave(clave3,pos)== RES_OVERFLOW && pos== 0);
	assert(nodoI.insertar_clave(clave4,pos)== RES_OVERFLOW && pos==3);
	assert(nodoI.get_cantidad_claves() == 4);

	assert(nodoI.remover_clave(clave1,pos)== RES_OVERFLOW);
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
			assert( ni.insertar_clave(claves[i],ocurrenciaInsercion)== RES_OK );
			assert( ocurrenciaInsercion== i );
			assert( ni.insertar_clave(claves[i],ocurrenciaInsercion)== RES_ERROR );
			assert( ni.get_tamanio_ocupado()== (signed)TAMANIO_INICIAL +
					claves[i].get_tamanio_empaquetado() * (i+1) );
		}

		unsigned short ocurrenciaInsercion= 0;
		ClaveX cx;
		cx.set_clave(1);
		assert( ni.insertar_clave(cx,ocurrenciaInsercion)== RES_OK );
		assert( ocurrenciaInsercion== 1 );

		cx.set_clave(3);
		assert( ni.insertar_clave(cx,ocurrenciaInsercion)== RES_OK );
		assert( ocurrenciaInsercion== 3 );

		ni.imprimir_claves();//TODO remover

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
		assert( ni.get_tamanio_ocupado()== TAMANIO_INICIAL +
				TAMANIO_EMPAQUETADO_CLAVE*2 );
		ni.insertar_hijo(hijos[0],1);



	}


	{

		cout<<endl<<"imprimirNodoInterno: "<<endl;
		NodoInterno ni;
		typedef NodoInterno::TipoHijo TipoHijo;
		string claveAcumulada= "dato";
		const unsigned short CANTIDAD_DATOS= 5;
		const unsigned short CANTIDAD_HIJOS= CANTIDAD_DATOS+1;
		ni.modificar_hijo(0,0);

		ni.imprimir_claves();
		ni.imprimir_hijos();

		for(unsigned short i=0;i<CANTIDAD_DATOS;i++){

			unsigned short ocurrenciaInsercion;
			ClaveX unaClave;
			unaClave.set_clave(claveAcumulada);
			assert( ni.insertar_clave(unaClave,ocurrenciaInsercion)== RES_OK );
			claveAcumulada+= (char)(67+i);
			assert( ni.insertar_hijo(i+1,i+1)== RES_OK );

		}assert(ni.get_cantidad_claves()== CANTIDAD_DATOS && ni.get_cantidad_hijos()== CANTIDAD_HIJOS);

		cout<<"imprimo despues de insertar  5 claves y 5 hijos"<<endl;
		ni.imprimir_claves();
		ni.imprimir_hijos();

		string datoClaveMitad= "datoCD";
		ClaveX claveMitad;
		claveMitad.set_clave(datoClaveMitad);
		ClaveX claveMitadNodo;
		ni.get_clave_mitad(claveMitadNodo);
		assert( claveMitad== claveMitadNodo );

		unsigned short numeroClaveMitad;
		ni.buscar_clave(claveMitad,numeroClaveMitad);
		vector<TipoHijo> hijosMover;
		vector<ClaveX> clavesMover;
		for(unsigned short i=numeroClaveMitad;i<CANTIDAD_DATOS;i++){

			ClaveX unaClave;
			assert( ni.remover_clave(numeroClaveMitad,unaClave)== RES_OK );
			clavesMover.push_back(unaClave);
			TipoHijo unHijo;
			ni.get_hijo(unHijo,numeroClaveMitad+1);
			hijosMover.push_back(unHijo);
			assert( ni.remover_hijo(numeroClaveMitad+1)== RES_OK );


		}/*pruebo remover la clave de la mitad y los hijos de la misma*/

		cout<<"imprimo despues de remover la clave mitad y los hijos posteriores de claveMitad"<<endl;
		ni.imprimir_claves();
		ni.imprimir_hijos();

		NodoInterno nodoNuevo;
		nodoNuevo.limpiar();
		nodoNuevo.insertar_hijo(hijosMover.at(0));

		for(unsigned short i=0; i< (CANTIDAD_DATOS-numeroClaveMitad); i++){

			unsigned short ocurrenciaInsercion;
			nodoNuevo.insertar_clave(clavesMover[i],ocurrenciaInsercion);
			nodoNuevo.insertar_hijo(hijosMover[i+1]);

		}

		cout<<"imprimo nodoNuevo"<<endl;
		nodoNuevo.imprimir_claves();
		nodoNuevo.imprimir_hijos();


		cout<<endl<<"fin imprimirNodoInterno"<<endl;

	}


}


void TestNodoInterno::todo_tipo_empaquetado(){

	typedef NodoInterno::TipoHijo TipoHijo;
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
		assert( ni.modificar_hijo(hijos[0],0)== RES_OK ) ;


		for(unsigned short i=0;i<CANTIDAD_CLAVES;i++){
			unsigned short posicionOcurrencia= 0;
			ni.insertar_clave(claves[i],posicionOcurrencia);
			assert(ni.get_cantidad_claves()== i+1);
			assert( ni.insertar_hijo_derecho(claves[i],hijos[i+1])== RES_OK );
		}

		assert( ni.empaquetar(b)==RES_OK );
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
		assert( ni.insertar_hijo_derecho(clave,(TipoHijo)ocurrencia) == RES_OK);

	}

	ni.imprimir_claves();
	Bloque b;
	assert( ni.empaquetar(&b)== RES_OK );
	assert( ni2.desempaquetar(&b)== RES_OK );

	assert( ni2.get_cantidad_claves() == ni.get_cantidad_claves() );
	assert( ni2.get_cantidad_hijos()== ni.get_cantidad_hijos() );

	for(unsigned short i=0;i<CANTIDAD_DATOS;i++){
		ClaveX c1,c2;
		ni.get_clave(i,c1);
		ni2.get_clave(i,c2);
		assert( c1==c2 );

		TipoHijo h1,h2;
		ni.get_hijo_derecho(h1,c1);
		ni2.get_hijo_derecho(h2,c2);
		assert( h1==h2 );
		ni.get_hijo(h1,i+1);
		ni2.get_hijo(h2,i+1);
		assert(h1==h2);
	}

	NodoArbol na(TIPO_HOJA);
	assert( na.desempaquetar(&b)== RES_OK );
	assert( na.es_interno() );


}



