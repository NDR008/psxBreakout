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
#include "logic.h"


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
Box frame;
int framing=16;

PSXTimer TrialTimer;

int brickSpacing = 10;
int brickSizeY = 15;
int brickSizeX = 40;
int xOff = 40;
int yOff = 30;

Player player1;
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
Image bgSprite;
unsigned long cachedPadValue;

int pause=0;

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
	colourList[1]=createColor(200, 200, 50);
	colourList[2]=createColor( 50, 200, 50);
	colourList[3]=createColor(200,  50, 50);
	initialiseLevel();
	initialiseScene();
	ballSprite = createImage(img_ball);
	ballSprite = scaleImage(ballSprite, 50, 50);
	bgSprite = createImage(img_wall1);
	bgSprite = scaleImage(bgSprite, 200, 200);
	bgSprite = moveImage(bgSprite, 160, 120);
}

void initialiseGame() {
	lives=3;
	gameWon=0;
	level=0;
	initialiseLevel();
	initialiseScene();
}

void initialiseScene() {
	player1 = createPlayer(img_playerBar, 320/2, 210, 50, 10, framing+50/2, 320-framing-50/2);
	ballR = 0;
	bricksCounter = 0;
	motion = 0;
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
	
	FntPrint("    [ Bricks: %d   Lives: %d   Level: %d ]", bricksCounter, lives, level+1);

	if (!motion) { 
		ballX = player1.xPos;
		ballY = player1.yPos-player1.ySize/2-ballRad;
	}
	else{
		ballMotion();
	}
	ballSprite = rotImage(ballSprite, ballR);
	ballSprite = moveImage(ballSprite, ballX, ballY); 
	drawImage(ballSprite);
	drawImage(player1.image);
	drawImage(bgSprite);
}

void updateControls() {
	cachedPadValue = PadRead(0);
	int speed;
	int xPos;

	if((cachedPadValue & PADstart ) && (motion)) {
		pause = !pause;
	}

	if (pause) { return; }

	if(cachedPadValue & PADRdown) {
		speed = 3;
	}
	else {
		speed = 1;
	}

	if(cachedPadValue & PADLleft) {
		printf("\nxPos %d, speed %d, rangeL %d, rangeR, %d", player1.xPos, -speed, player1.rangeL, player1.rangeR);
		//xPos = saturate(player1.xPos, -speed, player1.rangeL, player1.rangeR);
		xPos = -speed;
		printf("\nxPos %d, speed %d, rangeL %d, rangeR, %d", player1.xPos, -speed, player1.rangeL, player1.rangeR);
		player1 = updatePlayer(player1, xPos);
		printf("  new xPos %d", player1.xPos);
	}

	if(cachedPadValue & PADLright) {
		printf("\nxPos %d, speed %d, rangeL %d, rangeR, %d", player1.xPos, +speed, player1.rangeL, player1.rangeR);
		//xPos = saturate(player1.xPos, +speed, player1.rangeL, player1.rangeR);
		xPos= +speed;
		printf("\nxPos %d, speed %d, rangeL %d, rangeR, %d", player1.xPos, +speed, player1.rangeL, player1.rangeR);
		player1 = updatePlayer(player1, xPos);
		printf("  new xPos %d", player1.xPos);
	}

	if((cachedPadValue & PADRright ) && !(motion))	{
		motion = 1;
	}
}

void ballMotion() {
	if (pause) { return; }
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
	hit = checkPlayerHit(player1, ballX, ballRad, ballY);
	//detect hitting the upper frame
	if (ballY-ballRad < framing) {
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
		case 'k': {
			lives--;
			initialiseScene();
			return;
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