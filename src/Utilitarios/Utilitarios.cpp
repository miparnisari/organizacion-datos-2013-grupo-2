#include "Utilitarios.h"

namespace utilitarios
{
    void copyIntToBuffer (char* buffer, int integer)
	{
		buffer[0] = (integer >> 0) & 0xff;
		buffer[1] = (integer >> 8) & 0xff;
		buffer[2] = (integer >> 16) & 0xff;
		buffer[3] = (integer >> 24) & 0xff;
	}/* copia un int a un char**/
	
	bool validFileName (std::string fileName)
	{
		int len = fileName.length();
		if (len < 5) return false;
		if (fileName[len-4] != '.') return false;
		return true;
	}/*verifica que un string sea un nombre de archivo valido.*/
	
	void imprimir_argumentos(int argc,char** args)
	{
		std::cout<<" Argumentos programa: "<<std::endl<<std::endl;

		std::cout<<argc<<std::endl;
		for(int i=0;i<argc;i++)
			std::cout<<args[i]<<std::endl;

	}/*imprime argumentos del programa*/
	
	int listpath (std::string dir, std::vector<std::string> & files, std::string extension)
	{
		DIR* dp;
		struct dirent* dirp;
		if ( (dp  = opendir(dir.c_str()))  == NULL) {
			std::cout << "Error(" << errno << ") abriendo " << dir << std::endl;
			return errno;
		}

		std::string filename = "";
		
		while ((dirp = readdir(dp)) != NULL) {
			filename = std::string(dirp->d_name);
			if ( extension.size() > 0 )
			{
				if(filename.find_last_of(".") != std::string::npos)
				{
					if( filename.substr(filename.find_last_of(".") ) == extension ){
						files.push_back(filename);
					}
				}
			}
			else {
				files.push_back(filename);
			}
		}
		closedir(dp);
		return 0;
	}/* Dado un directorio, crea un vector con los nombres de los archivos. */
}
