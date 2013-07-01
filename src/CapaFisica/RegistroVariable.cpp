/*
 * RegistroVariable.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: martin
 */

#include "RegistroVariable.h"

RegistroVariable::RegistroVariable()
{
	buffer= NULL;
	tamanio= 0;
}

RegistroVariable& RegistroVariable::operator = (const RegistroVariable& otro)
{
	if (this != &otro)
	{
		delete[] buffer;
		this->tamanio = otro.tamanio;
		this->buffer = new char[tamanio];
		memcpy(buffer,otro.buffer,tamanio);
	}
	return (*this);
}

RegistroVariable::~RegistroVariable()
{
	delete[] buffer;
}

RegistroVariable::RegistroVariable(const RegistroVariable& otro)
{
	tamanio = otro.tamanio;
	buffer = new char[tamanio];
	memcpy(buffer,otro.buffer,tamanio);
}


void RegistroVariable::_inicializar_buffer(){

	if (buffer == NULL)
		return;
	for(TamanioCampos i=0;i<this->tamanio;i++)
		buffer[i]= '*';
}

int RegistroVariable::agregar_datos(const char* datos, TamanioCampos tamanioDatos)throw(){

	if (this->fue_eliminado() )
	{
		return RES_ERROR;
	}
	TamanioCampos tamanioFinal = tamanio + tamanioDatos;

	if (tamanioFinal > REG_VAR_MAX_TAM)
	{
		return RES_RECORD_TOO_LONG;
	}

	char* copia = new char[tamanioFinal]();
	memcpy(copia,buffer,tamanio);
	memcpy(copia+tamanio,datos,tamanioDatos);


	tamanio = tamanioFinal;
	delete[] buffer;
	buffer = new char[tamanioFinal]();
	memcpy(buffer,copia,tamanioFinal);

	delete[] copia;
	return RES_OK;

}/*agrega datos al buffer menor que REG_VAR_MAX_TAM.
Los datos se agregan de forma directa sin separadores*/


int RegistroVariable::agregar_campo(const char* campo,TamanioCampos tamanioCampo)throw(){

	int res=this->agregar_datos( (char*)&tamanioCampo, sizeof(tamanioCampo) );

	if (res!= RES_OK)
		return RES_ERROR;

	return this->agregar_datos(campo,tamanioCampo);

}/*agrega un campo al registro junto con su delimitador de espacio.*/


void RegistroVariable::limpiar_campos()throw()
{
	if(buffer){
		delete[] buffer;
		buffer = NULL;
		tamanio = 0;
	}
}


std::string RegistroVariable::mostrar_campo(TamanioCampos numeroCampo){

	if(buffer==NULL)
		return "***";
	if(this->fue_eliminado())
		return "REGISTRO_ELIMINADO";
	if(numeroCampo >= this->get_cantidad_campos())
		return "***";

	const TamanioCampos TAMANIO_CAMPO= this->get_tamanio_campo(numeroCampo);
	char* campo= new char[TAMANIO_CAMPO]();
	this->recuperar_campo(campo,numeroCampo);
	string retornar(campo,TAMANIO_CAMPO);
	delete[] campo;
	return retornar;

}


std::string RegistroVariable::mostrar()throw()
{
	string retornar;

	for(TamanioCampos i= 0; i<this->get_cantidad_campos() ; i++){
		retornar+= mostrar_campo(i) + '|';
	}

	return retornar;
}


int RegistroVariable::eliminar()throw(){

	if(this->fue_eliminado())
		return RES_OK;


	TamanioCampos uno= 1;

	memcpy(buffer,&uno,sizeof(uno));
	memcpy(buffer+sizeof(uno),&MARCA_BORRADO,sizeof(MARCA_BORRADO));


	return RES_OK;
}/*marca al registro como eliminado:
en el primer campo, en su longitud, pone "1", y en el valor pone
un caracter de marca de borrado * */

bool RegistroVariable::fue_eliminado()throw(){

	if(buffer==NULL)
		return false;

	TamanioCampos tamanioPrimerCampo= 0;

	memcpy(&tamanioPrimerCampo,buffer,sizeof(tamanioPrimerCampo));

	if(tamanioPrimerCampo > 1)
		return false;

	char primerCampo=1;
	memcpy(&primerCampo,buffer+sizeof(tamanioPrimerCampo),sizeof(primerCampo));

	if( primerCampo != MARCA_BORRADO )
		return false;

	return true;


}/*verifica si un registro fue marcado como eliminado*/


int RegistroVariable::empaquetar(char* copia)throw(){

	if(buffer== NULL || copia== NULL)
		return RES_ERROR;

	memcpy(copia,&tamanio,sizeof(tamanio));
	memcpy(copia+sizeof(tamanio),buffer,tamanio);

	return RES_OK;

}/*exporta en un arreglo de caracteres
 el contenido del registro junto con
 el tamanio del mismo insertado al principio del arreglo*/


TamanioCampos RegistroVariable::get_cantidad_campos()throw(){

	if(this->fue_eliminado())
		return 0;
	if(buffer== NULL)
		return 0;

	TamanioCampos contadorOffset= 0;
	TamanioCampos contadorCampos= 0;
	TamanioCampos tamanioCampo;

	while(contadorOffset < tamanio){
		memcpy(&tamanioCampo,buffer+contadorOffset,sizeof(tamanioCampo));
		contadorOffset += tamanioCampo + sizeof(tamanioCampo);
		contadorCampos ++;
	}

	return contadorCampos;

}/*cuenta la cantidad de campos almacenados en el registro*/


int RegistroVariable::seek_numero_campo(TamanioCampos numeroCampo){

	TamanioCampos contadorCampo= 0;
	TamanioCampos offsetCampo= 0;
	TamanioCampos tamanioCampo;

	while(contadorCampo < numeroCampo){

		memcpy(&tamanioCampo,buffer+offsetCampo,sizeof(tamanioCampo));
		offsetCampo += sizeof(tamanioCampo)+tamanioCampo;
		contadorCampo++;
	}

	return offsetCampo;
}


int RegistroVariable::get_tamanio_campo(TamanioCampos numeroCampo)
{
	if (this->fue_eliminado())
		return RES_ERROR;
	if (numeroCampo >= this->get_cantidad_campos())
		return RES_ERROR;

	int offsetCampo = this->seek_numero_campo(numeroCampo);
	TamanioCampos tamanioCampo;
	memcpy(&tamanioCampo,buffer+offsetCampo,sizeof(tamanioCampo));

	return tamanioCampo;
}

int RegistroVariable::recuperar_campo(char* copia,TamanioCampos numeroCampo)throw(){

	if(this->fue_eliminado() && numeroCampo> CAMPO_ENCADENAMIENTO_LIBRES)
		return RES_ERROR;

	if (numeroCampo >= this->get_cantidad_campos())
		return RES_ERROR;

	TamanioCampos contadorCampos= 0;
	TamanioCampos tamanioCampo;
	TamanioCampos offsetCampos = 0;

	while(contadorCampos < numeroCampo){

		memcpy(&tamanioCampo,buffer+offsetCampos,sizeof(tamanioCampo));
		contadorCampos++;
		offsetCampos += tamanioCampo + sizeof(tamanioCampo);
	}

	memcpy(&tamanioCampo,buffer+offsetCampos,sizeof(tamanioCampo));
	memcpy(copia,buffer+offsetCampos+sizeof(tamanioCampo),tamanioCampo);

	return tamanioCampo;

}/*recupera un campo del registro y lo inserta en el buffer copia.
Retorna la cantidad de bytes leidos*/


int RegistroVariable::desempaquetar(const char* copia)throw()
{
	if(copia== NULL) {
		return RES_ERROR;
	}

	limpiar_campos();

	TamanioCampos tamanioLeido;

	memcpy(&tamanioLeido, copia, sizeof(tamanioLeido));
	if(tamanioLeido > REG_VAR_MAX_TAM)
		return RES_RECORD_TOO_LONG;

	tamanio = tamanioLeido;
	buffer= new char[tamanio]();
	memcpy(buffer , copia+sizeof(tamanio) , tamanio);


	return RES_OK;

}/*importa un registro a partir de un arreglo
con el formato tamanioRegistro|tamanioPrimerCampo|datos...*/


int RegistroVariable::get_tamanio_empaquetado()const
{
	return (sizeof(tamanio)+tamanio) ;
}


char* RegistroVariable::get_buffer()throw()
{
	return buffer;

}

TamanioCampos RegistroVariable::get_tamanio()throw()
{
	return tamanio;
}

int RegistroVariable :: descomprimir(Compresor * compresor, RegistroVariable* reg_descomp)
{
	TamanioCampos tamanioOriginal;
	recuperar_campo((char*)&tamanioOriginal,0);

	TamanioCampos tamanioComprimido= this->get_tamanio_campo(1);
	char* bufferComprimido= new char[tamanioComprimido]();
	recuperar_campo(bufferComprimido,1);

	char* descomprimido = new char[tamanioOriginal + sizeof(tamanioOriginal)]();
	memcpy( descomprimido, (char*)&tamanioOriginal , sizeof(tamanioOriginal) );
	compresor->descomprimir_todo(bufferComprimido,tamanioComprimido,descomprimido+sizeof(tamanioOriginal),tamanioOriginal);

	reg_descomp->desempaquetar( descomprimido );

	delete[] bufferComprimido;
	delete[] descomprimido;

	return RES_OK;
}

RegistroVariable* RegistroVariable::comprimir (Compresor * compresor)
{
	char* bufferComprimido = new char[tamanio]();
	int tamanioCompresion = compresor->comprimir_todo(buffer,tamanio,bufferComprimido);

	RegistroVariable* reg_comprimido = new RegistroVariable();
	reg_comprimido->agregar_campo( (char*)&tamanio, sizeof(tamanio) );
	/*agrego un campo que representa la cantidad de caracteres originales de la fuente . */
	reg_comprimido->agregar_campo(bufferComprimido,tamanioCompresion);

	delete[] bufferComprimido;

	return reg_comprimido;
}/*se debe liberar la memoria de la variable retornada*/
