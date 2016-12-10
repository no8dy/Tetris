# The executable program
TARGET = tetris
# Teh source programs [multiple files]
SOURCES =
INSTDIR = /usr/local/bin
INCLUDE = .
CXX = g++
CXXFLAGS = -Wall -lncurses
all:
	$(CXX) -I$(INCLUDE) $(CXXFLAGS) main.cpp -o $(TARGET) -lncurses
clean:
	rm -rf tetris *.o
