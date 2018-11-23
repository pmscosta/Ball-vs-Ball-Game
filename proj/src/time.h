/**
 * @file time.h
 * @author Pedro Costa, João Fidalgo
 * @date 29 Dec 2017
 * @brief File containing all the functions and informations regarding the time counter in game and the score.
 *
 */



#ifndef __TIME_H
#define __TIME_H



#include "bitmap.h"

/** @defgroup time time
 * @{
 *
 * Module that handles everything related to the time and the score.
 */


/**
 * MACROS FOR TIME
 */
#define STARTING_TIME 300


/** @name Time Structure */
/**@{
 *
 *@brief Time Structure containing all of its attributes and bitmaps
 *
 */
typedef struct {

	//time in seconds
	int t;/**< Time remaining in the counter. */
	//digits from time in (minutes:sec)
	int min;/**< Digit of minutes. */
	int dezsec;/**< Digit dozens of seconds. */
	int unisec;/**< Digit unity of seconds. */
	//position of the digits
	int xmin;/**< Position in x axis of min. */
	int ymin;/**< Position in y axis of min. */
	int xTwPoints;/**< Position in x axis of twopoints. */
	int yTwPoints;/**< Position in y axis of twopoints. */
	int xDsec;/**< Position in x axis of dezsec. */
	int yDsec;/**< Position in y axis of dezsec. */
	int xUsec;/**< Position in x axis of unisec. */
	int yUsec;/**< Position in y axis of unisec. */


	//draw part - time
	Bitmap * twopoints;/**< Pointer to the bmp two points. */
	Bitmap * numbers[10];/**< Array containing pointer's to the bmp's of all the number's to time. */


	//draw part - score
	Bitmap * goals_score[4];/**< Array containing pointer's to the bmp's of all the number's to use on the score. */
	Bitmap * score_divider;/**< Pointer to the bmp two points to use on score. */



	//color
	uint16_t ignored_color;/**< Time's Color to be ignored, not drawn */

} Time;

/** @} end of Time Structure*/
/**
 * @brief Starts the time structure and loads all bitmaps.
 *
 * Allocates space in memory to create the structure.
 *
 * @return A pointer to the time,NULL in case of error while creating.
 *
 */
Time * createTime();
/**
 * @brief Updates the content of time.
 *
 * Subtract one to the time and updates all of the digits.
 *
 */
void updateTime();
/**
 * @brief Deletes time and all the bitmaps of it.
 *
 * Free the memory that was allocated before.
 *
 */
void deleteTime();
/**
 * @brief Draw's all the digits of the time.
 *
 *
 */
void drawTime();
/**
 * @brief Returns a pointer to the time. If it wasn't previously created, it also creates it.
 *
 * @return Pointer to the time, NULL in case of error while creating
 *
 */
Time * getTime();
/**
 * @brief Resets time to starting point.
 *
 *
 */
void resetTime();


/** @} end of time */

#endif
