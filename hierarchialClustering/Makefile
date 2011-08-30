#makefile for hierarchical clustering
CC=g++
CFLAGS=-c -g
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
EXE=test

$(EXE):$(OBJ)
	$(CC) $(OBJ) -o $@

test.o:Clustering.h
Cluster.o:Cluster.h string_util.h
Doc.o:Doc.h string_util.h
Clustering.o:Clustering.h Cluster.h Config.h DimArray.h

%.o:%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ) $(EXE)

