#ifndef CONSTANTES_H
#define CONSTANTES_H

#include<string>
#include<iostream>

using std::cout;
using std::endl;

#define IMPRIMIR_MY_VARIABLE(v)\
	cout<<#v<<" ="<<v<<endl;

typedef unsigned char Byte;
typedef unsigned short TamanioCampos;


/*
 * Retorno de funciones
 */
const int RES_OK = 					 0;
const int RES_ERROR = 					-1;
const int RES_INSUFFICIENT_ARGUMENTS =	-2;
const int RES_FILE_EXISTS = 			-3;
const int RES_FILE_DOESNT_EXIST =		-4;
const int RES_RECORD_EXISTS =			-5;
const int RES_RECORD_DOESNT_EXIST =	-6;
const int RES_INVALID_FILENAME =		-7;
const int RES_FILE_FULL =				-8;
const int RES_RECORD_TOO_LONG = 		-9;
const int RES_EOF = 					-10;
const int RES_ALL_BLOCKS_OCCUPIED =	-11;
const int RES_OVERFLOW =			 	-12;
const int RES_UNDERFLOW =				-13;
const int RES_DIRECTORY_DOESNT_EXIST =	-14;
const int RES_INSUFFICIENT_SPACE =		-15;
const int RES_INVALID_OFFSET =			-16;
const int RES_BLOQUE_NO_BORRADO	=	-17;
const int RES_FIN 					=	-18;
const int RES_ESCAPE 				= 	-19;


/*
 * Nombres de archivos
 */
const char FILENAME_IDX_PRIM[]			= "idx_prim";
const char FILENAME_IDX_SECUN_AUTOR[]	= "idx_sec_autor.dat";
const char FILENAME_IDX_SECUN_TITULO[]	= "idx_sec_titulo";
const char FILENAME_IDX_SECUN_FRASES[]	= "idx_sec_frases";
const char FILENAME_ARCH_MAESTRO[]		= "arch_maestro.dat";
const char FILENAME_ID_DOCS[]			= "id_docs";

/*
 * Manejo de bloques
 */
const unsigned short BLOQUE_TAM_DEFAULT			= 4096;		/*4 Kb*/
const unsigned short REG_VAR_MAX_TAM 				= 4096;     /*4 Kb*/

/*
 * Parseo de canciones
 */
const char MARCA_BORRADO							= (char)0;
const std::string AUTOR_INVALIDO 					= "-";

const char SEPARADOR_PARAMETROS					= '-';
const char IDIOMA_INVALIDO[]						= "IDIOMA_INVALIDO";
const unsigned short HIJO_INVALIDO				= 0;


/*
 * Funcionamiento basico
 */
const unsigned short CANTIDAD_MINIMA_PARAMETROS	= 3;
const unsigned short CANTIDAD_MAXIMA_PARAMETROS	= 4;

/*
 * Tipos de clave
 */
const unsigned int CANTIDAD_TIPOS_CLAVE= 2;
enum TipoClave {CLAVE_STRING=0 , CLAVE_NUMERICA};


/*
 * ArbolBMas
 * */

const char TIPO_HOJA= 'h';
const char TIPO_INTERNO= 'i';
const int NUMERO_BLOQUE_RAIZ = 1;

const long FIN_PILA_REGISTROS_LIBRES = -1;

const int RES_NO_UNDERFLOW 		= 0;
const int RES_UNDERFLOW_HOJA       = -1;
const int RES_UNDERFLOW_INTERNO	= -2;
const int RES_UNDERFLOW_RAIZ 		= -3;
const int RES_MERGE				= -4;
const int RES_BALANCEO				= -5;

/*
 * Ordenamiento Externo
 */

const unsigned int TAM_MAX_BUFFER_ORDENAMIENTO	=8192; //en bytes
const int CLAVE_TOPE								= 99999;

/*
 *Manejo del IndiceInvertido
 */
const unsigned int LONGITUD_MAXIMA_NUMERO_INDEXABLE = 4;
const int NO_EXISTE_INDICE                         	=-1;
const int NO_PERTENECE								= -2;
const int LISTA_VACIA								=-100;

/*
 * Compresion
 */
const unsigned int TAMANIO_ALFABETO = 256 + 1; // + ESC
const unsigned int PRECISION = 32;//se prueba con 9 se corre con 32 . CON 32 BITS ANDA EL ARITMETICO
const unsigned int CANTIDAD_DE_CONTEXTOS = 2;
const unsigned int VALOR_DEL_ESCAPE = 256;

#endif /* CONSTANTES_H */
