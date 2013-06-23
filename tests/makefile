include ../src/makefile.inc

DIRS	= ../src/Utilitarios ../src/CapaFisica ../src/CapaLogica ../src/CapaInterfaz 
SUBDIRS = ../src/CapaLogica/ManejoArchivos ../src/CapaLogica/ArbolBMas ../src/CapaLogica/Compresor ../src/CapaLogica/HashingExtensible ../src/CapaLogica/Indexador  ../src/CapaLogica/Indices ../src/CapaLogica/Parser
OBJS = TestModeloProbabilistico.o TestPPMC.o TestUtilitarios.o TestIntervalo.o TestParserCanciones.o TestClave.o TestArchivoListas.o TestIndicePorTitulo.o TestTabla.o TestArbol.o TestManejadorArchivoDatosBasicos.o TestRegistroCancion.o TestOrdenamientoExterno.o TestHeap.o  TestRegistroClave.o TestNodoInterno.o TestNodoSecuencial.o TestBloque.o TestHashingExtensible.o TestManejadorBloques.o TestManejadorRegistrosVariables.o TestRegistroVariable.o TestIndiceInvertidoPorFrase.o TestIndicePorAutor.o TestNormalizador.o TestCompresorAritmetico.o TestBufferBits.o TestContexto.o
GTEST_LIB = ../lib/gtest-1.6.0/make/gtest.a
EXTERNOBJ =  $(foreach path, $(DIRS), $(wildcard $(path)/*.o))
EXTERNOBJ-CAPALOGICA = $(foreach path, $(SUBDIRS), $(wildcard $(path)/*.o))

TESTS_NECESARIOS = lostests
SRC_NECESARIO = source
TARGETS = tests.cpp
EXE	= tests
OBJETOS = objetos

all : $(EXE)

%.o: %.cpp
	$(ECHO) $(CC) -c $(CFLAGS) $<
	$(CC) -c $(CFLAGS) $<

$(TESTS_NECESARIOS): $(OBJS)

$(SRC_NECESARIO):
	-for d in ../src; do (cd $$d; make compilar); done

tests: $(SRC_NECESARIO) $(TESTS_NECESARIOS)
	$(ECHO) Linkeando $(EXE)...
	$(CC) $(CFLAGS) $(EXTERNOBJ) $(OBJS) $(EXTERNOBJ-CAPALOGICA) $(GTEST_LIB) -lpthread -o $@ $(TARGETS)

clean :
	-$(RM) -f ./tests
	cd "../src"; make clean
	find . -name "*.o" -type f -print | xargs /bin/rm -f
	find . -name "*.dat" -type f -print | xargs /bin/rm -f
