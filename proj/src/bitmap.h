/**
 * @file bitmap.h
 * @author Henrique Ferrolho (henriqueferrolho@gmail.com)
 *
 *
 * All the following code was developed and provided by Henrique Ferrolho, we do not take any credit.
 * Only made some alterations to match what we wanted for our project.
 * The full source code can be seen here: http://difusal.blogspot.pt/
 *@brief Functions for bitmap manipulation
 *
 */

#ifndef __BITMAP_H
#define __BITMAP_H

/** @defgroup bitmap bitmap
 * @{
 * Functions for manipulating bitmaps
 */

#include <stdint.h>

typedef struct {
	unsigned short type; // specifies the file type
	unsigned int size; // specifies the size in bytes of the bitmap file
	unsigned int reserved; // reserved; must be 0
	unsigned int offset; // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BitmapFileHeader;

typedef struct {
	unsigned int size; // specifies the number of bytes required by the struct
	int width; // specifies width in pixels
	int height; // specifies height in pixels
	unsigned short planes; // specifies the number of color planes, must be 1
	unsigned short bits; // specifies the number of bit per pixel
	unsigned int compression; // specifies the type of compression
	unsigned int imageSize; // size of image in bytes
	int xResolution; // number of pixels per meter in x axis
	int yResolution; // number of pixels per meter in y axis
	unsigned int nColors; // number of colors used by the bitmap
	unsigned int importantColors; // number of colors that are important
} BitmapInfoHeader;

/// Represents a Bitmap
typedef struct {
	BitmapInfoHeader bitmapInfoHeader;
	char* bitmapData;
} Bitmap;

/**
 * @brief Loads a bmp image
 *
 * @param filename Path of the image to load
 * @return Non NULL pointer to the image buffer
 */
Bitmap* loadBitmap(const char* filename);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position
 *
 * @see video_gr.h
 * @see vg_draw_background()
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 */
void drawBackGroundBitmap(Bitmap* bitmap, int x, int y);

/*
 * @brief Draws a shaped bitmap at the given position.
 * The param ignored_color represent the color that won't be drawn in the screen.
 * A good analogy could be the green screen on movies.
 *
 * @param bmp bitman to be drawn
 * @param x the x coordinate
 * @param y the y coordinate
 * @param alignment image alignment
 * @param ignored_color the color that won't be drawn
 *
 */
void drawBitmapShape(Bitmap* bmp, int x, int y, uint16_t ignored_color);

/**
 * @brief Destroys the given bitmap, freeing all resources used by it.
 *
 * @param bitmap bitmap to be destroyed
 */
void deleteBitmap(Bitmap* bmp);

/**@}*/
#endif
