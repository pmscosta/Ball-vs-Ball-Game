#include "pixmap.h"
#include<stdio.h>
#include <string.h>

//intended to search for a name but c d

char **getPixMap(char * name) {

	if (strcmp(name, "ship") == 0) {
		return ship;
	} else if (strcmp(name, "shot") == 0) {
		return shot;
	} else if (strcmp(name, "penguin") == 0) {
		return penguin;
	} else if (strcmp(name, "thing") == 0) {
		return thing;
	} else if (strcmp(name, "cross") == 0) {
		return cross;
	}

	return NULL;

}
