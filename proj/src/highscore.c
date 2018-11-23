#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <minix/sysutil.h>
#include "highscore.h"
#include "video_gr.h"
#include "rtc.h"
#include "time.h"
#include "field.h"

Top * top1 = NULL;
Top * top2 = NULL;
Top * top3 = NULL;
uint16_t ignored_color;
Bitmap * numbers[10];
Bitmap * twopoints = NULL;
Bitmap * bar = NULL;

void startNumbers() {
	if (twopoints == NULL)
		createNumbers();
	return;
}

void deleteTops() {

	if (top1 != NULL)
		free(top1);

	top1 = NULL;

	if (top2 != NULL)
		free(top2);

	top2 = NULL;

	if (top3 != NULL)
		free(top3);

	top3 = NULL;

	int i;

	for (i = 0; i < 10; i++) {
		deleteBitmap(numbers[i]);
	}

	deleteBitmap(bar);
	deleteBitmap(twopoints);

}

void startHighScores(FILE * high) {

	fseek(high, 0, SEEK_END);
	if (0 == ftell(high)) {
		return;
	}
	fseek(high, 0, SEEK_SET);
	top1 = malloc(sizeof(Top));
	if (top1 == NULL)
		return;
	fscanf(high, "%d\t%d\t%d\t%d\t%d\t%d\t%d", &top1->tempo, &top1->hora[0],
			&top1->hora[1], &top1->hora[2], &top1->dia[0], &top1->dia[1],
			&top1->dia[2]);

	if ((feof(high)))
		return;
	top2 = malloc(sizeof(Top));
	if (top2 == NULL)
		return;
	fscanf(high, "%d\t%d\t%d\t%d\t%d\t%d\t%d", &top2->tempo, &top2->hora[0],
			&top2->hora[1], &top2->hora[2], &top2->dia[0], &top2->dia[1],
			&top2->dia[2]);

	if ((feof(high)))
		return;
	top3 = malloc(sizeof(Top));
	if (top3 == NULL)
		return;
	fscanf(high, "%d\t%d\t%d\t%d\t%d\t%d\t%d", &top3->tempo, &top3->hora[0],
			&top3->hora[1], &top3->hora[2], &top3->dia[0], &top3->dia[1],
			&top3->dia[2]);

	return;

}
void helpfull(Top * t1, Top * t2) {
	t1->tempo = t2->tempo;
	t1->hora[0] = t2->hora[0];
	t1->hora[1] = t2->hora[1];
	t1->hora[2] = t2->hora[2];
	t1->dia[0] = t2->dia[0];
	t1->dia[1] = t2->dia[1];
	t1->dia[2] = t2->dia[2];
	return;
}

void saveHighScores() {
	FILE * high;
	high = fopen(FILE_PATH "highscores.txt", "w");

	if (top1 != NULL) {
		fprintf(high, "%d\t%d\t%d\t%d\t%d\t%d\t%d", top1->tempo, top1->hora[0],
				top1->hora[1], top1->hora[2], top1->dia[0], top1->dia[1],
				top1->dia[2]);
	}
	if (top2 != NULL) {
		fprintf(high, "\n%d\t%d\t%d\t%d\t%d\t%d\t%d", top2->tempo,
				top2->hora[0], top2->hora[1], top2->hora[2], top2->dia[0],
				top2->dia[1], top2->dia[2]);
	}
	if (top3 != NULL) {
		fprintf(high, "\n%d\t%d\t%d\t%d\t%d\t%d\t%d", top3->tempo,
				top3->hora[0], top3->hora[1], top3->hora[2], top3->dia[0],
				top3->dia[1], top3->dia[2]);
	}
	fclose(high);
}

void testeNewHighscore() {
	unsigned long hour, min, sec, day, month, year;
	int tempoaux = STARTING_TIME - (getTime()->t);
	rtc_get_date(&hour, &min, &sec, &day, &month, &year);
	//test if the new time is better than the top1
	if (top1 == NULL || (tempoaux < (top1->tempo))) {
		//if top1 does not exist , malloc for space
		if (top1 == NULL) {
			top1 = malloc(sizeof(Top));
			if (top1 == NULL)
				return;
		} else {
			//if top2 does not exist , malloc for space
			if (top2 == NULL) {
				top2 = malloc(sizeof(Top));
				if (top2 == NULL)
					return;
			} else {
				//if top3 does not exist , malloc for space
				if (top3 == NULL) {
					top3 = malloc(sizeof(Top));
					if (top3 == NULL)
						return;
				}
				helpfull(top3, top2);
			}
			helpfull(top2, top1);
		}
		top1->tempo = tempoaux;
		top1->hora[0] = (int) hour;
		top1->hora[1] = (int) min;
		top1->hora[2] = (int) sec;
		top1->dia[0] = (int) day;
		top1->dia[1] = (int) month;
		top1->dia[2] = (int) year;
		return;
	} else {
		//test if the new time is better than the top2
		if (top2 == NULL || (tempoaux < (top2->tempo))) {
			//if top2 does not exist , malloc for space
			if (top2 == NULL) {
				top2 = malloc(sizeof(Top));
				if (top2 == NULL)
					return;
			} else {
				//if top3 does not exist , malloc for space
				if (top3 == NULL) {
					top3 = malloc(sizeof(Top));
					if (top3 == NULL)
						return;
				}
				helpfull(top3, top2);
			}
			top2->tempo = tempoaux;
			top2->hora[0] = (int) hour;
			top2->hora[1] = (int) min;
			top2->hora[2] = (int) sec;
			top2->dia[0] = (int) day;
			top2->dia[1] = (int) month;
			top2->dia[2] = (int) year;
			return;
		} else {
			//test if the new time is better than the top3
			if (top3 == NULL || (tempoaux < (top3->tempo))) {
				//if top3 does not exist , malloc for space
				if (top3 == NULL) {
					top3 = malloc(sizeof(Top));
					if (top3 == NULL)
						return;
				}
				top3->tempo = tempoaux;
				top3->hora[0] = (int) hour;
				top3->hora[1] = (int) min;
				top3->hora[2] = (int) sec;
				top3->dia[0] = (int) day;
				top3->dia[1] = (int) month;
				top3->dia[2] = (int) year;
				return;
			}
		}
	}

	return;
}

void drawTop1() {
	if (top1 == NULL)
		return;
	//==================Draw the time of the top1, in minutes:sec=============
	drawBitmapShape(numbers[top1->tempo / 60], 260, TOP1_Y, ignored_color);
	drawBitmapShape(twopoints, 280, TOP1_Y, ignored_color);
	drawBitmapShape(numbers[(top1->tempo % 60) / 10], 300, TOP1_Y,
			ignored_color);
	drawBitmapShape(numbers[(top1->tempo % 60) % 10], 320, TOP1_Y,
			ignored_color);
	//==================Draw the hours the record was made, in hour:min:sec============
	drawBitmapShape(numbers[top1->hora[0] / 10], 380, TOP1_Y, ignored_color);
	drawBitmapShape(numbers[top1->hora[0] % 10], 400, TOP1_Y, ignored_color);
	drawBitmapShape(twopoints, 420, TOP1_Y, ignored_color);
	drawBitmapShape(numbers[top1->hora[1] / 10], 440, TOP1_Y, ignored_color);
	drawBitmapShape(numbers[top1->hora[1] % 10], 460, TOP1_Y, ignored_color);
	drawBitmapShape(twopoints, 480, TOP1_Y, ignored_color);
	drawBitmapShape(numbers[top1->hora[2] / 10], 500, TOP1_Y, ignored_color);
	drawBitmapShape(numbers[top1->hora[2] % 10], 520, TOP1_Y, ignored_color);
	//==================Draw the day the record was made, in day/month/year============
	drawBitmapShape(numbers[top1->dia[0] / 10], 580, TOP1_Y, ignored_color);
	drawBitmapShape(numbers[top1->dia[0] % 10], 600, TOP1_Y, ignored_color);
	drawBitmapShape(bar, 620, TOP1_Y, ignored_color);
	drawBitmapShape(numbers[top1->dia[1] / 10], 640, TOP1_Y, ignored_color);
	drawBitmapShape(numbers[top1->dia[1] % 10], 660, TOP1_Y, ignored_color);
	drawBitmapShape(bar, 680, TOP1_Y, ignored_color);
	drawBitmapShape(numbers[top1->dia[2] / 10], 700, TOP1_Y, ignored_color);
	drawBitmapShape(numbers[top1->dia[2] % 10], 720, TOP1_Y, ignored_color);
}

void drawTop2() {
	if (top2 == NULL)
		return;
	//==================Draw the time of the top2, in minutes:sec=============
	drawBitmapShape(numbers[top2->tempo / 60], 260, TOP2_Y, ignored_color);
	drawBitmapShape(twopoints, 280, TOP2_Y, ignored_color);
	drawBitmapShape(numbers[(top2->tempo % 60) / 10], 300, TOP2_Y,
			ignored_color);
	drawBitmapShape(numbers[(top2->tempo % 60) % 10], 320, TOP2_Y,
			ignored_color);
	//==================Draw the hours the record was made, in hour:min:sec============
	drawBitmapShape(numbers[top2->hora[0] / 10], 380, TOP2_Y, ignored_color);
	drawBitmapShape(numbers[top2->hora[0] % 10], 400, TOP2_Y, ignored_color);
	drawBitmapShape(twopoints, 420, TOP2_Y, ignored_color);
	drawBitmapShape(numbers[top2->hora[1] / 10], 440, TOP2_Y, ignored_color);
	drawBitmapShape(numbers[top2->hora[1] % 10], 460, TOP2_Y, ignored_color);
	drawBitmapShape(twopoints, 480, TOP2_Y, ignored_color);
	drawBitmapShape(numbers[top2->hora[2] / 10], 500, TOP2_Y, ignored_color);
	drawBitmapShape(numbers[top2->hora[2] % 10], 520, TOP2_Y, ignored_color);
	//==================Draw the day the record was made, in day/month/year============
	drawBitmapShape(numbers[top2->dia[0] / 10], 580, TOP2_Y, ignored_color);
	drawBitmapShape(numbers[top2->dia[0] % 10], 600, TOP2_Y, ignored_color);
	drawBitmapShape(bar, 620, TOP2_Y, ignored_color);
	drawBitmapShape(numbers[top2->dia[1] / 10], 640, TOP2_Y, ignored_color);
	drawBitmapShape(numbers[top2->dia[1] % 10], 660, TOP2_Y, ignored_color);
	drawBitmapShape(bar, 680, TOP2_Y, ignored_color);
	drawBitmapShape(numbers[top2->dia[2] / 10], 700, TOP2_Y, ignored_color);
	drawBitmapShape(numbers[top2->dia[2] % 10], 720, TOP2_Y, ignored_color);
}

void drawTop3() {
	if (top3 == NULL)
		return;
	//==================Draw the time of the top3, in minutes:sec=============
	drawBitmapShape(numbers[top3->tempo / 60], 260, TOP3_Y, ignored_color);
	drawBitmapShape(twopoints, 280, TOP3_Y, ignored_color);
	drawBitmapShape(numbers[(top3->tempo % 60) / 10], 300, TOP3_Y,
			ignored_color);
	drawBitmapShape(numbers[(top3->tempo % 60) % 10], 320, TOP3_Y,
			ignored_color);
	//==================Draw the hours the record was made, in hour:min:sec============
	drawBitmapShape(numbers[top3->hora[0] / 10], 380, TOP3_Y, ignored_color);
	drawBitmapShape(numbers[top3->hora[0] % 10], 400, TOP3_Y, ignored_color);
	drawBitmapShape(twopoints, 420, TOP3_Y, ignored_color);
	drawBitmapShape(numbers[top3->hora[1] / 10], 440, TOP3_Y, ignored_color);
	drawBitmapShape(numbers[top3->hora[1] % 10], 460, TOP3_Y, ignored_color);
	drawBitmapShape(twopoints, 480, TOP3_Y, ignored_color);
	drawBitmapShape(numbers[top3->hora[2] / 10], 500, TOP3_Y, ignored_color);
	drawBitmapShape(numbers[top3->hora[2] % 10], 520, TOP3_Y, ignored_color);
	//==================Draw the day the record was made, in day/month/year============
	drawBitmapShape(numbers[top3->dia[0] / 10], 580, TOP3_Y, ignored_color);
	drawBitmapShape(numbers[top3->dia[0] % 10], 600, TOP3_Y, ignored_color);
	drawBitmapShape(bar, 620, TOP3_Y, ignored_color);
	drawBitmapShape(numbers[top3->dia[1] / 10], 640, TOP3_Y, ignored_color);
	drawBitmapShape(numbers[top3->dia[1] % 10], 660, TOP3_Y, ignored_color);
	drawBitmapShape(bar, 680, TOP3_Y, ignored_color);
	drawBitmapShape(numbers[top3->dia[2] / 10], 700, TOP3_Y, ignored_color);
	drawBitmapShape(numbers[top3->dia[2] % 10], 720, TOP3_Y, ignored_color);
}

void createNumbers() {

	twopoints = loadBitmap(IMAGES_PATH "twopoints.bmp");
	ignored_color = rgb(0, 255, 0);
	numbers[0] = loadBitmap(IMAGES_PATH "0.bmp");
	numbers[1] = loadBitmap(IMAGES_PATH "1.bmp");
	numbers[2] = loadBitmap(IMAGES_PATH "2.bmp");
	numbers[3] = loadBitmap(IMAGES_PATH "3.bmp");
	numbers[4] = loadBitmap(IMAGES_PATH "4.bmp");
	numbers[5] = loadBitmap(IMAGES_PATH "5.bmp");
	numbers[6] = loadBitmap(IMAGES_PATH "6.bmp");
	numbers[7] = loadBitmap(IMAGES_PATH "7.bmp");
	numbers[8] = loadBitmap(IMAGES_PATH "8.bmp");
	numbers[9] = loadBitmap(IMAGES_PATH "9.bmp");
	bar = loadBitmap(IMAGES_PATH "bar.bmp");

}
void drawHighscores() {
	drawTop1();
	drawTop2();
	drawTop3();

}
