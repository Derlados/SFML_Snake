#ifndef MAP_H
#define MAP_H

#include <string>

using namespace std;

const int MAP_HEIGHT = 12;
const int MAP_WIDTH = 22;

//Шаблон карты
extern string mapTemplate[MAP_HEIGHT];

//Используемая карта
extern string Map[MAP_HEIGHT];

void createMap();

#endif
