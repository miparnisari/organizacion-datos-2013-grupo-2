#ifndef REGISTRO_FIJO_H
#define REGISTRO_FIJO_H

#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "../Constantes.h"

// Clase que modela un registro que tiene
// campos de longitud fija, y por ende todo el registro es 
// de longitud fija.

class RegistroFijo
{
	public:
	
	// Por si en tiempo de compilacion se saben los tamanios de los campos
		RegistroFijo (int numFields, int fieldSizes[]);
	// Por si en tiempo de compilacion NO se saben los tamanios de los campos	
		RegistroFijo (int maxNumFields, int maxRecordSize);

		~RegistroFijo();
		
		void clear ();
		bool addFieldOfSize (int fieldSize);
		
		int getNumberOfFields ();
		int getRecordSize ();		
		int getFieldNumber (int number, std::string & field);
		bool pack (const char* string); //Empaqueta para escribir en disco
		bool unpack (char* string); //Desempaqueta lo que se leyo de disco
		int write (FILE* file) const;
		int read (FILE* file);
		
		void show ();
		
	private:
		char* record; //Contiene los valores de los campos
		int recordSize; //Suma de los tamanios de los campos declarados
		
		int* fieldSizes; //Tamanio de cada campo
		int numFields;  //Cantidad de campos declarados
		
		int maxNumFields;
		int maxRecordSize;
		
		int nextField; //Siguiente campo a leer o escribir
		int nextByte; //Siguiente byte a leer o escribir
		
};

#endif /* REGISTRO_FIJO_H */
