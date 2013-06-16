include ../src/makefile.inc

DIRS	= ../src/Utilitarios ../src/CapaFisica ../src/CapaLogica ../src/CapaInterfaz 
SUBDIRS = ../src/CapaLogica/ManejoArchivos ../src/CapaLogica/ArbolBMas ../src/CapaLogica/Compresor ../src/CapaLogica/HashingExtensible ../src/CapaLogica/Indexador  ../src/CapaLogica/Indices ../src/CapaLogica/Parser
OBJS = TestUtilitarios.o TestIntervalo.o TestParserCanciones.o TestClave.o TestArchivoListas.o TestIndicePorTitulo.o TestTabla.o TestArbol.o TestManejadorArchivoDatosBasicos.o TestRegistroCancion.o TestOrdenamientoExterno.o TestHeap.o  TestRegistroClave.o TestNodoInterno.o TestNodoSecuencial.o TestBloque.o TestHashingExtensible.o TestManejadorBloques.o TestManejadorRegistrosVariables.o TestRegistroVariable.o TestIndiceInvertidoPorFrase.o TestIndicePorAutor.o TestNormalizador.o TestCompresorAritmetico.o TestCodigo.o TestBufferBits.o
OBJLIBS	= Utilitarios.a CapaFisica.a CapaLogica.a CapaInterfaz.a 
GTEST_LIB = ../lib/gtest-1.6.0/make/gtest.a
EXTERNOBJ =  $(foreach path, $(DIRS), $(wildcard $(path)/*.o))
EXTERNOBJ-CAPALOGICA = $(foreach path, $(SUBDIRS), $(wildcard $(path)/*.o))

TARGETS = tests.cpp
EXE	= tests

all : $(EXE)

%.o: %.cpp
	$(ECHO) $(CC) $(CFLAGS) -c -o  $@ $<
	$(CC) $(CFLAGS) -c -o  $@ $<

%.a :
	-for d in $(DIRS); do (cd $$d; $(MAKE)); done
	ar cr $@ $^

tests:  $(OBJLIBS) $(OBJS)
	$(ECHO) Linkeando...
	$(CC) $(CFLAGS) $(EXTERNOBJ) $(OBJS) $(EXTERNOBJ-CAPALOGICA) $(GTEST_LIB) -lpthread -o $@ $(TARGETS)

clean :
	-$(RM) -f ./tests
	cd "../src"; make clean
	find . -name "*.o" -type f -print | xargs /bin/rm -f
	find . -name "*.a" -type f -print | xargs /bin/rm -f
	find . -name "*.dat" -type f -print | xargs /bin/rm -f
