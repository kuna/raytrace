CC = /usr/local/cuda-6.5/bin/nvcc 
CFLAGS = -lpng
MAIN = cudaTest

$(MAIN): cudaTest.cu pnglib.o
	$(CC) $(CFLAGS) $(MAIN).cu pnglib.o -o $(MAIN)

pnglib.o: pnglib.cpp
	g++ $(CFLAGS) -c pnglib.cpp

clean:
	rm -rf *.o
	rm -f cudaTest
