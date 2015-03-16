/*
 * Game.h
 *
 *  Created on: 1 mars 2015
 *      Author: Gabriel
 */

#ifndef GAME_H_
#define GAME_H_

#include "Scene.h"


typedef struct Game_s{
char* name;
Scene* scenes;
int count_scenes;
}Game_s;

extern Game_s Game;

void game_launch();
//int game_load_scenes();	//Charger la scene depuis un fichier
int game_load_scene(char*);

#endif /* GAME_H_ */
