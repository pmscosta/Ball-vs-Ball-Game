#include "ball.h"
#include "video_gr.h"
#include "vector.h"
#include "field.h"
#include <math.h>
#include <stdlib.h>

Ball_t * ball = NULL;

static Ball_t * createBall() {

	ball = malloc(sizeof(Ball_t));

	if (ball == NULL)
		return NULL;

	ball->starting_x = BALL_STARTING_X;
	ball->starting_y = BALL_STARTING_Y;

	ball->x = BALL_STARTING_X;
	ball->y = BALL_STARTING_Y;
	ball->vx = 0;
	ball->vy = 0;

	ball->color = LIGHT_GRAY;

	ball->size = BALLSIZE;

	return ball;

}

Ball_t * getBall() {

	if (ball == NULL)
		ball = createBall();

	return ball;

}

void deleteBall() {
	if (ball == NULL)
		return;
	free(ball);
	ball = NULL;
}

void drawBall() {

	vg_draw_circle_with_limit(ball->x, ball->y, ball->size, ball->color);

	return;

}

int checkBallBoundaries() {

	//the x limits will be different if the ball is in the y area of the goals
	if ((ball->y - ball->size) >= GOAL_Y_INIT
			&& (ball->y + ball->size) <= GOAL_Y_END) {

		if ((ball->x - ball->size) <= LEFT_GOAL_X_INIT) {
			return 1;
		}

		if ((ball->x + ball->size) >= RIGHT_GOAL_X_END) {
			return 1;
		}
	} else {
		if ((ball->x - ball->size) <= FIELD_X_INIT) {
			ball->x = (FIELD_X_INIT + ball->size);
			return 1;
		} else if ((ball->x + ball->size) >= FIELD_X_END) {
			ball->x = (FIELD_X_END - ball->size);
			return 1;
		}

		if ((ball->y - ball->size) <= FIELD_Y_INIT) {
			ball->y = (FIELD_Y_INIT + ball->size);
			return 2;
		}

		else if ((ball->y + ball->size) >= FIELD_Y_END) {
			ball->y = (FIELD_Y_END - ball->size);
			return 2;
		}

	}

	return 0;
}

int checkBallPlayerCollision(Player_t * player) {

	int x_distance = 0;
	int y_distance = 0;
	int radii = 0;

	radii = player->size + ball->size;
	radii = radii * radii;

	x_distance = player->x - ball->x;
	x_distance = x_distance * x_distance;

	y_distance = player->y - ball->y;
	y_distance = y_distance * y_distance;

	if ((x_distance + y_distance) <= radii) {
		return 1;
	}

	return 0;

}

void updateBall(Player_t * player_one, Player_t * player_two) {

	if (checkBallPlayerCollision(player_one)) {

		if (player_one->action == 1) {
			BallInteraction(KICKED, player_one);
		} else
			BallInteraction(PUSHED, player_one);

	}

	if (checkBallPlayerCollision(player_two)) {

		if (player_two->action == 1) {
			BallInteraction(KICKED, player_two);
		} else
			BallInteraction(PUSHED, player_two);

	}

	ball->vx = ball->vx > BALL_MAX_VELOCITY ? BALL_MAX_VELOCITY : ball->vx;
	ball->vx = ball->vx < -BALL_MAX_VELOCITY ? -BALL_MAX_VELOCITY : ball->vx;
	ball->vy = ball->vy > BALL_MAX_VELOCITY ? BALL_MAX_VELOCITY : ball->vy;
	ball->vy = ball->vy < -BALL_MAX_VELOCITY ? -BALL_MAX_VELOCITY : ball->vy;

	if (checkBallBoundaries() == 1) {
		ball->vx *= -1.0;
	} else if (checkBallBoundaries() == 2) {
		ball->vy *= -1.0;
	}

	ball->x += ball->vx;
	ball->y += ball->vy;

	fixBallOverlaping(player_one);
	fixBallOverlaping(player_two);

	checkBallBoundaries();

	double speed_x = fabsf(ball->vx);

	speed_x = speed_x * BALL_DRAG_COEFFIECIENT;

	double speed_y = fabsf(ball->vy);

	speed_y = speed_y * BALL_DRAG_COEFFIECIENT;

	if (speed_x < MINIMUM_VELOCITY_ALLOWED) {

		ball->vx = 0;
	} else {

		if (ball->vx > 0)
			ball->vx = speed_x;
		else if (ball->vx < 0)
			ball->vx = -speed_x;
		else
			ball->vx = 0;
	}

	if (speed_y < MINIMUM_VELOCITY_ALLOWED) {
		ball->vy = 0;
	} else {
		if (ball->vy > 0)
			ball->vy = speed_y;
		else if (ball->vy < 0)
			ball->vy = -speed_y;
		else
			ball->vy = 0;
	}
}

void BallInteraction(BALLACTIONS action, Player_t * player) {

	if (action == PUSHED) {

		Vector_t v1 = createVec(player->vx, player->vy);
		Vector_t v2 = createVec(ball->vx, ball->vy);

		Vector_t v21 = subtractVectors(v2, v1);

		Vector_t x1 = createVec(player->x, player->y);
		Vector_t x2 = createVec(ball->x, ball->y);

		Vector_t x21 = subtractVectors(x2, x1);

		double movingAway = dotProduct(v21, x21);

		if (movingAway > 0)
			return;

		if (player->vx > 0)
			ball->vx = player->vx + PUSHED_ADDER;
		else if (player->vx < 0)
			ball->vx = player->vx - PUSHED_ADDER;
		else
			//if the player is not moving, he will act like a goal keeper
			ball->vx = 0;

		if (player->vy > 0)
			ball->vy = player->vy + PUSHED_ADDER;
		else if (player->vy < 0)
			ball->vy = player->vy - PUSHED_ADDER;
		else
			ball->vy = 0;

		/*
		 * The code below simulates what we would describe as a more realistic pushing action
		 * but, unfortunately, it makes controlling the ball away harder than what we want.
		 *
		 */

//		Vector_t v_player = createVec(player->vx, player->vy);
//		Vector_t v_ball = createVec(ball->vx, ball->vy);
//
//		Vector_t x1 = createVec(ball->x, ball->y);
//		Vector_t x2 = createVec(player->x, player->y);
//
//		Vector_t x21 = subtractVectors(x2, x1);
//		Vector_t x12 = subtractVectors(x1, x2);
//		Vector_t v21 = subtractVectors(v_ball, v_player);
//
//		double movingAway = dotProduct(v21, x12);
//
//		if (movingAway > 0)
//			return;
//
//		double factor = dotProduct(v_player, x21) / dotProduct(x21, x21);
//
//		Vector_t inc_velocity = vectorMultiplyFactor(x21, factor);
//
//		double speed_x = fabs(inc_velocity.x);
//		double speed_y = fabs(inc_velocity.y);
//
//		if (speed_x < 1.0)
//			ball->vx = 0;
//		else {
//			if (inc_velocity.x > 0)
//				ball->vx = inc_velocity.x + PUSHED_ADDER;
//			else if (inc_velocity.x < 0)
//				ball->vx = inc_velocity.x - PUSHED_ADDER;
//			else
//				ball->vx = 0;
//		}
//
//		if (speed_y < 1.0) {
//			ball->vy = 0;
//		} else {
//			if (inc_velocity.y > 0)
//				ball->vy = inc_velocity.y + PUSHED_ADDER;
//			else if (inc_velocity.y < 0)
//				ball->vy = inc_velocity.y - PUSHED_ADDER;
//			else
//				ball->vy = 0;
//		}
	}

	else if (action == KICKED) {

		if (player->vx != 0 || player->vy != 0) {

			Vector_t v_player = createVec(player->vx, player->vy);

			Vector_t x1 = createVec(ball->x, ball->y);
			Vector_t x2 = createVec(player->x, player->y);

			Vector_t x21 = subtractVectors(x2, x1);

			double factor = dotProduct(v_player, x21) / dotProduct(x21, x21);

			Vector_t inc_velocity = vectorMultiplyFactor(x21, factor);

			ball->vx += inc_velocity.x * KICKED_MULTIPLIER;
			ball->vy += inc_velocity.y * KICKED_MULTIPLIER;

		}
		//if the player is not moving he will still be able to kick the ball
		else {

			Vector_t v1;
			if (ball->x > player->x)

				v1 = createVec(KICKED_IMPULSE, KICKED_IMPULSE);
			else
				v1 = createVec(-KICKED_IMPULSE, -KICKED_IMPULSE);
			Vector_t v2 = createVec(ball->vx, ball->vy);

			Vector_t v21 = subtractVectors(v2, v1);

			Vector_t x1 = createVec(player->x, player->y);
			Vector_t x2 = createVec(ball->x, ball->y);

			Vector_t x21 = subtractVectors(x2, x1);

			double size2 = vecSize(v21);

			double factor2 = dotProduct(v21, x21) / size2;
			x21 = vectorMultiplyFactor(x21, factor2);

			Vector_t twoVelocity = subtractVectors(v2, x21);

			ball->vx += twoVelocity.x;
			ball->vy += twoVelocity.y;

		}

	}
}

void resetBallPosition() {

	ball->x = ball->starting_x;
	ball->y = ball->starting_y;

	ball->vx = 0;
	ball->vy = 0;

}
int ballIsMoving() {

	if (ball->vx != 0 || ball->vy != 0)
		return 1;
	return 0;

}

int checkGoal() {
	if ((ball->y - ball->size) >= GOAL_Y_INIT
			&& (ball->y + ball->size) <= GOAL_Y_END) {

		//if it is the left side goal, returns 1
		if ((ball->x - ball->size) <= LEFT_GOAL_X_END - 5)
			return 1;

		//if it is the right side goal, returns 2
		if ((ball->x + ball->size) >= RIGHT_GOAL_X_INIT + 5)
			return 2;
	}

	return 0;

}

void fixBallOverlaping(Player_t * player) {

	int x_distance = 0;
	int y_distance = 0;
	int radii = 0;

	radii = player->size + ball->size;
	radii = radii * radii;

	x_distance = player->x - ball->x;
	x_distance = x_distance * x_distance;

	y_distance = player->y - ball->y;
	y_distance = y_distance * y_distance;

	if ((x_distance + y_distance) < (radii - OVERLAPING_MEASURE)) {

		if (ball->x < player->x)
			ball->x = player->x - (player->size + ball->size);
		else if (ball->x > player->x)
			ball->x = player->x + (player->size + ball->size);

		if (ball->y < player->y)
			ball->y = player->y - (player->size + ball->size);
		else if (ball->y > player->y)
			ball->y = player->y + (player->size + ball->size);

	}

}
