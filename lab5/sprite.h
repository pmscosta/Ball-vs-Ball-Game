/**
 *  @author Joao Cardoso (jcard@fe.up.pt) ????
 *  Added by pfs@fe.up.pt
 */

#ifndef _SPRITE_H_
#define _SPRITE_H_

/** @defgroup sprite Sprite
 * @{
 *
 * Sprite related functions
 */

/** A Sprite is an "object" that contains all needed information to
 * create, animate, and destroy a pixmap.  The functions assume that
 * the background is BLACK and they take into account collision with
 * other graphical objects or the screen limits.
 */
typedef struct {
	int x, y; /**< current sprite position */
	int width, height; /**< sprite dimensions */
	int xspeed, yspeed; /**< current speeds in the x and y direction */
	char *map; /**< the sprite pixmap (use read_xpm()) */
} Sprite;

/** Creates a new sprite with pixmap "pic", with specified
 * position (within the screen limits) and speed;
 * Does not draw the sprite on the screen
 * Returns NULL on invalid pixmap.
 */
Sprite *create_sprite(char *pic[], int x, int y, int xspeed, int yspeed);

/** The "fig" sprite is erased from memory
 * and used resources released.
 */
void destroy_sprite(Sprite *sp);

int animate_sprite(Sprite *sp);

int draw_sprite(Sprite *sp);


/** @} end of sprite */

#endif
