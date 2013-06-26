#include "../src/CapaFisica/ManejadorRegistrosVariables.h"
#include "../src/Utilitarios/OrdenamientoExterno.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestOrdenamientoExterno : public testing::Test {
 protected:
		// Declares the variables your tests want to use.
		ManejadorRegistrosVariables mv;
		std::string filename;
		OrdenamientoExterno* ordenador;

  // virtual void SetUp() will be called before each test is run.  You
  // should define it if you need to initialize the varaibles.
  // Otherwise, this can be skipped.
  virtual void SetUp() {
	  ordenador = new OrdenamientoExterno(filename);
	  filename = "archivoOrdenamiento.dat";
	  mv.crear_archivo(filename);
	  mv.abrir_archivo(filename);
  }

  // TearDown() is invoked immediately after a test finishes.
  virtual void TearDown() {
	  mv.eliminar_archivo(filename);
	  delete ordenador;
  }

  // A helper function that some test uses.

};

TEST_F(TestOrdenamientoExterno,Generar_runs)
{

	while (mv.get_tamanio_archivo()<1024*3) //3kB
	{
		int clave=1;

		int campoClave = clave;

		RegistroVariable regVariable;

		regVariable.agregar_campo((char*)&campoClave,sizeof(int));

		mv.agregar_registro(& regVariable);
	}



	ordenador->_generar_runs();

	std::vector<string> runs = ordenador->_getVector();

	Heap heap;

	for (unsigned short i= 0; i< runs.size(); i++)
	{

		ManejadorRegistrosVariables mv2;
		assert(mv2.abrir_archivo(runs[i])==RES_OK);

		for (int j = 0; j < mv2.get_cantidad_registros_ocupados()-1; j++)
		{
			RegistroVariable rv1,rv2;

			mv2.get_registro_ocupado(&rv1,j);
			mv2.get_registro_ocupado(&rv2,j+1);

			assert(heap.comparar_registros_variables(rv1,rv2)<=0);
		}
		mv2.eliminar_archivo(runs[i]);
	}

}

TEST_F(TestOrdenamientoExterno,Merge_runs)
{
	while (mv.get_tamanio_archivo()<1024*3) //3kB
	{
		int clave=1;
		clave = (rand() % 20);
//
//		stringstream conversor;
//		conversor << clave;
//		string campoClave= conversor.str();

		RegistroVariable regVariable;

		regVariable.agregar_campo((char*)&clave,sizeof(int));

		mv.agregar_registro(& regVariable);
	}

	ordenador->_generar_runs();

	ordenador->_merge();

	Heap heap;

	for (int i = 0; i < mv.get_cantidad_registros_ocupados()-1; i++)
	{
		RegistroVariable rv1,rv2;

		mv.get_registro_ocupado(&rv1,i);
		mv.get_registro_ocupado(&rv2,i+1);

		assert(heap.comparar_registros_variables(rv1,rv2)<=0);
	}

}

TEST_F(TestOrdenamientoExterno,Ordenar)
{
	while (mv.get_tamanio_archivo()<1024*3) //3kB
	{
		//agrego clave en el primer campo
		int clave=0;
		clave = (rand() % 20) + 65;
//
//		stringstream conversor;
//		conversor << clave;
//		string campoClave= conversor.str();

		RegistroVariable regVariable;

		regVariable.agregar_campo((char*)&clave,sizeof(int));

		//agrego clave en el segundo campo
		int clave2=0;
		clave2 = (rand() % 20) + 65;

//		conversor << clave2;
//		string campoClave2= conversor.str();

		regVariable.agregar_campo((char*)&clave2,sizeof(int));

		mv.agregar_registro(& regVariable);
	}

	ordenador->ordenar_archivo();

	Heap heap;

	for (int i = 0; i < mv.get_cantidad_registros_ocupados()-1; i++)
	{
		RegistroVariable rv1,rv2;

		mv.get_registro_ocupado(&rv1,i);
		mv.get_registro_ocupado(&rv2,i+1);

		assert(heap.comparar_registros_variables(rv1,rv2)<=0);
	}

}
