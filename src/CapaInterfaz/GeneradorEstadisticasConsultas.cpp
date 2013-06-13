/*
 * GeneradorEstadisticasConsultas.cpp
 *
 *  Created on: Jun 12, 2013
 *      Author: maine
 */

#include "GeneradorEstadisticasConsultas.h"

GeneradorEstadisticasConsultas::GeneradorEstadisticasConsultas() {

}

GeneradorEstadisticasConsultas::~GeneradorEstadisticasConsultas() {
}

template<typename U>
int GeneradorEstadisticasConsultas::_get_pos_max_freq (std::vector<U>& vector)
{
	// Caso en que todavia no hay estadisticas
	if (vector.size() == 0)
		return -1;

	int max_frecuencia = 0;
	int pos_maximo = 0;
	for (unsigned int i=0 ; i < vector.size() ; i++)
	{
		if (vector[i].second > max_frecuencia)
			pos_maximo = i;
	}

	return pos_maximo;
}

template<typename T, typename U>
void GeneradorEstadisticasConsultas::_aumentar_frecuencia (T& valor, std::vector<U>& vector)
{
    for (unsigned int i=0; i < vector.size(); i++)
    {
        if (vector[i].first == valor)
        {
        	vector[i].second += 1;
            return;
        }
    }
    vector.push_back(std::make_pair (valor, 0));
}

std::string GeneradorEstadisticasConsultas::get_titulo_mas_consultado()
{
	int pos_max = _get_pos_max_freq(consultasTitulo);
	if (pos_max == -1)
	{
		std::cout << "Aún no hay estadísticas sobre este tipo de consultas." << std::endl;
		return "";
	}
	else
		return consultasTitulo[pos_max].first;
}

std::string GeneradorEstadisticasConsultas::get_frase_mas_consultado()
{
	int pos_max = _get_pos_max_freq(consultasFrase);
	if (pos_max == -1)
	{
		std::cout << "Aún no hay estadísticas sobre este tipo de consultas." << std::endl;
		return "";
	}
	else
		return consultasFrase[pos_max].first;
}

std::string GeneradorEstadisticasConsultas::get_autor_mas_consultado()
{
	int pos_max = _get_pos_max_freq(consultasAutor);
	if (pos_max == -1)
	{
		std::cout << "Aún no hay estadísticas sobre este tipo de consultas." << std::endl;
		return "";
	}
	else
		return consultasAutor[pos_max].first;
}

int GeneradorEstadisticasConsultas::get_tema_mas_consultado()
{
	int pos_max = _get_pos_max_freq(consultasTema);
	if (pos_max == -1)
	{
		std::cout << "Aún no hay estadísticas sobre este tipo de consultas." << std::endl;
		return -1;
	}
	else
		return consultasTema[pos_max].first;

}

void GeneradorEstadisticasConsultas::aumentar_frecuencia_titulo  (std::string & titulo)
{
	_aumentar_frecuencia (titulo, consultasTitulo);
}

void GeneradorEstadisticasConsultas::aumentar_frecuencia_autor (std::string & autor)
{
	_aumentar_frecuencia (autor, consultasAutor);
}

void GeneradorEstadisticasConsultas::aumentar_frecuencia_frase  (std::string & frase)
{
	_aumentar_frecuencia (frase, consultasFrase);
}

void GeneradorEstadisticasConsultas::aumentar_frecuencia_tema (int id_doc)
{
	_aumentar_frecuencia (id_doc, consultasTema);
}
