include ../src/makefile.inc

DIRS	= ../src/CapaFisica ../src/CapaLogica ../src/CapaInterfaz ../src/Utilitarios
SUBDIRS = ../src/CapaLogica/ArbolBMas ../src/CapaLogica/Compresor ../src/CapaLogica/HashingExtensible ../src/CapaLogica/Indexador ../src/CapaLogica/ManejoArchivos ../src/CapaLogica/Parser
OBJS = TestNodoInterno.o
OBJLIBS	= CapaFisica.a CapaLogica.a CapaInterfaz.a Utilitarios.a 

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
