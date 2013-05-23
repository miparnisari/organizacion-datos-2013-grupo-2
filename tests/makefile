include ../src/makefile.inc

DIRS	= ../src/Utilitarios ../src/CapaFisica ../src/CapaLogica ../src/CapaInterfaz 
SUBDIRS = ../src/CapaLogica/ManejoArchivos ../src/CapaLogica/ArbolBMas ../src/CapaLogica/Compresor ../src/CapaLogica/HashingExtensible ../src/CapaLogica/Indexador  ../src/CapaLogica/Indices ../src/CapaLogica/Parser
OBJS = Test.o TestTabla.o TestArbol.o TestManejadorArchivoDatosBasicos.o TestRegistroCancion.o TestHeap.o TestRegistroClave.o TestNodoInterno.o TestNodoSecuencial.o TestBloque.o TestHashingExtensible.o TestManejadorBloques.o TestManejadorRegistrosVariables.o TestRegistroVariable.o
OBJLIBS	= Utilitarios.a CapaFisica.a CapaLogica.a CapaInterfaz.a 

EXTERNOBJ =  $(foreach path, $(DIRS), $(wildcard $(path)/*.o))
EXTERNOBJ-CAPALOGICA = $(foreach path, $(SUBDIRS), $(wildcard $(path)/*.o))

TARGETS = tests.cpp
EXE	= tests

all : $(EXE)

%.o: %.cpp %.h
	$(CC) -c $(CFLAGS) $<

%.a :
	-for d in $(DIRS); do (cd $$d; $(MAKE)); done
	ar cr $@ $^

tests:  $(OBJLIBS) $(OBJS)
	$(CC) $(CFLAGS) $(EXTERNOBJ) $(OBJS) $(EXTERNOBJ-CAPALOGICA) -o $@ $(TARGETS)

clean :
	-$(RM) -f ./tests
	cd "../src"; make clean
	find . -name "*.o" -type f -print | xargs /bin/rm -f
	find . -name "*.a" -type f -print | xargs /bin/rm -f
	find . -name "*.dat" -type f -print | xargs /bin/rm -f
