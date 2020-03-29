#include "map.h"


string mapTemplate[MAP_HEIGHT] = {
	"0000000000000000000000",
	"0                    0",
	"0                    0",
	"0                    0",
	"0                    0",
	"0                    0",
	"0                    0",
	"0                    0",
	"0                    0",
	"0                    0",
	"0                    0",
	"0000000000000000000000"
};

string Map[MAP_HEIGHT];

//—оздание карты котора€ будет использоватьс€
void createMap()
{
	int i, j;

	Map->clear();

	for (i = 0; i < MAP_HEIGHT; ++i)
		for (j = 0; j < MAP_WIDTH; ++j)
			Map[i].push_back(mapTemplate[i][j]);
}