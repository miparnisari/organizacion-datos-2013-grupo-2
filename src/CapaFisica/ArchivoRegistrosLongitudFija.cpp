#include "ArchivoRegistrosLongitudFija.h"

ArchivoRegistrosLongitudFija :: ArchivoRegistrosLongitudFija ()
{
	file = NULL;
	recordSize = 0;
	maxRecs = 0;
	currentRecord = 0;
}

// Crea un archivo vacio con header en 0 y flags bandera para cada reg.
int ArchivoRegistrosLongitudFija :: create (std::string dir, std::string fileName, int recSize, int p_maxRecs)
{
	// Inicializo atributos del archivo
	recordSize = recSize;
	maxRecs = p_maxRecs;
	currentRecord = 0;
			
	if (! utilitarios::validFileName(fileName))
		return RES_INVALID_FILENAME;
		
	// Abro el archivo
	file = fopen((dir+fileName).c_str(),"wb+");
	if (file == NULL)
		return RES_ERROR;
	
	// Inicializo el file header con datos importantes
	if (initFileHeader() == RES_OK) 
	{
		// Marco cada registro como libre
		if (initEmptyRecords() == RES_OK)
			return RES_OK;
	}
	
	return RES_ERROR;		
}

int ArchivoRegistrosLongitudFija :: open (std::string dir, std::string fileName, std::string mode)
{
	// Abro el archivo
	file = fopen((dir+fileName).c_str(),mode.c_str());
	if (file == NULL) {
		std::cout << "File doesn't exist." << std::endl;
		return RES_ERROR;
	}
	
	// Leo el file header para obtener info del archivo
	struct header_arlf head;
	if (getFileHeader(head) == RES_ERROR)
		return RES_ERROR;
	
	recordSize = head.h_recordSize;
	maxRecs = head.h_maxRecords;
		
	return RES_OK;
}

int ArchivoRegistrosLongitudFija :: close ()
{	
	if (fclose(file) == 0)
		return RES_OK;
		
	return RES_ERROR;	
}

int ArchivoRegistrosLongitudFija :: destroy (std::string dir, std::string fileName)
{
	if (remove((dir+fileName).c_str()) == 0)
		return RES_OK;
		
	return RES_ERROR;	
}

// Precondicion: debe llamarse despues de initFileHeader ().
// Escribe el flag bandera (L/O) de cada registro.
int ArchivoRegistrosLongitudFija :: initEmptyRecords ()
{
	for (int i = 0; i < maxRecs; i++)
	{
		char recordFlag = 'L';
		fwrite(&recordFlag,sizeof(char),1,file);
		fseek(file,recordSize,SEEK_CUR);
	}
	
	return RES_OK;
}

// Precondicion: antes debe llamarse a open() y a create()
int ArchivoRegistrosLongitudFija :: getFileHeader (struct header_arlf& head)
{
	fseek(file,0,SEEK_SET);
	
	int read = fread(&head,sizeof(struct header_arlf),1,file);
	
	if (read != 1) {
		std::cout << "Couldn't read file header." << std::endl;
		return RES_ERROR;
	}
		
	return RES_OK;	
}

int ArchivoRegistrosLongitudFija :: getMaxRecs() 
{
	return maxRecs;
}

int ArchivoRegistrosLongitudFija :: getRecordSize()
{
	return recordSize;
}

/*
int ArchivoRegistrosLongitudFija :: getFirstEmptyRecordNumber()
{
	struct header_arlf head;
	if (getFileHeader(head) == RES_OK)
		return head.h_firstEmptyRecordNumber;
		
	return -1; // No hay registros vacios
}
* 
* */

/*
 * Precondicion: antes, debe llamarse a "open()".
 *  Inicializa el header del archivo con:
 * - offset al 1er registro libre (-1 si no hay libres)
 * - tamanio de cada registro
 * - cantidad maxima de registros
 */ 
int ArchivoRegistrosLongitudFija :: initFileHeader () 
{
	// El header
	struct header_arlf head;
	//~ head.h_firstEmptyRecordNumber = 0;
	head.h_recordSize = recordSize;
	head.h_maxRecords = maxRecs;
	
	// Lo escribo
	fseek(file,0,SEEK_SET);
	int result = fwrite(&head,sizeof(struct header_arlf),1,file);
	
	if (result != 1) {
		std::cout << "Error writing the data file header." << std::endl;
		return RES_ERROR;
	}
		
	return RES_OK;
}

// Posiciona el record pointer en el numero de celda indicado en numRec, 
// aun si la celda esta vacia. No devuelve ningun registro de datos.
int ArchivoRegistrosLongitudFija :: seek (int numRec)
{
	if (numRec > maxRecs) 
	{
		std::cout << "Record " << numRec << "doesn't exist." << std::endl;
		return RES_RECORD_DOESNT_EXIST;
	}
	
	// Avanzo (header) + (chars bandera de los registros) + (registros)
	int pos = sizeof(struct header_arlf) + (numRec * sizeof(char)) + (numRec * recordSize);
	int res = fseek(file,pos,SEEK_SET);
	if (res != 0) {
		return RES_ERROR;
	}
		
	return RES_OK;
}

// Agrega registros uno atras del otro.
int ArchivoRegistrosLongitudFija :: append (const RegistroFijo & rec)
{
	int result = write(currentRecord, rec);
	if (result == RES_OK) 
	{
		currentRecord ++;
		return result;
	}
	
	return result;
}

// Si la celda indicada por numRec esta vacia, escribe los datos del 
// registro fijo en dicha celda. Si esta ocupada,
// no escribe nada e informa de que no pudo concretar la operación.
int ArchivoRegistrosLongitudFija :: write (const int numRec, const RegistroFijo & rec)
{
	// Me posiciono ANTES del flag bandera del registro
	int positioning = seek (numRec);
	if (positioning == RES_RECORD_DOESNT_EXIST || positioning == RES_ERROR)
		return RES_ERROR;
	
	// Leo el flag bandera
	char flag;
	int res = fread(&flag,sizeof(flag),1,file);
	if (res != 1) {
		std::cout << "Couldn't read record flag of " << numRec << std::endl;
		return RES_ERROR;
	}
	
	// Si el flag es "O" (registro ocupado), devuelvo error
	if (flag == 'O')
		return RES_RECORD_EXISTS;
	
	// Si no, escribo el flag como 'O' y escribo el registro
	seek(numRec);
		
	return rec.write(file);
}

// Lee el registro dado por numRec y lo guarda en rec
// Si el registro esta libre, devuelve error.
int ArchivoRegistrosLongitudFija :: read (int numRec, RegistroFijo & rec) 
{
	// Me posiciono en el flag bandera del registro
	int positioning = seek (numRec);
	if (positioning == RES_RECORD_DOESNT_EXIST)
		return RES_RECORD_DOESNT_EXIST;
	
	// Leo el flag bandera
	char flag;
	int res = fread(&flag,sizeof(char),1,file);
	if (res != 1) {
		std::cout << "Couldn't read record flag of " << numRec << std::endl;
		return RES_ERROR;
	}
	
	// Si el registro no esta marcado como O (ocupado), hay error
	if (flag != 'O')
		return RES_RECORD_DOESNT_EXIST;
	
	// Si es 'O', Leo el registro	
	return rec.read(file);
}

/* Precondicion: se debe hacer un seek al primer registro buscado.
 */
int ArchivoRegistrosLongitudFija :: readNext (RegistroFijo & rec)
{
	// Leo el flag bandera
	char flag;
	int res = fread(&flag,sizeof(char),1,file);
	if (res != 1) {
		std::cout << "Couldn't read record flag. " << std::endl;
		return RES_ERROR;
	}
	
	// Si el registro no esta marcado como O (ocupado), hay error
	while (flag != 'O') 
	{
		fseek(file,recordSize,SEEK_CUR);
		res = fread(&flag,sizeof(char),1,file);
		if (res != 1) {
			std::cout << "Couldn't read record flag. " << std::endl;
			return RES_ERROR;
		}
		
	}
	
	// Si es 'O', Leo el registro	
	return rec.read(file);
}

/*
 * Si hay datos en la celda indicada por numRec, actualiza con los datos en reg. 
 * Si la celda está vacía, devuelve RES_RECORD_DOESNT_EXIST.
*/
int ArchivoRegistrosLongitudFija :: updateRecord (int numRec, const RegistroFijo & rec)
{
	// Me posiciono en el flag bandera del registro
	int positioning = seek (numRec);
	if (positioning == RES_RECORD_DOESNT_EXIST)
		return RES_RECORD_DOESNT_EXIST;
	
	// Leo el flag bandera
	char flag;
	int res = fread(&flag,sizeof(char),1,file);
	if (res != 1) {
		std::cout << "Couldn't read record flag of " << numRec << std::endl;
		return RES_ERROR;
	}
	
	// Si el registro no esta marcado como O (ocupado), hay error
	if (flag != 'O')
		return RES_RECORD_DOESNT_EXIST;
	
	// Si esta ocupado, simplemente lo sobreescribo
	seek (numRec);
	return rec.write(file);
}

int ArchivoRegistrosLongitudFija :: deleteRecord (int numRec)
{
	// Me posiciono en el flag bandera del registro
	int positioning = seek (numRec);
	if (positioning == RES_RECORD_DOESNT_EXIST)
		return RES_RECORD_DOESNT_EXIST;
	
	// Leo el flag bandera
	char flag;
	int res = fread(&flag,sizeof(char),1,file);
	if (res != 1) {
		std::cout << "Couldn't read record flag of " << numRec << std::endl;
		return RES_ERROR;
	}
	
	// Si el registro no esta marcado como "ocupado", no puedo borrarlo!
	if (flag != 'O')
		return RES_RECORD_DOESNT_EXIST;
	
	// Si estaba ocupado, lo marco como libre
	flag = 'L';
	seek (numRec);
	res = fwrite(&flag,sizeof(flag),1,file);
	if (res != 1) {
		std::cout << "Couldn't mark record as empty. " << std::endl;
		return RES_ERROR;
	}
	
	// ... y escribo el registro con ceros
	char* recordWithZeroes = (char*) calloc (1,recordSize);
	res = fwrite(recordWithZeroes,recordSize,1,file);
	free(recordWithZeroes);
	if (res != 1) {
		std::cout << "Couldn't fill free records with zero. " << std::endl;
		return RES_ERROR;
	}
	
	std::cout << "Record deleted succesfully! " << std::endl;
	return RES_OK;
}
