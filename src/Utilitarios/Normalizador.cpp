/*
 * Normalizador.cpp
 *
 *  Created on: Apr 25, 2013
 *      Author: martin
 */

#include "Normalizador.h"

#define IMP(v)\
	cout<<#v<<" ="<<v<<endl;

char Normalizador::transformar_char(wchar_t c){

    int cint = (int) c;

    // Son los posibles caracteres "raros" que encontré
    switch (cint){
            case (int) L'á':
            case (int) L'ä':
                    return 'a';

            case (int) L'é':
            case (int) L'ë':
                    return 'e';

            case (int) L'í':
            case (int) L'ï':
            case (int) L'î':
                    return 'i';

            case (int) L'ó':
            case (int) L'ö':
                    return 'o';

            case (int) L'ú':
            case (int) L'ü':
                    return 'u';

            case (int) L'ñ':
                    return 'n';

            // Tambien hay palabras que tienen puntos (ejem: T.N.T.)
            case (int) L'.':
                    return '.';

            default:
                    // Un caracter que no se que es
                    return (char)cint;
    }

}


char Normalizador::filtrar_char(wchar_t c){


    // Convierte el caracter en numero
    int ci = (int) c;

    // Los valores de los caracters permitidos
    int a = (int) 'a';
    int z = (int) 'z';
    int A = (int) 'A';
    int Z = (int) 'Z';
    int const GUION_ALTO = (int) '-';
    int const ESPACIO= 32;
    int const PUNTO_COMA= (int)';';
    int const INTRO= (int)'\n';
    int const CERO= (int)'0';
    int const NUEVE= (int)'9';

    // Compara que el caracter este dentro de los valores de las letras "normales"
    if ( (ci >= a && ci <= z) || (ci >= A && ci <=Z) || ci==GUION_ALTO
    		|| ci== ESPACIO || ci== PUNTO_COMA || ci==INTRO ||
    		(ci>=CERO && ci<=NUEVE) ){

            // Es una letra normal, lo casteo y lo devuelvo
            return c;

    } else  {
            // Es un caracter "raro", con asento o algo parecido,
            // hay que aplicarle una transformacion
            return transformar_char(c);
    }

}


wstring Normalizador::a_minuscula(wstring & linea){

    transform(linea.begin(), linea.end(), linea.begin(),
            bind2nd(ptr_fun(&tolower<wchar_t>),
            locale("es_ES.UTF8")));

	return linea;

}


string Normalizador::normalizar_string(wstring & linea){

	wstring lineaMinuscula=  this->a_minuscula(linea);

	string s = "";
	for (unsigned i=0; i < lineaMinuscula.length(); ++i){

			// Se filtra el caracter y se lo agrega al final del string
			char c = filtrar_char(lineaMinuscula[i]);
			s.push_back(c);
	}
	return s;

}


string Normalizador::normalizar_input( string linea ){

	string nombreArchivoTemporal= "normalizar_input.txt";
	string nombreArchivoTemporalNormalizado= "normalizar_input_normalizado.txt";
	ofstream archivoTemporal(nombreArchivoTemporal.c_str());
//	archivoTemporal<<linea;
//	archivoTemporal<<'\n';

	IMP(linea);
	archivoTemporal.write( linea.c_str(),linea.length() );
	archivoTemporal.close();

	normalizar(nombreArchivoTemporal,nombreArchivoTemporalNormalizado);
	ifstream archivoResultado(nombreArchivoTemporalNormalizado.c_str());
//	const unsigned short TAMANIO_BUFFER_INPUT= 4096;
//	char bufferInput[TAMANIO_BUFFER_INPUT];

//	archivoResultado.getline(bufferInput , TAMANIO_BUFFER_INPUT);
//	unsigned short longitudLineaNormalizada= strlen(bufferInput);

	string resultado;
	while(!archivoResultado.eof()){
//		string temp;
//		archivoResultado>>temp;
//		if(!archivoResultado.eof())
//			resultado+=temp+'\n';
		const unsigned short TAMANIO_BUFFER_INPUT= 512;
		char bufferInput[TAMANIO_BUFFER_INPUT];
		archivoResultado.getline(bufferInput,TAMANIO_BUFFER_INPUT);
		if(!archivoResultado.eof()){
			string temp(bufferInput, strlen(bufferInput) );
			resultado+= temp+'\n';
		}
	}

	remove(nombreArchivoTemporal.c_str());
	remove(nombreArchivoTemporalNormalizado.c_str());

//	string resultado(bufferInput,longitudLineaNormalizada);
	return resultado;

}


int Normalizador::normalizar(string pathArchivoEntrada , string pathArchivoSalida){

    // Abre el archivo como wide char, el archivo debe ser UTF-8
    wifstream archEntrada(pathArchivoEntrada.c_str());

    if(!archEntrada.is_open())
    	return RES_ERROR;

    // Se fuerza que el locale con el que trabajamos sea UTF-8
    // (este deberia ser el valor por defecto, pero varia por PC)
    locale loc("es_ES.UTF8");
    archEntrada.imbue(loc);

    // Donde guardamos el resultado
    ofstream archsalida(pathArchivoSalida.c_str());
    if(!archsalida.is_open())
    	return RES_ERROR;


    while ( !archEntrada.eof() ){

            // Leemos una linea entera del archivo
            wstring linea;
            getline(archEntrada, linea);

            // To lower case, robado de:
            // http://stackoverflow.com/questions/313970/stl-string-to-lower-case
            transform(linea.begin(), linea.end(), linea.begin(),
                    bind2nd(ptr_fun(&tolower<wchar_t>),
                    locale("es_ES.UTF8")));


            // Procesamos los caracteres uno a uno
            string s = "";
            for (unsigned i=0; i < linea.length(); ++i){

                    // Se filtra el caracter y se lo agrega al final del string
                    char c = filtrar_char(linea[i]);
                    s.push_back(c);
            }

            // Se guarda la linea en el archivo de salida
            archsalida << s << endl;
    }

    archEntrada.close();
    archsalida.close();

    return RES_OK;

}
