#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <minix/driver.h>
#include <minix/com.h>
#include <sys/mman.h>
#include <minix/sysutil.h>
#include <stdio.h>
#include <stdlib.h>
#include "proj.h"
#include "bitmap.h"
#include "mouse.h"
#include "vbe.h"
#include "timer.h"
#include "keyboard.h"
#include "video_gr.h"
#include "i8042.h"
#include "player.h"
#include "rectangle.h"
#include "time.h"
#include "ball.h"
#include "field.h"
#include "rtc.h"
#include "highscore.h"

static Bitmap * menuBackground = NULL;
static Bitmap * fieldBackground = NULL;
static Bitmap * highscores = NULL;
static Bitmap * goal = NULL;
static Bitmap * player_one_won = NULL;
static Bitmap * player_two_won = NULL;

static Rectangle * startRect = NULL;
static Rectangle * exitRect = NULL;
static Rectangle * highScoreRect = NULL;
static Rectangle * exitScore = NULL;

static Player_t * player_one = NULL;
static Player_t * player_two = NULL;

static MOVEMENT_EVENT * player_one_movement = NULL;
static MOVEMENT_EVENT * player_two_movement = NULL;

char * pwd = NULL;

static void resetGame() {

	resetPlayerPosition(player_one);
	resetPlayerPosition(player_two);
	resetMovement(player_one_movement);
	resetMovement(player_two_movement);
	resetGoals(player_one);
	resetGoals(player_two);
	resetBallPosition();
	resetTime();

}

static void resetPositions() {

	resetPlayerPosition(player_one);
	resetPlayerPosition(player_two);
	resetMovement(player_one_movement);
	resetMovement(player_two_movement);
	resetBallPosition();

}

int initializeGame() {

	sef_startup();
	sys_enable_iop(SELF);

	/*
	 * Also tests if the mode is supported
	 *
	 */

	if (vg_init(VBE_GRAPHIC_MODE_1024_768_64K) == NULL) {
		vg_exit();
		printf("Nao foi possivel trocar de modo\n");
		return 1;
	}

	//===========FILE OPEN====================================
	FILE * high;
	high = fopen(FILE_PATH "highscores.txt", "r");
	startHighScores(high);
	fclose(high);

	//============BACKGROUND BITMAP==============================

	menuBackground = loadBitmap(IMAGES_PATH "main-menu.bmp");
	if (menuBackground == NULL) {

		printf("erro ao abrir o bitmap do menuBackground.\n");
		return 1;

	}

	fieldBackground = loadBitmap(IMAGES_PATH "field.bmp");

	if (fieldBackground == NULL) {

		printf("erro ao abrir o bitmap do fieldBackground.\n");
		return 1;

	}

	highscores = loadBitmap(IMAGES_PATH "highscores_with_label.bmp");

	if (highscores == NULL) {

		printf("erro ao abrir o bitmap do highscores.\n");
		return 1;

	}

	goal = loadBitmap(IMAGES_PATH "goal.bmp");

	if (goal == NULL) {

		printf("erro ao abrir o bitmap do goal.\n");
		return 1;

	}

	player_two_won = loadBitmap(IMAGES_PATH "player_two_won.bmp");

	if (player_two_won == NULL) {

		printf("erro ao abrir o bitmap do player_two_won.\n");
		return 1;

	}

	player_one_won = loadBitmap(IMAGES_PATH "player_one_won.bmp");

	if (player_one_won == NULL) {

		printf("erro ao abrir o bitmap do player_one_won.\n");
		return 1;

	}

	startRect = createRectangle(START_RECT_X, START_RECT_Y, START_RECT_X_SIZE,
	START_RECT_Y_SIZE, COLOR_BLUE);

	if (startRect == NULL) {

		printf("erro ao criar o startRect\n");
		return 1;

	}

	exitRect = createRectangle(EXIT_RECT_X, EXIT_RECT_Y, EXIT_RECT_X_SIZE,
	EXIT_RECT_Y_SIZE, COLOR_RED);

	if (exitRect == NULL) {

		printf("erro ao criar o exitRect\n");
		return 1;

	}

	highScoreRect = createRectangle(HIGH_RECT_X, HIGH_RECT_Y, HIGH_RECT_X_SIZE,
	HIGH_RECT_Y_SIZE, COLOR_BLUE);

	if (highScoreRect == NULL) {

		printf("erro ao criar o highScoreRect\n");
		return 1;

	}

	exitScore = createRectangle(EXIT_HIGH_RECT_X, EXIT_HIGH_RECT_Y,
	EXIT_HIGH_RECT_X_SIZE, EXIT_HIGH_RECT_Y_SIZE, COLOR_RED);

	if (exitScore == NULL) {

		printf("erro ao criar o exitScore\n");
		return 1;

	}

	//============================================================

	//======================Player================================

	player_one = createPlayer(KEY_W, KEY_S, KEY_A, KEY_D, KEY_SPACEBAR,
	SOFT_RED, PLAYER_ONE_STARTING_X, PLAYER_ONE_STARTING_Y);
	player_one_movement = createMovementEvent(player_one);

	player_two = createPlayer(KEY_ARROWUP, KEY_ARROWDOWN, KEY_ARROWLEFT,
			KEY_ARROWRIGHT, KEY_NUMENTER, SOFT_BROW, PLAYER_TWO_STARTING_X,
			PLAYER_TWO_STARTING_Y);
	player_two_movement = createMovementEvent(player_two);

	//============================================================

	drawBackGroundBitmap(menuBackground, 0, 0);

	//==============================MOUSE==========================
	//=============================================================

	configureGameMouse();

	//=============================================================

	getMouse();

	getBall();

	return 0;
}

int main(int argc, char ** argv) {

	if (initializeGame() == 1)
		return 1;

	int r;   //to store driver_receiver return value

	int ipc_status;

	message msg;

	int irq_timer_set = timer_subscribe();

	int irq_mouse_set = mouse_subscribe();

	int irq_kbd_set = keyboard_subscribe();

	if (irq_timer_set == -1) //failed to subscribe timer
		return 1;

	if (irq_mouse_set == -1) //failed to subscribe mouse
		return 1;

	if (irq_kbd_set == -1) //failed to subscribe kbd
		return 1;

	while (1) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {

			printf("driver_receive failed with: %d", r);

			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			/* received notification */
			if (_ENDPOINT_P(msg.m_source) == HARDWARE) {

				/*every time an interrupt is received,
				 * it will be handled, and then processed by the event handler.
				 */

				if (msg.NOTIFY_ARG & irq_timer_set) {

					timer_int_handler();
					if (game_tick()) {
						if (eventHandler(NEW_FRAME_EVENT) == 1) {
							break;
						}
					}

				}

				if (msg.NOTIFY_ARG & irq_mouse_set) {

					mouse_int_handler();

					if (eventHandler(MOUSESTROKE_EVENT) == 1) {
						break;
					}

				}

				if (msg.NOTIFY_ARG & irq_kbd_set) {

					kbd_int_handler();
					if (eventHandler(KEYSTROKE_EVENT) == 1) {
						break;
					}

				}

			}
		}

	}

	vg_exit();

	//========UNSUBSCRIBES=============
	timer_unsubscribe();
	mouse_unsubscribe();
	keyboard_unsubscribe();

	//========DELETES==================

	deleteRectangle(startRect);
	deleteRectangle(exitRect);
	deleteRectangle(highScoreRect);
	deleteRectangle(exitScore);

	deleteBitmap(menuBackground);
	deleteBitmap(fieldBackground);
	deleteBitmap(highscores);
	deleteBitmap(goal);
	deleteBitmap(player_one_won);
	deleteBitmap(player_two_won);

	deletePlayer(player_two);
	deletePlayer(player_one);

	deleteMovement(player_one_movement);
	deleteMovement(player_two_movement);
	deleteTime();
	deleteBall();
	deleteMouse();
	deleteTops();

	return 0;

}

int eventHandler(EVENT_TYPE event) {

	//Initializing to the first state, the main menu state.
	static STATE_TYPE state = MAIN_MENU_STATE;

	switch (state) {

	case MAIN_MENU_STATE: {
		state = menuEventHandler(event);
		break;

	}
	case GAME_STATE: {
		if (event != MOUSESTROKE_EVENT)
			state = gameEventHandler(event);
		break;
	}

	case HIGH_SCORE_STATE: {
		state = gameHighScoreHandler(event);
		break;

	}

	case EXIT_STATE: {
		return 1;
	}
	}

	return 0;

}

STATE_TYPE menuEventHandler(EVENT_TYPE event) {

	switch (event) {

	case NEW_FRAME_EVENT: {

		vg_draw_background();

		if (insideRect(startRect))
			drawRectangle(startRect);
		else if (insideRect(exitRect))
			drawRectangle(exitRect);
		else if (insideRect(highScoreRect))
			drawRectangle(highScoreRect);

		drawMouse();
		vg_swap_video();

		return MAIN_MENU_STATE;

	}
	case MOUSESTROKE_EVENT: {
		if (insideRect(startRect)) {

			if (getMouse()->LButton == 1) {
				drawBackGroundBitmap(fieldBackground, 0, 0);
				return GAME_STATE;
			}

		} else if (insideRect(exitRect)) {

			if (getMouse()->LButton == 1) {
				saveHighScores();
				return EXIT_STATE;
			}

		} else if (insideRect(highScoreRect)) {

			if (getMouse()->LButton == 1) {
				drawBackGroundBitmap(highscores, 0, 0);

				return HIGH_SCORE_STATE;
			}

		}

		return MAIN_MENU_STATE;
	}

	case KEYSTROKE_EVENT: {
		if (g_key->makecode == KEY_ESC) {
			saveHighScores();
			return EXIT_STATE;
		}
		return MAIN_MENU_STATE;

	}

	}

	return MAIN_MENU_STATE;

}

STATE_TYPE gameEventHandler(EVENT_TYPE event) {

	getTime();

	switch (event) {

	case NEW_FRAME_EVENT: {

		vg_draw_background();

		updatePlayerWithMovement(player_one, player_one_movement);
		updatePlayerWithMovement(player_two, player_two_movement);

		if (checkColision(player_one, player_two)) {

			dealWithColision(player_one, player_two);

		}

		updateBall(player_one, player_two);

		fixPlayersOverlaping(player_one, player_two);

		drawPlayer(player_one);

		drawPlayer(player_two);

		drawBall();

		int goal_occured = 0;

		if (ballIsMoving()) {

			if (checkGoal() == 1) {

				addGoal(player_two);

				if (player_two->goals >= 3) {
					testeNewHighscore();
					drawTime();
					drawScores(player_one, player_two);
					drawBitmapShape(player_two_won, WON_X, WON_Y,
					IGNORE_PURE_GREEN);
					vg_swap_video();
					tickdelay(WON_TIMEOUT);
					drawBackGroundBitmap(menuBackground, 0, 0);
					resetGame();
					return MAIN_MENU_STATE;
				}

				resetPositions();

				goal_occured = 1;

			}

			else if (checkGoal() == 2) {
				addGoal(player_one);

				if (player_one->goals >= 3) {
					testeNewHighscore();
					drawTime();
					drawScores(player_one, player_two);
					drawBitmapShape(player_one_won, WON_X, WON_Y,
					IGNORE_PURE_GREEN);

					vg_swap_video();
					tickdelay(WON_TIMEOUT);
					drawBackGroundBitmap(menuBackground, 0, 0);
					resetGame();
					return MAIN_MENU_STATE;
				}

				resetPositions();

				goal_occured = 1;
			}

		}

		if (seconds_tick()) {
			updateTime();

			//time's up
			if (getTime()->t == 0) {
				if (player_one->goals != player_two->goals)
					testeNewHighscore();

				drawBackGroundBitmap(menuBackground, 0, 0);
				resetGame();
				return MAIN_MENU_STATE;
			}
		}

		drawTime();
		drawScores(player_one, player_two);
		if (goal_occured == 1) {

			drawBitmapShape(goal, GOAL_X, GOAL_Y, IGNORE_PURE_GREEN);
			vg_swap_video();
			tickdelay(GOAL_TIMEOUT);
			goal_occured = 0;

		} else
			vg_swap_video();

		return GAME_STATE;

	}
	case KEYSTROKE_EVENT: {
		if (g_key->makecode == KEY_ESC) {

			drawBackGroundBitmap(menuBackground, 0, 0);
			resetGame();
			return MAIN_MENU_STATE;

		}

		if (belongsToPlayer(player_one, g_key) == 1)
			updateMovement(player_one_movement, g_key);
		else if (belongsToPlayer(player_one, g_key) == 2)
			updatePlayerAction(player_one, g_key);

		if (belongsToPlayer(player_two, g_key) == 1)
			updateMovement(player_two_movement, g_key);
		else if (belongsToPlayer(player_two, g_key) == 2)
			updatePlayerAction(player_two, g_key);

		return GAME_STATE;
	}
	default: {
		return GAME_STATE;
	}

	}

	return GAME_STATE;
}

STATE_TYPE gameHighScoreHandler(EVENT_TYPE event) {

	switch (event) {

	case NEW_FRAME_EVENT: {
		startNumbers();
		vg_draw_background();

		if (insideRect(exitScore))
			drawRectangle(exitScore);
		drawHighscores();
		drawMouse();

		vg_swap_video();

		return HIGH_SCORE_STATE;

	}
	case MOUSESTROKE_EVENT: {
		if (insideRect(exitScore)) {

			if (getMouse()->LButton == 1) {
				drawBackGroundBitmap(menuBackground, 0, 0);
				return MAIN_MENU_STATE;
			}

		}

		return HIGH_SCORE_STATE;
	}
	case KEYSTROKE_EVENT: {
		if (g_key->makecode == KEY_ESC) {
			drawBackGroundBitmap(menuBackground, 0, 0);
			return MAIN_MENU_STATE;
		}
		return HIGH_SCORE_STATE;
	}
	default:
		return HIGH_SCORE_STATE;
	}

	return HIGH_SCORE_STATE;
}


