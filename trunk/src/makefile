include makefile.inc

DIRS	= CapaFisica CapaLogica CapaInterfaz Utilitarios
SUBDIRS = CapaLogica/ArbolBMas CapaLogica/Compresor CapaLogica/HashingExtensible CapaLogica/Indices CapaLogica/ManejoArchivos CapaLogica/Parser
OBJLIBS	= CapaFisica.a CapaLogica.a CapaInterfaz.a Utilitarios.a

EXTERNOBJ =  $(foreach path, $(DIRS), $(wildcard $(path)/*.o))
EXTERNOBJ-CAPALOGICA = $(foreach path, $(SUBDIRS), $(wildcard $(path)/*.o))

TARGETS = main.cpp
EXE	= main

all : $(EXE)

%.o: %.cpp %.h
	$(ECHO) $(CC) -c $(CFLAGS) $<
	$(CC) -c $(CFLAGS) $<

%.a :
	-for d in $(DIRS); do (cd $$d; $(MAKE)); done
	ar cr $@ $^
	
main: $(OBJLIBS)
	$(ECHO) Linkeando...
	$(CC) $(CFLAGS) $(EXTERNOBJ) $(EXTERNOBJ-CAPALOGICA) -o $@ $(TARGETS)

clean :
	$(ECHO) Borrando archivos.
	-$(RM) -f ./main
	find . -name "*.o" -type f -print | xargs /bin/rm -f
	find . -name "*.a" -type f -print | xargs /bin/rm -f
