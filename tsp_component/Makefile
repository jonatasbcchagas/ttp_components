# macros
CPP= g++
CFLAGS= -O3 -w -m64 -std=c++11 -fopenmp
EXECUTABLE=concorde
CONCORDEPATH=../../../concorde
# targets
all: data.o main.o
	$(CPP) $(CFLAGS) -w -I$(CONCORDEPATH) -L$(CONCORDEPATH) -o $(EXECUTABLE) data.o main.o -lconcorde -lcplex -lm -lpthread -ldl

data.o: data.cpp data.h
	$(CPP) $(CFLAGS) -c data.cpp

main.o: main.cpp data.h
	$(CPP) $(CFLAGS) -I$(CONCORDEPATH) -L$(CONCORDEPATH) -c main.cpp

# remove
clean:
	rm -f ${EXECUTABLE} *.o
