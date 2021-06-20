# List all the drivers
all: mstdriver

mstdriver: msttest.cpp
	g++ -std=c++11 -o  mstdriver msttest.cpp

# List all the executables under 'all:'
clean:
	rm mstdriver