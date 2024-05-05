#include <gl/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>              // Подключение стандартных библиотечных функций
#include <iostream>
#include <vector>
#include <string>
#include <malloc.h>              // Подключение функций управления памятью
#include <windows.h>             // Подключение функций для работы с Windows
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION // Макрос для включения реализации в stb_image.h
#include "stb-master/stb_image.h" // Подключение stb_image.h для загрузки изображений

// Константы для состояний движения игрока
const int go = 0;
const int up = 1;
const int statica = 2;

// Размеры экрана
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 678;

// Размер квадратного тайла
const int TILE_SIZE = 50;

// Структура для кнопки меню
typedef struct Button
{
    char name[50]; // Название кнопки
    float vert[8]; // Вершины кнопки
    char isHover, isDown; // Флаги состояния наведения и нажатия на кнопку
    float buffer[500]; // Буфер для текстуры кнопки
    int num_quads; // Количество квадратов в текстуре кнопки
    float textPosX, textPosY, textS; // Положение текста на кнопке и его масштаб
};

// Структура для меню
struct Menu
{
    bool isActive = false; // Флаг активности меню
};
// Глобальная переменная, содержащая свойства меню
extern Menu menuProperties;


struct Player
{
    float velocity = 5;
    float jumpHeight = -10;
    float gravity = 20;
    float posX = 500.0;
    float posY = 100.0;
    bool isOnFloor = true;
    bool lookRight = true;
    bool hit_wall_up = false;
    int frameLine = 1;
};
extern Player player;

// Структура для хранения координат спрайта
typedef struct spriteCoord
{
    float left, right, top, bottom; // Грани спрайта
};

// Глобальная переменная, содержащая координаты спрайта
spriteCoord vrtcoord;

// Загрузка текстуры из файла
void Load_Texture(char *filename, GLuint *textureID, int swarp, int twarp, int filter)
{
    int twidth, thight, tcnt;
    // Загрузка изображения из файла
    unsigned char *data = stbi_load(filename, &twidth, &thight, &tcnt, 0);

    // Генерация текстурного объекта
    glGenTextures(1, textureID); //для создания одного или нескольких текстурных объектов. Она принимает два аргумента: количество
    //текстурных объектов, которые нужно создать, и массив идентификаторов, куда будут сохранены созданные идентификаторы текстур.
    //
    glBindTexture(GL_TEXTURE_2D, *textureID);

    // Установка параметров текстуры
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, swarp);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, twarp);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    // Загрузка изображения в текстурный объект
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, thight, 0, tcnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

    // Освобождение памяти, занятой изображением
    stbi_image_free(data);

    // Отвязка текстурного объекта
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Background(GLuint texture)
{
    // Массив вершин для прямоугольника
    static float svertix[] = {0, 0,   1024, 0,   1024, 678,   0, 678};
    // Массив текстурных координат для прямоугольника
    static float TexCord[] = {0, 0, 1, 0, 1, 1, 0, 1};

    // Включение режима текстурирования 2D
    glEnable(GL_TEXTURE_2D);

    // Привязка текстуры
    glBindTexture(GL_TEXTURE_2D, texture);

    // Установка параметров текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Включение режима массива вершин
    glEnableClientState(GL_VERTEX_ARRAY);

    // Включение режима массива текстурных координат
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Установка указателя на массив вершин
    glVertexPointer(2, GL_FLOAT, 0, svertix);

    // Установка указателя на массив текстурных координат
    glTexCoordPointer(2, GL_FLOAT, 0, TexCord);

    // Отрисовка прямоугольника
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // Отключение режима массива вершин
    glDisableClientState(GL_VERTEX_ARRAY);

    // Отключение режима массива текстурных координат
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    // Отвязка текстуры
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite_animation(GLuint texture, int n, int frameLine, float posX, float posY, bool lookRight)
{
/////////////////////
    // Массив вершин для прямоугольника
    float svertix[] = {0 + posX, 0 + posY, 75 + posX, 0 + posY, 75 + posX, 112.5 + posY, 0 + posX, 112.5 + posY};
    // Массив текстурных координат для прямоугольника
    static float TexCord[] = {0, 0,   1, 0,   1, 1,   0, 1};

    // Альфа-канал (α-канал) - это дополнительный канал цвета, который часто используется в графике компьютерных изображений для определения прозрачности или непрозрачности каждого пикселя изображения. Альфа-канал определяет уровень прозрачности от 0 (полностью прозрачный) до 1 (полностью непрозрачный).
    // Включение режима отсечения по альфа-каналу
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);

    // Включение режима текстурирования 2D
    glEnable(GL_TEXTURE_2D);

    // Привязка текстуры
    glBindTexture(GL_TEXTURE_2D, texture);

    // Сохранение текущей матрицы
    glPushMatrix();

    // Включение режима массива вершин
    glEnableClientState(GL_VERTEX_ARRAY);

    // Включение режима массива текстурных координат
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // Установка указателя на массив вершин
    glVertexPointer(2, GL_FLOAT, 0, svertix);

    // Установка указателя на массив текстурных координат
    glTexCoordPointer(2, GL_FLOAT, 0 , TexCord);
//////////////
    // Размеры спрайта по X и Y
    static const float spriteXsize = 800;
    static const float spriteYsize = 240;
    static const float charsizey = 80;

    // Вычисление координат текстуры для текущего кадра
    vrtcoord.left = (80 * n) / spriteXsize;
    vrtcoord.right = vrtcoord.left + (80 / spriteXsize);
    vrtcoord.top = (charsizey * frameLine) / spriteYsize;
    vrtcoord.bottom = vrtcoord.top + (charsizey / spriteYsize);

    // Установка текстурных координат в зависимости от направления взгляда
    if (lookRight)
    {
        TexCord[0] = TexCord[6] = vrtcoord.left;
        TexCord[2] = TexCord[4] = vrtcoord.right;
    }
    else
    {
        TexCord[2] = TexCord[4] = vrtcoord.left;
        TexCord[0] = TexCord[6] = vrtcoord.right;
    }

    // Установка текстурных координат для вертикальной ориентации спрайта
    TexCord[1] = TexCord[3] = vrtcoord.top;
    TexCord[5] = TexCord[7] = vrtcoord.bottom;

    // Отрисовка прямоугольника
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // Отключение режима массива вершин
    glDisableClientState(GL_VERTEX_ARRAY);

    // Отключение режима массива текстурных координат
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    // Отключение режима отсечения по альфа-каналу
    glDisable(GL_ALPHA_TEST);

    // Восстановление матрицы
    glPopMatrix();

    // Отвязка текстуры
    glBindTexture(GL_TEXTURE_2D, 0);
}



std::vector<std::vector<int>> collisionMap =
{
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1},
    { 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1},
    { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1},
    { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1},
    { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1},
    { 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,  1},
    { 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,  1},
    { 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,  1},
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  1}
};

const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 14;

float tileWidth = (SCREEN_WIDTH / MAP_WIDTH);
float tileHeight = (SCREEN_HEIGHT / MAP_HEIGHT);

bool check_Gravity(float deltaX, float deltaY)
{
    int x = floor((player.posX + deltaX)/ tileWidth);
    int y = floor((player.posY + deltaY)/ tileHeight);

    if(collisionMap[y][x] == 1 || collisionMap[y][x+1] == 1)
    {
        return false;
    }
    else if(collisionMap[y][x] == 0)
    {
        return true;
    }
}


float check_Collision_horizon(float deltaX, float deltaY)
{
    int x = floor((player.posX + deltaX)/ tileWidth);
    int y = floor((player.posY + 110 + deltaY)/ tileHeight);

    if(collisionMap[y][x+1] == 0)
    {
        return player.velocity;
    }
    else
    {
        return 0;
    }
}

bool check_Collision_UP(float deltaX, float deltaY)
{
    int x = floor((player.posX + deltaX)/ tileWidth);
    int y = floor((player.posY + deltaY)/ tileHeight);
    //std::cout<<" x="<<x<<" y="<<y;

    if(collisionMap[y-2][x] == 1 || collisionMap[y-2][x+1] == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Player_Move()
{

    if(check_Gravity(80,160) == true) //deltaX и deltaY добавляются к текущей позиции игрока по оси X и Y соответственно для вычисления новой позиции. Затем эта новая позиция используется для определения наличия столкновений с горизонтальными препятствиями.
    {
        player.posY+=player.gravity/2;
    }
    else
    {
        player.isOnFloor = true;
    }

    if (GetKeyState(VK_UP) < 0 && player.isOnFloor == true && player.hit_wall_up == false)
    {
        if (check_Collision_UP(80, 160) == false)
        {
            player.jumpHeight = 50;
            player.isOnFloor = false;
            player.frameLine = up;
        }
    }

    if (check_Collision_UP(80, 160) == true)
    {
        // Персонаж столкнулся с препятствием сверху, сразу начинаем движение вниз
        do
        {
            //player.posY = 100;
            //player.posX = 500;
            player.posY += player.velocity; // Сдвигаем персонажа вниз

        }
        while (check_Collision_UP(80, 160) == true);

    }


    if (GetKeyState(VK_RIGHT) < 0 )
    {
        player.lookRight = true;
        player.posX += check_Collision_horizon(100, 0);
        if(player.isOnFloor)
        {
            player.frameLine = go;
        }

    }
    else if (GetKeyState(VK_LEFT) < 0)
    {
        player.lookRight = false;
        player.posX -= check_Collision_horizon(0, 0);
        if(player.isOnFloor)
        {
            player.frameLine = go;
        }

    }
    else if (player.isOnFloor)
    {
        player.frameLine = statica;
    }

    if(player.jumpHeight == -1)
    {
        player.isOnFloor = true;
    }

    else if(player.isOnFloor == false)
    {
        player.posY -= player.jumpHeight - player.gravity;
        player.jumpHeight -= 1;
    }
}


void Draw_CollisionBlocks()
{
    // Отрисовка заполненных блоков
    for (int i = 1; i < MAP_HEIGHT; i++)   // Проходим по строкам карты
    {
        for (int j = 1; j < MAP_WIDTH; j++)   // Проходим по столбцам карты
        {
            if (collisionMap[i][j] == 1)   // Если текущий блок заполнен
            {
                glBegin(GL_QUADS); // Начинаем отрисовку квадрата
                glColor3f(0.5, 0.5, 0.5); // Задаем цвет заполненных блоков (серый)
                // Определяем размеры блока
                float blockSizeX = tileWidth;
                float blockSizeY = tileHeight;
                // Определяем координаты вершин квадрата в пространстве
                float startX = (j - 1) * blockSizeX; // Начальная x-координата блока
                float startY = (i - 0.5) * blockSizeY; // Начальная y-координата блока
                float endX = j * blockSizeX; // Конечная x-координата блока
                float endY = i * blockSizeY; // Конечная y-координата блока
                // Отрисовываем квадрат
                glVertex2f(startX, startY); // Левая верхняя вершина
                glVertex2f(endX, startY); // Правая верхняя вершина
                glVertex2f(endX, endY); // Правая нижняя вершина
                glVertex2f(startX, endY); // Левая нижняя вершина
                glEnd(); // Завершаем отрисовку квадрата
            }
        }
    }
}




