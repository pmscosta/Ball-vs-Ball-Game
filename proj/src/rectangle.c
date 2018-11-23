#include "rectangle.h"
#include "video_gr.h"
#include <stdlib.h>

Rectangle * createRectangle(int xi, int yi, int x_size, int y_size,
		unsigned long color) {

	Rectangle * rect = (Rectangle *) malloc(sizeof(Rectangle));

	if (rect == NULL)
		return NULL;

	rect->xi = xi;
	rect->yi = yi;
	rect->xf = xi + x_size;
	rect->yf = yi + y_size;
	rect->x_size = x_size;
	rect->y_size = y_size;
	rect->color = color;

	return rect;
}

void drawRectangle(Rectangle * rect) {

	vg_draw_empty_rect(rect->xi, rect->yi, rect->x_size, rect->y_size,
			rect->color);

}

void deleteRectangle(Rectangle * rect) {

	if (rect == NULL)
		return;
	free(rect);
	rect = NULL;
	return;
}
