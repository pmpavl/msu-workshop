lib = -std=c++11 -larmadillo
path = ../bin/main

all: clean
	g++ main.cpp -o ${path} ${lib}

run:
	${path} 100000

clean:
	rm -f ${path}
