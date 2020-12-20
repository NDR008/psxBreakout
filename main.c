#include "sys/types.h"
#include <libetc.h>
#include <libpad.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include "basics.h"
#include "timerz.h"

#define OT_LENGTH 1
#define PACKETMAX 300
#define __ramsize   0x00200000
#define __stacksize 0x00004000

void initialize();
void update();
void draw();

PSXTimer TrialTimer;

Box box;
#define cols 5
#define rows 3
int grid[rows][cols] = {
	{ 1, 1, 0, 0, 1 },
	{ 1, 0, 1, 0, 1 }, 
	{ 1, 0, 0, 1, 1 }};
Box bricks[cols][rows];
Box player;
Box frame;

int brickSpacing = 10;
int brickSizeY = 15;
int brickSizeX = 40;
int xOff = 30;
int yOff = 30;

int playerX = 100;
int playerY = 210;

int main() {
	initialize();

	while(1) {
		update(); // do the staff
		draw();	 // draw it
		TrialTimer=incTimer(TrialTimer);
		display(); // dump it to the screen
	}
}

void initialize() {
	initializeScreen();
	initializeDebugFont();
	PadInit(0);
	setBackgroundColor(createColor(55, 55, 55));
	box = createBox(createColor(0, 0, 255), xOff, yOff, xOff+brickSizeX, yOff+brickSizeY);
	for (int i=0; i<cols; i++){
		for (int j=0; j<rows; j++){
			if (1) {
				bricks[i][j] = createBox(createColor(50, 50, 255), xOff+(brickSizeX+brickSpacing)*i, yOff+(brickSizeY+brickSpacing)*j, 
											xOff+brickSizeX+(brickSizeX+brickSpacing)*i, yOff+brickSizeY+(brickSizeY+brickSpacing)*j);
			}

		}
	}
	player = createBox(createColor(200, 50, 50), playerX, playerY, playerX+100, playerY+10);
	frame = createBox(createColor(200, 155, 155), 15, 15, 300, playerY+20);
	TrialTimer = createTimer();
}

void update() {
}

void draw() {
	FntPrint("Breakout Game Time ");
	FntPrint("%02d : %02d : %d", TrialTimer.min, TrialTimer.sec, TrialTimer.vsync);
	printf("\n%02d : %02d : %d", TrialTimer.min, TrialTimer.sec, TrialTimer.vsync);
	for (int i=0; i<cols; i++){
		for (int j=0; j<rows; j++){
			if (grid[j][i]) {
				drawBox(bricks[i][j]);
			}

		}
	}
	drawBox(player);
	drawBox(frame);
	//drawLine(line);
}

