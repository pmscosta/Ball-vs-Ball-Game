/**
 * @file video_gr.h
 * @author Professor Pedro Souto (pfs@fe.up.pt), Pedro Costa and João Fidalgo
 * @brief File containing all the functions and informations regarding the output of data to the screen.
 *
 */

#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "stdint.h"
#include "player.h"
#include "bitmap.h"
#include "ball.h"

/**
 * MACROS RELEVANT FOR OUR PROJECT
 */
#define COLOR_RED rgb(255,0,0)
#define SOFT_RED rgb(253, 42, 42)
#define LIGHT_GRAY rgb(128, 128, 128)
#define SOFT_BROW rgb(153,76,0)
#define COLOR_BLUE rgb(0,0,255)
#define SOFT_BLUE rgb(21,115, 255)
#define IGNORE_PURE_GREEN rgb(0,255,0)
#define COLOR_WHITE rgb(255,255,255)
#define COLOR_BLACK rgb(0,0,0)

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

/**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

/**
 * @brief Copies the content of the double buffer to the main buffer.
 *
 * Displays the graphics, using the double buffering technique.
 *
 */
void vg_swap_video();

/**
 * @brief Sets a specific pixel to a certain color.
 *
 * Interprets the memory allocated for the video ram as a matrix.
 *
 * @param x - the x position on the screen
 * @param y - the y position on the screen
 * @param color - the color to be set
 */
void vg_colorize_pixel(unsigned short x, unsigned short y, uint16_t color);

/**
 * @brief Draws the background image on the double buffer
 *
 * Every time a new background image is read from the bitmap data, it is read to a variable in video_gr.c and stored there.
 * Thus, when we need to draw the background image, all we need to do it copy the contents of that variable to the double buffer. This way
 * we only need to call the function that reads the bitmap data when the background changes, otherwise it will just be a memcopy.
 *
 *@see drawBackGroundBitmap @ bitmap.h
 *
 */
void vg_draw_background();

/**
 * @brief Clears the screen, setting every thing to black
 *
 */
void vg_clear_screen();

/**
 * @brief Draws a simple solid circle on the screen
 *
 * It prevents the circle from going over the screen limits.
 *
 * @param x - the x position for the circle center
 * @param y - the y position for the circle center
 * @param radius - the circle radius
 * @param color - the color radius
 *
 */
void vg_draw_circle(int x, int y, unsigned short radius, unsigned long color);

/**
 * @brief Draws a solid circle of a given color with a black line around it.
 *
 * It prevents the circle from going over the screen limits.
 *
 * @param x - the x position for the circle center
 * @param y - the y position for the circle center
 * @param radius - the circle radius
 * @param color - the color radius
 */

void vg_draw_circle_with_limit(int x0, int y0, unsigned short radius,
		unsigned long color);

/**
 * @brief Draws a vertical line on the screen
 *
 * @param x - the constant x position of the line
 * @param yi - the starting y position of the line
 * @param size - the length of the line
 * @param color - the color
 *
 */
void vg_draw_vert_line(unsigned short x, unsigned short yi, unsigned short size,
		unsigned long color);

/**
 * @brief Draws an horizontal line on the screen
 *
 * @param y - the constant y position of the line
 * @param xi - the starting x position of the line
 * @param size - the length of the line
 * @param color - the color
 *
 */
void vg_draw_hor_line(unsigned short xi, unsigned short y, unsigned short size,
		unsigned long color);

/**
 * @brief Draws a simple empty rectangle, only with its borders
 *
 * @see vg_draw_vert_line
 * @see vg_draw_hor_line
 *
 * @param x - the upper left corner x position
 * @þaram y - the upper left corner y position
 * @param x_size - the width of the rectangle
 * @param y_size - the height of the rectangle
 * @param color - the color
 *
 */
void vg_draw_empty_rect(unsigned short x, unsigned short y,
		unsigned short x_size, unsigned short y_size, unsigned long color);

/**
 * @brief Returns the Screen Horizontal Resolution
 *
 * We need this information outside this file in order to draw the Bitmaps
 *
 * @return The screen width
 */
unsigned getHRes();

/**
 * @brief Returns the Screen Vertical Resolution
 *
 * We need this information outside this file in order to draw the Bitmaps
 *
 * @return The screen height
 */
unsigned getVRes();

/**
 * @brief Returns a pointer to the initial position in memory of the background image storage.
 * To be used when drawing a new background bitmap
 *
 * @return First address of background image data
 */
uint16_t * getBackgroundImage();

/**
 * @brief Returns a pointer to the initial position of the Double Buffer
 *
 * Used when we need to draw the mouse.
 *
 * @return Address of Double Buffer
 */
uint16_t * getDBuf();

/**
 * Converts an rgb value to 16 bit value.
 *
 * return color value in 16 bits.
 */
uint16_t rgb(int r, int g, int b);

/**
 * @brief Checks if the mouse current position is within the limits of the screen.
 *
 * If the mouse if off the limits, it will reset its position to the nearest legal position.
 *
 * @param * x - a pointer to the mouse x position
 * @param * y - a pointer to the mouse y position
 * @param height - the height of the mouse image
 * @param width - the width of the mouse image
 */
void checkMouseBoundaries(int * x, int * y, int height, int width);

/** @} end of video_gr */

#endif /* __VIDEO_GR_H */
