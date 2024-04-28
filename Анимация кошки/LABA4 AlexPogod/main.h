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

const int go = 0;
const int up = 1;
const int statica = 2;
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 678;
const int TILE_SIZE = 50;

typedef struct Button{
    char name[50];
    float vert[8];
    char isHover, isDown;
    float buffer[500];
    int num_quads;
    float textPosX,textPosY,textS;
};

struct Menu {
    bool isActive = false;
};
extern Menu menuProperties;

struct Player {
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

typedef struct spriteCoord{
    float left, right, top, bottom;
};

spriteCoord vrtcoord;

void Load_Texture(char *filename, GLuint *textureID, int swarp, int twarp, int filter) {
    int twidth, thight, tcnt;
    unsigned char *data = stbi_load(filename, &twidth, &thight, &tcnt, 0);

    glGenTextures(1, textureID);
    glBindTexture(GL_TEXTURE_2D, *textureID);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, swarp);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, twarp);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, thight, 0, tcnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Show_Background(GLuint texture) {
    static float svertix[]= {0,0,  1024,0,  1024,678,  0,678};
    static float TexCord[]= {0,0, 1,0, 1,1, 0,1 };

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, svertix);
    glTexCoordPointer(2, GL_FLOAT, 0, TexCord);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite_animation(GLuint texture, int n, int frameLine, float posX, float posY, bool lookRight) {
    float svertix[]= {0+posX,0+posY, 75+posX,0+posY, 75+posX,112.5+posY, 0+posX,112.5+posY};
    static float TexCord[]= {0,0, 1,0, 1,1, 0,1};

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2,GL_FLOAT,0,svertix);
    glTexCoordPointer(2,GL_FLOAT,0,TexCord);

    static const float spriteXsize = 800;
    static const float spriteYsize = 240;
    static const float charsizey = 80;

    vrtcoord.left = (80 * n) / spriteXsize;
    vrtcoord.right = vrtcoord.left + (80 / spriteXsize);
    vrtcoord.top = (charsizey * frameLine) / spriteYsize;
    vrtcoord.bottom = vrtcoord.top + (charsizey / spriteYsize);

    TexCord[5] = TexCord[7] = vrtcoord.bottom;
    TexCord[1] = TexCord[3] = vrtcoord.top;

    if (lookRight) {
        TexCord[0] = TexCord[6] = vrtcoord.left;
        TexCord[2] = TexCord[4] = vrtcoord.right;
    } else {
        TexCord[2] = TexCord[4] = vrtcoord.left;
        TexCord[0] = TexCord[6] = vrtcoord.right;
    }

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}



std::vector<std::vector<int>> collisionMap = {
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

bool check_Gravity(float deltaX, float deltaY) {
    int x = floor((player.posX + deltaX)/ tileWidth);
    int y = floor((player.posY + deltaY)/ tileHeight);

        if(collisionMap[y][x] == 1 || collisionMap[y][x+1] == 1) {
            return false;
        } else if(collisionMap[y][x] == 0) {
            return true;
        }
}


float check_Collision_horizon(float deltaX, float deltaY) {
    int x = floor((player.posX + 0 + deltaX)/ tileWidth);
    int y = floor((player.posY + 110 + deltaY)/ tileHeight);

        if(collisionMap[y][x+1] == 0) {
            return player.velocity;
        } else {
            return 0;
        }
}

bool check_Collision_UP(float deltaX, float deltaY) {
    int x = floor((player.posX + deltaX)/ tileWidth);
    int y = floor((player.posY + deltaY)/ tileHeight);
        //std::cout<<" x="<<x<<" y="<<y;

        if(collisionMap[y-2][x] == 1 || collisionMap[y-2][x+1] == 1) {
            return true;
        } else {
            return false;
        }
}

void Player_Move() {

    if(check_Gravity(75,150) == true) {
        player.posY+=player.gravity/2;
    }
    else {
        player.isOnFloor = true;
    }

if (GetKeyState(VK_UP) < 0 && player.isOnFloor == true && player.hit_wall_up == false) {
    if (check_Collision_UP(75, 150) == false) {
        player.jumpHeight = 50;
        player.isOnFloor = false;
        player.frameLine = up;
    }
}

if (check_Collision_UP(75, 150) == true) {
        // Персонаж столкнулся с препятствием сверху, сразу начинаем движение вниз
         do{
            //player.posY = 100;
            //player.posX = 500;
            player.posY += player.velocity; // Сдвигаем персонажа вниз

        }while (check_Collision_UP(75, 150) == true);

    }


    if (GetKeyState(VK_RIGHT) < 0 ) {
        player.lookRight = true;
        player.posX += check_Collision_horizon(100, 0);
        if(player.isOnFloor) {
            player.frameLine = go;
        }

    } else if (GetKeyState(VK_LEFT) < 0) {
        player.lookRight = false;
        player.posX -= check_Collision_horizon(0, 0);
        if(player.isOnFloor) {
            player.frameLine = go;
        }

    } else if (player.isOnFloor) {
        player.frameLine = statica;
    }

    if(player.jumpHeight == -1) {
        player.isOnFloor = true;
    }

    else if(player.isOnFloor == false) {
        player.posY -= player.jumpHeight - player.gravity;
        player.jumpHeight -= 1;
    }
}


void Draw_CollisionBlocks()
{
    // Отрисовка заполненных блоков
    for (int i = 1; i < MAP_HEIGHT; i++) { // Начинаем с i = 1, чтобы сместить на один блок вверх
        for (int j = 1; j < MAP_WIDTH; j++) { // Начинаем с j = 1, чтобы сместить на один блок влево
            if (collisionMap[i][j] == 1) {
                glBegin(GL_QUADS);
                glColor3f(0.5, 0.5, 0.5); // Цвет заполненных блоков
                float blockSizeX = tileWidth;
                float blockSizeY = tileHeight;
                glVertex2f((j - 1) * blockSizeX, (i - 0.5) * blockSizeY); // Смещение на один блок влево и вверх
                glVertex2f(j * blockSizeX, (i - 0.5) * blockSizeY); // Смещение на один блок вверх
                glVertex2f(j * blockSizeX, i * blockSizeY);
                glVertex2f((j - 1) * blockSizeX, i * blockSizeY); // Смещение на один блок влево
                glEnd();
            }
        }
    }
}




