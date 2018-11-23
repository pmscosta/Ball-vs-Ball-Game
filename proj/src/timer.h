/**
 * @file timer.h
 * @author Pedro Costa, João Fidalgo
 * @date 27 Dec 2017
 * @brief File containing all the functions and informations regarding the use of the i8254 timers.
 *
 */

#ifndef __TIMER_H
#define __TIMER_H

/** @defgroup timer timer
 * @{
 *
 * Functions for manipulating and using the i8254 timers
 */

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int timer_subscribe();

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timer_unsubscribe();

/**
 * @brief Timer 0 interrupt handler
 *
 * Increments the counter. A static variable encapsulated in timer.c.
 * To accessing it during the game, when we need to update the screen,
 * two functions were created to access the counter: game_tick and seconds_tick.
 *
 * @see game_tick()
 * @see seconds_tick()
 */
void timer_int_handler();


/**
 * @brief Checks if a new game frame, given the game FPS, has occurred
 *
 * @return 1 if it is a new frame, 0 otherwise
 */
int game_tick();

/**
 * @brief Checks if a second has passed by, in the default timer frequency
 *
 * @return 1 if a second has passed, 0 otherwise
 */
int seconds_tick();

/**@} end of timer */


#endif
