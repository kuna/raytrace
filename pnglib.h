typedef unsigned short int uint8_t;
#define getR(v) ((v) & 0xFF)
#define getG(v) (((v) >> 8) & 0xFF)
#define getB(v) (((v) >> 16) & 0xFF)
#define getA(v) (((v) >> 24) & 0xFF)
#define RGB(r,g,b) ((r) | ((g) << 8) | ((b) << 16) | (0xFF << 24))
inline void setRGB(int *ptr, uint8_t R, uint8_t G, uint8_t B)
{
	*ptr = RGB(R, G, B);
}

int writeImage(char *filename, int width, int height, int *buffer, char *title);
int readImage(char* filename, int *width, int *height, int **buffer);

class image {
	public:
		image();
		~image();
		bool open(char *filename);
		bool write(char *filename);
		void create(int width, int height);
		void release();
		bool isopened();
		int getPixel(int x, int y);
		void setPixel(int x, int y, int v);
		int getWidth();
		int getHeight();

	private:
		int width, height;
		int *data;
};
