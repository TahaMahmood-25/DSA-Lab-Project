CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
SRC = $(wildcard src/*.cpp)
all: plagcheck
plagcheck: $(SRC)
	$(CXX) $(CXXFLAGS) -o plagcheck $(SRC)
clean:
	rm -f plagcheck plagcheck.exe
