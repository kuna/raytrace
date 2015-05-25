#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include "pnglib.h"
using namespace std;

int main () {
  image test1, test2;
  int pixel;
  const int testX = 100, testY = 0;

	// test writing image
  test1.create(64, 64);
  test1.setPixel(testX, testY, RGB(255, 200, 0));
	if (!test1.write("test.png"))
    cout << "failed to write img" << endl;
  pixel = test1.getPixel(testX, testY);
  cout << "pixel: " << getR(pixel) << ", " << getG(pixel) << ", " << getB(pixel) << endl;

	// test reading image
	if (!test2.open("test.png"))
    cout << "failed to read img" << endl;
  pixel = test2.getPixel(testX, testY);
	cout << "image info: (" << test2.getWidth() << ", " <<  test2.getHeight() << ") "<<endl;
  cout << "pixel: " << getR(pixel) << ", " << getG(pixel) << ", " << getB(pixel) << endl;

  image kaede;
  if (!kaede.open("kaede.png"))
    cout << "failed to read img 2" << endl;
	cout << "image info: (" << kaede.getWidth() << ", " << kaede.getHeight() << ") "<<endl;

	return 0;
}
