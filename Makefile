all:smoothEx.cpp
	g++ -o smoothEx -std=c++11 -lOpenMeshCore -lOpenMeshTools -Wall -Wextra smoothEx.cpp
clean:
	rm smoothEx