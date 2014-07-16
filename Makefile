CC     = g++
CFLAGS = -O2 -Wall

SOURCES       = main.cpp \
                Led.cpp \
                Server.cpp
OBJECTS       = main.o \
                Led.o \
                Server.o
EXECUTABLE    = server

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@

clean_all:
	rm -f *.o
	rm -f server
clean:
	rm -f *.o
