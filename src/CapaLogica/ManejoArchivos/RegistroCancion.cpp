#include "RegistroCancion.h"

RegistroCancion::RegistroCancion():RegistroVariable()
{
	autores= NULL;
	cantidadAutores= 0;
	titulo= "";
	letra= "";
}

RegistroCancion::~RegistroCancion()
{
	if(autores)
		delete[] autores;
}


bool RegistroCancion::obtener_parametro(unsigned short numeroParametro,string& parametro)throw()
{
	parametro= "";
	if(numeroParametro>= contar_parametros())
		return false;

	stringstream stream;
	stream.write(buffer,tamanio);
	stream.seekg(0,ios::beg);
	string parametros;

	const unsigned short TAMANIO_BUFFER_PARAMETROS= 2048;
	char* bufferParametros= new char[TAMANIO_BUFFER_PARAMETROS]();
	stream.getline(bufferParametros , TAMANIO_BUFFER_PARAMETROS);
	parametros= bufferParametros;
	delete[] bufferParametros;

	unsigned short offset= 0;
	unsigned short contadorParametro= 0;
	const char SEPARACION_PARAMETROS= '-';

	while(contadorParametro< numeroParametro){
		char c;
		c= parametros.at(offset);

		if(c== SEPARACION_PARAMETROS)
			contadorParametro++;

		offset++;

	}

	char c='x';
	string parametroLeido;
	while(offset < parametros.length() && c!= SEPARACION_PARAMETROS){

		c= parametros.at(offset);
		if(c!=SEPARACION_PARAMETROS)
			parametroLeido.push_back(c);
		offset++;

	}

	parametro= parametroLeido;
	return true;

}

unsigned short RegistroCancion::contar_parametros()throw()
{
	stringstream stream;
	stream.write(buffer,tamanio); //Inserts the first n characters of the array pointed by s into the stream.
	stream.seekg(0,ios::beg);
		
	string stringCancion;

	const unsigned short TAMANIO_BUFFER_PARAMETROS= 2048;
	char* bufferParametros= new char[TAMANIO_BUFFER_PARAMETROS]();
	stream.getline(bufferParametros , TAMANIO_BUFFER_PARAMETROS);
	stringCancion= bufferParametros;
	delete[] bufferParametros;

	const unsigned short OFFSET_FIN_PARAMETROS= stringCancion.length();
	const char SEPARACION_PARAMETRO= '-';
	unsigned short contadorGuiones= 0;

	for(unsigned short i=0;i<OFFSET_FIN_PARAMETROS;i++){
		if(stringCancion.at(i)== SEPARACION_PARAMETRO)
			contadorGuiones++;
		}


	unsigned short contadorParametros= contadorGuiones+1;
	if(contadorParametros < CANTIDAD_MINIMA_PARAMETROS)
		return 0;
	if(contadorParametros > CANTIDAD_MAXIMA_PARAMETROS)
		return 0;

	return contadorParametros;

}/*cuenta la cantidad de parametros de la cancion*/


int RegistroCancion::obtener_anio_grabacion()throw(){

	const unsigned short CANTIDAD_PARAMETROS_CON_ANIO= 4;
	const unsigned short CANTIDAD_PARAMETROS= contar_parametros();

	if(CANTIDAD_PARAMETROS!= CANTIDAD_PARAMETROS_CON_ANIO){
		anioGrabacion.cargar("-1");
		return RES_OK;
	}

	string anioString;
	const unsigned short NUMERO_PARAMETRO_ANIO= 1;
	obtener_parametro(NUMERO_PARAMETRO_ANIO , anioString);

	anioGrabacion.cargar(anioString);
	return anioGrabacion.get_anio();


}/*recupera el anio de grabacion*/


void RegistroCancion::limpiar_autores()throw(){

	if(autores)
		delete[] autores;
	this->cantidadAutores= 0;

}/*setea los autores a 0*/


unsigned short RegistroCancion::obtener_autores()throw(){

	string autoresRecuperados;
	this->obtener_parametro(0,autoresRecuperados);
	const char SEPARACION_AUTOR= ';';
	this->limpiar_autores();

	if(autoresRecuperados.length()== 0)
		return 0;

	for(unsigned int i=0;i<autoresRecuperados.length();i++){
		if(autoresRecuperados.at(i)== SEPARACION_AUTOR)
			cantidadAutores++;
	}cantidadAutores++;

	autores= new string[cantidadAutores];

	unsigned short contadorAutor= 0;
	for(unsigned short i=0;i<autoresRecuperados.length();i++){

		char c;
		c= autoresRecuperados.at(i);

		if(c!= SEPARACION_AUTOR)
			autores[contadorAutor].push_back(c);
		else
			contadorAutor++;

	}

	return cantidadAutores;


}/*obtiene los autores del buffer y retorna el byte offset donde termina el campo de autores*/


bool RegistroCancion::obtener_titulo()throw(){

	unsigned short numeroParametroTitulo;
	const unsigned short CANTIDAD_PARAMETROS= contar_parametros();

	if(CANTIDAD_PARAMETROS== 3)
		numeroParametroTitulo= 1;
	else
	if(CANTIDAD_PARAMETROS== 4)
		numeroParametroTitulo= 2;
	else
		return false;

	this->obtener_parametro(numeroParametroTitulo , titulo);
	return true;


}/*recupera el titulo de la linea de caracteres y lo almacena en la variable titulo*/


bool RegistroCancion::obtener_idioma()throw(){

	const unsigned short CANTIDAD_PARAMETROS= contar_parametros();
	unsigned short numeroParametroIdioma;

	if(CANTIDAD_PARAMETROS== 3)
		numeroParametroIdioma= 2;
	else
	if(CANTIDAD_PARAMETROS== 4)
		numeroParametroIdioma= 3;
	else
		return false;

	string idiomaString;
	obtener_parametro(numeroParametroIdioma , idiomaString);

	return idioma.cargar(idiomaString);

}/*recupera el idioma de la cadena de caracteres y lo almacena en la variable idioma*/


bool RegistroCancion::obtener_letra()throw(){

	stringstream stream;
	stream.write(buffer,tamanio);
	stream.seekg(0,ios::beg);

	char bufferDescarte[2048];
	stream.getline(bufferDescarte,2048);


	const unsigned short TAMANIO_BUFFER_LETRA= 2048;


	while(stream.good()){

		char* bufferLetra= new char[TAMANIO_BUFFER_LETRA]();
		stream.getline(bufferLetra , TAMANIO_BUFFER_LETRA);
		if(stream.good()){
			letra+= bufferLetra;
			letra+= '\n';
		}

		delete[] bufferLetra;

	}

	if(letra.length()== 0)
		return false;

	return true;


}/*recupera la letra de la cadena de caracteres y la carga en la variable.*/


unsigned short RegistroCancion::get_cantidad_autores()throw(){
	return cantidadAutores;
}

string RegistroCancion::get_autor(unsigned short numeroAutor)throw(){

	if(autores== NULL)
		return AUTOR_INVALIDO;
	if(numeroAutor >= cantidadAutores)
		return AUTOR_INVALIDO;

	return autores[numeroAutor];

}/*retorna un autor deseado*/


string RegistroCancion::get_letra()throw(){

	return letra;

}


int RegistroCancion::get_anio()throw(){

	return anioGrabacion.get_anio();

}/*retorna el anio de grabacion*/


string RegistroCancion::get_idioma()throw(){

	return idioma.getIdioma();

}/*retorna el idioma de la cancion*/


string RegistroCancion::get_titulo()throw(){

	return titulo;

}/*retorna el titulo de la cancion*/


int RegistroCancion::cargar(const char* dato,unsigned short tamanioDato)throw(){
 	int resCarga = RegistroVariable::agregar_datos(dato,tamanioDato);
	if (resCarga != RES_OK)
		return resCarga;
	
	bool val1 = (contar_parametros() == 0);
	bool val2 = (this->obtener_autores() == 0);
	bool val3 = (this->obtener_anio_grabacion() == -1);
	bool val4 = (this->obtener_titulo() == false);
	bool val5 = (this->obtener_idioma() == false);
	bool val6 = (this->obtener_letra() == false);
	
	bool noPudoValidar = val1 || val2 || val3 || val4 || val5 || val6;

	if (noPudoValidar){
		limpiar_buffer();
		return RES_ERROR;
	}

	return RES_OK;
}/*carga los datos del registro a partir de un buffer de datos*/


void RegistroCancion::limpiar_buffer()throw()
{

	RegistroVariable::limpiar_campos();

	if(autores)
		delete[] autores;
	autores= NULL;
	this->cantidadAutores= 0;
	anioGrabacion.resetear();
	titulo= "";
	letra= "";
}

string RegistroCancion::generar_clave()const throw()
{
	return "***";
}

int RegistroCancion::comprimir (Compresor & compresor)
{
	return RES_OK;
}
