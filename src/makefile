include makefile.inc

DIRS	= CapaFisica CapaLogica CapaInterfaz Utilitarios
SUBDIRS = CapaLogica/ArbolBMas CapaLogica/Compresor CapaLogica/HashingExtensible CapaLogica/Indexador CapaLogica/ManejoArchivos CapaLogica/Parser
OBJLIBS	= CapaFisica.a CapaLogica.a CapaInterfaz.a Utilitarios.a

EXTERNOBJ =  $(foreach path, $(DIRS), $(wildcard $(path)/*.o))
EXTERNOBJ-CAPALOGICA = $(foreach path, $(SUBDIRS), $(wildcard $(path)/*.o))

TARGETS = main.cpp
EXE	= main

all : $(EXE)

%.a :
	-for d in $(DIRS); do (cd $$d; $(MAKE)); done
	ar cr $@ $^
	
main: $(OBJLIBS)
	$(CC) $(CFLAGS) $(EXTERNOBJ) $(EXTERNOBJ-CAPALOGICA) -o $@ $(TARGETS)

clean :
	$(ECHO) Borrando archivos.
	-$(RM) -f ./main
	find . -name "*.o" -type f -print | xargs /bin/rm -f
	find . -name "*.a" -type f -print | xargs /bin/rm -f
