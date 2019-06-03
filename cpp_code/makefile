CXX= g++
CFLAGS=  -g -O2	

all: chain.o linkage.o
	python setup.py build_ext --inplace

chain.o: chain.cpp chain.h
	$(CXX) $(CFLAGS) -c chain.cpp

linkage.o: linkage.cpp linkage.h
	$(CXX) $(CFLAGS) -c linkage.cpp

clean: # -f so this will succeed even if the files don't exist
	rm -f protoclust chain.o linkage.o 
	rm -f pylinkage.cpp pychain.cpp
	rm -f pychain.cpython-36m-x86_64-linux-gnu.so pylinkage.cpython-36m-x86_64-linux-gnu.so
