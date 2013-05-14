#include "CapaLogica/Indexador/Indexador.h"
#include "Constantes.h"

int main (int argc, char* argv[])
{
	if (argc < 4) {
		std::cout << "Se requieren al menos 4 parametros." << std::endl;
		return RES_OK;
	}
	std::string directorioEntrada = std::string(argv[2]);
	std::string directorioSalida = std::string(argv[3]);
	std::string operacion = std::string(argv[1]);
	
	Indexador indexador;
	
	// Indexar por primera vez
	if (operacion == "-indexar")
	{
		indexador.indexar ( directorioEntrada, directorioSalida );
	}
	
	// Re-indexar
	if (operacion == "-reindexar")
	{
		// TODO
	}
	
	// Consultar por titulo
	if (operacion == "-consultarTitulo")
	{
		// TODO		
	}
	
	// Consultar por autor
	if (operacion == "-consultarAutor")
	{
		// TODO	
	}
	
}

