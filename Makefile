# Nejaky makra
PRODUCTOBJS = myvoc.o word.o actions.o dictionary.o
PRODUCTFLAGS = -std=c99 -O
DEBUGOBJS = myvoc.do word.do actions.do dictionary.do unit_tests.do
DEBUGFLAGS = -std=c99 -DDEBUG -ggdb3

# cile
myvoc: $(PRODUCTOBJS)
	gcc -o $@ $(PRODUCTFLAGS) $(PRODUCTOBJS)

myvoc.o: myvoc.c
	gcc -o $@ $(PRODUCTFLAGS) -c myvoc.c

word.o: word.c word.h
	gcc -o $@ $(PRODUCTFLAGS) -c word.c

dictionary.o: dictionary.c dictionary.h
	gcc -o $@ $(PRODUCTFLAGS) -c dictionary.c

#Debuging target
myvoc.dbg: $(DEBUGOBJS)
	gcc -o $@ $(DEBUGFLAGS) $(DEBUGOBJS)

myvoc.do: myvoc.c minunit.h
	gcc -o $@ $(DEBUGFLAGS) -c myvoc.c

word.do: word.c word.h
	gcc -o $@ $(DEBUGFLAGS) -c word.c

dictionary.do: dictionary.c dictionary.h
	gcc -o $@ $(DEBUGFLAGS) -c dictionary.c

actions.do: actions.c actions.h
	gcc -o $@ $(DEBUGFLAGS) -c actions.c

#tady to je taky zavisly na novejch .do souborech
unit_tests.do: unit_tests.c unit_tests.h
	gcc -o $@ $(DEBUGFLAGS) -c unit_tests.c
