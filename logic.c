#include "basics.h"
#include "logic.h"


int saturate(int parameter, int increment, int min, int max){
    int result = parameter+increment;
    if (result > max) {return max;}
    else if (result < max) {return min;}
    return result;
}

Player createPlayer(unsigned char imageData[], int xPos, int yPos, int xSize, int ySize, int rangeL, int rangeR){
    printf("\nRangeL received is... %d\n", rangeL);
    Player localPlayer;
    localPlayer.xPos = xPos;
    localPlayer.yPos = yPos;
    localPlayer.xSize = xSize;
    localPlayer.ySize = ySize;
    localPlayer.paddleL = xPos-xSize/2;
    localPlayer.paddleR = xPos+xSize/2;
    localPlayer.rangeL = rangeL;
    localPlayer.rangeR = rangeR;
    localPlayer.image = createImage(imageData);
    localPlayer.image = moveImage(localPlayer.image, localPlayer.xPos, localPlayer.yPos);
    return localPlayer;
}

Player updatePlayer(Player localPlayer, int xPosNew){ // let us change to delta input! Then check saturation....
    int delta =  xPosNew - localPlayer.xPos;
    localPlayer.xPos = xPosNew;
    localPlayer.paddleL += delta;
    localPlayer.paddleR += delta;
    localPlayer.image = moveImage(localPlayer.image, localPlayer.xPos, localPlayer.yPos);
    return localPlayer;
}

char checkPlayerHit(Player player1, int ballX, int ballRad, int ballY){
    if (ballY-ballRad+3 >= player1.yPos-player1.ySize){// if ball is touching or up to 10 pixels into paddle...
        if ((ballX+ballRad < player1.paddleL) || (ballX-ballRad > player1.paddleR)){ // ball is not on player1
            return 'k';
        }
        else
        {
            return 'd';
        }
        
    }
    else return 'z';
}