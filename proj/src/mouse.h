/**
 * @file mouse.h
 * @author Pedro Costa, João Fidalgo
 * @date 27 Dec 2017
 * @brief File containing all the functions for manipulating and using the i8042 mouse.
 *
 */


#ifndef __MOUSE_H
#define __MOUSE_H

#include "bitmap.h"
#include "rectangle.h"
#include <stdint.h>


/** @defgroup mouse mouse
 * @{
 *
 * Module that handles everything related to the i8042 mouse, allowing the user to interact with the game and menus.
 */

/**
 * MACROS FOR THE MOUSE
 */
#define MOUSE_STARTING_X 512
#define MOUSE_STARTING_Y 420
#define MOUSE_SENSIVITY 1.8



/** @name Mouse Structure */
/**@{
 *
 *@brief Mouse Structure containing all of its attributes
 * Since there will be only one mouse, we took a singleton approach.
 * The only mouse variable is a static variable under mouse.c only acessible by the funcion getMouse().
 *
 */

typedef struct {

	//packet
	unsigned long packet[3]; /**< The mouse full packet containing all the information */

	//current position
	int x; /**< Current x position */
	int y; /**< Current y position*/

	//displacement
	int deltaX; /**< The displacement given in the x axis by the packet */
	int deltaY; /**< The displacement given in the y axis by the packet */

	//buttons
	char RButton; /**< Indicates if the right button was pressed, 1 if so, 0 otherwise*/
	char LButton;/**< Indicates if the left button was pressed, 1 if so, 0 otherwise*/
	char MButton;/**< Indicates if the middle button was pressed, 1 if so, 0 otherwise*/

	//interrupts
	int beingRead; /**< 1 if it is still gathering the 3 bytes, 0 if it is complete */

	//draw part
	Bitmap * cursor; /**< Pointer to the mouse's cursor bmp, used to be drawn */

	//color
	uint16_t ignored_color; /**< Mouse's Color to be ignored, not drawn */

} Mouse;

/** @} end of Mouse Structure */

/**
 * @brief Returns a pointer to the mouse. If it wasn't previously created, it also creates it.
 *
 * @return Pointer to the mouse, NULL in case of error while creating
 *
 */
Mouse * getMouse();

/**
 * @brief Updates all the information regarding the mouse.
 *
 * Updates the position, accordingly to the displacement, and the pressing of the buttons.
 * It also checks if it is offscreen, correcting it.
 */
void updateMouse();

/**
 * @brief Deletes the mouse, freeing the memory allocated for it.
 */
void deleteMouse();

/**
 * @brief Draws the mouse on the screen.
 *
 * @see bitmap.h
 */
void drawMouse();

/**
 * @brief Checks if the mouse is inside any rectangle.
 *
 * Used in the menus, to highlight some choices.
 *
 * @return 1 is inside, 0 otherwise.
 *
 */
int insideRect(Rectangle * rect);

/**
 * @brief Subscribes and enables mouse interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int mouse_subscribe();

/**
 * @brief Unsubscribes mouse  interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe();


/**
 * @brief Writes a byte to Mouse
 *
 * Also reads and deals with the response
 *
 * @param cmd - the command argument passed to command 0xD4
 *
 * @return - Return 0 upon sucess and non-zero otherwise
 */
int writeCommandMouse(unsigned long cmd);


/**
 *@brief Configures the mouse in the beginning of the same to the desired perfomance
 *
 *Sets stream Mode and enables data report
 *
 *@return - Return 0 upon sucess and non-zero otherwise
 */
int configureGameMouse();



/**
 * @brief Mouse Interrupt Handler
 *
 * Receives the bytes given by the KBC and stores them.
 * When the packet is full and ready, it updates the mouse.
 *
 */
void mouse_int_handler();


/*
 * @brief Gives the value of the x-axis displacement
 *
 * Can be either positive or negative
 *
 * @param packet - the mouse packet
 */
int get_X_Delta(Mouse * mouse);

/**
 * @brief Gives the value of the y-axis displacement
 *
 * Can be either positive or negative
 *
 * @param packet - the mouse packet
 */
int get_Y_Delta(Mouse * mouse);

/** @} end of mouse */

#endif
