#include "../src/CapaFisica/ManejadorRegistrosVariables.h"
#include "../src/Utilitarios/OrdenamientoExterno.h"
#include "../lib/gtest-1.6.0/include/gtest/gtest.h"

// To use a test fixture, derive a class from testing::Test.
class TestOrdenamientoExterno : public testing::Test {
 protected:
	ManejadorRegistrosVariables mv;
	std::string nombre_archivo;
	Heap heap;

  virtual void SetUp() {
	  nombre_archivo = "archivo_a_ordenar.dat";
	  mv.crear_archivo(nombre_archivo);
	  mv.abrir_archivo(nombre_archivo);
  }

  virtual void TearDown() {
	  mv.eliminar_archivo(nombre_archivo);
  }

  // A helper function that some test uses.
  void comparar_registros()
  {
	for (int i = 0; i < mv.get_cantidad_registros_ocupados()-1; i++)
	{
		RegistroVariable rv1,rv2;

		mv.get_registro_ocupado(&rv1,i);
		mv.get_registro_ocupado(&rv2,i+1);

		// El primero menor o igual al segundo
		ASSERT_TRUE(heap.comparar_registros_variables(rv1,rv2) <= 0);
	}
  }

};

TEST_F(TestOrdenamientoExterno,Generar_runs)
{
	RegistroVariable regVariable;

	while (mv.get_tamanio_archivo()<5*1024) // 5 kB
	{
		int clave=(rand() % 20) +65;

		regVariable.limpiar_campos();

		regVariable.agregar_campo((char*)&clave,sizeof(clave));

		mv.agregar_registro(& regVariable);
	}

	OrdenamientoExterno ordenador (nombre_archivo);
	ordenador._generar_runs();

	std::vector<string> runs = ordenador._getVector();

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

			ASSERT_TRUE(heap.comparar_registros_variables(rv1,rv2)<=0);
		}
		mv2.eliminar_archivo(runs[i]);
	}

}

TEST_F(TestOrdenamientoExterno,Merge_runs)
{
	RegistroVariable regVariable;
	while (mv.get_tamanio_archivo()<2*1024) // 2 kB
	{
		int clave = (rand() % 20) +65;

		regVariable.limpiar_campos();

		regVariable.agregar_campo((char*)&clave,sizeof(clave));

		mv.agregar_registro(& regVariable);
	}

	OrdenamientoExterno ordenador (nombre_archivo);
	ordenador.ordenar_archivo();

	comparar_registros();
}

TEST_F(TestOrdenamientoExterno,Ordenar)
{
	RegistroVariable regVariable;
	while (mv.get_tamanio_archivo()<2*1024) // 2 kB
	{
		//agrego clave en el primer campo
		int clave = (rand() % 20) + 65;

		regVariable.limpiar_campos();

		regVariable.agregar_campo((char*)&clave,sizeof(clave));

		//agrego clave en el segundo campo
		int clave2 = (rand() % 20) + 65;

		regVariable.agregar_campo((char*)&clave2,sizeof(clave2));

		mv.agregar_registro(& regVariable);
	}
	const int CANTIDAD_REGISTROS = mv.get_cantidad_registros_ocupados();

	OrdenamientoExterno ordenador (nombre_archivo);
	ordenador.ordenar_archivo();
	ASSERT_EQ(mv.get_cantidad_registros_ocupados(),CANTIDAD_REGISTROS);
	comparar_registros();
}

TEST_F(TestOrdenamientoExterno,Ordenar_3812_registros)
{
	RegistroVariable regVariable;
	const int CANTIDAD_REGISTROS = 3812;
	for (int i = 0; i < CANTIDAD_REGISTROS; i++)
	{
		//agrego clave en el primer campo
		int clave = (rand() % 20) + 65;

		regVariable.limpiar_campos();

		regVariable.agregar_campo((char*)&clave,sizeof(clave));

		//agrego clave en el segundo campo
		int clave2 = (rand() % 20) + 65;

		regVariable.agregar_campo((char*)&clave2,sizeof(clave2));

		mv.agregar_registro(& regVariable);
	}

	OrdenamientoExterno ordenador (nombre_archivo);
	ordenador.ordenar_archivo();
	ASSERT_EQ(mv.get_cantidad_registros_ocupados(),CANTIDAD_REGISTROS);
	comparar_registros();
}

TEST_F(TestOrdenamientoExterno,Ordenar_2588_registros)
{
	RegistroVariable regVariable;
	const int CANTIDAD_REGISTROS = 2588;
	for (int i = 0; i < CANTIDAD_REGISTROS; i++)
	{
		//agrego clave en el primer campo
		int clave = (rand() % 20) + 65;

		regVariable.limpiar_campos();

		regVariable.agregar_campo((char*)&clave,sizeof(clave));

		//agrego clave en el segundo campo
		int clave2 = (rand() % 20) + 65;

		regVariable.agregar_campo((char*)&clave2,sizeof(clave2));

		mv.agregar_registro(& regVariable);
	}

	OrdenamientoExterno ordenador (nombre_archivo);
	ordenador.ordenar_archivo();
	ASSERT_EQ(mv.get_cantidad_registros_ocupados(),CANTIDAD_REGISTROS);
	comparar_registros();
}

TEST_F(TestOrdenamientoExterno,Ordenar_1240_registros)
{
	RegistroVariable regVariable;
	const int CANTIDAD_REGISTROS = 1240;
	for (int i = 0; i < CANTIDAD_REGISTROS; i++)
	{
		//agrego clave en el primer campo
		int clave = (rand() % 20) + 65;

		regVariable.limpiar_campos();

		regVariable.agregar_campo((char*)&clave,sizeof(clave));

		//agrego clave en el segundo campo
		int clave2 = (rand() % 20) + 65;

		regVariable.agregar_campo((char*)&clave2,sizeof(clave2));

		mv.agregar_registro(& regVariable);
	}

	OrdenamientoExterno ordenador (nombre_archivo);
	ordenador.ordenar_archivo();
	ASSERT_EQ(mv.get_cantidad_registros_ocupados(),CANTIDAD_REGISTROS);
	comparar_registros();
}

TEST_F(TestOrdenamientoExterno,Ordenar_505_registros)
{
	RegistroVariable regVariable;
	const int CANTIDAD_REGISTROS = 505;
	for (int i = 0; i < CANTIDAD_REGISTROS; i++)
	{
		//agrego clave en el primer campo
		int clave = (rand() % 20) + 65;

		regVariable.limpiar_campos();

		regVariable.agregar_campo((char*)&clave,sizeof(clave));

		//agrego clave en el segundo campo
		int clave2 = (rand() % 20) + 65;

		regVariable.agregar_campo((char*)&clave2,sizeof(clave2));

		mv.agregar_registro(& regVariable);
	}

	const int CANTIDAD_REGISTROS_ANTES = mv.get_cantidad_registros_ocupados();
	OrdenamientoExterno ordenador (nombre_archivo);
	ordenador.ordenar_archivo();
	ASSERT_EQ(mv.get_cantidad_registros_ocupados(),CANTIDAD_REGISTROS_ANTES);
	comparar_registros();
}
