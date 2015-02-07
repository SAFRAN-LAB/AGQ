CC	=g++-4.9
CFLAGS	=-c -Wall -Ofast -ffast-math -I ./../Quadrature/Gauss_Legendre/header/ -I ./header/
LDFLAGS	=-Ofast
SOURCES	=./examples/test2D.cpp
OBJECTS	=$(SOURCES:.cpp=.o)
EXECUTABLE	=./exec/quadrature2D

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.out ./examples/*.o ./exec/*

tar:
	tar -zcvf AGQ.tar.gz ./makefile1D.mk ./makefile2D.mk ./exec ./header ./examples ./README.md ./LICENSE.md