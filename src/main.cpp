#include "CapaInterfaz/Controlador.h"
#include "Constantes.h"
#include <iostream>
#include <fstream>

using std::ofstream;

void mostrar_operaciones()
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

int pedir_operacion()
{
	mostrar_operaciones();
	int operacion;
	std::cout << "Ingrese operacion: ";
	std::cin >> operacion;
	return operacion;
}

std::string pedir_directorio_entrada()
{
	std::string directorioEntrada;
	std::cout << "Ingrese directorio de entrada: ";
	std::cin >> directorioEntrada;
	return directorioEntrada;
}

std::string pedir_directorio_salida()
{
	std::string directorioSalida;
	std::cout << "Ingrese directorio de salida: ";
	std::cin >> directorioSalida;
	return directorioSalida;
}

int pedir_id_cancion()
{
	int id;
	std::cout << "Ingrese el ID de la canción: ";
	std::cin >> id;
	return id;
}

std::string pedir_consulta()
{
	std::string consulta;
	std::cout << "Ingrese consulta: ";
	std::getline(cin, consulta);
	std::getline(cin, consulta);
	return std::string(consulta);
}

int main (int argc, char* argv[])
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
			std::cout << "ERROR: La operación " << operacion << " no está permitida." << std::endl;
		}
		operacion = pedir_operacion();
	}
	
}

