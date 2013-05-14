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
	test_nodo_sec_crear();
	test_nodo_sec_insertar_simple();
	test_nodo_sec_insertar_eliminar();
	test_nodo_sec_empaquetar_desempaquetar();
	test_nodo_sec_overflow();
}

void TestNodoSecuencial::test_nodo_sec_crear()
{
	NodoSecuencial* nodo = new NodoSecuencial(100,1000);
	assert(nodo->get_bytes_ocupados() == sizeof(int) + sizeof(char));
	assert(nodo->get_proximo_nodo() == -1);
	assert(nodo->get_cantidad_registros() == 0);
	assert(nodo->esta_vacio() == true);
	delete(nodo);

	print_test_ok("test_nodo_secuencial_crear");
}

void TestNodoSecuencial::test_nodo_sec_insertar_simple()
{
	NodoSecuencial* nodo = new NodoSecuencial(5,100);
	std::vector <RegistroClave> regsOverflow;
	RegistroClave registro;
	RegistroClave* registroCopia = NULL;

	ClaveX clave;
	clave.set_clave("una clave");
	std::string campo = "un campo";

	registro.set_clave(clave);
	registro.agregar_campo(campo.c_str(), campo.size());

	assert(registro.get_tamanio() == 22);

	assert(nodo->buscar(clave,&registroCopia) < 0);
	assert(registroCopia == NULL);

	assert(nodo->insertar(registro,regsOverflow) == RES_OK);

	assert(nodo->buscar(clave,&registroCopia) == 0);
	assert(registroCopia != NULL);

	assert(registroCopia->get_tamanio() == registro.get_tamanio());

	char* buffer = new char[registroCopia->get_tamanio_campo(1) +1]();
	registroCopia->recuperar_campo(buffer,1);
	buffer[registroCopia->get_tamanio_campo(1)] = '\0';
	assert (strcmp(buffer,campo.c_str()) == 0);

	delete[] buffer;
	delete(nodo);
	delete(registroCopia);

	print_test_ok("test_nodo_secuencial_insertar_simple");
}

void TestNodoSecuencial::test_nodo_sec_empaquetar_desempaquetar()
{
	// SET UP
	ManejadorBloques manejador;
	manejador.crear_archivo("bloques_nodos_secuenciales.dat", 4096);
	manejador.abrir_archivo("bloques_nodos_secuenciales.dat","rb+");

	Bloque* bloque = manejador.crear_bloque();
	NodoSecuencial* nodo = new NodoSecuencial(100,4096);

	std::string campoEscrito = "un campo";
	RegistroClave registro;
	ClaveX claveEscrita;
	claveEscrita.set_clave("una clave");
	registro.set_clave(claveEscrita);
	registro.agregar_campo(campoEscrito.c_str(),campoEscrito.size());
	std::vector<RegistroClave> regsOverflow;

	// Inserto "una clave | un campo"
	assert(nodo->insertar(registro,regsOverflow) == RES_OK);

	assert(nodo->get_cantidad_registros() == 1);
	assert(nodo->get_proximo_nodo() == -1);
	assert(nodo->buscar(claveEscrita,NULL) == 0);
	assert(regsOverflow.empty());

	assert(nodo->empaquetar(bloque) == RES_OK);
	assert(manejador.escribir_bloque(bloque) == RES_OK);

	NodoSecuencial* nodoLeido = new NodoSecuencial(100,4096);
	Bloque* bloqueLeido = manejador.obtener_bloque(0);

	nodoLeido->empaquetar(bloqueLeido);

	RegistroClave* registroLeido = NULL;
	ClaveX claveLeida;
	assert(nodoLeido->buscar(claveEscrita,&registroLeido) == 0);
	assert(registroLeido != NULL);
	assert(registroLeido->get_clave() == claveEscrita);

	char* campoLeido = new char[registroLeido->get_tamanio_campo(1)+1];
	assert(registroLeido->recuperar_campo(campoLeido,1) == RES_OK);
	campoLeido[registroLeido->get_tamanio_campo(1)] = '\0';
	assert(strcmp(campoLeido,campoEscrito.c_str()) == 0);

	delete[] campoLeido;
	delete(nodo);
	delete(bloque);
	delete(nodoLeido);
	delete(bloqueLeido);

	manejador.cerrar_archivo();

	print_test_ok("test_nodo_sec_empaquetar_desempaquetar");
}

void TestNodoSecuencial::test_nodo_sec_insertar_eliminar()
{
	// Set up
	NodoSecuencial* nodo = new NodoSecuencial(8,100);
	RegistroClave registro;
	ClaveX clave;
	std::vector<RegistroClave> regsUnderflow;
	std::vector<RegistroClave> regsOverflow;

	// Pruebo eliminar una clave vacia -> no debe andar
	assert(nodo->eliminar(clave,regsUnderflow) != RES_OK);
	assert(regsUnderflow.empty() == true);
	// Pruebo eliminar una clave que no existe -> no debe andar
	clave.set_clave("una clave");
	assert(nodo->eliminar(clave,regsUnderflow) == RES_RECORD_DOESNT_EXIST);
	assert(regsUnderflow.empty() == true);

	// Agrego una clave (no se produce overflow)
	// intento insertarla de nuevo (no se puede)
	// y la borro (se produce underflow!)
	registro.set_clave(clave);
	std::string campo = "un campo";
	registro.agregar_campo(campo.c_str(), campo.size());
	assert(nodo->insertar(registro,regsOverflow) == RES_OK);
	assert(regsOverflow.empty() == true);
	assert(nodo->insertar(registro,regsOverflow) == RES_RECORD_EXISTS);
	assert(regsOverflow.empty() == true);

	assert(nodo->eliminar(clave,regsUnderflow) == RES_UNDERFLOW);
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

	// El nodo tiene "una clave|un campo"
	assert(nodo->insertar(reg2,regsOverflow) == RES_OK);
	// El nodo tiene "una clave|un campo" y "aa|2"
	assert(regsOverflow.empty() == true);
	assert(nodo->insertar(reg1,regsOverflow) == RES_OK);
	assert(regsOverflow.empty() == true);

	// Los registros deben estar ordenados dentro del nodo
	RegistroClave* regleido1 = NULL;
	RegistroClave* regleido2 = NULL;
	assert(nodo->buscar(clave1,&regleido1) == 0);
	assert(regleido1 != NULL);
	assert(nodo->buscar(clave2,&regleido2) == 1);
	assert(regleido2 != NULL);

	// Elimno una clave
	assert(nodo->eliminar(clave1,regsUnderflow) == RES_UNDERFLOW);
	assert(regsUnderflow.empty() == false);

	assert (regsUnderflow.at(0).get_clave() == clave2);

	assert(nodo->get_cantidad_registros() == 1);


	delete(nodo);

	print_test_ok("test_nodo_sec_insertar_eliminar");

}

void TestNodoSecuencial::test_nodo_sec_overflow()
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
		assert(nodo->insertar(registros.at(i),regsOverflow) == RES_OK);
		assert(regsOverflow.empty() == true);
	}

	RegistroClave regOverflow;
	ClaveX clave;
	clave.set_clave(valorClave+1);
	regOverflow.set_clave(clave);
	regOverflow.agregar_campo(campo.c_str(), campo.size());

	assert(nodo->insertar(regOverflow,regsOverflow) == RES_OVERFLOW);
	assert(regsOverflow.empty() == false);
	assert(regsOverflow.size() == 500);

	print_test_ok("test_nodo_sec_overflow");

}
