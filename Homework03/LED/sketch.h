#include <poll.h>
#include "gpio-utils.h"

typedef struct {
	int x;
	int y;
} point;

int height, width; //height and width are specified here for prototyping purposes
void drawGrid(int (*board)[width], int file);
void clearGrid(int (*board)[width], point * p);
int control(struct pollfd * fdset, int * switchPorts, point * p, int (*board)[width]);
unsigned char kbhit();



