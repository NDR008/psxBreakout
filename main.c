#include <sys/types.h>
#include <libetc.h>
#include <libpad.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include "images.h"
#include "basics.h"
#include "timerz.h"


#define OT_LENGTH 1
#define PACKETMAX 300
#define __ramsize   0x00200000
#define __stacksize 0x00004000

void initialize();
void updateControls();
void draw();

#define cols 5
#define rows 3

int grid[rows][cols] = {
	{ 1, 1, 0, 0, 1 },
	{ 1, 0, 1, 0, 1 }, 
	{ 1, 0, 0, 1, 1 }};
Box bricks[cols][rows];
Box player;
Box frame;
int framing=15;

PSXTimer TrialTimer;

int brickSpacing = 10;
int brickSizeY = 15;
int brickSizeX = 40;
int xOff = 30;
int yOff = 30;

int playerSize = 50;
int playerX = 0;
int playerY = 210;

Image ps1;

unsigned long cachedPadValue;

int main() {
	initialize();

	while(1) {
		TrialTimer=incTimer(TrialTimer);
		updateControls(); // do the staff
		clearDisplay();
		draw();	 // draw it
		display(); // dump it to the screen
	}
}

void initialize() {
	initializeScreen();
	PadInit(0);	
	setBackgroundColor(createColor(30, 30, 30));
	initializeDebugFont();

	
	ps1 = createImage(img_ball);
	playerX = (320-playerSize)/2;


	
	for (int i=0; i<cols; i++){
		for (int j=0; j<rows; j++){
			if (1) {
				bricks[i][j] = createBox(createColor(50, 50, 255), xOff+(brickSizeX+brickSpacing)*i, yOff+(brickSizeY+brickSpacing)*j, 
											xOff+brickSizeX+(brickSizeX+brickSpacing)*i, yOff+brickSizeY+(brickSizeY+brickSpacing)*j);
			}

		}
	}
	player = createBox(createColor(200, 50, 50), playerX, playerY, playerX+50, playerY+10);
	frame = createBox(createColor(200, 155, 155), framing, framing, 320-framing, 240-framing);
	TrialTimer = createTimer();
}

void draw() {
	FntPrint("Breakout Game Time ");
	FntPrint("%02d : %02d : %d", TrialTimer.min, TrialTimer.sec, TrialTimer.vsync);
	printf("\n%02d : %02d : %d", TrialTimer.min, TrialTimer.sec, TrialTimer.vsync);
	printf("\nthis works");

	for (int i=0; i<cols; i++){
		for (int j=0; j<rows; j++){
			if (grid[j][i]) {
				drawBox(bricks[i][j]);
			}

		}
	}
	player = moveBox(player, playerX, playerY);
	ps1 = moveImage(ps1, playerX+playerSize/2-32/2, playerY-32);
	drawImage(ps1);
	drawBox(player);
	drawBox(frame);
}

void updateControls() {
	cachedPadValue = PadRead(0);
	printf("\nbut this not? or?  %d", cachedPadValue);

	if(cachedPadValue & PADLleft) {
		if (framing < playerX) { 
		//if(1){
			playerX -= 10;
		}
	}

	if(cachedPadValue & PADLright) {
		if (320-framing-playerSize > playerX) { 
		//if(1){
			playerX += 10;
		}
	}
}
