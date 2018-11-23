#include "bitmap.h"
#include "stdio.h"
#include "stdlib.h"
#include "video_gr.h"
#include "string.h"
#include <stdint.h>

Bitmap* loadBitmap(const char* filename) {
	// allocating necessary size
	Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));

	// open filename in read binary mode
	FILE *filePtr;
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	BitmapFileHeader bitmapFileHeader;
	fread(&bitmapFileHeader, 2, 1, filePtr);

	// verify that this is a bmp file by check bitmap id
	if (bitmapFileHeader.type != 0x4D42) {
		fclose(filePtr);
		return NULL;
	}

	int rd;
	do {
		if ((rd = fread(&bitmapFileHeader.size, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.reserved, 4, 1, filePtr)) != 1)
			break;
		if ((rd = fread(&bitmapFileHeader.offset, 4, 1, filePtr)) != 1)
			break;
	} while (0);

	if (rd == !1) {
		fprintf(stderr, "Error reading file\n");
		exit(-1);
	}

	// read the bitmap info header
	BitmapInfoHeader bitmapInfoHeader;
	fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

	// move file pointer to the begining of bitmap data
	fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

	// allocate enough memory for the bitmap image data
	bmp->bitmapData = (char*) malloc(bitmapInfoHeader.imageSize);

	// verify memory allocation
	if (!bmp->bitmapData) {
		free(bmp->bitmapData);
		free(bmp);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(bmp->bitmapData, bitmapInfoHeader.imageSize, 1, filePtr);

	// make sure bitmap image data was read
	if (bmp->bitmapData == NULL) {
		free(bmp->bitmapData);
		free(bmp);
		fclose(filePtr);
		return NULL;
	}

	// close file and return bitmap image data
	fclose(filePtr);

	bmp->bitmapInfoHeader = bitmapInfoHeader;

	return bmp;

}

void drawBackGroundBitmap(Bitmap* bmp, int x, int y) {
	if (bmp == NULL)
		return;

	int width = bmp->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = bmp->bitmapInfoHeader.height;

	if (x + width < 0 || x > getHRes() || y + height < 0 || y > getVRes())
		return;

	uint16_t* bufferStartPos;
	uint16_t* imgStartPos;

	int i;

	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		if (pos < 0 || pos >= getVRes())
			continue;

		bufferStartPos = getBackgroundImage();
		bufferStartPos += x + pos * getHRes();

		imgStartPos = (uint16_t *) bmp->bitmapData + i * width;

		memcpy(bufferStartPos, imgStartPos, drawWidth * 2);
	}
}

void drawBitmapShape(Bitmap* bmp, int x, int y, uint16_t ignored_color) {
	if (bmp == NULL)
		return;

	int width = bmp->bitmapInfoHeader.width;
	int drawWidth = width;
	int height = bmp->bitmapInfoHeader.height;

	uint16_t * bufferStartPos;
	uint16_t * imgStartPos;

	int i;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		bufferStartPos = getDBuf();
		bufferStartPos += x + pos * getHRes();

		imgStartPos = (uint16_t *) bmp->bitmapData + i * width;

		int counter;

		for (counter = 0; counter < drawWidth; counter++) { //iterating over a line

			if (*imgStartPos != ignored_color)  //ignoring the color
				(*bufferStartPos) = (*imgStartPos);

			++bufferStartPos;
			++imgStartPos;
		}
	}
}

void deleteBitmap(Bitmap* bmp) {
	if (bmp == NULL)
		return;

	free(bmp->bitmapData);
	free(bmp);
}
