/**
 * @file player.h
 * @author Pedro Costa, João Fidalgo
 * @date 20 Dec 2017
 * @brief File containing all the functions and informations regarding the player during the game.
 *
 */

#ifndef __PLAYER_H
#define __PLAYER_H

#include "keyboard.h"
#include <stdint.h>

/** @defgroup player player
 * @{
 *
 * Module that handles everything related to the players during the game state.
 */

/**
 * MACROS FOR PLAYERS
 */

#define PLAYER_ACCELERATION 3.5
#define PLAYER_SIZE 20
#define PLAYER_MAX_VELOCITY 9

/** @name Player Structure */
/**@{
 *
 * @brief Player Structure containing all of its attributes and
 * informations in order to interact with other and the ball
 *
 */
typedef struct {

	int starting_x; /**< Saving hte starting x-position here. */
	int starting_y;/**< Saving the starting y-position here.*/

	int x; /**< The current x-position. */
	int y; /**< The current y-position. */

	double vx; /**< The player velocity in the x-axis*/
	double vy;/**< The player velocity in the y-axis*/

	double x_acceleration; /**< The player acceleration in the x-axis*/
	double y_acceleration;/**< The player acceleration in the y-axis*/

	int size; /**< Player's size, also the radius, given that visually the player is a circle */
	int action; /**< Player's action, will be set to 1 if he's pressing his action key, 0 if not. */

	int goals; /**< The amount of player's goal during each game*/

	/**
	 * User interaction with player
	 * @see keyboard.h
	 */

	KEY UPKey;/**< Player's up key movement */
	KEY LEFTKey; /**< Player's down key movement*/
	KEY RIGHTKey;/**< Player's right key movement*/
	KEY DOWNKey;/**< Player's down key movement*/
	KEY ACTIONKey;/**< Player's action key*/

	uint16_t color;/**< Player's color*/

} Player_t;

/** @} end of Player Structure */

/** @name Movement Structure */
/** @{
 *
 * @brief Contains information regarding the user interface and the player movement
 *
 * Given that the user can press and release several keys between each frame, if we kept updating
 * the player with every single action, when the time to draw the player in the screen, it could "skip" some
 * spaces. Thus, to avoid this, when the user presses the player's movement keys, we will update a structure
 * that holds information regarding which key was pressed / released, and when a new frame appears, we will update
 * with that information.
 *
 */

typedef struct {

	int key_up_pressed;/**< Set to 1 if the up key was pressed, 0 otherwise*/
	int key_down_pressed;/**< Set to 1 if the down key was pressed, 0 otherwise*/
	int key_left_pressed;/**< Set to 1 if the left key was pressed, 0 otherwise*/
	int key_right_pressed;/**< Set to 1 if the right key was pressed, 0 otherwise*/
	Player_t * player; /**< The player associated with this movement */

} MOVEMENT_EVENT;

/** @} end of Movement Structure */

/* PLAYER FUNCTIONS */

/**
 * @brief Creates a player
 *
 * @param KEY up - the up key associated with the player
 * @param KEY down - the down key associated with the player
 * @param KEY left - the left key associated with the player
 * @param KEY right - the right key associated with the player
 * @param color - the player's color
 * @param starting_x - player's x starting position
 * @param starting_y - player's y starting position
 *
 * @return pointer to Player upon creation, NULL otherwise
 *
 */
Player_t * createPlayer(KEY up, KEY down, KEY left, KEY right, KEY action,
		uint16_t color, int starting_x, int starting_y);

/*
 * @brief Frees the memory occupied by the player, deleting him
 *
 * @param * player - the player to be eliminated
 */
void deletePlayer(Player_t * player);

/*
 * @brief Draws the player on the double buffer to the displayed
 *
 * @see vg_draw_circle_with_limit
 *
 * @param * player - the player to be drawn
 *
 */
void drawPlayer(Player_t * player);

/*
 * @brief Updates all the information regarding the player movement, before being drawn
 *
 * Takes in consideration the player's current acceleration in each axis,
 * adds it to the velocity and then to the position. Checks for eventual collision with the walls
 * of the field. In the end applies a drag coefficient to slow it down.
 *
 * @param - * player - the player to be updated
 *
 */
void updatePositionAndSpeed(Player_t * player);

/*
 * @brief Checks if a certain key belongs to the player set of keys
 *
 * @param * key - the key to be tested
 * @param * player - the player
 *
 *
 * @return 1 if it belongs, 0 otherwise
 */

int belongsToPlayer(Player_t * player, kbd_key * key);

/*
 * @brief Checks if two players are colliding
 *
 * @param * player_one - one of the players
 * @param * player_two - the other player
 *
 * @return 1 upon collision, 0 otherwise
 */

int checkColision(Player_t * player_one, Player_t * player_two);

/**
 * @brief Upon collision, deals with the reaction
 * Involves quite a bit of physics and an abstraction of a two dimensional
 * vector was created to make it easier. It will calculate the direction and intensity
 * of both player's velocity after the collision.
 *
 * @see vector.h
 *
 * @param * player_one - one of the players
 * @param * player_two - the other player
 *
 */
void dealWithColision(Player_t * player_one, Player_t * player_two);

/**
 * @brief Checks if the player is off the field limits and resets its position
 *
 * By resetting, it changes the y or x the the next "legal" position
 *
 * @param * player - the player to be checked
 */
void checkPlayerBoundaries(Player_t * player);

/**
 * @brief Updates the player action member accordingly
 *
 * @param * player - the player
 * @param * key - the action key, containing info, if the key was pressed or released
 */
void updatePlayerAction(Player_t * player, kbd_key * key);

/**
 * @brief Resets player position to the starting position.
 * Occurs when someone scores a goal or when the game ends.
 * It also resets the velocity.
 *
 * @param  * player - the player to be reseted
 */
void resetPlayerPosition(Player_t * player);

/**
 * @brief Draws Player score on the screen
 *
 * @param * player - the player
 * @param position_x - x position to draw
 * @param position_y - y position to draw
 */

void drawPlayerScore(Player_t * player, int position_x, int position_y);

/**
 * @brief Adds a goal to the player
 *
 * @param * player - player
 */
void addGoal(Player_t * player);

/**
 * @brief Resets player's goal, when the game ends.
 *
 * @param * player - player
 */

void resetGoals(Player_t * player);

/**
 * @brief Draws the game current score
 *
 * @see drawPlayerScore
 *
 * @param * player_one - the first player
 * @param * player_two - the second player
 */

void drawScores(Player_t * player_one, Player_t * player_two);

/**
 * @brief Separates two players if they, some for reason, end up overlaping
 *
 * Sometimes, due to the slow velocity after the collisions, the players end up overlaping.
 * This functions aims to prevent that by resetting their positions, to the minimum distance apart
 * to not cause overlaping.
 *
 * @param * player_one - one of the players
 * @param * player_two - the other player
 */

void fixPlayersOverlaping(Player_t * player_one, Player_t * player_two);

/* MOVEMENT FUNCTIONS */

/**
 * @brief Updates Player movement information with it's associated movement structure
 *
 * Takes in consideration some keys combination, e.g., if both the up key and the down key
 * are pressed, the player should not move in the y axis. In this case, the function will
 * set both the acceleration and the velocity to zero. Otherwise, when such combinations do not happen
 * it will only change the acceleration, which will be taken in consideration in the function
 * updatePositionAndSpeed.
 *
 * @see updatePositionAndSpeed
 *
 * @param * player - the player to be updated
 * @param * mov - the player's movement structure
 *
 */
void updatePlayerWithMovement(Player_t * player, MOVEMENT_EVENT * mov);

/**
 * @brief Creates the Movement Structure
 *
 * @param * player - the player to wich the movement structure will be associated
 *
 * @return Pointer to structure upon success, NULL otherwise
 *
 */
MOVEMENT_EVENT * createMovementEvent(Player_t * player);

/**
 * @brief Updates the movement structure when a given key is received
 *
 * @param * key - the received key
 * @param * mov - the structure to be updated
 *
 */

void updateMovement(MOVEMENT_EVENT * mov, kbd_key * key);

/**
 * @brief Resets the movement structure
 *
 * It will be used when a goal is scored or when the game ends
 *
 * @param * mov - the structure to be reset
 */
void resetMovement(MOVEMENT_EVENT * mov);

/**
 * @brief Frees the memory occupied by the movement structure, deleting it
 *
 * @param * mov - the movement structure
 */

void deleteMovement(MOVEMENT_EVENT * mov);

/** @} end of player */

#endif
