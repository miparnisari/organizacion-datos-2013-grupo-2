#include "Idioma.h"

Idioma::Idioma()
{
	std::vector<std::string> ingles;
	ingles.push_back("en");
	ingles.push_back("english");
	ingles.push_back("ingles");
	std::vector<std::string> espaniol;
	espaniol.push_back("sp");
	espaniol.push_back("spanish");
	espaniol.push_back("español");
	espaniol.push_back("espanol");
	espaniol.push_back("espaniol");
		
	mapaIdiomasValidos["en"]= ingles;
	mapaIdiomasValidos["sp"]= espaniol;
	
	idioma = IDIOMA_INVALIDO;
}

Idioma::~Idioma()
{
}

std::string Idioma::getIdioma()
{
	return idioma;
}

bool Idioma::cargar(std::string idioma_p)
{
	if (validar(idioma_p))
	{
		idioma = idioma_p;
		return true;
	}
	
	return false;	
}

bool Idioma::validar(std::string idioma)
{
	std::map< std::string, std::vector<std::string> >::iterator it;
	for (it = mapaIdiomasValidos.begin(); it!=mapaIdiomasValidos.end(); ++it)
	{
		for (unsigned int i = 0; i < it->second.size(); ++i)
		{
			if (it->second[i].compare(idioma) == 0)
				return true;
		}
	}
	
	return false;
}
