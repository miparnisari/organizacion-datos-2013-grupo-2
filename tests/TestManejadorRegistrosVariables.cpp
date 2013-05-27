/*
 * TestManejadorRegistrosVariables.cpp
 *
 *  Created on: May 5, 2013
 *      Author: maine
 */

#include "TestManejadorRegistrosVariables.h"

TestManejadorRegistrosVariables::TestManejadorRegistrosVariables()
	: Test()
{
}

TestManejadorRegistrosVariables::~TestManejadorRegistrosVariables()
{
}

void TestManejadorRegistrosVariables::ejecutar()
{
	test_chequear_registros_ocupados();
	test_eliminar_por_offset();
	test_recuperar_espacio_libre();
	test_masivo();
	test_contar_registros();
	test_recuperar_por_offset();
}


void TestManejadorRegistrosVariables::test_recuperar_por_offset(){

	{
		ManejadorRegistrosVariables mrv;
		string nombreArchivo= "test_recuperar_por_offset.dat";
		mrv.eliminar_archivo(nombreArchivo);
		mrv.crear_archivo(nombreArchivo);


		const unsigned short CANTIDAD_REGISTROS= 3;
		long* offsets= new long[CANTIDAD_REGISTROS];
		for(unsigned short i=0;i<CANTIDAD_REGISTROS;i++){
			ClaveX clave;
			string claveS= "clavea";
			claveS[ claveS.length()-1 ]= (char)(65+i);
			clave.set_clave(claveS);
			RegistroClave registro;
			registro.set_clave(clave);
			offsets[i]= mrv.agregar_registro( &registro );
		}

		for(unsigned short i=0;i<CANTIDAD_REGISTROS;i++){

			RegistroClave registro;
			mrv.get_registro_por_offset( &registro,offsets[i] );
			registro.get_clave().imprimir_dato();
			int unCampo= 99;
			registro.agregar_campo( (char*)&unCampo,sizeof(unCampo) );
			cout<<endl;

		}

		mrv.eliminar_registro_ocupado(1);
		for(unsigned short i=0;i<CANTIDAD_REGISTROS;i++){

			if(i== 1)
				continue;
			RegistroClave registro;
			mrv.get_registro_por_offset( &registro,offsets[i] );
			registro.get_clave().imprimir_dato();
			cout<<endl;

		}


		delete[] offsets;

	}
	cout<<endl<<"exito en test_recuperar_por_offset"<<endl;

}



void TestManejadorRegistrosVariables::test_contar_registros(){

	ManejadorRegistrosVariables mrv;
	string nombreArchivo= "test_contar_registros_mrv.dat";
	mrv.eliminar_archivo(nombreArchivo);
	assert(mrv.crear_archivo(nombreArchivo)== RES_OK);

	const unsigned short CANTIDAD_REGISTROS_INICIAL= 10;
	const unsigned short CANTIDAD_REGISTROS_ELIMINAR= 3;

	{
		for(unsigned short i=0;i<CANTIDAD_REGISTROS_INICIAL;i++){
			string dato= "datoCuyoNombreResultaSerExtremadamenteLargoYTediosoDeEscribira";
			const unsigned short LONGITUD_DATO= dato.length();
			dato[ LONGITUD_DATO-1 ]= (char)(65+i);

			RegistroVariable registro;
			registro.agregar_campo( dato.c_str() , dato.length() );
			mrv.agregar_registro(&registro);
		}
		assert( mrv.get_cantidad_registros()== CANTIDAD_REGISTROS_INICIAL && mrv.get_cantidad_registros_ocupados()== CANTIDAD_REGISTROS_INICIAL );

	}


	{

		for(unsigned short i=0;i<CANTIDAD_REGISTROS_ELIMINAR;i++){
			mrv.eliminar_registro_ocupado(i+3);

			string dato= "datoa";
			const unsigned short LONGITUD_DATO= dato.length();
			dato[ LONGITUD_DATO-1 ]= (char)(65+i);

			RegistroVariable registro;
			registro.agregar_campo( dato.c_str() , dato.length() );
			mrv.agregar_registro(&registro);
		}
		assert( mrv.get_cantidad_registros()== (CANTIDAD_REGISTROS_ELIMINAR+CANTIDAD_REGISTROS_INICIAL) );
		assert( mrv.get_cantidad_registros_ocupados()== CANTIDAD_REGISTROS_INICIAL );

	}

}

void TestManejadorRegistrosVariables::test_masivo(){

	string datoLargo= "este es un dato de longitud muy muy larga . Como para que ocupe mucho espacio en el archivo";
	string datoCorto= "corto";

	RegistroVariable registroLargo , registroCorto;
	registroLargo.agregar_campo( datoLargo.c_str() , datoLargo.length() );
	registroCorto.agregar_campo( datoCorto.c_str() , datoCorto.length() );

	int const CANTIDAD_REGISTROS= 100;
	ManejadorRegistrosVariables mrv;
	string nombreArchivo= "tmrvm.dat";
	mrv.eliminar_archivo(nombreArchivo);
	mrv.crear_archivo(nombreArchivo);
	long tamanioInicial;


	{
		for(int i=0;i<CANTIDAD_REGISTROS;i++){
			assert( mrv.agregar_registro(&registroLargo)!= RES_ERROR );
		}
		tamanioInicial= mrv.get_tamanio_archivo();
	}//carga de registros largos

	{
		for(int i=0;i<CANTIDAD_REGISTROS;i++)
			assert( mrv.eliminar_registro_ocupado(0)!= RES_ERROR );
		assert( mrv.get_tamanio_archivo()== tamanioInicial );

		for(int i=0;i<CANTIDAD_REGISTROS;i++)
			assert( mrv.agregar_registro(&registroCorto) < tamanioInicial );
		assert( mrv.get_cantidad_registros_ocupados()== CANTIDAD_REGISTROS );
	}

	print_test_ok("test_manejador_registros_variables_masivo");


}

void TestManejadorRegistrosVariables::test_recuperar_espacio_libre()
{
	const int CANT_DATOS= 6;
	string datos[] = {"aaaaaa","bbbbbbbbbbbb",
			"cccccccccccccccccc","dddddddddddddddddddddddd",
			"eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
			"ffffffffffffffffffffffffffffffffffffffffff"};
	ManejadorRegistrosVariables mrv;
	string nombreArchivo= "tmrv3.dat";
	mrv.eliminar_archivo(nombreArchivo);
	mrv.crear_archivo(nombreArchivo);
	int offsetsRegistros[CANT_DATOS];
	assert( mrv.get_cantidad_registros_ocupados()== 0 );
	RegistroVariable registros[CANT_DATOS];
	int tamanioInicial;

	{

		for(int i=0;i<CANT_DATOS;i++){
			offsetsRegistros[i]= mrv.get_tamanio_archivo();
			registros[i].agregar_campo( datos[i].c_str() , datos[i].length() );
			mrv.agregar_registro( &registros[i] );
		}
		assert( mrv.get_cantidad_registros_ocupados()== CANT_DATOS );
		tamanioInicial= mrv.get_tamanio_archivo();

	}

	{

		assert( mrv.eliminar_registro_ocupado(5)== offsetsRegistros[5] );
		assert( mrv.agregar_registro( &registros[0] )>= offsetsRegistros[5] );
		assert( mrv.get_tamanio_archivo() == tamanioInicial );
		assert( mrv.agregar_registro( &registros[0] )== tamanioInicial );
		assert( mrv.get_tamanio_archivo() > tamanioInicial );
		/*el archivo se fragmenta a medida que se agregan registros y se busca
		 * recuperar el espacio libre*/

	}

	{

		assert( mrv.eliminar_registro_ocupado( 1 )== offsetsRegistros[1] );
		RegistroVariable rv;
		assert( mrv.get_registro_ocupado( &rv,1 )== offsetsRegistros[2] );
		char campo[64];
		int tamanioCampo= rv.recuperar_campo(campo,0);
		string s(campo,tamanioCampo);
		assert( s== datos[2] );

	}

	{
		for(int i=0;i<3;i++){
			assert( mrv.eliminar_registro_ocupado(1)== offsetsRegistros[2+i] );
		}
		for(int i=4;i<=2;i--){
			assert( mrv.agregar_registro( &registros[0] ) >= offsetsRegistros[i] );
		}



	}

	ManejadorRegistrosVariables mrv2;
	nombreArchivo= "tmrv4.dat";
	mrv2.eliminar_archivo(nombreArchivo);
	mrv2.crear_archivo(nombreArchivo);
	assert( mrv2.get_cantidad_registros_ocupados()== 0 );
	RegistroVariable registros2[CANT_DATOS];
	int tamanioInicial2;

	{

		for(int i=0;i<CANT_DATOS;i++){
			registros2[i].agregar_campo( datos[i].c_str() , datos[i].length() );
			mrv2.agregar_registro( &registros2[i] );
		}
		assert( mrv2.get_cantidad_registros_ocupados()== CANT_DATOS );
		tamanioInicial2= mrv2.get_tamanio_archivo();

	}

	{

		for(int i=0;i<CANT_DATOS;i++){
			assert( mrv2.eliminar_registro_ocupado(0)== offsetsRegistros[i] );
		}
		RegistroVariable rv;
		rv.agregar_campo("x",1);
		for(int i=0;i<CANT_DATOS-1;i++){
			assert( mrv2.agregar_registro(&rv)< tamanioInicial2 );
		}
		/*el archivo se fragmenta de forma considerable*/

	}


}/*funcionaaaaa
TODO mas tests que verifiquen que el espacio libre es recuperado sin afectar la
integridad del archivo*/


void TestManejadorRegistrosVariables::test_eliminar_por_offset()
{

	const int CANT_DATOS= 4;
	string datos[] = {"aaaa","bbbbbbbb","cccccccccccc","dddddddddddddddd"};
	ManejadorRegistrosVariables mrv;
	string nombreArchivo= "tmrv3.dat";
	mrv.eliminar_archivo(nombreArchivo);
	mrv.crear_archivo(nombreArchivo);
	int offsetsRegistros[CANT_DATOS];
	assert( mrv.get_cantidad_registros_ocupados()== 0 );
	RegistroVariable registros[CANT_DATOS];


	for(int i=0;i<CANT_DATOS;i++){
		offsetsRegistros[i]= mrv.get_tamanio_archivo();
		registros[i].agregar_campo( datos[i].c_str() , datos[i].length() );
		mrv.agregar_registro( &registros[i] );
	}
	assert( mrv.get_cantidad_registros_ocupados()== CANT_DATOS );

	assert( mrv.eliminar_registro_por_offset( offsetsRegistros[3] )== RES_OK );
	assert( mrv.get_cantidad_registros_ocupados()== (CANT_DATOS - 1) );
	assert( mrv.agregar_registro( &registros[0] ) > offsetsRegistros[3] );
	assert( mrv.get_cantidad_registros_ocupados() == CANT_DATOS );
	assert( mrv.get_cantidad_registros()== CANT_DATOS + 1 );

	print_test_ok("test_manejador_registros_variables_eliminar_por_offset");


}


void TestManejadorRegistrosVariables::test_chequear_registros_ocupados(){

	const int CANT_DATOS= 4;
	string datos[] = {"aaaa","bbbbbbbb","cccccccccccc","dddddddddddddddd"};
	ManejadorRegistrosVariables mrv;
	string nombreArchivo= "tmrv3.dat";
	mrv.eliminar_archivo(nombreArchivo);
	mrv.crear_archivo(nombreArchivo);
	int offsetsRegistros[CANT_DATOS];
	assert( mrv.get_cantidad_registros_ocupados()== 0 );
	RegistroVariable registros[CANT_DATOS];


	{

		for(int i=0;i<CANT_DATOS;i++){
			offsetsRegistros[i]= mrv.get_tamanio_archivo();
			registros[i].agregar_campo( datos[i].c_str() , datos[i].length() );
			mrv.agregar_registro( &registros[i] );
		}
		assert( mrv.get_cantidad_registros_ocupados()== CANT_DATOS );

		assert( mrv.eliminar_registro_ocupado( 0 )== offsetsRegistros[0] );
		RegistroVariable rv;
		char campo[32];
		assert( mrv.get_registro_ocupado(&rv,0)== offsetsRegistros[1] );
		int tamanioCampo= rv.recuperar_campo(campo,0);
		string s(campo,tamanioCampo);
		assert(s==datos[1]);

	}

	{

		assert( mrv.eliminar_registro_ocupado( 0 )== offsetsRegistros[1] );
		RegistroVariable rv;
		char campo[32];
		assert( mrv.get_registro_ocupado(&rv,0)== offsetsRegistros[2] );
		int tamanioCampo= rv.recuperar_campo(campo,0);
		string s(campo,tamanioCampo);
		assert(s==datos[2]);

	}

	{

		assert( mrv.eliminar_registro_ocupado( 0 )== offsetsRegistros[2] );
		assert( mrv.get_cantidad_registros_ocupados()== 1 );
		assert( mrv.eliminar_registro_ocupado( 0 )== offsetsRegistros[3] );
		assert( mrv.get_cantidad_registros_ocupados()== 0 );
		assert( mrv.agregar_registro(&registros[0])>= offsetsRegistros[3] );
		long offsetAgregarRegistro= mrv.agregar_registro(&registros[0]);
		assert( offsetAgregarRegistro>= offsetsRegistros[2] &&
				offsetAgregarRegistro < offsetsRegistros[3]);

	}

	print_test_ok("test_manejador_registros_variables_chequear_registros_ocupados");

}
