PROG = TwoPipesTwoChildren TwoPipesThreeChildren dynpipe
CC = g++
CPPFLAGS = -g -Wall
OBJS = TwoPipesTwoChildren.o TwoPipesThreeChildren.o DynPipe.o

all : $(PROG)

TwoPipesTwoChildren : TwoPipesTwoChildren.o
	$(CC) $(CPPFLAGS) -o TwoPipesTwoChildren TwoPipesTwoChildren.o
 
TwoPipesThreeChildren : TwoPipesThreeChildren.o
	$(CC) $(CPPFLAGS) -o TwoPipesThreeChildren TwoPipesThreeChildren.o

dynpipe : DynPipe.o
	$(CC) $(CPPFLAGS) -o dynpipe DynPipe.o

TwoPipesTwoChildren.o :
	$(CC) $(CPPFLAGS) -c TwoPipesTwoChildren.cpp

TwoPipesThreeChildren.o :
	$(CC) $(CPPFLAGS) -c TwoPipesThreeChildren.cpp

DynPipe.o :
	$(CC) $(CPPFLAGS) -c DynPipe.cpp

clean :
	rm -f core $(PROG) $(OBJS)
