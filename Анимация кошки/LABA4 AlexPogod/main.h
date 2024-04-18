#include <gl/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>              // ����������� ����������� ������������ �������
#include <iostream>
#include <vector>
#include <string>
#include <malloc.h>              // ����������� ������� ���������� �������
#include <windows.h>             // ����������� ������� ��� ������ � Windows
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION // ������ ��� ��������� ���������� � stb_image.h
#include "stb-master/stb_image.h" // ����������� stb_image.h ��� �������� �����������

const int go = 0;
const int up = 1;
const int statica = 2;
const int left = 1;
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 678;
const int TILE_SIZE = 50;

typedef struct Button{           // ����������� ��������� ��� ������
    char name[50];               // ��� ������
    float vert[8];               // ������� ������
    char isHover, isDown;        // ����� ��������� � �������
    float buffer[500];           // ����� ������ ������
    int num_quads;               // ���������� ��������� � ������
    float textPosX,textPosY,textS; // ������� � ������� ������ �� ������
};

struct Menu {                     // ����������� ��������� ��� ������� ����
    bool isActive = false;        // ���� ���������� ����
};
extern Menu menuProperties;       // ������������� ���������� menuProperties

struct Player {                   // ����������� ��������� ��� ������� ������
    float velocity = 5;           // �������� ������
    float jumpHeight = -10;       // ������ ������
    float gravity = 20;           // ����������, �������������� �� ������
    float posX = 500.0;           // ������� X ������
    float posY = 100.0;           // ������� Y ������
    bool isOnFloor = true;        // ����, �����������, ��������� �� ����� �� ����
    bool lookRight = true;        // ����, �����������, ������� �� ����� ������
    int frameLine = 1;            // ������� ����� ����� ��� ��������
};
extern Player player;             // ������������� ���������� player

typedef struct spriteCoord{       // ����������� ��������� ��� ��������� �������
    float left, right, top, bottom; // ���������� �������� �������
};

spriteCoord vrtcoord;              // ���������� vrtcoord ���� spriteCoord

void Load_Texture(char *filename, GLuint *textureID, int swarp, int twarp, int filter) {
    int twidth, thight, tcnt;       // ���������� ��� ������, ������ � ���������� ������� �����������
    unsigned char *data = stbi_load(filename, &twidth, &thight, &tcnt, 0); // �������� �����������

    glGenTextures(1, textureID);    // ��������� ��������
    glBindTexture(GL_TEXTURE_2D, *textureID); // �������� ��������
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, swarp); // ��������� ���������� ������� �������� S
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, twarp); // ��������� ���������� ������� �������� T
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter); // ��������� ���������� ���������� ����������� ��������
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter); // ��������� ���������� ���������� ������������ ��������
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, twidth, thight, 0, tcnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data); // �������� ������ ��������
    stbi_image_free(data);          // ������������ ������ �� ������ �����������
    glBindTexture(GL_TEXTURE_2D, 0); // ������ �������� ��������
}                                  // ���������� ������� �������� ��������

void Show_Background(GLuint texture) {
    static float svertix[]= {0,0,  1024,0,  1024,678,  0,678}; // ������ ������ ��� ������� �������������� ����
    static float TexCord[]= {0,0, 1,0, 1,1, 0,1 };             // ������ ��������� ���������� ���������

    glEnable(GL_TEXTURE_2D);                                  // ��������� 2D ���������������
    glBindTexture(GL_TEXTURE_2D, texture);                    // �������� ��������
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // ��������� ���������� ������� �������� �� ��� S
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // ��������� ���������� ������� �������� �� ��� T
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // ��������� ������������ ������� ��������
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // ��������� ������������� ������� ��������
    glEnableClientState(GL_VERTEX_ARRAY);                     // ��������� ������� ������
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);              // ��������� ������� ���������� ���������
    glVertexPointer(2, GL_FLOAT, 0, svertix);                // ��������� ��������� �� ������ ������
    glTexCoordPointer(2, GL_FLOAT, 0, TexCord);              // ��������� ��������� �� ������ ���������� ���������
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);                     // ��������� �������������� � �������������� �������� ������ � ���������� ���������
    glDisableClientState(GL_VERTEX_ARRAY);                    // ���������� ������� ������
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);             // ���������� ������� ���������� ���������
    glBindTexture(GL_TEXTURE_2D, 0);                         // ������ �������� ��������
}

void Sprite_animation(GLuint texture, int n, int frameLine, float posX, float posY, bool lookRight) {
    float svertix[]= {0+posX,0+posY, 75+posX,0+posY, 75+posX,112.5+posY, 0+posX,112.5+posY}; // ������ ������ ��� ������� �������������� �������
    static float TexCord[]= {0,0, 1,0, 1,1, 0,1};            // ������ ��������� ���������� ���������

    glEnable(GL_ALPHA_TEST);                                  // ��������� ����� ������������
    glAlphaFunc(GL_GREATER, 0);                               // ��������� ������� ����� ������������
    glEnable(GL_TEXTURE_2D);                                  // ��������� 2D ���������������
    glBindTexture(GL_TEXTURE_2D, texture);                    // �������� ��������
    glPushMatrix();                                            // ���������� ������� ������� � �����
    glEnableClientState(GL_VERTEX_ARRAY);                     // ��������� ������� ������
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);              // ��������� ������� ���������� ���������

    glVertexPointer(2,GL_FLOAT,0,svertix);                    // ��������� ��������� �� ������ ������
    glTexCoordPointer(2,GL_FLOAT,0,TexCord);                 // ��������� ��������� �� ������ ���������� ���������

    static const float spriteXsize = 800;                     // ������ ������� �� ��� X
    static const float spriteYsize = 240;                     // ������ ������� �� ��� Y
    static const float charsizey = 80;                        // ������ ������� �� ��� Y

    vrtcoord.left = (80 * n) / spriteXsize;                   // ���������� ����� ���������� ����������
    vrtcoord.right = vrtcoord.left + (80 / spriteXsize);      // ���������� ������ ���������� ����������
    vrtcoord.top = (charsizey * frameLine) / spriteYsize;     // ���������� ������� ���������� ����������
    vrtcoord.bottom = vrtcoord.top + (charsizey / spriteYsize); // ���������� ������ ���������� ����������

    TexCord[5] = TexCord[7] = vrtcoord.bottom;               // ��������� ��������� Y ��� ������ ������
    TexCord[1] = TexCord[3] = vrtcoord.top;                  // ��������� ��������� Y ��� ������� ������

    if (lookRight) {                                          // ���� �������� ������� ������
        TexCord[0] = TexCord[6] = vrtcoord.left;             // ��������� ��������� X ��� ����� ������
        TexCord[2] = TexCord[4] = vrtcoord.right;            // ��������� ��������� X ��� ������ ������
    } else {                                                  // ���� �������� ������� �����
        TexCord[2] = TexCord[4] = vrtcoord.left;             // ��������� ��������� X ��� ������ ������
        TexCord[0] = TexCord[6] = vrtcoord.right;            // ��������� ��������� X ��� ����� ������
    }

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);                      // ��������� �������������� � �������������� �������� ������ � ���������� ���������
    glDisableClientState(GL_VERTEX_ARRAY);                    // ���������� ������� ������
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);             // ���������� ������� ���������� ���������
    glDisable(GL_ALPHA_TEST);                                 // ���������� ����� ������������
    glPopMatrix();                                            // �������������� ���������� ������� �� �����
    glBindTexture(GL_TEXTURE_2D, 0);                         // ������ �������� ��������
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

int checkCollisionVertical(float deltaX, float deltaY, int side) { // ������� �������� ������������ ��������
    int x = floor((player.posX - 0 + deltaX)/50); // ����������� ������� ������ �� ��� X
    int y = floor((player.posY + 0 + deltaY)/50); // ����������� ������� ������ �� ��� Y
    for(int i = 1; i <= 1; i++) { // ���� �������� �������� ����� ���������
        if (collisionMap[y][x] == 2) { // ���� ���������� �������� � �������� 2

            collisionMap[y][x] = 0; // ��������� �������� ������ � 0
        } else if(collisionMap[y][x] == 1 || collisionMap[y][x+1] == 1) { // ���� ���������� ����� ��� �����������
            return 1; // ������� �������� 1 ��� ����������� ��������
        } else if(collisionMap[y][x] == 0) { // ���� ��������� ������������
            return 0; // ������� �������� 0 ��� ���������� ��������
        } else { // � ��������� �������
            return -1; // ������� �������� -1 ��� ����������� �������������� ��������
        }
    }
}

float checkCollisionHorizontal(float deltaX, float deltaY, int side) { // ������� �������� �������������� ��������
    int x = floor((player.posX + 0 + deltaX)/50); // ����������� ������� ������ �� ��� X
    int y = floor((player.posY + 111 + deltaY)/50); // ����������� ������� ������ �� ��� Y
    if(collisionMap[y][x] == 2) { // ���� ���������� �������� � �������� 2

        collisionMap[y][x] = 0; // ��������� �������� ������ � 0
    }
    if(collisionMap[y][x+side] == 0 || collisionMap[y][x+side] == 2) { // ���� ��������� ������������ ��� ������ 2
        return player.velocity; // ������� �������� ������
    } else { // � ��������� ������
        return 0; // ������� ���� ��� ����������� ��������
    }
}

void Player_Move() { // ������� ��������� �������� ������
    if(checkCollisionVertical(75,150,0) != 1 && checkCollisionVertical(75,150,1) != 2) { // �������� ������������ �������� ��� �������
        player.posY+=player.gravity/2; // ����������� ������ ���� (�������)
    } else { // ���� ���� �������� ��� �����������, �� ������� ����� �����
        player.isOnFloor = true; // ��������� ����� "�� ����"
    }

    if (GetKeyState(VK_UP) < 0 && player.isOnFloor == true && checkCollisionVertical(75,150,left) == 1 ) { // �������� ������� ������� ����� � ����������� ������
        player.jumpHeight = 50; // ��������� ������ ������
        player.isOnFloor=false; // ����� ����� "�� ����"
        player.frameLine = up; // ��������� �������� ������
    }

    if (GetKeyState(VK_RIGHT) < 0 ) { // �������� ������� ������� ������
        player.lookRight = true; // ��������� ����������� ������
        player.posX += checkCollisionHorizontal(100, 0.0, 1); // ����������� ������ ������ � ������ �������������� ��������
        if(player.isOnFloor) { // ���� ����� ��������� �� ����
            player.frameLine = go; // ��������� �������� ������
        }
    } else if (GetKeyState(VK_LEFT) < 0) { // �������� ������� ������� �����
        player.lookRight = false; // ��������� ����������� �����
        player.posX -= checkCollisionHorizontal(0, 0.0, left); // ����������� ������ ����� � ������ �������������� ��������
        if(player.isOnFloor) { // ���� ����� ��������� �� ����
            player.frameLine = go; // ��������� �������� ������
        }
    } else if (player.isOnFloor) { // ���� ����� ��������� �� ���� � �� ������ ������� ��������
        player.frameLine = statica; // ��������� �������� �������
    }

    if(player.jumpHeight == -1) { // ���� ������ ��������
        player.isOnFloor = true; // ��������� ����� "�� ����"
    } else if(player.isOnFloor == false) { // ���� ����� �� �� ���� � �������
        player.posY -= player.jumpHeight - player.gravity; // ����������� ������ ����� (������)
        player.jumpHeight -= 1; // ���������� ������ ������
    }
}

