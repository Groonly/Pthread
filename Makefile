BUILD_CPP =$(wildcard *.cpp)
OBJECTS=$(BUILD_CPP:.cpp=.o)
CC=g++
LIBS = -lpthread
LFLAGS = -g
CFLAGS = -c -std=c++14

main: $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) $(LIBS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $(BUILD_CPP)

clean:
	-rm -f $(OBJECTS) 
