#ifndef ARCHIVOREGISTROSLONGITUDFIJA_H
#define ARCHIVOREGISTROSLONGITUDFIJA_H

#define REC_SIZE 4096
#define MAX_RECS 10000

#include <stdio.h>
#include "../Constantes.h"
#include "../Utilitarios/Utilitarios.h"
#include "RegistroFijo.h"

struct header_arlf
{
	//~ unsigned int h_firstEmptyRecordNumber; // Maximo 4294967295 registros
	unsigned int h_recordSize;
	unsigned int h_maxRecords;
};

// Clase que modela un archivo que tiene
// registros de longitud fija.
// Los registros NO estan ordenados.
class ArchivoRegistrosLongitudFija
{
	private:
		FILE* file;
		int recordSize;
		int maxRecs;
		int currentRecord;
		
		int initFileHeader ();
		int initEmptyRecords ();
		int getFileHeader(struct header_arlf& head);
		
	public:
		ArchivoRegistrosLongitudFija ();
		int create (std::string dir, std::string fileName, int recSize = REC_SIZE, int maxRecs = MAX_RECS);
		int open (std::string dir, std::string fileName, std::string mode);
		int destroy (std::string dir, std::string fileName);
		int close ();
		int seek (int numRec);		
		int read (int numRec, RegistroFijo & rec);
		int append (const RegistroFijo & rec);
		int write (const int numRec, const RegistroFijo & rec);
		int readNext (RegistroFijo & rec);		
		int updateRecord (int numRec, const RegistroFijo & rec);
		int deleteRecord (int numRec);

		int getMaxRecs ();
		int getRecordSize ();
		int getFirstEmptyRecordNumber ();
};

#endif /* ARCHIVOREGISTROSLONGITUDFIJA_H */
