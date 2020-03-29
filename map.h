#include <string>

const int MAP_HEIGHT = 12;
const int MAP_WIDTH = 22;

//������ �����
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

//������������ �����
std::string Map[MAP_HEIGHT];

//�������� ����� ������� ����� ��������������
void createMap()
{
	int i, j;
	for (i = 0; i < MAP_HEIGHT; ++i)
		for (j = 0; j < MAP_WIDTH; ++j)
			Map[i][j] = mapTemplate[i][j];
}