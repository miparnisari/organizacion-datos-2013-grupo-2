include makefile.inc

DIRS	= CapaFisica CapaLogica CapaInterfaz Utilitarios
SUBDIRS = CapaLogica/ArbolBMas CapaLogica/Compresor CapaLogica/HashingExtensible CapaLogica/Indices CapaLogica/ManejoArchivos CapaLogica/Parser
OBJETOS	= objetos

EXTERNOBJ =  $(foreach path, $(DIRS), $(wildcard $(path)/*.o))
EXTERNOBJ-CAPALOGICA = $(foreach path, $(SUBDIRS), $(wildcard $(path)/*.o))

TARGETS = main.cpp
EXE	= main

all : $(EXE)
compilar : $(OBJETOS)

$(OBJETOS) :
	-for d in $(DIRS); do (cd $$d; make); done
	
$(EXE): $(OBJETOS)
	$(ECHO) Linkeando $(EXE)...
	$(CC) $(CFLAGS) $(EXTERNOBJ) $(EXTERNOBJ-CAPALOGICA) -o $@ $(TARGETS)

clean :
	$(ECHO) Borrando archivos.
	-$(RM) -f ./main
	find . -name "*.o" -type f -print | xargs /bin/rm -f
