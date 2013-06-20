include ../../makefile.inc

EXTERNOBJ =  $(filter-out $(TARGETS), $(foreach path, ., $(wildcard $(path)/*.cpp)))
OBJS = $(EXTERNOBJ:.cpp=.o)

%.o: %.cpp
	$(ECHO) $(CC) -c $(CFLAGS) $<
	$(CC) -c $(CFLAGS) $<

main: $(OBJS)

clean:
	rm -f *.o
