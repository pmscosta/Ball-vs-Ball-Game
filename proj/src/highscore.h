/**
 * @file highscore.h
 * @author Pedro Costa, João Fidalgo
 * @date 29 Dec 2017
 * @brief File containing all the functions and informations regarding the highscores and the file reading.
 *
 */

#ifndef __HIGHSCORE_H
#define __HIGHSCORE_H



#define TOP1_Y 270
#define TOP2_Y 320
#define TOP3_Y 370

#include "bitmap.h"
/** @defgroup highscore highscore
 * @{
 *
 * Module that handles everything related to the highscores.
 */

/** @name Highscore Structure */
/**@{
 *
 *@brief Highscore Structure containing all of its attributes
 *
 */
typedef struct {

	int tempo;/**< Time that the game took. */

	int hora[3];/**< Array that contains the hours ,minutes and seconds of the day the highscore was made. */

	int dia[3];/**< Array that contains the day, month and year of the day the highscore was made. */

} Top;

/** @} end of Highscore Structure*/

/**
 * @brief Read from file the highscores
 *
 * Only allocates memory if there's is a highscore
 *
 */
void startHighScores(FILE * high);
/**
 * @brief Draw the highscores of the structures  in the screen
 *
 */
void drawHighscores();
/**
 * @brief Loads all the bitmaps necessary for highscores
 *
 * 	Loads the numbers , twopoints and bar
 */
void createNumbers();
/**
 * @brief Calls the function to load the bitmaps
 *
 *	If one of the elements that needs to be loaded all of
 *	them are loaded already so no need to call
 */
void startNumbers();
/**
 * @brief Tests if the new time that was done is better than the highscores
 *
 *	If there is no highscore the game goes directly to the highscores
 */
void testeNewHighscore();
/**
 * @brief Equals t1 to t2 the right way
 *
 */
void helpfull(Top * t1, Top * t2);
/**
 * @brief Write on the file the highscores
 *
 */
void saveHighScores();
/**
 * @brief Draw the highscores of the top3 in the screen
 *
 */
void drawTop3();
/**
 * @brief Draw the highscores of the top2 in the screen
 *
 */
void drawTop2();
/**
 * @brief Draw the highscores of the top1 in the screen
 *
 */
void drawTop1();

/**
 * @brief Frees all the memory occupied by the highscore data.
 */

void deleteTops();


/** @} end of highscore */

#endif
