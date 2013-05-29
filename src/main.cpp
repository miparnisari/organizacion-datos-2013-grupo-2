#include "CapaInterfaz/Indexador.h"
#include "Constantes.h"

int main (int argc, char* argv[])
{
	if (argc < 4) {
		std::cout << "ERROR: Se requieren al menos 4 parametros." << std::endl;
		return RES_OK;
	}

	std::string operacion = std::string(argv[1]);
	Indexador indexador;
	
	if (operacion == "-indexar")
	{
		std::string directorioEntrada = std::string(argv[2]);
		std::string directorioSalida = std::string(argv[3]);
		indexador.indexar ( directorioEntrada, directorioSalida );
	}
	
	else if (operacion == "-consultarTitulo")
	{
		std::string directorioSalida = std::string(argv[2]);
		std::string titulo = std::string(argv[3]);
		indexador.consultar_titulo(directorioSalida, titulo);
	}
	
	else if (operacion == "-consultarAutor")
	{
		std::string directorioSalida = std::string(argv[2]);
		std::string autor = std::string(argv[3]);
		indexador.consultar_autor(directorioSalida, autor);
	}

	else if (operacion == "-consultarFrase")
	{
		std::string directorioSalida = std::string(argv[2]);
		std::string frase = std::string(argv[3]);
		indexador.consultar_frase(directorioSalida,frase);
	}
	else
	{
		std::cout << "ERROR: La operación " << operacion << " no está permitida." << std::endl;
	}
	
}
