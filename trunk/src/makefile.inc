# Definiciones comunes a todos los make
CC	= g++
CFLAGS = -Wall -std=c++0x -Wunused -Wunreachable-code -g -ggdb

#Comando para remover
RM	= rm

#Comando para imprimir por consola
ECHO	= echo

#Shell a utilizar
SHELL	= /bin/bash

.SILENT :
