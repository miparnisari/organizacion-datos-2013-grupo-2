include ../../makefile.inc

EXTERNOBJ =  $(filter-out $(TARGETS), $(foreach path, ., $(wildcard $(path)/*.cpp)))
OBJS = $(EXTERNOBJ:.cpp=.o)
AOBJS = $(OBJS:.o=.a)

%.a : $(OBJS)
	ar cr $@ $^
 
%.o: %.cpp
	$(ECHO) $(CC) -c $(CFLAGS) $<
	$(CC) -c $(CFLAGS) $<

main: $(OBJS)

clean:
	rm -f *.o
