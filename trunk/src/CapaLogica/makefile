include ../makefile.inc

DIRS	= ManejoArchivos Parser ArbolBMas Compresor Indices HashingExtensible
OBJLIBS	= ManejoArchivos.a Parser.a ArbolBMas.a Compresor.a Indices.a HashingExtensible.a
EXTERNOBJ =  $(foreach path, $(DIRS), $(wildcard $(path)/*.o))

TARGETS = main.cpp

all : $(OBJLIBS)

%.a:
	-for d in $(DIRS); do (cd $$d; $(MAKE)); done

main: $(EXTERNOBJ)
	$(CC) $(CFLAGS) $(EXTERNOBJ) -o $@ $(TARGETS)

clean :
	$(ECHO) Borrando archivos.
	find . -name "*.o" -type f -print | xargs /bin/rm -f
	find . -name "*.a" -type f -print | xargs /bin/rm -f
