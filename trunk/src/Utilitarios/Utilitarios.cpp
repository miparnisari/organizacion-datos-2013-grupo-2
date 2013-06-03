#include "Utilitarios.h"
#include <iostream>
#include <sstream>

 namespace utilitarios
{
 	 int string_a_int (std::string & s_int)
 	 {
 		 stringstream ss;
 		 ss << s_int;

 		 int i_int;
 		 ss >> i_int;
 		 return i_int;
 	 }

 	 std::string int_a_string (int i_int)
 	 {
 		 stringstream ss;
 		 ss << i_int;

 		 std::string s_int;
 		 ss >> s_int;
 		 return s_int;
 	 }

 	 int remover_archivos_de_directorio(std::string & directorio)
 	 {
		  struct dirent *next_file;
		  DIR *dir;

		  char filepath[256];

		  dir = opendir(directorio.c_str());
		  next_file = readdir(dir);
		  while ( next_file != NULL  )
		  {
			  sprintf(filepath, "%s/%s", directorio.c_str(), next_file->d_name);
			  remove(filepath);
			  next_file = readdir(dir);
		  }
		  return RES_OK;
	}
	bool directorio_existe( std::string & dir )
	{
		DIR *pDir;
		bool existe = false;

		pDir = opendir (dir.c_str());

		if (pDir != NULL)
		{
			existe = true;
			(void) closedir (pDir);
		}
		return existe;
	}

	wstring StringToWString(const string& s) {
			wstring temp(s.length(),L' ');
			copy(s.begin(), s.end(), temp.begin());
			return temp;
	}

	string WStringToString (const wstring& s) {
			string simpleString;
			simpleString.assign(s.begin(), s.end());
			return simpleString;
	}

	bool validFileName (std::string & fileName)
	{
		int len = fileName.length();
		if (len < 5) return false;
		if (fileName[len-4] != '.') return false;
		return true;
	}/*verifica que un string sea un nombre de archivo valido.*/

	int listpath (std::string & dir, std::vector<std::string> & files, std::string extension)
	{
		DIR* dp = opendir(dir.c_str());
		struct dirent* dirp;
		std::string filename = "";

		while ((dirp = readdir(dp)) != NULL) {
			filename = std::string(dirp->d_name);
			if ( extension.size() > 0 )
			{
				if(filename.find_last_of(".") != std::string::npos)
				{
					if( filename.substr(filename.find_last_of(".") ) == extension ){
						files.push_back(dir+'/'+filename);
					}
				}
			}
			else {
				files.push_back(dir+'/'+filename);
			}
		}
		closedir(dp);
		return RES_OK;
	}/* Dado un directorio, crea un vector con los nombres de los archivos. */
}
