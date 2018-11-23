#include <stdlib.h>
#include "sprite.h"
#include "video_gr.h"
#include "video_test.h"

/** Creates a new sprite with pixmap "pic", with specified
 * position (within the screen limits) and speed;
 * Does not draw the sprite on the screen
 * Returns NULL on invalid pixmap.
 */
Sprite *create_sprite(char *pic[], int x, int y, int xspeed, int yspeed) {

	Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
	if (sp == NULL)
		return NULL;

// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if (sp->map == NULL) {
		free(sp);
		return NULL;
	}

	sp->x = x;
	sp->y = y;
	sp->xspeed = xspeed;
	sp->yspeed = yspeed;

	return sp;
}

void destroy_sprite(Sprite *sp) {
	if (sp == NULL)
		return;
	if (sp->map)
		free(sp->map);
	free(sp);
	sp = NULL; // XXX: pointer is passed by value
// should do this @ the caller
}

int draw_sprite(Sprite *sp) {

	return vg_draw_pixmap(sp->map, sp->x, sp->y, sp->width, sp->height);

}

int animate_sprite(Sprite * sp) {

	sp->x += sp->xspeed;
	sp->y += sp->yspeed;
	return draw_sprite(sp);



}
