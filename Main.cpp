#include <SFML/Graphics.hpp>
#include "map.h"
#include <ctime>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;

struct AllBody
{
    Sprite head;
    Sprite bodyFragm;
    Sprite tail;
    Sprite angleFragm;
};

class Snake
{
private:
    const int CORRECT_ORIGIN = 16; // корректировка центра
    vector<pair<int, int>> bodyCoord;
    Image image;
    Texture texture;
    AllBody body;
    int angleHead; // для временного удержания угла поворота головы
    bool endGame = false;

    pair<int, bool> angle(pair<int, int> body1, pair<int, int> body2, pair<int, int> center)
    {

        int k = 0;

        if (center.first < body1.first || center.first < body2.first)
            k = 180;

        if ((body1.first < body2.first && body1.second > body2.second) || (body1.first > body2.first&& body1.second < body2.second))
        {
            return make_pair(0 + k, true);
        }
        else if ((body1.first < body2.first && body1.second < body2.second) || (body1.first > body2.first&& body1.second > body2.second))
        {
            return make_pair(90 + k, true);
        }

       if (body1.first == body2.first)
            return make_pair(0, false);
        return make_pair(90, false);
    }

    float angleTail(pair<int, int> body, pair<int, int> tail)
    {
        if (tail.first < body.first)
            return 90;
        if (tail.first > body.first)
            return -90;
        if (tail.second > body.second)
            return 180;
        return 0;
    }

public:
    enum {left, right, down, up} dir = right, temp_dir;

    bool getEndGame()
    {
        return endGame;
    }

    void setEndGame()
    {
        endGame =false;
    }

    int getSize()
    {
        return bodyCoord.size();
    }

    Snake(RenderWindow& window)
    {
        bodyCoord.push_back(make_pair(5, 7)); bodyCoord.push_back(make_pair(5, 6)); bodyCoord.push_back(make_pair(5, 5)); bodyCoord.push_back(make_pair(5, 4)); bodyCoord.push_back(make_pair(5, 3));
        Map[5][7] = 'S'; Map[5][6] = 'S'; Map[5][5] = 'S'; Map[5][4] = 'S'; Map[5][3] = 'S';

        // Загружаем всё тело 
        image.loadFromFile("images/Snake.png");
        texture.loadFromImage(image);
        
        body.head.setTexture(texture);
        body.bodyFragm.setTexture(texture);
        body.tail.setTexture(texture);
        body.angleFragm.setTexture(texture);
        
        body.head.setTextureRect(IntRect(64, 0, 32, 32));
        body.bodyFragm.setTextureRect(IntRect(32, 0, 32, 32));
        body.tail.setTextureRect(IntRect(0, 0, 32, 32));
        body.angleFragm.setTextureRect(IntRect(96, 0, 32, 32));

        // центр спрайта - цент самого спрайта
        body.head.setOrigin(CORRECT_ORIGIN, CORRECT_ORIGIN);
        body.bodyFragm.setOrigin(CORRECT_ORIGIN, CORRECT_ORIGIN);
        body.tail.setOrigin(CORRECT_ORIGIN, CORRECT_ORIGIN);
        body.angleFragm.setOrigin(CORRECT_ORIGIN, CORRECT_ORIGIN);

        void update();
    }

    // считывания направление 
    void control(Event& event)
    {
        if (Keyboard::isKeyPressed(Keyboard::W) && dir != down)
            temp_dir = up;
        else if (Keyboard::isKeyPressed(Keyboard::S) && dir != up)
            temp_dir = down;
        else if (Keyboard::isKeyPressed(Keyboard::D) && dir != left)
            temp_dir = right;
        else if (Keyboard::isKeyPressed(Keyboard::A) && dir != right)
            temp_dir = left;
    } 

    // смена координат змейки
    void update()
    {
        int size = bodyCoord.size();

        for (int i = 0; i < size; ++i)
            Map[bodyCoord[i].first][bodyCoord[i].second] = ' ';

        dir = temp_dir;

        pair <int, int> temp_head;
        switch (dir)
        {
        case up:
            angleHead = -90;
            temp_head = make_pair(bodyCoord[0].first - 1, bodyCoord[0].second);
            break;
        case down:
            angleHead = 90;
            temp_head = make_pair(bodyCoord[0].first + 1, bodyCoord[0].second);
            break;
        case right:
            angleHead = 0;
            temp_head = make_pair(bodyCoord[0].first, bodyCoord[0].second + 1);
            break;
        case left:
            angleHead = 180;
            temp_head = make_pair(bodyCoord[0].first, bodyCoord[0].second - 1);
            break;
        }


        pair<int, int> tail = bodyCoord[bodyCoord.size() - 1]; //сохранение хвоста на случай поглощения еды и последующего добавления этого хвоста
        for (int i = bodyCoord.size() - 1; i > 0; --i)
            bodyCoord[i] = bodyCoord[i - 1];
        bodyCoord[0] = temp_head;

        if (Map[temp_head.first][temp_head.second] == 'X')
        {
            Map[temp_head.first][temp_head.second] = ' ';
            bodyCoord.push_back(tail);
        }

        if (Map[temp_head.first][temp_head.second] == '0')
        {
            endGame = true;
            body.head.setTextureRect(IntRect(160, 0, 32, 32));
            body.head.setOrigin(CORRECT_ORIGIN, CORRECT_ORIGIN);
            return;
        }

        for (int i = 0; i < size; ++i)
        {
            if (Map[bodyCoord[i].first][bodyCoord[i].second] == 'S')
            {
                endGame = true;
                body.head.setTextureRect(IntRect(160, 0, 32, 32));
                body.head.setOrigin(CORRECT_ORIGIN, CORRECT_ORIGIN);
                return;
            }
            Map[bodyCoord[i].first][bodyCoord[i].second] = 'S';    
        }

    }

    //прорисовка змейки  
    void Paint(RenderWindow&  window)
    {
       
        int size = bodyCoord.size() - 1, i;
        pair<int, bool> temp_angle;

        for (i = 1; i < size; ++i)
        {
            temp_angle = angle(bodyCoord[i + 1], bodyCoord[i - 1], bodyCoord[i]);
            
            if (temp_angle.second)
            {
                body.angleFragm.rotate(temp_angle.first);
                body.angleFragm.setPosition(bodyCoord[i].second * 32 + CORRECT_ORIGIN, bodyCoord[i].first * 32 + CORRECT_ORIGIN);
                window.draw(body.angleFragm);
                body.angleFragm.rotate(-temp_angle.first);
            }
            else
            {
                body.bodyFragm.rotate(temp_angle.first);
                body.bodyFragm.setPosition(bodyCoord[i].second * 32 + CORRECT_ORIGIN, bodyCoord[i].first * 32 + CORRECT_ORIGIN);
                window.draw(body.bodyFragm);
                body.bodyFragm.rotate(-temp_angle.first);
            }
        }

        float temp = angleTail(bodyCoord[size - 1], bodyCoord[size]);
        body.tail.rotate(temp);
        body.tail.setPosition(bodyCoord[size].second * 32 + CORRECT_ORIGIN, bodyCoord[size].first * 32 + CORRECT_ORIGIN);
        window.draw(body.tail);
        body.tail.rotate(-temp);

        body.head.rotate(angleHead);
        body.head.setPosition(bodyCoord[0].second * 32 + 16, bodyCoord[0].first * 32 + 16);
        window.draw(body.head);
        body.head.rotate(-angleHead);
    }
};

class Food
{
private:
    int x, y, randVal, empty = MAP_HEIGHT * MAP_WIDTH - 3;;

    Image image;
    Texture texture;
    Sprite food;

public:
    void check()
    {
        if (Map[x][y] != 'X')
            spawnFood();
    }

    Food()
    {
        spawnFood();

        //Загрузка еды
        image.loadFromFile("images/Snake.png");
        texture.loadFromImage(image);
        food.setTexture(texture);
        food.setTextureRect(IntRect(128, 0, 32, 32));
    }

    void spawnFood()
    {
        int i, j;
        srand(unsigned(time(NULL)));
        --empty;
        randVal = rand() % empty;

        if (Map[randVal % (MAP_HEIGHT - 2) + 1][randVal % (MAP_WIDTH - 2) + 1] == ' ')
        {
            x = randVal % (MAP_HEIGHT - 2) + 1;
            y = randVal % (MAP_WIDTH - 2) + 1;
            Map[x][y] = 'X';
            return;
        }

        for (i = 0; i < MAP_HEIGHT; ++i)
            for (j = 0; j < MAP_WIDTH; ++j)
                if (Map[i][j] == ' ')
                {
                    --randVal;
                    if (randVal == 0)
                    {
                        x = i;
                        y = j;
                        Map[x][y] = 'X';
                    }

                }
    }

    void Paint(RenderWindow& window)
    {
        food.setPosition(y * 32, x * 32);
        window.draw(food);
    }
};

void menu(RenderWindow& window)
{
    Texture startText, endText;
    startText.loadFromFile("images/start.png");
    endText.loadFromFile("images/end.png");
    Sprite startSprite(startText), endSprite(endText);

    bool isMenu = true;
    char choose = 0;
    startSprite.setPosition(108, 90);
    endSprite.setPosition(227, 214);

    while (isMenu)
    {
        startSprite.setColor(Color::White);
        endSprite.setColor(Color::White);
        window.clear(Color(255, 31, 31));
        choose = 0;

        if (IntRect(108, 80, 487, 66).contains(Mouse::getPosition(window)))
        {
            startSprite.setColor(Color::Blue);
            choose = 1;
        }
        if (IntRect(227, 204, 250, 55).contains(Mouse::getPosition(window)))
        {
            endSprite.setColor(Color::Blue);
            choose = 2;
        }
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape) || (Mouse::isButtonPressed(Mouse::Left) && choose == 2))
                window.close();

            if (Mouse::isButtonPressed(Mouse::Left) && choose == 1)
                isMenu = false;
        }
        

        window.draw(startSprite);
        window.draw(endSprite);

        window.display();
    }
}

int main()
{    
    int i, j;
    bool start = false;

    //Подгрузка карты
    Image field_map;
    field_map.loadFromFile("images/map.png");
    Texture texture_map;
    texture_map.loadFromImage(field_map);
    Sprite sprite_map;
    sprite_map.setTexture(texture_map);
    createMap();

    sf::RenderWindow window(sf::VideoMode(704, 384), "Snake!");
    menu(window);

    Texture beginBack_T;
    beginBack_T.loadFromFile("images/BeginBack.png");
    Sprite beginBack(beginBack_T);
    beginBack.setPosition(33, 33);


    Font font;
    font.loadFromFile("CyrilicOld.TTF");
    Text restart("", font, 20), begin("", font, 20);
    begin.setFillColor(Color::White);
    begin.setString("Нажмите w,s,a или d\n   для начала игры");
    begin.setPosition(40, 40);
    restart.setFillColor(Color::White);
    restart.setPosition(40, 40);

    //Таймер
    Clock clock;
    // Змейка
    Snake *snake = new Snake(window);
    Food *food = new Food();

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
                window.close();

            if (!snake->getEndGame() && (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::D)))
            {
                start = true;
                snake->control(event);
            }

            if (snake->getEndGame() && Keyboard::isKeyPressed(Keyboard::Tab))
            {
                snake->setEndGame();
                start = false;
                createMap(); //Восстановление карты
                
                //Пересоздание элементов
                delete snake;
                delete food;
                snake = new Snake(window);
                food = new Food();  
            }
        }

        if (snake->getEndGame())
            continue;

        //Смена позиции змейки каждые 200 мс
        if (start && clock.getElapsedTime().asMilliseconds() > 200)
        {
            snake->update();
            clock.restart();
        }

        window.clear();

        //Прорисовка карты
        for (i = 0; i < MAP_HEIGHT; ++i)
            for (j = 0; j < MAP_WIDTH; ++j)
            {
                if (Map[i][j] == '0') sprite_map.setTextureRect(IntRect(64 , 0, 32, 32));
                else 
                    if ((j + i) % 2)
                        sprite_map.setTextureRect(IntRect(0, 0, 32, 32));
                    else
                        sprite_map.setTextureRect(IntRect(32, 0, 32, 32));
            
                sprite_map.setPosition(j * 32, i * 32);
                window.draw(sprite_map);
            }

        food->check(); // проверить есть ли еда на карте
        snake->Paint(window);
        food->Paint(window);

        if (!start)
        {
            window.draw(beginBack);
            window.draw(begin);
        }

        if (snake->getEndGame())
        {
            start = false;
            window.draw(beginBack);
            ostringstream size;
            size << snake->getSize();
            restart.setString("Длина змейки: " + size.str() + "\nESC - Выход\nTab - Рестарт");
            window.draw(restart);
        }

        window.display();
    }

    return 0;
}