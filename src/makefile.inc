# Definiciones comunes a todos los make

CC	= g++
CFLAGS = -Wall -pedantic -pedantic-errors -Wunused -Wunreachable-code -g -ggdb

#Comando para remover
RM	= rm

#Comando para imprimir por consola
ECHO	= echo

#Shell a utilizar
SHELL	= /bin/bash

.SILENT :
