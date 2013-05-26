#include "CapaInterfaz/Indexador.h"
#include "Constantes.h"

int main (int argc, char* argv[])
{
	if (argc < 4) {
		std::cout << "Se requieren al menos 4 parametros." << std::endl;
		return RES_OK;
	}

	std::string operacion = std::string(argv[1]);
	Indexador indexador;
	
	// Indexar por primera vez
	if (operacion == "-indexar")
	{
		std::string directorioEntrada = std::string(argv[2]);
		std::string directorioSalida = std::string(argv[3]);
		indexador.indexar ( directorioEntrada, directorioSalida );
	}
	
	// Re-indexar
	else if (operacion == "-reindexar")
	{
		// TODO
	}
	
	// Consultar por titulo
	else if (operacion == "-consultarTitulo")
	{
		std::string directorioSalida = std::string(argv[2]);
		std::string titulo = std::string(argv[3]);
		indexador.consultar_titulo(directorioSalida, titulo);
	}
	
	// Consultar por autor
	else if (operacion == "-consultarAutor")
	{
		std::string directorioSalida = std::string(argv[2]);
		std::string autor = std::string(argv[3]);
		indexador.consultar_autor(directorioSalida, autor);
	}
	// Consultar por frase
	else if (operacion == "-consultarFrase")
	{

	}
	else
	{
		std::cout << "La operación " << operacion << " no está permitida." << std::endl;
	}
	
}
