/*
 * TestArbol.cpp
 *
 *  Created on: May 16, 2013
 *      Author: martin
 */

#include "TestArbol.h"

typedef NodoInterno::TipoHijo TipoHijo;

void TestArbol::ejecutar(){

	test_insertar_pocos_registros();

}


void TestArbol::test_insertar_pocos_registros(){

	ArbolBMas abm;
	header_arbol ha;
	ha.minCantBytesClaves= 0;
	ha.maxCantBytesClaves= 4096;
	string nombreArchivoArbol= "testArbol.dat";
	assert( abm.eliminar(nombreArchivoArbol)==RES_OK );

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

	ClaveX clavePromocionada;
	TipoHijo bloquePromocionado;
	abm._split_interno(&ni,&clavePromocionada,bloquePromocionado);
	cout<<"clave promocionada: ";clavePromocionada.imprimir_dato();cout<<endl;
	cout<<"bloque promocionado: "<<bloquePromocionado<<endl;

	assert(abm.crear(nombreArchivoArbol)== RES_OK);


}
