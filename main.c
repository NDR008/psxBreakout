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
void ballMotion();
void initialiseScene();
void initialiseLevel();
void initialiseGame();
void drawGameOver();
void drawWon();

#define cols 5
#define rows 3

int gridMaster[3][rows][cols] = {
	{{ 0, 0, 0, 0, 0 },
	 { 0, 0, 1, 0, 0 }, 
	 { 3, 2, 0, 2, 3 }},
	 {{ 2, 2, 0, 0, 2 },
	  { 2, 0, 2, 0, 2 }, 
	  { 1, 0, 0, 1, 1 }},
	 {{ 1, 3, 3, 3, 1 },
	  { 1, 0, 0, 0, 1 }, 
	  { 3, 3, 3, 3, 3 }}
};
int grid[rows][cols];

int maxLevel = 2;
int level = 0;
	
Box bricks[cols][rows];
Box player;
Box frame;
int framing=15;

PSXTimer TrialTimer;

int brickSpacing = 10;
int brickSizeY = 15;
int brickSizeX = 40;
int xOff = 40;
int yOff = 30;

int playerSize = 50;
int playerX = 0;
int playerY = 210;
int gameWon = 0;
int lives = 3;

Color colourList[4];

int motion = 0;
int ballX;
int ballY;
int ballR = 0;
int velX=1;
int velY=2;
int ballRad=16/2;
char hit = 'z'; 
int bricksCounter=0;
int bricksLeft=0;

Image ballSprite;
unsigned long cachedPadValue;

int main() {
	initialize();

	while(1) {
		//printf("\nballX: %d, playerX: %d", ballX, playerX);
		TrialTimer=incTimer(TrialTimer);
		updateControls(); // do the staff
		clearDisplay();
		if (gameWon) {drawGameOver();}
		else if (lives==-1) {drawGameOver();}
		else draw();
		display(); // dump it to the screen
	}
}

void initialize() {
	initializeScreen();
	PadInit(0);	
	setBackgroundColor(createColor(30, 30, 30));
	initializeDebugFont();
	colourList[0]=createColor(150, 150, 50); //player colour
	colourList[1]=createColor(50, 50, 255);
	colourList[2]=createColor(50, 255, 50);
	colourList[3]=createColor(255, 50, 50);
	initialiseLevel();
	initialiseScene();
	ballSprite = createImage(img_ball);
	ballSprite = scaleImage(ballSprite, 50, 50);
}

void initialiseGame() {
	lives=3;
	gameWon=0;
	level=0;
	initialiseLevel();
	initialiseScene();
}

void initialiseScene() {
	playerX = (320-playerSize)/2;
	ballR = 0;
	bricksCounter = 0;
	motion = 0;
	player = createBox(colourList[0], playerX, playerY, playerX+50, playerY+10);
	frame = createBox(createColor(200, 155, 155), framing, framing, 320-framing, 240-framing);
	TrialTimer = createTimer();
}

void draw() {
	bricksCounter = 0;
	for (int i=0; i<cols; i++){
		for (int j=0; j<rows; j++){
			if (grid[j][i]>0) {
				bricks[i][j] = createBox(colourList[grid[j][i]], xOff+(brickSizeX+brickSpacing)*i, yOff+(brickSizeY+brickSpacing)*j, 
							xOff+brickSizeX+(brickSizeX+brickSpacing)*i, yOff+brickSizeY+(brickSizeY+brickSpacing)*j);
				drawBox(bricks[i][j]);
				bricksCounter++;
			}
		}
	}

	FntPrint("Bricks: %d   Lives: %d   Level: %d", bricksCounter, lives, level+1);

	player = moveBox(player, playerX, playerY);
	if (!motion) { 
		ballX = playerX+playerSize/2;
		ballY = playerY-ballRad;
	}
	else{
		ballMotion();
	}
	ballSprite = rotImage(ballSprite, ballR);
	ballSprite = moveImage(ballSprite, ballX, ballY); 
	drawImage(ballSprite);
	drawBox(player);
	drawBox(frame);
}

void updateControls() {
	cachedPadValue = PadRead(0);
	if(cachedPadValue & PADLleft) {
		if (framing < playerX) { 
			playerX -= 5;
		}
	}

	if(cachedPadValue & PADLright) {
		if (320-framing-playerSize > playerX) { 
			playerX += 5;
		}
	}

	if((cachedPadValue & PADstart) && !(motion))
	{
		motion = 1;
	}
}

void ballMotion() {
	for (int i=0; i<cols; i++){
		for (int j=0; j<rows; j++){
			if (grid[j][i]>0) {

				int leftX, rightX, upperY, lowerY = 0;
				leftX = xOff+(brickSizeX+brickSpacing)*i;
				rightX = xOff+brickSizeX+(brickSizeX+brickSpacing)*i;
				upperY = yOff+(brickSizeY+brickSpacing)*j;
				lowerY = yOff+brickSizeY+(brickSizeY+brickSpacing)*j;

				//Hit brick from above
				if  ((ballY + ballRad >= upperY && ballY + ballRad <= upperY+ballRad) && ( ballX >= leftX && ballX <= rightX) &&  velY>0 ){
					hit = 'd';
					if(grid[j][i]==1){bricksCounter--;}
					grid[j][i]--;
				}
				//Hit brick from below
				else if ((ballY - ballRad <= lowerY && ballY - ballRad >= lowerY-ballRad) && ( ballX >= leftX && ballX <= rightX) &&  velY<0 ){
					hit = 'u';
					if(grid[j][i]==1){bricksCounter--;}
					grid[j][i]--;
				}
				//Hit brick from left
				else if ((ballX + ballRad >= leftX && ballX + ballRad <= leftX+ballRad ) && ( ballY <= lowerY && ballY >= upperY) &&  velX>0){
					hit = 'r';
					if(grid[j][i]==1){bricksCounter--;}
					grid[j][i]--;
				}
				//Hit brick from right
				else if ((ballX - ballRad <= rightX && ballX - ballRad >= rightX-ballRad ) && ( ballY <= lowerY && ballY >= upperY) && velX<0 ){
					hit = 'l';
					if(grid[j][i]==1){bricksCounter--;}
					grid[j][i]--;
				}
			}
		}
	}
	
	if (bricksCounter == 0){
		if(level<maxLevel){ 
			level++;
			hit = 'z';
			initialiseLevel();
			initialiseScene();
			return;
		}
		else if(level==maxLevel){
			gameWon = 1;
			return;
		}
	}

	//detect hitting the bottom limit
	if ((ballY+ballRad >= playerY) && (ballY+ballRad <= playerY+10)) {
		if ((ballX-ballRad < playerX+playerSize) && (ballX+ballRad>playerX) )
		{
			hit = 'd';
		}
		else {
			hit = 'z';
			lives--;
			initialiseScene();
			return;
		}
	}
	//detect hitting the upper frame
	else if (ballY-ballRad < framing) {
		hit = 'u';
		//velY = abs(velY);
	}
	if (ballX+ballRad> 320-framing) { 
		//velX = -1*abs(velX);
		hit = 'r';
	}
	else if (ballX-ballRad < framing) { 
		//velX = abs(velX);
		hit = 'l';
	}

	switch (hit)
	{
		case 'u': {
			velY = abs(velY);
			break;
		}
		case 'd': {
			velY = -1*abs(velY);
			break;
		}
		case 'l': {
			velX = abs(velX);
			break;
		}
		case 'r': {
			velX = -1*abs(velX);
			break;
		}
	}

	hit = 'z';
	ballY = ballY + velY;
	ballX = ballX + velX;
	ballR += 2*velY*velX;
}

void drawGameOver(){
	initialiseGame();
	initialiseScene();
	return;
}

void drawWon(){
	initialiseGame();
	initialiseScene();
	return;
}

void initialiseLevel(){
	for (int i=0; i<cols; i++){
		for (int j=0; j<rows; j++){
			grid[j][i] = gridMaster[level][j][i];
		}
	}
}