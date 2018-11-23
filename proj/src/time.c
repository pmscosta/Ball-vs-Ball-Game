#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "time.h"
#include "i8254.h"
#include "video_gr.h"
#include "field.h"

Time * time = NULL;

Time * createTime() {

	time = malloc(sizeof(Time));
	if (time == NULL)
		return NULL;

	//starting time
	time->t = STARTING_TIME;
	time->min = STARTING_TIME / 60;
	time->dezsec = 0;
	time->unisec = 0;
	time->twopoints = loadBitmap(IMAGES_PATH "twopoints.bmp");
	time->numbers[0] = loadBitmap(IMAGES_PATH "0.bmp");
	time->numbers[1] = loadBitmap(IMAGES_PATH "1.bmp");
	time->numbers[2] = loadBitmap(IMAGES_PATH "2.bmp");
	time->numbers[3] = loadBitmap(IMAGES_PATH "3.bmp");
	time->numbers[4] = loadBitmap(IMAGES_PATH "4.bmp");
	time->numbers[5] = loadBitmap(IMAGES_PATH "5.bmp");
	time->numbers[6] = loadBitmap(IMAGES_PATH "6.bmp");
	time->numbers[7] = loadBitmap(IMAGES_PATH "7.bmp");
	time->numbers[8] = loadBitmap(IMAGES_PATH "8.bmp");
	time->numbers[9] = loadBitmap(IMAGES_PATH "9.bmp");
	time->xmin = 920;
	time->ymin = 40;
	time->xTwPoints = 935;
	time->yTwPoints = 40;
	time->xDsec = 950;
	time->yDsec = 40;
	time->xUsec = 965;
	time->yUsec = 40;

	time->goals_score[0] = loadBitmap(IMAGES_PATH "bigger_0.bmp");
	time->goals_score[1] = loadBitmap(IMAGES_PATH "bigger_1.bmp");
	time->goals_score[2] = loadBitmap(IMAGES_PATH "bigger_2.bmp");
	time->goals_score[3] = loadBitmap(IMAGES_PATH "bigger_3.bmp");
	time->score_divider = loadBitmap(IMAGES_PATH "bigger_two_points.bmp");

	time->ignored_color = rgb(0, 255, 0);

	return time;
}

void updateTime() {
	time->t = (time->t) - 1;
	time->min = (time->t) / 60;
	time->dezsec = ((time->t) % 60) / 10;
	time->unisec = ((time->t) % 60) % 10;

}

Time * getTime() {

	if (time == NULL)
		time = createTime();

	return time;

}

void deleteTime() {
	if (time == NULL)
		return;

	int i;

	for (i = 0; i < 10; i++) {

		deleteBitmap(time->numbers[i]);

	}

	for (i = 0; i < 4; i++) {
		deleteBitmap(time->goals_score[i]);
	}

	deleteBitmap(time->twopoints);

	deleteBitmap(time->score_divider);

	free(time);

	time = NULL;
}

void drawTime() {
	drawBitmapShape(time->numbers[time->min], time->xmin, time->ymin,
			time->ignored_color);
	drawBitmapShape(time->twopoints, time->xTwPoints, time->yTwPoints,
			time->ignored_color);
	drawBitmapShape(time->numbers[time->dezsec], time->xDsec, time->yDsec,
			time->ignored_color);
	drawBitmapShape(time->numbers[time->unisec], time->xUsec, time->yUsec,
			time->ignored_color);
}

void resetTime() {

	time->t = STARTING_TIME;
	time->min = STARTING_TIME / 60;
	time->dezsec = 0;
	time->unisec = 0;

}

