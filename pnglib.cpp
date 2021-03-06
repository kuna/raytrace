#include <stdlib.h>
#include <png.h>
#include <assert.h>
#include "pnglib.h"

//
// int writeImage
//
// writes PNG image
//
// refered: http://www.labbookpages.co.uk/software/imgProc/libPNG.html
//
int writeImage(char *filename, int width, int height, int *buffer, char *title) {
	FILE *fp=0;
	png_structp png_ptr=0;
	png_infop info_ptr=0;
	png_bytep row=0;
	int code = 1;

	fp = fopen(filename, "wb");
	if (!fp) {
		// error occured
		code = 0;
		goto error_;
	}

	// initalize structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		printf("creating writing structure failed\n");
		code = 0;
		goto error_;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		printf("creating info struct failed\n");
		code = 0;
		goto error_;
	}
	if (setjmp(png_jmpbuf(png_ptr))) {
		printf("setjmp failed\n");
		code = 0;
		goto error_;
	}

	// start writing process (init)
	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, width, height,
		8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	if (title != NULL) {
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = title;
		png_set_text(png_ptr, info_ptr, &title_text, 1);
	}
	png_write_info(png_ptr, info_ptr);
	row = (png_bytep) malloc(3 * width * sizeof(png_byte));

	// write image data
	int x, y;
	for (y=0 ; y<height ; y++) {
		for (x=0 ; x<width ; x++) {
			*(int*)(&row[x*3]) = buffer[y*width + x];
		}
		png_write_row(png_ptr, row);
	}
	png_write_end(png_ptr, NULL);


	error_:
	// clear process
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);
	return code;
}

//
// int readImage
//
// reads PNG image
//
// referred: http://zarb.org/~gc/html/libpng.html
//
int readImage(char* filename, int *width, int *height, int **buffer) {
	FILE *fp=0;
	png_structp png_ptr=0;
	png_infop info_ptr=0;
	png_bytep *row=0;
	int code = 1;
	int x, y;

	fp = fopen(filename, "rb");
	if (!fp) {
		// error occured
		printf("%s - file not found\n", filename);
		code = 0;
		goto error_2;
	}

	unsigned char sig[8];
	fread(sig, 1, 8, fp);
	if (png_sig_cmp(sig, 0, 8)) {
		printf("not a png file \n");
		code = 0;
		goto error_2;
	}

	// initalize structure
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		printf("creating reading struct failed\n");
		code = 0;
		goto error_2;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		printf("creating info struct failed\n");
		png_destroy_read_struct(&png_ptr, 0, 0);
		code = 0;
		goto error_2;
	}
	if (setjmp(png_jmpbuf(png_ptr))) {
		printf("somethin may be failed\n");
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		code = 0;
		goto error_2;
	}

	// starting reading process
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	*width = png_get_image_width(png_ptr, info_ptr);
	*height = png_get_image_height(png_ptr, info_ptr);
	//color_type = png_get_color_type(png_ptr, info_ptr);
	//bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	//number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);

	// read file pixel
	if (setjmp(png_jmpbuf(png_ptr))) {
		code = 0;
		goto error_2;
	}
	row = (png_bytep*) malloc(sizeof(png_bytep) * (*height));
	for (y=0; y<*height; y++)
		row[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
	png_read_image(png_ptr, row);

	// allocate buffer
	*buffer = (int*)malloc((*width)*(*height)*sizeof(int));

	// and copy pixel
	for (y=0; y<*height; y++)
		for (x=0; x<*width; x++)
			((int*)(*buffer))[y*(*width) + x] = RGB(row[y][x*3], row[y][x*3+1], row[y][x*3+2]);

	// clear process
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	error_2:
	if (fp != NULL) fclose(fp);
	if (row != NULL) for (int i=0; i<(*height); i++) free(row[i]);
	if (row != NULL) free(row);
	return code;
}


/*
 *
 * implementation of image class starts from here
 *
 */

image::image() {
	data = 0;
}

image::~image() {
	// automatically release image data
	release();
}

bool image::open(char *filename) {
	return readImage(filename, &width, &height, &data);
}

bool image::write(char *filename) {
	return writeImage(filename, width, height, data, "Graphics HW");
}

void image::create(int width, int height) {
	if (isopened()) release();
	this->width = width;
	this->height = height;
	data = (int*)malloc(sizeof(int)*width*height);
	memset(data, 0, sizeof(int)*width*height);
}

void image::release() {
	if (data) {
		free(data);
		data = 0;
	}
}

bool image::isopened() {
	return (data != 0);
}

int image::getPixel(int x, int y) {
	assert(isopened());
	return data[width*y+x];
}

void image::setPixel(int x, int y, int v) {
	assert(isopened());
	data[width*y+x] = v;
}

int image::getWidth() {
	return width;
}

int image::getHeight() {
	return height;
}
