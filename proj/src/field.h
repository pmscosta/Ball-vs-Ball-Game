/**
 * @file field.h
 * @author Pedro Costa, João Fidalgo
 * @date 27 Dec 2017
 * @brief File containing all the useful Macros for managing all the objects inside the field
 *
 */

#ifndef __FIELD_H
#define __FIELD_H

/** @defgroup field field
 * @{
 *
 * Constants for managing all the objects inside the field.
 */

#define IMAGES_PATH "/home/lcom/proj/src/resources/images/"
#define FILE_PATH "/home/lcom/proj/src/resources/files/"

#define FIELD_X_INIT 59  /**< @brief x position of the field's upper left corner */
#define FIELD_X_END 963  /**< @brief x position of the field's down right corner */
#define FIELD_X_SIZE 904
#define FIELD_Y_INIT 194 /**< @brief y position of the field's upper left corner */
#define FIELD_Y_END 614  /**< @brief y position of the field's down right corner */
#define FIELD_Y_SIZE 420

#define LEFT_GOAL_X_INIT 29  /**< @brief x position of the left-side goal's front line */
#define LEFT_GOAL_X_END 62   /**< @brief x position of the left-side goal's back line line */
#define LEFT_GOAL_X_SIZE 33

#define RIGHT_GOAL_X_INIT 962 /**< @brief x position of the right-side goal's front line */
#define RIGHT_GOAL_X_END 995  /**< @brief x position of the right-side goal's back line */
#define RIGHT_GOAL_X_SIZE 33

#define GOAL_Y_INIT 332 /**< @brief y position of the goal's upper limit */
#define GOAL_Y_END 479  /**< @brief y position of the goal's lower limit */
#define GOAL_Y_SIZE 147

#define PLAYER_ONE_STARTING_X 129  /**<@brief x starting position for player one (left half of the field) */
#define PLAYER_ONE_STARTING_Y 404  /**<@brief y starting position for player one (left half of the field) */

#define PLAYER_TWO_STARTING_X 893  /**<@brief x starting position for player two (right half of the field) */
#define PLAYER_TWO_STARTING_Y 404  /**<@brief y starting position for player two (right half of the field) */

#define BALL_STARTING_X 512  /**<@brief Ball's x starting position (middle of the field) */
#define BALL_STARTING_Y 404  /**<@brief Ball's y starting position (middle of the field) */

#define FIRST_SCORE_X 402  /**<@brief Upper left corner x position to draw player one score */
#define FIRST_SCORE_Y 10   /**<@brief Upper left corner y position to draw player one score */
#define SECOND_SCORE_X 527 /**<@brief Upper left corner x position to draw player two score */
#define SECOND_SCORE_Y 10  /**<@brief Upper left corner y position to draw player two score */
#define SCORE_DIVIDER_X 462 /**<@brief Upper left corner x position to draw the two points that separate the score */
#define SCORE_DIVIDER_Y 14 /**<@brief Upper left corner y position to draw the two points that separate the score */

/**@}*/

#endif
