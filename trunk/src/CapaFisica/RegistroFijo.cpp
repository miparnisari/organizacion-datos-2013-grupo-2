#include "RegistroFijo.h"

// Por si en tiempo de compilacion SI se saben los tamanios de los campos
RegistroFijo :: RegistroFijo(int p_numFields, int p_fieldSizes[])
{
	// Calculamos el tamanio del registro buffer
	recordSize = 0;
	for (int i = 0; i < p_numFields ; i ++)
	{
		recordSize += p_fieldSizes[i] + sizeof(char); // Incluye los '\0'
	}
	
	maxNumFields = p_numFields;
	maxRecordSize = recordSize;
	
	// Pedimos memoria para el registro buffer
	record = (char*) calloc (1,recordSize); // 1 elemento de tamanio recSize
	
	numFields = p_numFields;
	nextField = 0;
	nextByte = 0;
	
	// Copiamos los tamanios de cada campo
	fieldSizes = (int*) calloc (numFields,sizeof(int)); //numFields elementos de 1 byte
	for (int j = 0; j < numFields; j++)
	{
		fieldSizes[j] = p_fieldSizes[j];		 
	}
}
	
// Por si en tiempo de compilacion NO se saben los tamanios de los campos
RegistroFijo :: RegistroFijo(int p_maxNumFields, int p_maxRecordSize)
{
	recordSize = 0; // Todavia no se sabe cuanto mide el registro
	numFields = 0; // Todavia no se sabe cuantos campos hay
	
	maxNumFields = p_maxNumFields;
	maxRecordSize = p_maxRecordSize + p_maxNumFields * sizeof(char);
	
	nextField = 0;
	nextByte = 0;
	
	record = (char*)calloc(1,p_maxRecordSize + p_maxNumFields * sizeof(char)); // Incluyo los \0
	fieldSizes = (int*) calloc (p_maxNumFields,sizeof(int));
}

RegistroFijo :: ~RegistroFijo()
{
	free(record);
	free(fieldSizes);
}

int RegistroFijo :: getNumberOfFields ()
{
	return numFields;
}

int RegistroFijo :: getRecordSize () 
{
	return recordSize;
}

int RegistroFijo :: getFieldNumber (int number, std::string & field)
{
	if (number > maxNumFields)
		return RES_ERROR;

	int pos = 0;
	
	for (int i = 0; i < number - 1; i++)
		pos += fieldSizes[i];
	
	return RES_OK;
}

bool RegistroFijo :: addFieldOfSize (int p_fieldSize)
{
	// Si no hay mas campos, o el campo es muy grande, no lo agregamos
	if (numFields == maxNumFields)
		return false;
	if (recordSize + p_fieldSize > maxRecordSize)
		return false;
	
	// Agregamos un campo al vector de tamanios de campos
	fieldSizes[numFields] = p_fieldSize;
	numFields ++;
	
	// Aumentamos el tamanio del registro
	recordSize += p_fieldSize + sizeof(char); // Incluyo el \0
	return true;
}

void RegistroFijo :: clear ()
{
	record[0] = 0;
	nextField = 0;
	nextByte = 0;
	memset(record,0,recordSize);
	memset(fieldSizes,0,numFields);
}

// Escribe en el proximo campo del registro buffer el valor de <string>
// Si <string> es mas chico que el tamanio del campo, rellena con blancos.
// Si es muy grande, trunca.
bool RegistroFijo :: pack (const char* string)
{
	if (nextField == numFields) 
		return false;
		
	int strlength = strlen(string);
	int start = nextByte;
	int bytesToPack = fieldSizes[nextField];
	
	strncpy (&record[start],string,strlength);
	
	nextField++;
	nextByte += bytesToPack;
	
	// Rellenamos con blancos y con el \0 al final
	for (int i = start + strlength + 1; i < nextByte ; i++)
		record[i] = ' ';
	record[nextByte] = '\0';
	
	return true;
}

// Precondicion: el archivo debe posicionarse en el registro,
// despues del flag (L/O).
// Lee el registro del archivo y lo escribe en el record.
int RegistroFijo :: read (FILE* file)
{
	int res = fread(record,recordSize,1,file);
	if (res != 1) {
		std::cout << "Couldn't read record. " << std::endl;
		return RES_ERROR;
	}
	
	std::cout << "Record read successfully! " << std::endl;
	return RES_OK;	
}

// Precondicion: el archivo debe posicionarse en el registro,
// antes del flag (L/O).
// Escribe el registro en el archivo.
int RegistroFijo :: write (FILE* file) const
{
	char flag = 'O';
	
	// Escribo el flag...
	int res = fwrite(&flag,sizeof(flag),1,file);
	if (res != 1) {
		std::cout << "Couldn't mark record as occupied. " << std::endl;
		return RES_ERROR;
	}
	
	// ... y escribo el registro.
	res = fwrite(record,recordSize,1,file);
	if (res != 1) {
		std::cout << "Couldn't write record. " << std::endl;
		return RES_ERROR;
	}
	
	std::cout << "Record written successfully! " << std::endl;
	return RES_OK;	
}

// Escribe en <string> el valor del proximo campo del registro buffer
bool RegistroFijo :: unpack (char* string)
{
	if (nextField == numFields)  
		return false;

	int start = nextByte;
	int bytesToUnpack = fieldSizes[nextField];
	
	strncpy(string, &record[start], bytesToUnpack);
	string[bytesToUnpack] = 0;
	nextByte += bytesToUnpack;
	nextField ++;
	
	if (nextField == numFields);
		clear();
		
	return true;
}

void RegistroFijo :: show ()
{
	std::cout << "Record size including /0 = " << getRecordSize() << std::endl;
	std::cout << "Number of fields = " << getNumberOfFields() << std::endl;
	
	int count = 0;
	
	for (int i = 0; i < numFields; i++)
	{
		char* field = (char*) calloc (1,fieldSizes[i] + 1);
		strncpy(field,&record[count],fieldSizes[i]);
		
		std::cout << "Field " << i << " = " << field << std::endl;
		
		count += fieldSizes[i];
		free(field);
	}	
}
