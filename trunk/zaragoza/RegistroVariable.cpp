/*
 * RegistroVariable.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: martin
 */

#include "RegistroVariable.h"

int RegistroVariable::agregar_datos(char* datos,unsigned int tamanioDatos)throw(){

	if(!datos)
		return FRACASO;
	int tamanioFinal= tamanio + tamanioDatos;
	if(tamanioFinal >= TAMANIO_MAXIMO)
		return TAMANIO_EXCEDIDO;

	stringstream stream(ios::in | ios::out);
	if(buffer)
		stream.write(buffer,tamanio);
	stream.write(datos,tamanioDatos);
	limpiar();

	tamanio= tamanioFinal;
	buffer= new char[tamanioFinal];
	stream.seekg(0,ios::beg);
	stream.read(buffer,tamanio);

	return EXITO;

}/*agrega datos al buffer menor que TAMANIO_MAXIMO.*/


void RegistroVariable::limpiar()throw(){

	if(buffer){
		delete[] buffer;
		buffer= NULL;
		tamanio= 0;
	}

}/*limpia el buffer*/


int RegistroVariable::copiar_datos(char* copia,unsigned int tamanioCopia,unsigned int offset)throw(){

	if(!copia)
		return FRACASO;
	if(tamanioCopia== 0)
		return FRACASO;
	if(!buffer)
		return FRACASO;
	if(offset > tamanio)
		return FRACASO;

	unsigned int bytesCopiar= tamanioCopia;
	if(bytesCopiar > tamanio)
		bytesCopiar= tamanio;
	unsigned int byteFinal= bytesCopiar+offset;
	if(byteFinal > tamanio){
		unsigned int diferencia= byteFinal - tamanio;
		bytesCopiar-= diferencia;
	}

	stringstream stream(ios::in | ios::out);
	stream.write( (char*)(buffer+offset) , bytesCopiar );
	stream.seekg(0,ios::beg);
	stream.read(copia,bytesCopiar);

	return bytesCopiar;


}/*copia datos del buffer al arreglo copia. Si tamanioCopia > tamanioBuffer -> se copian todos los datos*/


void RegistroVariable::mostrar()throw(){

	if(!buffer)
		return;

	stringstream stream;

	stream.write(buffer,tamanio);
	stream<<'\n';
	stream.seekg(0,ios::beg);

	cout<<stream.rdbuf();

}/*muestra el registro*/


int RegistroVariable::emitir()throw(){

	if(!buffer)
		return FRACASO;

	cout<<buffer;

	return EXITO;

}


int RegistroVariable::guardar(ostream& stream)throw(){

	if(!stream.good())
		return FRACASO;

	stream.write((char*)(&tamanio),sizeof(tamanio));
	stream.write(buffer,tamanio);

	return EXITO;

}/*escribe los datos dentro del registro en el stream*/


int RegistroVariable::eliminar()throw(){

	if(buffer== NULL)
		return FRACASO;

	stringstream stream;
	stream.write(buffer,tamanio);
	unsigned int uno= 1;
	stream.seekp(0,ios::beg);
	char marcaBorrado= MARCA_BORRADO;

	stream.write( (char*)(&uno),sizeof(uno) );
	stream.write( (char*)(&marcaBorrado) , sizeof(marcaBorrado) );
	stream.seekg(0,stream.beg);

	stream.read(buffer,tamanio);

	return EXITO;
}/*marca al registro como eliminado colocando en el primer campo del mismo un caracter de marca de borrado * */


bool RegistroVariable::fue_eliminado()throw(){

	stringstream stream;
	stream.write(buffer,tamanio);
	stream.seekg(0,stream.beg);
	unsigned int tamanioPrimerCampo;
	char primerCampo;

	stream.read( (char*)(&tamanioPrimerCampo) , sizeof(tamanioPrimerCampo) );
	if(tamanioPrimerCampo > 1)
		return false;

	stream.read( (char*)(&primerCampo) , sizeof(primerCampo) );
	if( primerCampo!= MARCA_BORRADO )
		return false;

	return true;


}/*verifica si un registro fue marcado como eliminado*/


int RegistroVariable::exportar(char* copia)throw(){

	if(buffer== NULL || copia== NULL)
		return FRACASO;

	stringstream stream;

	stream.write( (char*)(&tamanio) , sizeof(tamanio) );
	stream.write( buffer,tamanio );
	stream.seekg(0,stream.beg);
	stream.read(copia, tamanio+sizeof(tamanio) );

	return EXITO;

}/*exporta en un arreglo de caracteres el contenido del registro junto con el tamanio del mismo insertado al principio del arreglo*/




int RegistroVariable::importar(char* copia)throw(){

	if(copia== NULL)
		return FRACASO;

	limpiar();
	stringstream stream;
	stream.write(copia,sizeof(tamanio));
	stream.seekg(0,stream.beg);
	unsigned int tamanioLeido;

	stream.read( (char*)(&tamanioLeido),sizeof(tamanioLeido) );
	if(tamanioLeido > TAMANIO_MAXIMO)
		return TAMANIO_EXCEDIDO;

	tamanio= tamanioLeido;
	buffer= new char[tamanio];
	stream.read(buffer,tamanio);

	return EXITO;


}/*importa un registro a partir de un arreglo con el formato tamanioRegistro|tamanioPrimerCampo|datos...*/


RegistroVariable::RegistroVariable(){

	buffer= NULL;
	tamanio= 0;

}/*constructor*/



RegistroVariable::~RegistroVariable(){

	if(buffer)
		delete[] buffer;

}/*destructor*/
