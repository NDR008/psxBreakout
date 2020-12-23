#pragma once

#pragma once

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>

typedef struct Player {
	Image image;
	int xPos;   //center
    int yPos;   //center
    int xSize;
    int ySize;
    int paddleL; //coverage
    int paddleR;
    int rangeL; //range of motion
    int rangeR;
} Player;

int saturate(int parameter, int increment, int min, int max);

Player createPlayer(unsigned char imageData[], int xPos, int yPos, int xSize, int ySize, int boundL, int boundR);

Player updatePlayer(Player localPlayer, int xPosNew);

char checkPlayerHit(Player player1, int ballX, int ballRad, int ballY);