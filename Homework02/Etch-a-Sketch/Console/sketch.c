#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <poll.h>
#include <termios.h>
#include "sketch.h"

/**********************************************
 * Created by Robert Fendricks
 *
 * Code borrowed from the internet is documented where applicable
 **********************************************/


/*
 * clearGrid will set all of the points on the board to zero
 * except for the point where the sketcher is currently at. This
 * gets set to 2.
 */

void clearGrid(int (*board)[width], point * p){
	int i, j;
	for(i=0; i<height; i++){
		for(j=0; j<width; j++){
			board[i][j] = 0;
		}
	}
	board[p->y][p->x] = 2;
}

/*
 * This prints the current board state to the terminal.
 * a 2 in the board array indicates where the cursor currently is
 * a 1 indicates where the cursor has been
 * a 0 represents where the cursor hasn't yet been.
 */

void drawGrid(int (*board)[width]){
	int i, j;
	printf("\e[1;1H\e[2J"); //clear the terminal's screen
	for(i=0; i<height; i++){
		for(j=0; j<width ; j++){
			if(board[i][j] == 2){
				printf("O");
			}
			else if(board[i][j]==1){
				printf("X");
			}
			else{
				printf(".");
			}
		}
		printf("\n");
	}
}

void goUp(point * p, int (*board)[width]){
	if(p->y ==0){
		//We are at the top of the board, so do nothing
	}
	else{
		board[p->y][p->x] = 1;
		p->y --; //Moves up
		board[p->y][p->x] = 2;
	}
}

void goLeft(point * p, int (*board)[width]){
	if(p->x == 0){
		//We are at the leftmost part of the board, so do nothing
	}
	else{	
		board[p->y][p->x] = 1;
		p->x --; //moves left
		board[p->y][p->x] = 2;
	}
}

void goDown(point * p, int (*board)[width]){
	if(p->y == height-1){
		//WE are at the bottom of the board, so do nothing
		}
	else{
		board[p->y][p->x] = 1;
		p->y ++; //move down
		board[p->y][p->x] = 2;
	}

}

void goRight(point * p, int (*board)[width]){
	if(p->x == width-1){
		//We are at the rightmost part of the board, so do nothing
		}
	else{
		board[p->y][p->x] = 1;
		p->x ++; //move right
		board[p->y][p->x] = 2;
	}
}

/*
 * This controls where the cursor moves to next.
 * Press w to go up
 * Press a to go left
 * Press s to go down
 * Press d to go right
 */
void control(unsigned char c, point * p, int (*board)[width]){
	if(c == 'w'){
		goUp(p, board);
	}
	if(c == 'a'){
		goLeft(p, board);	
	}
	if(c == 's'){
		goDown(p,board);		
	}

	if(c == 'd'){
		goRight(p,board);		
	}
	//9 corresponds to Tab
	if(c == 9){
		clearGrid(board, p);
	}
}



/**
 * I found this code on the internet at:
 * https://groups.google.com/forum/#!topic/comp.os.linux.development/09C0_1pA3sc
 * What this code does is it changes the behavior of the terminal so that instead of
 * printing characters straight to the screen and waiting for the user to hit enter,
 * it instead immediately gets fed to this program.
 */


unsigned char kbhit(){
 	struct termios argin, argout;
	unsigned char ch;

	tcgetattr(0, &argin);
	argout = argin;
	argout.c_lflag &= ~(ICANON);
	argout.c_iflag &= ~(ICRNL);
	argout.c_oflag &= ~(OPOST);
	argout.c_cc[VMIN] = 1;
	argout.c_cc[VTIME] = 0;
	tcsetattr(0, TCSADRAIN, &argout);
	read(0, &ch, 1);
	tcsetattr(0,TCSADRAIN, &argin);
	return ch;
}

int main(int argc, char** argv){
	unsigned char command; //This will track what keys are pressed during use

	if(argc != 3){
		printf("To use this program, please specify the height and width of the grid you want to draw on.\n");
		exit(-1);
	}
	//height and width are global variables defined in the header
	height = atoi(argv[1]);
	width = atoi(argv[2]);


	int board[height][width]; //This will keep track of the state of the board
	point p = {0,0}; //point is where the sketcher is currently at

	clearGrid(board, &p);
	while(1){
		drawGrid(board);
		control(kbhit(), &p, board);
		fflush(stdout);
	}
}
