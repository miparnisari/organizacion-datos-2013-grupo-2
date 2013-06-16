#include "../src/CapaFisica/ManejadorRegistrosVariables.h"
#include "../src/CapaLogica/ManejoArchivos/RegistroClave.h"
#include "../src/CapaLogica/ManejoArchivos/ClaveX.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestManejadorRegistrosVariables : public testing::Test {
 protected:
	// Declares the variables your tests want to use.
	ManejadorRegistrosVariables mrv;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	  ASSERT_TRUE(mrv.crear_archivo("mrv.dat") == RES_OK);
  }

  virtual void TearDown() {
	  ASSERT_TRUE(mrv.eliminar_archivo("mrv.dat") == RES_OK);
  }

};

TEST_F(TestManejadorRegistrosVariables,Refactorizar)
{
	{
		const unsigned short CANTIDAD_REGISTROS_INICIAL= 20;
		for(unsigned short i=0;i<CANTIDAD_REGISTROS_INICIAL;i++){

			string claveAcumulada= "clavea";
			claveAcumulada[claveAcumulada.length()-1]= (char)(65+i);
			ClaveX clave;
			clave.set_clave(claveAcumulada);
			RegistroClave registro;
			registro.set_clave(clave);

			mrv.agregar_registro(&registro);

		}//registros agregados
		ASSERT_TRUE( mrv.get_cantidad_registros()== CANTIDAD_REGISTROS_INICIAL );

		for(unsigned short i=0;i< (short)(CANTIDAD_REGISTROS_INICIAL/2);i++ ){

			mrv.eliminar_registro_ocupado(0);
			ASSERT_TRUE( mrv.get_cantidad_registros_ocupados() == (CANTIDAD_REGISTROS_INICIAL-i-1) );
			ASSERT_TRUE( mrv.get_cantidad_registros()== CANTIDAD_REGISTROS_INICIAL );

		}

		ASSERT_TRUE( mrv.refactorizar()== RES_OK );

		for(unsigned short i=0;i< mrv.get_cantidad_registros_ocupados();i++ ){

			RegistroClave unRegistro;
			mrv.get_registro_ocupado(&unRegistro,i);
			ClaveX clave;
			clave= unRegistro.get_clave();
//			clave.imprimir_dato();
			cout<<endl;

		}




	}

	{
		string nombreArchivo= "test_refactorizar.dat";
		ManejadorRegistrosVariables mrv;
		mrv.eliminar_archivo(nombreArchivo);
		mrv.crear_archivo(nombreArchivo);

		const unsigned short CANTIDAD_REGISTROS_INICIAL= 20;
		for(unsigned short i=0;i<CANTIDAD_REGISTROS_INICIAL;i++){

			string claveAcumulada= "clavea";
			claveAcumulada[claveAcumulada.length()-1]= (char)(65+i);
			ClaveX clave;
			clave.set_clave(claveAcumulada);
			RegistroClave registro;
			registro.set_clave(clave);

			mrv.agregar_registro(&registro);

		}//registros agregados
		ASSERT_TRUE( mrv.get_cantidad_registros()== CANTIDAD_REGISTROS_INICIAL );

		for(unsigned short i=0;i< (short)(CANTIDAD_REGISTROS_INICIAL/2);i++ ){

			mrv.eliminar_registro_ocupado(i);
			ASSERT_TRUE( mrv.get_cantidad_registros_ocupados() == (CANTIDAD_REGISTROS_INICIAL-i-1) );
			ASSERT_TRUE( mrv.get_cantidad_registros()== CANTIDAD_REGISTROS_INICIAL );

		}

		ASSERT_TRUE( mrv.refactorizar()== RES_OK );

		for(unsigned short i=0;i< mrv.get_cantidad_registros_ocupados();i++ ){

			RegistroClave unRegistro;
			mrv.get_registro_ocupado(&unRegistro,i);
			ClaveX clave;
			clave= unRegistro.get_clave();
//			clave.imprimir_dato();
//			cout<<endl;

		}

		mrv.eliminar_archivo("test_refactorizar.dat");

	}

	{
			string nombreArchivo= "test_refactorizar.dat";
			ManejadorRegistrosVariables mrv;
			mrv.eliminar_archivo(nombreArchivo);
			mrv.crear_archivo(nombreArchivo);

			const unsigned short CANTIDAD_REGISTROS_INICIAL= 20;
			for(unsigned short i=0;i<CANTIDAD_REGISTROS_INICIAL;i++){

				string claveAcumulada= "clavea";
				claveAcumulada[claveAcumulada.length()-1]= (char)(65+i);
				ClaveX clave;
				clave.set_clave(claveAcumulada);
				RegistroClave registro;
				registro.set_clave(clave);

				mrv.agregar_registro(&registro);

			}//registros agregados
			ASSERT_TRUE( mrv.get_cantidad_registros()== CANTIDAD_REGISTROS_INICIAL );

			for(unsigned short i=0;i< (short)(CANTIDAD_REGISTROS_INICIAL/2);i++ ){

				mrv.eliminar_registro_ocupado( mrv.get_cantidad_registros_ocupados()-1 );
				ASSERT_TRUE( mrv.get_cantidad_registros_ocupados() == (CANTIDAD_REGISTROS_INICIAL-i-1) );
				ASSERT_TRUE( mrv.get_cantidad_registros()== CANTIDAD_REGISTROS_INICIAL );

			}

			ASSERT_TRUE( mrv.refactorizar()== RES_OK );

			for(unsigned short i=0;i< mrv.get_cantidad_registros_ocupados();i++ ){

				RegistroClave unRegistro;
				mrv.get_registro_ocupado(&unRegistro,i);
				ClaveX clave;
				clave= unRegistro.get_clave();
//				clave.imprimir_dato();
//				cout<<endl;

			}
		}
}


TEST_F(TestManejadorRegistrosVariables,Recuperar_por_offset)
{
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
		int unCampo= 99;
		registro.agregar_campo( (char*)&unCampo,sizeof(unCampo) );

	}

	mrv.eliminar_registro_ocupado(1);
	for(unsigned short i=0;i<CANTIDAD_REGISTROS;i++){

		if(i== 1)
			continue;
		RegistroClave registro;
		mrv.get_registro_por_offset( &registro,offsets[i] );

	}


	delete[] offsets;
}

TEST_F(TestManejadorRegistrosVariables,Contar_registros)
{
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
		ASSERT_TRUE( mrv.get_cantidad_registros()== CANTIDAD_REGISTROS_INICIAL && mrv.get_cantidad_registros_ocupados()== CANTIDAD_REGISTROS_INICIAL );

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
		ASSERT_TRUE( mrv.get_cantidad_registros()== (CANTIDAD_REGISTROS_ELIMINAR+CANTIDAD_REGISTROS_INICIAL) );
		ASSERT_TRUE( mrv.get_cantidad_registros_ocupados()== CANTIDAD_REGISTROS_INICIAL );

	}

}

TEST_F(TestManejadorRegistrosVariables,Insertar_masivo)
{
	string datoLargo= "este es un dato de longitud muy muy larga . Como para que ocupe mucho espacio en el archivo";
	string datoCorto= "corto";

	RegistroVariable registroLargo , registroCorto;
	registroLargo.agregar_campo( datoLargo.c_str() , datoLargo.length() );
	registroCorto.agregar_campo( datoCorto.c_str() , datoCorto.length() );

	int const CANTIDAD_REGISTROS= 100;
	long tamanioInicial;


	{
		for(int i=0;i<CANTIDAD_REGISTROS;i++){
			ASSERT_TRUE( mrv.agregar_registro(&registroLargo)!= RES_ERROR );
		}
		tamanioInicial= mrv.get_tamanio_archivo();
	}//carga de registros largos

	{
		for(int i=0;i<CANTIDAD_REGISTROS;i++)
			ASSERT_TRUE( mrv.eliminar_registro_ocupado(0)!= RES_ERROR );
		ASSERT_TRUE( mrv.get_tamanio_archivo()== tamanioInicial );

		for(int i=0;i<CANTIDAD_REGISTROS;i++)
			ASSERT_TRUE( mrv.agregar_registro(&registroCorto) < tamanioInicial );
		ASSERT_TRUE( mrv.get_cantidad_registros_ocupados()== CANTIDAD_REGISTROS );
	}
}

TEST_F(TestManejadorRegistrosVariables,Recuperar_espacio_libre)
{
	const int CANT_DATOS= 6;
	string datos[] = {"aaaaaa","bbbbbbbbbbbb",
			"cccccccccccccccccc","dddddddddddddddddddddddd",
			"eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
			"ffffffffffffffffffffffffffffffffffffffffff"};

	int offsetsRegistros[CANT_DATOS];
	ASSERT_TRUE( mrv.get_cantidad_registros_ocupados()== 0 );
	RegistroVariable registros[CANT_DATOS];
	int tamanioInicial;

	{

		for(int i=0;i<CANT_DATOS;i++){
			offsetsRegistros[i]= mrv.get_tamanio_archivo();
			registros[i].agregar_campo( datos[i].c_str() , datos[i].length() );
			mrv.agregar_registro( &registros[i] );
		}
		ASSERT_TRUE( mrv.get_cantidad_registros_ocupados()== CANT_DATOS );
		tamanioInicial= mrv.get_tamanio_archivo();

	}

	{

		ASSERT_TRUE( mrv.eliminar_registro_ocupado(5)== offsetsRegistros[5] );
		ASSERT_TRUE( mrv.agregar_registro( &registros[0] )>= offsetsRegistros[5] );
		ASSERT_TRUE( mrv.get_tamanio_archivo() == tamanioInicial );
		ASSERT_TRUE( mrv.agregar_registro( &registros[0] )== tamanioInicial );
		ASSERT_TRUE( mrv.get_tamanio_archivo() > tamanioInicial );
		/*el archivo se fragmenta a medida que se agregan registros y se busca
		 * recuperar el espacio libre*/

	}

	{

		ASSERT_TRUE( mrv.eliminar_registro_ocupado( 1 )== offsetsRegistros[1] );
		RegistroVariable rv;
		ASSERT_TRUE( mrv.get_registro_ocupado( &rv,1 )== offsetsRegistros[2] );
		char campo[64];
		int tamanioCampo= rv.recuperar_campo(campo,0);
		string s(campo,tamanioCampo);
		ASSERT_TRUE( s== datos[2] );

	}

	{
		for(int i=0;i<3;i++){
			ASSERT_TRUE( mrv.eliminar_registro_ocupado(1)== offsetsRegistros[2+i] );
		}
		for(int i=4;i<=2;i--){
			ASSERT_TRUE( mrv.agregar_registro( &registros[0] ) >= offsetsRegistros[i] );
		}



	}

	ManejadorRegistrosVariables mrv2;
	std::string nombreArchivo= "tmrv4.dat";
	mrv2.eliminar_archivo(nombreArchivo);
	mrv2.crear_archivo(nombreArchivo);
	ASSERT_TRUE( mrv2.get_cantidad_registros_ocupados()== 0 );
	RegistroVariable registros2[CANT_DATOS];
	int tamanioInicial2;

	{

		for(int i=0;i<CANT_DATOS;i++){
			registros2[i].agregar_campo( datos[i].c_str() , datos[i].length() );
			mrv2.agregar_registro( &registros2[i] );
		}
		ASSERT_TRUE( mrv2.get_cantidad_registros_ocupados()== CANT_DATOS );
		tamanioInicial2= mrv2.get_tamanio_archivo();

	}

	{

		for(int i=0;i<CANT_DATOS;i++){
			ASSERT_TRUE( mrv2.eliminar_registro_ocupado(0)== offsetsRegistros[i] );
		}
		RegistroVariable rv;
		rv.agregar_campo("x",1);
		for(int i=0;i<CANT_DATOS-1;i++){
			ASSERT_TRUE( mrv2.agregar_registro(&rv)< tamanioInicial2 );
		}
		/*el archivo se fragmenta de forma considerable*/

	}

	mrv2.eliminar_archivo(nombreArchivo);
}

TEST_F(TestManejadorRegistrosVariables,Eliminar_por_offset)
{
	const int CANT_DATOS= 4;
	string datos[] = {"aaaa","bbbbbbbb","cccccccccccc","dddddddddddddddd"};
	int offsetsRegistros[CANT_DATOS];
	ASSERT_TRUE( mrv.get_cantidad_registros_ocupados()== 0 );
	RegistroVariable registros[CANT_DATOS];


	for(int i=0;i<CANT_DATOS;i++){
		offsetsRegistros[i]= mrv.get_tamanio_archivo();
		registros[i].agregar_campo( datos[i].c_str() , datos[i].length() );
		mrv.agregar_registro( &registros[i] );
	}
	ASSERT_TRUE( mrv.get_cantidad_registros_ocupados()== CANT_DATOS );

	ASSERT_TRUE( mrv.eliminar_registro_por_offset( offsetsRegistros[3] )== RES_OK );
	ASSERT_TRUE( mrv.get_cantidad_registros_ocupados()== (CANT_DATOS - 1) );
	ASSERT_TRUE( mrv.agregar_registro( &registros[0] ) > offsetsRegistros[3] );
	ASSERT_TRUE( mrv.get_cantidad_registros_ocupados() == CANT_DATOS );
	ASSERT_TRUE( mrv.get_cantidad_registros()== CANT_DATOS + 1 );
}

TEST_F(TestManejadorRegistrosVariables,Chequear_registros_ocupados)
{
	const int CANT_DATOS= 4;
	string datos[] = {"aaaa","bbbbbbbb","cccccccccccc","dddddddddddddddd"};

	int offsetsRegistros[CANT_DATOS];
	ASSERT_TRUE( mrv.get_cantidad_registros_ocupados()== 0 );
	RegistroVariable registros[CANT_DATOS];


	{

		for(int i=0;i<CANT_DATOS;i++){
			offsetsRegistros[i]= mrv.get_tamanio_archivo();
			registros[i].agregar_campo( datos[i].c_str() , datos[i].length() );
			mrv.agregar_registro( &registros[i] );
		}
		ASSERT_TRUE( mrv.get_cantidad_registros_ocupados()== CANT_DATOS );

		ASSERT_TRUE( mrv.eliminar_registro_ocupado( 0 )== offsetsRegistros[0] );
		RegistroVariable rv;
		char campo[32];
		ASSERT_TRUE( mrv.get_registro_ocupado(&rv,0)== offsetsRegistros[1] );
		int tamanioCampo= rv.recuperar_campo(campo,0);
		string s(campo,tamanioCampo);
		ASSERT_TRUE(s==datos[1]);

	}

	{

		ASSERT_TRUE( mrv.eliminar_registro_ocupado( 0 )== offsetsRegistros[1] );
		RegistroVariable rv;
		char campo[32];
		ASSERT_TRUE( mrv.get_registro_ocupado(&rv,0)== offsetsRegistros[2] );
		int tamanioCampo= rv.recuperar_campo(campo,0);
		string s(campo,tamanioCampo);
		ASSERT_TRUE(s==datos[2]);

	}

	{

		ASSERT_TRUE( mrv.eliminar_registro_ocupado( 0 )== offsetsRegistros[2] );
		ASSERT_TRUE( mrv.get_cantidad_registros_ocupados()== 1 );
		ASSERT_TRUE( mrv.eliminar_registro_ocupado( 0 )== offsetsRegistros[3] );
		ASSERT_TRUE( mrv.get_cantidad_registros_ocupados()== 0 );
		ASSERT_TRUE( mrv.agregar_registro(&registros[0])>= offsetsRegistros[3] );
		long offsetAgregarRegistro= mrv.agregar_registro(&registros[0]);
		ASSERT_TRUE( offsetAgregarRegistro>= offsetsRegistros[2] &&
				offsetAgregarRegistro < offsetsRegistros[3]);

	}

}

TEST_F(TestManejadorRegistrosVariables,Agregar_registro_clave)
{
	const unsigned short CANTIDAD_REGISTROS_ORIGINAL= 10;
	for(unsigned short i=0;i<CANTIDAD_REGISTROS_ORIGINAL;i++){
		string claveS= "clave";
		claveS[claveS.length()-1]= (char)(65+i);
		ClaveX clave;
		clave.set_clave(claveS);
		RegistroClave registro;
		registro.set_clave(clave);
		ASSERT_TRUE( mrv.agregar_registro(&registro)!= RES_ERROR );
	}

	RegistroClave primerRegistro;
	ASSERT_TRUE( mrv.get_registro_ocupado(&primerRegistro,0)!= RES_ERROR );
	primerRegistro.get_clave().imprimir_dato();
}
