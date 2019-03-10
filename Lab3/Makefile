CC=gcc
CFLAGS=-c -Wextra -std=c11
LDFLAGS=-pthread
SOURCES=main.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -g -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf ${EXECUTABLE} 

help:
	@echo "Valid targets:"
	@echo "  all(default):  generates $(EXECUTABLE) executable in ./bin directory"
	@echo "        clean:   removes binary files"
