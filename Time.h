/*
 * Time.h
 *
 *  Created on: 17 mars 2015
 *      Author: Gabriel
 */

#ifndef TIME_H_
#define TIME_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct Time_s {
	double timeSinceStart;
	float deltaTime;
} Time_s;

Time_s Time;

void time_init();
void time_update();

#endif /* TIME_H_ */
