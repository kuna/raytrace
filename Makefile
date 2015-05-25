CC = /usr/local/cuda-6.5/bin/nvcc 
CFLAGS = -lpng
MAIN = cudaTest

$(MAIN): cudaTest.cu pnglib.o
	$(CC) $(CFLAGS) $(MAIN).cu pnglib.o -o $(MAIN)

pnglibTest: pnglibTest.cpp pnglib.o
	g++ pnglibTest.cpp pnglib.o -lpng -o pnglibTest

pnglib.o: pnglib.cpp
	g++ -c pnglib.cpp -lpng

clean:
	rm -rf *.o
	rm -f cudaTest
