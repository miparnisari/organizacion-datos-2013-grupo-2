#include <string>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <cstring>

namespace utilitarios
{
	void copyIntToBuffer (char* buffer, int integer);
	bool validFileName (std::string fileName);
	void imprimir_argumentos (int argc,char** args);
	int listpath (std::string dir, std::vector<std::string> &files, const char* extension = "");
}
