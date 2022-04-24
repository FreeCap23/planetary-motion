# Compiler to use
CC = g++

# Compiler flags
SFMLDEPS = -lsfml-audio-d -lsfml-graphics-d -lsfml-system-d -lsfml-window-d

# Source files
SOURCES = src/main.cpp src/Planet.cpp

# Object files
OBJ = main.o Planet.o

# Target file
TARGET = planets

all:
	if [ ! -d "./build" ]; then \
		mkdir ./build; \
	fi
	$(CC) $(SOURCES) -c && $(CC) $(SFMLDEPS) $(OBJ) -o build/$(TARGET)

run: all
	./build/$(TARGET)

clean:
	$(RM) -r $(OBJ) build