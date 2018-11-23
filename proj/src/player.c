#include "player.h"
#include "video_gr.h"
#include "field.h"
#include "vector.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "time.h"

/*
 * PLAYER FUNCTIONS
 *
 *
 */

Player_t * createPlayer(KEY up, KEY down, KEY left, KEY right, KEY action,
		uint16_t color, int starting_x, int starting_y) {

	Player_t * player = malloc(sizeof(Player_t));

	if (player == NULL)
		return NULL;

	player->starting_x = starting_x;
	player->starting_y = starting_y;

	player->x = player->starting_x;
	player->y = player->starting_y;

	player->vx = 0;
	player->vy = 0;

	player->x_acceleration = 0;
	player->y_acceleration = 0;

	player->action = 0;

	player->goals = 0;

	player->UPKey = up;
	player->LEFTKey = left;
	player->RIGHTKey = right;
	player->DOWNKey = down;
	player->ACTIONKey = action;

	player->color = color;
	player->size = PLAYER_SIZE;

	return player;
}

void addGoal(Player_t * player) {

	player->goals++;

}

void checkPlayerBoundaries(Player_t * player) {

	if (player->y >= GOAL_Y_INIT && player->y <= GOAL_Y_END) {

		if ((player->x - player->size) < LEFT_GOAL_X_INIT)
			player->x = player->size + LEFT_GOAL_X_INIT;

		if ((player->x + player->size) > RIGHT_GOAL_X_END)
			player->x = RIGHT_GOAL_X_END - player->size;

	} else {
		if ((player->x - player->size) <= FIELD_X_INIT)

			player->x = player->size + FIELD_X_INIT;

		if ((player->y - player->size) <= FIELD_Y_INIT)
			player->y = player->size + FIELD_Y_INIT;

		if ((player->x + player->size) >= FIELD_X_END)
			player->x = FIELD_X_END - player->size;
		if ((player->y + player->size) >= FIELD_Y_END)
			player->y = FIELD_Y_END - player->size;

	}

	return;

}

int belongsToPlayer(Player_t * player, kbd_key * key) {

	if (key->makecode == player->UPKey || key->makecode == player->DOWNKey
			|| key->makecode == player->RIGHTKey
			|| key->makecode == player->LEFTKey)
		return 1;

	if (key->makecode == player->ACTIONKey)
		return 2;

	return 0;
}

void deletePlayer(Player_t * player) {

	free(player);
	player = NULL;

	return;
}

void drawPlayer(Player_t * player) {

	vg_draw_circle_with_limit(player->x, player->y, player->size,
			player->color);

	return;
}

void drawPlayerScore(Player_t * player, int position_x, int position_y) {

	drawBitmapShape(getTime()->goals_score[player->goals], position_x,
			position_y,
			IGNORE_PURE_GREEN);

}

void drawScores(Player_t * player_one, Player_t * player_two) {

	drawPlayerScore(player_one, FIRST_SCORE_X, FIRST_SCORE_Y);

	drawBitmapShape(getTime()->score_divider, SCORE_DIVIDER_X, SCORE_DIVIDER_Y,
	IGNORE_PURE_GREEN);

	drawPlayerScore(player_two, SECOND_SCORE_X, SECOND_SCORE_Y);

}

int checkColision(Player_t * player_one, Player_t * player_two) {

	int x_distance = 0;
	int y_distance = 0;
	int radii = 0;

	radii = player_one->size + player_two->size;
	radii = radii * radii;

	x_distance = player_one->x - player_two->x;
	x_distance = x_distance * x_distance;

	y_distance = player_one->y - player_two->y;
	y_distance = y_distance * y_distance;

	if ((x_distance + y_distance) <= radii) {

		return 1;

	}
	return 0;

}

void updatePositionAndSpeed(Player_t * player) {

	player->vx += player->x_acceleration;
	player->vy += player->y_acceleration;

	player->vx = player->vx >= PLAYER_MAX_VELOCITY ?
	PLAYER_MAX_VELOCITY :
														player->vx;
	player->vx =
			player->vx <= -PLAYER_MAX_VELOCITY ?
					-PLAYER_MAX_VELOCITY : player->vx;
	player->vy = player->vy >= PLAYER_MAX_VELOCITY ?
	PLAYER_MAX_VELOCITY :
														player->vy;
	player->vy =
			player->vy <= -PLAYER_MAX_VELOCITY ?
					-PLAYER_MAX_VELOCITY : player->vy;

	player->x += (int) player->vx;
	player->y += (int) player->vy;

	checkPlayerBoundaries(player);

	double speed_x = fabs(player->vx);

	speed_x *= 0.81;

	double speed_y = fabs(player->vy);

	speed_y *= 0.81;

	if (speed_x < 0.7) {
		player->vx = 0;
	} else {

		if (player->vx > 0)
			player->vx = speed_x;
		else if (player->vx < 0)
			player->vx = -speed_x;
		else
			player->vx = 0;
	}

	if (speed_y < 0.7) {
		player->vy = 0;
	} else {
		if (player->vy > 0)
			player->vy = speed_y;
		else if (player->vy < 0)
			player->vy = -speed_y;
		else
			player->vy = 0;
	}

	player->x_acceleration *= 0.8;
	player->y_acceleration *= 0.8;

}

void dealWithColision(Player_t * player_one, Player_t * player_two) {

	Vector_t v1 = createVec(player_one->vx, player_one->vy);
	Vector_t v2 = createVec(player_two->vx, player_two->vy);

	Vector_t v12 = subtractVectors(v1, v2);
	Vector_t v21 = subtractVectors(v2, v1);

	Vector_t x1 = createVec(player_one->x, player_one->y);
	Vector_t x2 = createVec(player_two->x, player_two->y);

	Vector_t x12 = subtractVectors(x1, x2);
	Vector_t x21 = subtractVectors(x2, x1);

	double movingAway = dotProduct(v21, x21);

	if (movingAway > 0)
		return;

	double size1 = vecSize(v12);
	//size1 *= size1;
	double size2 = vecSize(v21);
	//size2 *= size2;

	double factor1 = dotProduct(v12, x12) / size1;

	double factor2 = dotProduct(v21, x21) / size2;

	x12 = vectorMultiplyFactor(x12, factor1);
	x21 = vectorMultiplyFactor(x21, factor2);

	Vector_t oneVelocity = subtractVectors(v1, x12);
	Vector_t twoVelocity = subtractVectors(v2, x21);

	player_one->vx += fmod(oneVelocity.x, 20);
	player_one->vy += fmod(oneVelocity.y, 20);
	player_two->vx += fmod(twoVelocity.x, 20);
	player_two->vy += fmod(twoVelocity.y, 20);

	//need to reset accelerations
	player_one->x_acceleration = 0;
	player_one->y_acceleration = 0;
	player_two->x_acceleration = 0;
	player_two->y_acceleration = 0;

	updatePositionAndSpeed(player_one);
	updatePositionAndSpeed(player_two);

	return;

}

void resetPlayerPosition(Player_t * player) {

	player->x = player->starting_x;
	player->y = player->starting_y;

	player->vx = 0;
	player->vy = 0;

	player->x_acceleration = 0;
	player->y_acceleration = 0;

}

void resetGoals(Player_t * player) {
	player->goals = 0;
}

void fixPlayersOverlaping(Player_t * player_one, Player_t * player_two) {

	int x_distance = 0;
	int y_distance = 0;
	int radii = 0;

	radii = player_one->size + player_two->size;
	radii = radii * radii;

	x_distance = player_one->x - player_two->x;
	x_distance = x_distance * x_distance;

	y_distance = player_one->y - player_two->y;
	y_distance = y_distance * y_distance;

	double abs_one_vx = fabs(player_one->vx);
	double abs_two_vx = fabs(player_two->vx);
	double abs_one_vy = fabs(player_one->vx);
	double abs_two_vy = fabs(player_two->vy);

	if ((x_distance + y_distance) < (radii - 125)) {

		if (abs_one_vx > abs_two_vx) {

			if (player_one->x > player_two->x) {

				///	===== nested if block to test if they don't go beyond the field =====

				if ((player_one->x - (player_one->size + player_two->size))
						> FIELD_X_INIT)
					player_two->x = player_one->x
							- (player_one->size + player_two->size);
				else
					player_one->x = player_two->x
							- (player_one->size + player_two->size);

			} else {

				//	===== nested if block to test if they don't go beyond the field =====

				if ((player_one->x + (player_one->size + player_two->size))
						< FIELD_X_END)
					player_two->x = player_one->x
							+ (player_one->size + player_two->size);
				else
					player_one->x = player_two->x
							- (player_one->size + player_two->size);
			}

		} else {

			if (player_one->x > player_two->x) {

				//	===== nested if block to test if they don't go beyond the field =====

				if ((player_two->x + (player_one->size + player_two->size))
						< FIELD_X_END)

					player_one->x = player_two->x
							+ (player_one->size + player_two->size);
				else
					player_two->x = player_one->x
							- (player_one->size + player_two->size);

			} else {

				//	===== nested if block to test if they don't go beyond the field =====

				if ((player_two->x - (player_one->size + player_two->size))
						> FIELD_X_INIT)
					player_one->x = player_two->x
							- (player_one->size + player_two->size);
				else
					player_two->x = player_one->x
							+ (player_one->size + player_two->size);

			}
		}

		if (abs_one_vy > abs_two_vy) {

			if (player_one->y > player_two->y) {

				//	===== nested if block to test if they don't go beyond the field =====

				if ((player_one->y - (player_one->size + player_two->size))
						> FIELD_Y_INIT)
					player_two->y = player_one->y
							- (player_one->size + player_two->size);
				else
					player_one->y = player_two->y
							+ (player_one->size + player_two->size);

			} else {

				//	===== nested if block to test if they don't go beyond the field =====

				if ((player_one->y + (player_one->size + player_two->size))
						< FIELD_Y_END)
					player_two->y = player_one->y
							+ (player_one->size + player_two->size);
				else
					player_one->y = player_two->y
							- (player_one->size + player_two->size);

			}

		} else {

			if (player_one->y > player_two->y) {

				//	===== nested if block to test if they don't go beyond the field =====

				if ((player_two->y + (player_one->size + player_two->size))
						> FIELD_Y_END)
					player_one->y = player_two->y
							+ (player_one->size + player_two->size);
				else
					player_two->y = player_one->y
							- (player_one->size + player_two->size);

			} else {

				//	===== nested if block to test if they don't go beyond the field =====

				if ((player_two->y - (player_one->size + player_two->size))
						> FIELD_Y_INIT)

					player_one->y = player_two->y
							- (player_one->size + player_two->size);
				else
					player_two->y = player_one->y
							+ (player_one->size + player_two->size);

			}

		}
	}

}

void updatePlayerAction(Player_t * player, kbd_key * key) {

	if (key->pressed == 0)
		player->action = 0;
	else if (key->pressed == 1)
		player->action = 1;

}

//======================================================================================================

/*
 * MOVEMENT FUNCTIONS
 *
 *
 */

MOVEMENT_EVENT * createMovementEvent(Player_t * player) {

	MOVEMENT_EVENT * movement = malloc(sizeof(MOVEMENT_EVENT));

	if (movement == NULL)
		return NULL;

	movement->key_up_pressed = 0;
	movement->key_down_pressed = 0;
	movement->key_right_pressed = 0;
	movement->key_left_pressed = 0;
	movement->player = player;

	return movement;

}

void updateMovement(MOVEMENT_EVENT * mov, kbd_key * key) {

	if (key->makecode == mov->player->UPKey) {

		if (key->pressed == 0)
			mov->key_up_pressed = 0;
		else
			mov->key_up_pressed = 1;

	} else if (key->makecode == mov->player->DOWNKey) {

		if (key->pressed == 0)
			mov->key_down_pressed = 0;
		else
			mov->key_down_pressed = 1;

	} else if (key->makecode == mov->player->RIGHTKey) {

		if (key->pressed == 0)
			mov->key_right_pressed = 0;
		else
			mov->key_right_pressed = 1;

	} else if (key->makecode == mov->player->LEFTKey) {

		if (key->pressed == 0)
			mov->key_left_pressed = 0;
		else
			mov->key_left_pressed = 1;

	}

}

void updatePlayerWithMovement(Player_t * player, MOVEMENT_EVENT * mov) {

	if (mov->key_up_pressed == 1 && mov->key_down_pressed == 1) {
		player->vy = 0;
		player->y_acceleration = 0;
	} else if (mov->key_up_pressed == 1 && mov->key_down_pressed == 0)
		player->y_acceleration = -PLAYER_ACCELERATION;
	else if (mov->key_up_pressed == 0 && mov->key_down_pressed == 1)
		player->y_acceleration = PLAYER_ACCELERATION;
	else if (mov->key_up_pressed == 0 && mov->key_down_pressed == 0)
		player->y_acceleration = 0;

	if (mov->key_right_pressed == 1 && mov->key_left_pressed == 1) {
		player->vx = 0;
		player->x_acceleration = 0;
	} else if (mov->key_right_pressed == 0 && mov->key_left_pressed == 1)
		player->x_acceleration = -PLAYER_ACCELERATION;
	else if (mov->key_right_pressed == 1 && mov->key_left_pressed == 0)
		player->x_acceleration = PLAYER_ACCELERATION;
	else if (mov->key_right_pressed == 0 && mov->key_left_pressed == 0)
		player->x_acceleration = 0;

	updatePositionAndSpeed(player);

	return;

}

void deleteMovement(MOVEMENT_EVENT * mov) {

	if (mov == NULL)
		return;

	free(mov);
	mov = NULL;

}

void resetMovement(MOVEMENT_EVENT * mov) {

	mov->key_up_pressed = 0;
	mov->key_down_pressed = 0;
	mov->key_left_pressed = 0;
	mov->key_right_pressed = 0;

}

