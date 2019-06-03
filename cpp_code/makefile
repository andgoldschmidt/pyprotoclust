CFLAGS=  -g -O2	

protoclust: chain.o main.o linkage.o
	g++ $(CFLAGS) -o protoclust chain.o main.o

main.o: main.cpp
	g++ $(CFLAGS) -c main.cpp

chain.o: chain.cpp chain.h
	g++ $(CFLAGS) -c chain.cpp

linkage.o: linkage.cpp linkage.h
	g++ $(CFLAGS) -c linkage.cpp

all: protoclust
	python setup.py build_ext --inplace

clean:
	# -f so this will succeed even if the files don't exist
	rm -f protoclust main.o chain.o linkage.o 
	# python files
	rm -f pylinkage.cpp pychain.cpp
	rm -f pychain.cpython-36m-x86_64-linux-gnu.so pylinkage.cpython-36m-x86_64-linux-gnu.so
