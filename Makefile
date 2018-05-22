BUILD_CPP =$(wildcard *.cpp)
OBJECTS=$(BUILD_CPP:.cpp=.o)
CC=G++
LIBS = -lpthread
LFLAGS = -g
CFLAGS = -c

test: $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) $(LIBS) -o PhreadQueue

%.o: %.cpp
	$(CC) $(CFLAGS) $(BUILD_CPP)

clean:
	-rm -f $(OBJECTS) 
