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
	bool directorio_existe (std::string dir);
	wstring StringToWString(const string& s);
	string WStringToString (const wstring& s);
    unsigned int pasarBufferAInt(char* value);
	void copyIntToBuffer (char* buffer, int integer);
	bool validFileName (std::string fileName);
	int listpath (std::string dir, std::vector<std::string> &files, std::string extension);
}
