/**
 * @file rectangle.h
 * @author Pedro Costa, João Fidalgo
 * @date 29 Dec 2017
 * @brief File containing all the functions and informations regarding the draw of rectangles.
 *
 */

#ifndef __RECTANGLE_H
#define __RECTANGLE_H
/** @defgroup rectangle rectangle
 * @{
 *
 * Module that handles everything related to the creation of rectangles.
 */



/** @name Rectangle Structure */
/**@{
 *
 *@brief Rectangle Structure containing all of its attributes.
 *
 */
typedef struct{

	int xi;/**< Position in x axis of top left corner. */
	int xf;/**< Position in x axis of bottom right corner. */
	int yi;/**< Position in y axis of top left corner. */
	int yf;/**< Position in y axis of bottom right corner. */
	int x_size;/**< Size in x axis of the rectangle. */
	int y_size;/**< Size in y axis of the rectangle. */
	unsigned long color;/**< Color of the rectangle. */

} Rectangle;
/** @} end of Rectangle Structure*/


/**
 * @brief Creates the rectangle.
 *
 * Allocates space in memory to create it.
 *
 * @param xi - position in x axis of top left corner.
 * @param yi - position in y axis of top left corner.
 * @param x_size - size in x axis of the rectangle to create.
 * @param y_size - size in y axis of the rectangle to create.
 * @param color - color of the rectangle to create.
 *
 * @return - Return pointer to Rectangle , NULL in case of error while creating
 */
Rectangle * createRectangle(int xi, int yi, int x_size, int y_size, unsigned long color);
/**
 * @brief Draws the rectangle on the screen.
 *
 * @param rect - pointer to the Rectangle to draw.
 *
 */
void drawRectangle(Rectangle * rect);
/**
 * @brief Deletes the rectangle of memory.
 *
 * @param rect - pointer to the Rectangle to delete.
 *
 */
void deleteRectangle(Rectangle * rect);

/** @} end of rectangle */

#endif
