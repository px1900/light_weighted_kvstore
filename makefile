# makefile to build the project

INCLUDES = -I./includes -I. -L. -I/usr/local/include -I/usr/include -L/usr/local/lib
LINKLIB = -lstdc++ -lpthread
CFLAG = -std=c++17
COMPILER = g++

HEADFILEs = $(wildcard ./includes/*.h)

all: main

main: main.o unit_test_concurrent_hashmap.o $(HEADFILEs)
	$(COMPILER) $(CFLAG) -o $@ $^ $(INCLUDES) $(LINKLIB)

main.o: main.cpp $(HEADFILEs)

#concurrent_hashmap.o:  $(HEADFILEs)
#	$(COMPILER) $(CFLAG) -c $< $(INCLUDES) $(LINKLIB)

unit_test_concurrent_hashmap.o: unit_test_concurrent_hashmap.cpp $(HEADFILEs)
	$(COMPILER) $(CFLAG) -c $< $(INCLUDES) $(LINKLIB)

clean:
	rm -rf *.o main

