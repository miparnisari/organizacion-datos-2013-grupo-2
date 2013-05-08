/*
 * TestNodoSecuencial.cpp
 *
 *  Created on: May 6, 2013
 *      Author: maine
 */

#include "TestNodoSecuencial.h"

TestNodoSecuencial::TestNodoSecuencial()
{
}

TestNodoSecuencial::~TestNodoSecuencial()
{
}

void TestNodoSecuencial::ejecutar()
{
	test_insertar_eliminar();
	test_empaquetar_desempaquetar();
	test_overflow();
}

void TestNodoSecuencial::test_empaquetar_desempaquetar()
{
	ManejadorBloques manejador;
	manejador.crear_archivo("bloques_nodos_secuenciales.dat", 4096);
	manejador.abrir_archivo("bloques_nodos_secuenciales.dat","rb+");

	Bloque* bloque = manejador.crear_bloque();

	NodoSecuencial* nodo = new NodoSecuencial(100,4096);

	std::string campoEscrito = "un campo ";
	RegistroClave registro;
	ClaveX claveEscrita;
	claveEscrita.set_clave("una clave");
	registro.set_clave(claveEscrita);
	registro.agregar_campo(campoEscrito.c_str(),campoEscrito.size());
	std::vector<RegistroClave> regsOverflow;
	nodo->insertar(&registro,regsOverflow);

	assert(nodo->get_proximo_nodo() == -1);
	assert(nodo->buscar(&registro) == 1);
	assert(regsOverflow.empty());

	nodo->empaquetar(bloque);
	assert(manejador.escribir_bloque(bloque) == RES_OK);
	assert(manejador.get_cantidad_bloques() == 1);
	assert(manejador.get_primer_bloque_libre() == -1);

	NodoSecuencial* nodoLeido = new NodoSecuencial(100,4096);
	Bloque* bloqueLeido = manejador.obtener_bloque(0);

	nodoLeido->empaquetar(bloqueLeido);

	RegistroClave registroLeido;
	ClaveX claveLeida;
	registroLeido.set_clave(claveEscrita);
	assert(nodoLeido->buscar(&registroLeido) == 0);
	registroLeido.get_clave(claveLeida);
	assert(claveLeida == claveEscrita);

	char* campoLeido = new char[registroLeido.get_tamanio_campo(1)];
	assert(registroLeido.recuperar_campo(campoLeido,1) == RES_OK);
	assert(strcmp(campoLeido,campoEscrito.c_str()) == 0);

	delete[] campoLeido;
	delete(nodo);
	delete(bloque);
	delete(nodoLeido);
	delete(bloqueLeido);

	manejador.cerrar_archivo();
}

void TestNodoSecuencial::test_insertar_eliminar()
{
	// Set up
	NodoSecuencial* nodo = new NodoSecuencial(8,100);
	RegistroClave registro;
	ClaveX clave;
	std::vector<RegistroClave> regsUnderflow;
	std::vector<RegistroClave> regsOverflow;

	// Comprobaciones iniciales
	assert(nodo->esta_vacio() == true);
	assert(nodo->get_cantidad_registros() == 0);
	assert(nodo->get_proximo_nodo() == -1);

	// Pruebo eliminar una clave vacia
	assert(nodo->eliminar(&clave,regsUnderflow) != RES_OK);
	// Pruebo eliminar una clave que no existe
	clave.set_clave("una clave");
	assert(nodo->eliminar(&clave,regsUnderflow) == RES_RECORD_DOESNT_EXIST);

	// Agrego una clave (no se produce overflow)
	// y la borro (se produce underflow!)
	registro.set_clave(clave);
	std::string campo = "un campo";
	registro.agregar_campo(campo.c_str(), campo.size());
	assert(nodo->insertar(&registro,regsOverflow) == RES_OK);
	assert(regsOverflow.empty() == true);

	// Pruebo insertar una clave que ya estaba
	assert(nodo->insertar(&registro,regsOverflow) == RES_RECORD_EXISTS);
	assert(regsOverflow.empty() == true);

	assert(nodo->eliminar(&clave,regsUnderflow) == RES_UNDERFLOW);
	assert(regsUnderflow.empty() == true);
	assert(nodo->get_cantidad_registros() == 0);


	// Agrego dos claves (no se produce overflow)
	// y borro una (se produce underflow!)
	RegistroClave reg1, reg2;
	ClaveX clave1, clave2;
	std::string campo1, campo2;
	clave1.set_clave("aa");
	clave2.set_clave("bb");
	campo1 = "1";
	campo2 = "2";
	reg1.agregar_campo(campo1.c_str(), campo1.size());
	reg2.agregar_campo(campo2.c_str(), campo2.size());
	reg1.set_clave(clave1);
	reg2.set_clave(clave2);

	assert(nodo->insertar(&reg2,regsOverflow) == RES_OK);
	assert(regsOverflow.empty() == true);
	assert(nodo->insertar(&reg1,regsOverflow) == RES_OK);
	assert(regsOverflow.empty() == true);

	// Los registros deben estar ordenados dentro del nodo
	assert(nodo->buscar(&reg1) == 0);
	assert(nodo->buscar(&reg2) == 1);

	// Elimno una clave
	assert(nodo->eliminar(&clave1,regsUnderflow) == RES_UNDERFLOW);
	assert(regsUnderflow.empty() == false);

	ClaveX claveDeUnderflow;
	regsUnderflow.at(0).get_clave(claveDeUnderflow);
	assert (claveDeUnderflow == clave2);

	assert(nodo->get_cantidad_registros() == 1);


	delete(nodo);

}

void TestNodoSecuencial::test_overflow()
{
	std::vector<RegistroClave> regsOverflow;
	std::vector<RegistroClave> registros;
	std::vector<ClaveX> claves;
	std::string campo = "1";

	int valorClave = 0;
	for (; valorClave<1000 ; valorClave++)
	{
		ClaveX clave;
		clave.set_clave(valorClave);
		claves.push_back(clave);

		RegistroClave registro;
		registro.set_clave(clave);
		registro.agregar_campo(campo.c_str(), campo.size());
		registros.push_back(registro);
	}

	int tamReg = registros.at(0).get_tamanio_empaquetado();
	int tamRegTotal = tamReg * 1000;

	NodoSecuencial* nodo = new NodoSecuencial(5,tamRegTotal+sizeof(int));
	for (int i = 0; i<1000;i++)
	{
		assert(nodo->insertar(&(registros.at(i)),regsOverflow) == RES_OK);
		assert(regsOverflow.empty() == true);
	}

	RegistroClave regOverflow;
	ClaveX clave;
	clave.set_clave(valorClave+1);
	regOverflow.set_clave(clave);
	regOverflow.agregar_campo(campo.c_str(), campo.size());

	assert(nodo->insertar(&regOverflow,regsOverflow) == RES_OVERFLOW);
	assert(regsOverflow.empty() == false);
	assert(regsOverflow.size() == 500);

}
