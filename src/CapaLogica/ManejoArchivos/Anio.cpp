#include "Anio.h"

bool Anio::validar(string anio){

	int anioNumero= atoi(anio.c_str());
	if(anioNumero== 0){
		return false;
	}

	const unsigned short LIMITE_ANIO_SUPERIOR= 3000;

	if(anioNumero < 0 || anioNumero > LIMITE_ANIO_SUPERIOR){
		return false;
	}

	return true;

}/*valida un anio posible*/


bool Anio::cargar(string anioS){

	if(!validar(anioS)){
		anio= ANIO_DEFAULT;
		return false;
	}

	anio= atoi(anioS.c_str());

	return true;


}/*carga un anio*/


int Anio::get_anio(){return anio;}

void Anio::resetear(){
	anio= ANIO_DEFAULT;
}


Anio::Anio(){

	anio= ANIO_DEFAULT;

}/*constructor*/
