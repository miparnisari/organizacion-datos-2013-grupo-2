#ifndef CONSTANTES_H
#define CONSTANTES_H

#include<string>

/*
 * Retorno de funciones
 */
const int RES_OK = 						 0;
const int RES_ERROR = 					-1;
const int RES_INSUFFICIENT_ARGUMENTS =	-2;
const int RES_FILE_EXISTS = 			-3;
const int RES_FILE_DOESNT_EXIST =		-4;
const int RES_RECORD_EXISTS =			-5;
const int RES_RECORD_DOESNT_EXIST =		-6;
const int RES_INVALID_FILENAME =		-7;
const int RES_FILE_FULL =				-8;
const int RES_RECORD_TOO_LONG = 		-9;
const int RES_EOF = 					-10;
const int RES_ALL_BLOCKS_OCCUPIED =		-11;
const int RES_ESPACIO_INSUFICIENTE = 	-12;
const int RES_DIRECTORY_DOESNT_EXIST = 	-13;
const int RES_INSUFFICIENT_SPACE	=	-14;
const int RES_INVALID_OFFSET		=	-15;

/*
 * Nombres de archivos
 */
const char FILENAME_IDX_PRIM[]			= "idx_prim.txt";
const char FILENAME_IDX_SECUN_AUTOR[]	= "idx_sec_autor.txt";
const char FILENAME_IDX_SECUN_TITULO[]	= "idx_sec_titulo.txt";
const char FILENAME_ARCH_MAESTRO[]		= "arch_maestro.txt";

/*
 * Manejo de bloques
 */
const unsigned short BLOQUE_TAM_DEFAULT				= 4096;		/*4 Kb*/
const unsigned short REG_VAR_MAX_TAM 				= 4096;     /*4 Kb*/
const char MARCA_BORRADO							= '*';
const std::string AUTOR_INVALIDO 					= "-";

const char SEPARADOR_PARAMETROS						= '-';
const char IDIOMA_INVALIDO[]						= "IDIOMA_INVALIDO";


/*
 * Funcionamiento basico
 */
const unsigned short CANTIDAD_MINIMA_PARAMETROS		= 3;
const unsigned short CANTIDAD_MAXIMA_PARAMETROS		= 4;

/*
 *Manejo del HashingExtensible      ***Seran modificados en el futuro
 */

const int MAXTAM                                    = 256;
const int DESBORDO                                  = -1;
const int ESTA_VACIO                                = 1;
const int YA_EXISTE                                 = 2;
const int NO_EXISTE                                 = 3;

#endif /* CONSTANTES_H */

