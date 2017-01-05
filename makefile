OBJS=main.o Graph.o Algs.o Communities.o
main : $(OBJS)
	g++ -o main $(OBJS)
main.o : main.cpp Graph.h Algs.h Communities.h
Graph.o : Graph.h Graph.cpp
Algs.o : Algs.h Algs.cpp
Communities.o : Communities.h Communities.cpp

clean:
	rm -f main.o Graph.o Algs.o Communities.o