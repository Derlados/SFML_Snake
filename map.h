#include <string>

const int MAP_HEIGHT = 12;
const int MAP_WIDTH = 22;

//Шаблон карты
std::string mapTemplate[MAP_HEIGHT] = {
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

//Используемая карта
std::string Map[MAP_HEIGHT];

//Создание карты которая будет использоваться
void createMap()
{
	int i, j;

	Map->clear();

	for (i = 0; i < MAP_HEIGHT; ++i)
		for (j = 0; j < MAP_WIDTH; ++j)
			Map[i].push_back(mapTemplate[i][j]);
}
