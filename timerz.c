/*
 * timerz.h
 *  Re-Created on: Dec 19, 2020
 *      Author: NDR008
 * 
 *  Very basic library to count time based on vsync
 */

#include "sys/types.h"
#include "timerz.h"
#include <libetc.h>

PSXTimer createTimer(){
	PSXTimer timer;
    if(GetVideoMode()){
		timer.factor =50;
	} else {
		timer.factor =60;
	}
	return timer;
}

PSXTimer incTimer(PSXTimer timer){
	timer.vsync++;
	if (timer.vsync >= timer.factor) {
		timer.vsync = 0;
		timer.sec++;
	}
	if (timer.sec >= 60){
		timer.min++;
		timer.sec = 0;
	}
	return timer;
}