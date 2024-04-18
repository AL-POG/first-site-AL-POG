#include <gl/gl.h>
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

const int go = 0;
const int up = 1;
const int statica = 2;
const int left = 1;
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 678;
const int TILE_SIZE = 50;

typedef struct Button{           // Определение структуры для кнопки
    char name[50];               // Имя кнопки
    float vert[8];               // Вершины кнопки
    char isHover, isDown;        // Флаги наведения и нажатия
    float buffer[500];           // Буфер данных кнопки
    int num_quads;               // Количество квадратов в кнопке
    float textPosX,textPosY,textS; // Позиция и масштаб текста на кнопке
};

struct Menu {                     // Определение структуры для свойств меню
    bool isActive = false;        // Флаг активности меню
};
extern Menu menuProperties;       // Экстернальное объявление menuProperties

struct Player {                   // Определение структуры для свойств игрока
    float velocity = 5;           // Скорость игрока
    float jumpHeight = -10;       // Высота прыжка
    float gravity = 20;           // Гравитация, воздействующая на игрока
    float posX = 500.0;           // Позиция X игрока
    float posY = 100.0;           // Позиция Y игрока
    bool isOnFloor = true;        // Флаг, указывающий, находится ли игрок на полу
    bool lookRight = true;        // Флаг, указывающий, смотрит ли игрок вправо
    int frameLine = 1;            // Текущая линия кадра для анимации
};
extern Player player;             // Экстернальное объявление player

typedef struct spriteCoord{       // Определение структуры для координат спрайта
    float left, right, top, bottom; // Координаты текстуры спрайта
};

spriteCoord vrtcoord;              // Объявление vrtcoord типа spriteCoord

void Load_Texture(char *filename, GLuint *textureID, int swarp, int twarp, int filter) {
    int twidth, thight, tcnt;       // Переменные для ширины, высоты и количества каналов изображения
    unsigned char *data = stbi_load(filename, &twidth, &thight, &tcnt, 0); // Загрузка изображения

    glGenTextures(1, textureID);    // Генерация текстуры
    glBindTexture(GL_TEXTURE_2D, *textureID); // Привязка текстуры
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, swarp); // Установка параметров обертки текстуры S
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, twarp); // Установка параметров обертки текстуры T
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter); // Установка параметров фильтрации минимальной текстуры
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter); // Установка параметров фильтрации максимальной текстуры
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, thight, 0, tcnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data); // Загрузка данных текстуры
    stbi_image_free(data);          // Освобождение памяти от данных изображения
    glBindTexture(GL_TEXTURE_2D, 0); // Снятие привязки текстуры
}                                  // Завершение функции загрузки текстуры

void Show_Background(GLuint texture) {
    static float svertix[]= {0,0,  1024,0,  1024,678,  0,678}; // Массив вершин для задания прямоугольника фона
    static float TexCord[]= {0,0, 1,0, 1,1, 0,1 };             // Массив координат текстурных координат

    glEnable(GL_TEXTURE_2D);                                  // Включение 2D текстурирования
    glBindTexture(GL_TEXTURE_2D, texture);                    // Привязка текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Установка параметров обертки текстуры по оси S
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Установка параметров обертки текстуры по оси T
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Установка минимального фильтра текстуры
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Установка максимального фильтра текстуры
    glEnableClientState(GL_VERTEX_ARRAY);                     // Включение массива вершин
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);              // Включение массива текстурных координат
    glVertexPointer(2, GL_FLOAT, 0, svertix);                // Установка указателя на массив вершин
    glTexCoordPointer(2, GL_FLOAT, 0, TexCord);              // Установка указателя на массив текстурных координат
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);                     // Отрисовка прямоугольника с использованием массивов вершин и текстурных координат
    glDisableClientState(GL_VERTEX_ARRAY);                    // Отключение массива вершин
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);             // Отключение массива текстурных координат
    glBindTexture(GL_TEXTURE_2D, 0);                         // Снятие привязки текстуры
}

void Sprite_animation(GLuint texture, int n, int frameLine, float posX, float posY, bool lookRight) {
    float svertix[]= {0+posX,0+posY, 75+posX,0+posY, 75+posX,112.5+posY, 0+posX,112.5+posY}; // Массив вершин для задания прямоугольника спрайта
    static float TexCord[]= {0,0, 1,0, 1,1, 0,1};            // Массив координат текстурных координат

    glEnable(GL_ALPHA_TEST);                                  // Включение теста прозрачности
    glAlphaFunc(GL_GREATER, 0);                               // Установка функции теста прозрачности
    glEnable(GL_TEXTURE_2D);                                  // Включение 2D текстурирования
    glBindTexture(GL_TEXTURE_2D, texture);                    // Привязка текстуры
    glPushMatrix();                                            // Сохранение текущей матрицы в стеке
    glEnableClientState(GL_VERTEX_ARRAY);                     // Включение массива вершин
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);              // Включение массива текстурных координат

    glVertexPointer(2,GL_FLOAT,0,svertix);                    // Установка указателя на массив вершин
    glTexCoordPointer(2,GL_FLOAT,0,TexCord);                 // Установка указателя на массив текстурных координат

    static const float spriteXsize = 800;                     // Размер спрайта по оси X
    static const float spriteYsize = 240;                     // Размер спрайта по оси Y
    static const float charsizey = 80;                        // Размер символа по оси Y

    vrtcoord.left = (80 * n) / spriteXsize;                   // Вычисление левой текстурной координаты
    vrtcoord.right = vrtcoord.left + (80 / spriteXsize);      // Вычисление правой текстурной координаты
    vrtcoord.top = (charsizey * frameLine) / spriteYsize;     // Вычисление верхней текстурной координаты
    vrtcoord.bottom = vrtcoord.top + (charsizey / spriteYsize); // Вычисление нижней текстурной координаты

    TexCord[5] = TexCord[7] = vrtcoord.bottom;               // Установка координат Y для нижних вершин
    TexCord[1] = TexCord[3] = vrtcoord.top;                  // Установка координат Y для верхних вершин

    if (lookRight) {                                          // Если персонаж смотрит вправо
        TexCord[0] = TexCord[6] = vrtcoord.left;             // Установка координат X для левых вершин
        TexCord[2] = TexCord[4] = vrtcoord.right;            // Установка координат X для правых вершин
    } else {                                                  // Если персонаж смотрит влево
        TexCord[2] = TexCord[4] = vrtcoord.left;             // Установка координат X для правых вершин
        TexCord[0] = TexCord[6] = vrtcoord.right;            // Установка координат X для левых вершин
    }

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);                      // Отрисовка прямоугольника с использованием массивов вершин и текстурных координат
    glDisableClientState(GL_VERTEX_ARRAY);                    // Отключение массива вершин
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);             // Отключение массива текстурных координат
    glDisable(GL_ALPHA_TEST);                                 // Отключение теста прозрачности
    glPopMatrix();                                            // Восстановление предыдущей матрицы из стека
    glBindTexture(GL_TEXTURE_2D, 0);                         // Снятие привязки текстуры
}

std::vector<std::vector<int>> collisionMap = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
    { 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

int checkCollisionVertical(float deltaX, float deltaY, int side) { // Функция проверки вертикальной коллизии
    int x = floor((player.posX - 0 + deltaX)/50); // Определение индекса ячейки по оси X
    int y = floor((player.posY + 0 + deltaY)/50); // Определение индекса ячейки по оси Y
    for(int i = 1; i <= 1; i++) { // Цикл проверки коллизии вдоль вертикали
        if (collisionMap[y][x] == 2) { // Если обнаружена коллизия с маркером 2

            collisionMap[y][x] = 0; // Установка значения ячейки в 0
        } else if(collisionMap[y][x] == 1 || collisionMap[y][x+1] == 1) { // Если обнаружена стена или препятствие
            return 1; // Возврат значения 1 для обозначения коллизии
        } else if(collisionMap[y][x] == 0) { // Если свободное пространство
            return 0; // Возврат значения 0 для отсутствия коллизии
        } else { // В остальных случаях
            return -1; // Возврат значения -1 для обозначения неопределенной ситуации
        }
    }
}

float checkCollisionHorizontal(float deltaX, float deltaY, int side) { // Функция проверки горизонтальной коллизии
    int x = floor((player.posX + 0 + deltaX)/50); // Определение индекса ячейки по оси X
    int y = floor((player.posY + 111 + deltaY)/50); // Определение индекса ячейки по оси Y
    if(collisionMap[y][x] == 2) { // Если обнаружена коллизия с маркером 2

        collisionMap[y][x] = 0; // Установка значения ячейки в 0
    }
    if(collisionMap[y][x+side] == 0 || collisionMap[y][x+side] == 2) { // Если свободное пространство или маркер 2
        return player.velocity; // Возврат скорости игрока
    } else { // В противном случае
        return 0; // Возврат нуля для обозначения коллизии
    }
}

void Player_Move() { // Функция обработки движения игрока
    if(checkCollisionVertical(75,150,0) != 1 && checkCollisionVertical(75,150,1) != 2) { // Проверка вертикальной коллизии для падения
        player.posY+=player.gravity/2; // Перемещение игрока вниз (падение)
    } else { // Если есть коллизия или поверхность, на которой стоит игрок
        player.isOnFloor = true; // Установка флага "на полу"
    }

    if (GetKeyState(VK_UP) < 0 && player.isOnFloor == true && checkCollisionVertical(75,150,left) == 1 ) { // Проверка нажатия клавиши вверх и возможности прыжка
        player.jumpHeight = 50; // Установка высоты прыжка
        player.isOnFloor=false; // Сброс флага "на полу"
        player.frameLine = up; // Установка анимации прыжка
    }

    if (GetKeyState(VK_RIGHT) < 0 ) { // Проверка нажатия клавиши вправо
        player.lookRight = true; // Установка направления вправо
        player.posX += checkCollisionHorizontal(100, 0.0, 1); // Перемещение игрока вправо с учетом горизонтальной коллизии
        if(player.isOnFloor) { // Если игрок находится на полу
            player.frameLine = go; // Установка анимации ходьбы
        }
    } else if (GetKeyState(VK_LEFT) < 0) { // Проверка нажатия клавиши влево
        player.lookRight = false; // Установка направления влево
        player.posX -= checkCollisionHorizontal(0, 0.0, left); // Перемещение игрока влево с учетом горизонтальной коллизии
        if(player.isOnFloor) { // Если игрок находится на полу
            player.frameLine = go; // Установка анимации ходьбы
        }
    } else if (player.isOnFloor) { // Если игрок находится на полу и не нажата клавиша движения
        player.frameLine = statica; // Установка анимации стояния
    }

    if(player.jumpHeight == -1) { // Если прыжок завершен
        player.isOnFloor = true; // Установка флага "на полу"
    } else if(player.isOnFloor == false) { // Если игрок не на полу и прыгает
        player.posY -= player.jumpHeight - player.gravity; // Перемещение игрока вверх (прыжок)
        player.jumpHeight -= 1; // Уменьшение высоты прыжка
    }
}

