#include "TestNodoInterno.h"
#include "TestRegistroCancion.h"
#include "TestNodoSecuencial.h"
#include "TestHashingExtensible.h"
#include "TestManejadorBloques.h"
#include "TestManejadorRegistrosVariables.h"
#include "TestBloque.h"
#include "TestRegistroVariable.h"
#include "TestHeap.h"
#include "TestNodoSecuencial.h"
#include "TestRegistroClave.h"
#include "TestManejadorArchivoDatosBasicos.h"
#include "TestArbol.h"
#include "TestTabla.h"
#include "TestClave.h"
#include "TestOrdenamientoExterno.h"
#include "TestParserCanciones.h"
#include "TestIndiceInvertidoPorFrase.h"
#include "TestIndicePorAutor.h"
#include "TestIndicePorTitulo.h"
#include "TestArchivoListas.h"
#include "TestNormalizador.h"

int main(int argc,char** args)
{
	std::vector<Test*> tests;

	tests.push_back(new TestClave);
	tests.push_back(new TestNormalizador);

	tests.push_back(new TestManejadorRegistrosVariables);

	tests.push_back(new TestRegistroCancion);

	tests.push_back(new TestRegistroVariable);
	tests.push_back(new TestParserCanciones);
	tests.push_back(new TestManejadorArchivoDatosBasicos);

	tests.push_back(new TestRegistroClave);

	tests.push_back(new TestBloque);
	tests.push_back(new TestManejadorBloques);

	tests.push_back(new TestHashingExtensible);
	tests.push_back(new TestTabla);

	tests.push_back(new TestNodoInterno);
	tests.push_back(new TestNodoSecuencial);

	tests.push_back(new TestHeap);
	tests.push_back(new TestOrdenamientoExterno);

	tests.push_back(new TestArbol);

//	tests.push_back(new TestIndicePorAutor);
	tests.push_back(new TestIndicePorTitulo);

	tests.push_back(new TestArchivoListas);
	tests.push_back(new TestIndiceInvertidoPorFrase);

	for (unsigned int i = 0; i < tests.size(); i++)
	{
		tests.at(i)->ejecutar();
		delete(tests.at(i));
	}

	cout<<"---FIN TESTS---"<<endl;

	return 0;
}
