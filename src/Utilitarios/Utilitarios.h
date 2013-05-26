#include <string>
#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <cstring>

#include <sstream>

using namespace std;

namespace utilitarios
{
	wstring StringToWString(const string& s);
	string WStringToString (const wstring& s);
    unsigned int pasarBufferAInt(char* value);
	void copyIntToBuffer (char* buffer, int integer);
	bool validFileName (std::string fileName);
	void imprimir_argumentos (int argc,char** args);
	int listpath (std::string dir, std::vector<std::string> &files, std::string extension);
}
