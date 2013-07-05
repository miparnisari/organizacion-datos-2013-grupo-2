/*
 * Interfaz.cpp
 *
 *  Created on: Jul 4, 2013
 *      Author: maine
 */

#include "Interfaz.h"

Interfaz::Interfaz() {
}

Interfaz::~Interfaz() {
}

void Interfaz::mostrar_operaciones()
{
	std::cout << "Operaciones posibles: " << std::endl;
	std::cout << "(0) SALIR. " << std::endl;
	std::cout << "(1) Indexar carpeta." << std::endl;
	std::cout << "(2) Consultar título." << std::endl;
	std::cout << "(3) Consultar autor." << std::endl;
	std::cout << "(4) Consultar frase." << std::endl;
	std::cout << "(5) Borrar canción por ID." << std::endl;
	std::cout << "(6) Buscar titulo más consultado." << std::endl;
	std::cout << "(7) Buscar autor más consultado." << std::endl;
	std::cout << "(8) Buscar frase más consultado." << std::endl;
	std::cout << "(9) Buscar tema más consultado." << std::endl;
}

int Interfaz::_transformar_operacion(string& string){

	const int OPERACION_IVALIDA = -1;
	const int LONGITUD = string.length();
	const int CERO = (int)'0';
	const int NUEVE = (int)'9';

	for(int i=0;i<LONGITUD;i++){

		int c= (int)string.at(i);
		if ( c < CERO || c > NUEVE )
				return OPERACION_IVALIDA;

	}

	return string_a_int(string);
}


int Interfaz::pedir_operacion()
{
	mostrar_operaciones();

	string operacionString;
	std::cout << "Ingrese operacion: ";
	std::cin >> operacionString;
	int operacion = _transformar_operacion(operacionString);
	return operacion;
}

std::string Interfaz::pedir_directorio_entrada()
{
	std::string directorioEntrada;
	std::cout << "Ingrese directorio del índice: ";
	std::cin >> directorioEntrada;
	return directorioEntrada;
}

std::string Interfaz::pedir_directorio_salida()
{
	std::string directorioSalida;
	std::cout << "Ingrese directorio de salida: ";
	std::cin >> directorioSalida;
	return directorioSalida;
}

int Interfaz::pedir_id_cancion()
{
	std::string id;
	std::cout << "Ingrese el ID de la canción: ";
	std::cin >> id;
	if (utilitarios::es_numero(id))
		return string_a_int(id);
	return REFERENCIA_INVALIDA;
}

std::string Interfaz::pedir_consulta()
{
	std::string consulta;
	std::cout << "Ingrese la consulta: ";
	std::getline(cin, consulta);
	std::getline(cin, consulta);
	return std::string(consulta);
}

void Interfaz::ejecutar()
{
	Controlador controlador;

	int operacion = pedir_operacion();
	while (operacion != 0)
	{
		if (operacion == 1)
		{
			std::string directorioEntrada = pedir_directorio_entrada();
			std::string directorioSalida = pedir_directorio_salida();
			controlador.indexar ( directorioEntrada, directorioSalida );
		}

		else if (operacion == 2)
		{
			std::string directorioSalida = pedir_directorio_salida();
			std::string titulo = pedir_consulta();
			controlador.consultar_titulo(directorioSalida, titulo);
		}

		else if (operacion == 3)
		{
			std::string directorioSalida = pedir_directorio_salida();
			std::string autor = pedir_consulta();
			controlador.consultar_autor(directorioSalida, autor);
		}

		else if (operacion == 4)
		{
			std::string directorioSalida = pedir_directorio_salida();
			std::string frase = pedir_consulta();
			controlador.consultar_frase(directorioSalida,frase);
		}
		else if (operacion == 5)
		{
			std::string directorioSalida = pedir_directorio_salida();
			int idCancion = pedir_id_cancion();
			if (idCancion == REFERENCIA_INVALIDA)
				continue;
			controlador.borrar_cancion(directorioSalida,idCancion);
		}
		else if (operacion == 6)
		{
			controlador.mostrar_estadisticas("titulo");
		}
		else if (operacion == 7)
		{
			controlador.mostrar_estadisticas("autor");
		}
		else if (operacion == 8)
		{
			controlador.mostrar_estadisticas("frase");
		}
		else if (operacion == 9)
		{
			controlador.mostrar_estadisticas("tema");
		}
		else
		{
			std::cout << "ERROR: Operación inválida." << std::endl;
		}
		operacion = pedir_operacion();
	}
}
