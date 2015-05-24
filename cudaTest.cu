#include <iostream>
#include "pnglib.h"

__global__ void VecAdd(float* A, float* B, float* C) {
	int i = threadIdx.x;
	C[i] = A[i] + B[i];
}

//
// void setRGB
//
// sets RGB pixel data on png_byte
//
typedef unsigned short int uint8_t;
#define getR(v) ((v) & 0xFF)
#define getG(v) (((v) >> 2) & 0xFF)
#define getB(v) (((v) >> 4) & 0xFF)
#define getA(v) (((v) >> 6) & 0xFF)
inline void setRGB(int *ptr, uint8_t R, uint8_t G, uint8_t B)
{
	*ptr = R | (G << 2) | (B << 4);
}



int main () {
	float* A_t, *B_t, *C_t;
	cudaMalloc(&A_t, sizeof(float)*100);
	cudaMalloc(&B_t, sizeof(float)*100);
	cudaMalloc(&C_t, sizeof(float)*100);

	// test work
	VecAdd<<<1, 100 >>> (A_t, B_t, C_t);

	cudaFree(A_t);
	cudaFree(B_t);
	cudaFree(C_t);

	// test writing image
	int test[64*64];
	memset(test, 0xFF, sizeof(int)*64*64);
	setRGB(&test[0], 255, 0, 0);
	writeImage("test.png", 64, 64, test, "test");

	// test reading image
	int width=0, height=0;
	readImage("kaede.png", &width, &height, test);
	std::cout << "image info: " << width << ", " <<  height << ", "
		<< getR(test[0]) << ", " << getG(test[0]) << ", " << getB(test[0]);

	return 0;
}
