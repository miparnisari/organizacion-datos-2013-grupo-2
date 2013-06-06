#include "ArchivoListas.h"


ArchivoListas::ArchivoListas()
{

}

ArchivoListas::~ArchivoListas()
{

}

int ArchivoListas::crear(std::string directorioSalida, std::string fileNamee)
{
    return this->archivo.crear_archivo(directorioSalida+fileNamee+".dat");
}

int ArchivoListas::eliminar(std::string directorioSalida, std::string fileNamee)
{
    return this->archivo.eliminar_archivo(directorioSalida+fileNamee+".dat");
}

int ArchivoListas::get_cantidad_listas()
{
    return this->archivo.get_cantidad_registros_ocupados();
}

int ArchivoListas::abrir(std::string directorioSalidaa, std::string fileNamee)
{
    this->fileName = directorioSalidaa+fileNamee+".dat";
    this->directorioSalida = directorioSalidaa;
    return this->archivo.abrir_archivo(this->fileName);
}

short ArchivoListas::agregar(RegistroVariable *listaDeCanciones)
{
	short pos = this->archivo.get_cantidad_registros_ocupados();
	if (this->archivo.agregar_registro(listaDeCanciones) >0)
		return pos;
	return RES_ERROR;
}

int ArchivoListas::devolver(RegistroVariable *listaDeCanciones, unsigned short pos_lista)
{
    return (this->archivo.get_registro_ocupado(listaDeCanciones, pos_lista));
}

int ArchivoListas::recontruir_listas(unsigned short* ref_listas, unsigned short cant_ref, int ID)
{
	string file1 = this->directorioSalida+"ListaAuxiliar.dat";
	string file2 = this->fileName;
    ManejadorRegistrosVariables listas_nuevas;
    RegistroVariable lista;
    int resultado = listas_nuevas.crear_archivo(file1);
    if (resultado != RES_OK){
    	delete ref_listas;
    	return resultado;
    }
   resultado = listas_nuevas.abrir_archivo(file1);
    if (resultado != RES_OK)
    {
    	delete ref_listas;
    	return resultado;
    }
    int i, j=0;
    int cant_listas = this->archivo.get_cantidad_registros_ocupados();
    for(i=0; i<cant_listas; i++){
        //Obtengo la lista
        this->archivo.get_registro_ocupado(&lista, i);
        if(*(ref_listas+j) == i){
            //I es una de las referencias a las listas que hay que agregarle el IDcancion
            lista.agregar_campo((char*)&ID,sizeof(ID));
            j++;
        }
        listas_nuevas.agregar_registro(&lista);
    }
//    while(j<cant_ref){
//        //Creo una lista vacia y le agrego el IDcancion
//        RegistroVariable lista;
//        lista.agregar_campo((char*)&ID,sizeof(ID));
//        listas_nuevas.agregar_registro(&lista);
//    } // FIXME(nadia) Loop infinito!!
    this->archivo.eliminar_archivo(this->fileName);
    delete ref_listas;
    //Cambiamos el nombre del archivo por el original
    rename(file1.c_str(),file2.c_str());
    return RES_OK;
}

int ArchivoListas::recontruir_listas(unsigned short ref_lista, RegistroVariable &listaModificada)
{
	string file1 = this->directorioSalida+"ListaAuxiliar.dat";
	string file2 = this->fileName;
	ManejadorRegistrosVariables listas_nuevas;
	unsigned short i;
	int cant_listas= this->archivo.get_cantidad_registros_ocupados();
	RegistroVariable lista;
	int resultado;
	resultado = listas_nuevas.crear_archivo(file1);
	if (resultado != RES_OK)
		return resultado;
	listas_nuevas.abrir_archivo(file1);
	if (resultado != RES_OK)
		return resultado;
	for(i=0; i<cant_listas; i++)
	{
	        //Obtengo la lista
	        this->archivo.get_registro_ocupado(&lista, i);
	        if(ref_lista == i){
	            //I es una de las referencias a las listas que hay que agregarle el IDcancion
	        	listas_nuevas.agregar_registro(&listaModificada);
	        }else{
	        	listas_nuevas.agregar_registro(&lista);
	        }
	}
	this->archivo.eliminar_archivo(this->fileName);
	//Cambiamos el nombre del archivo por el original
	rename(file1.c_str(),file2.c_str());
	return RES_OK;
}
