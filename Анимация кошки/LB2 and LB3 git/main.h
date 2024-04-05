#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#define STB_IMAGE_IMPLEMENTATION
#define nameLen 20

#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <malloc.h>
#include "stb-master/stb_easy_font.h"
#include "stb-master/stb_image.h"

using namespace std;

GLuint texture;
GLuint sprite;
GLuint background;

int width = 1024;
int height = 678;
float gravity = 0.45;
float speed = 5;
float currentframe=0;
float chSiX;
float chSiY;
bool directional;
bool gameState=0;
int n=0;


typedef struct
{
    float left, right, top, buttom, textX, textY;
} sritCoord;

typedef struct
{
    float x, y, dx, dy;
} Hero;

Hero pers;
sritCoord vrtcoord;
sritCoord BackGroundCoord;


void Hero_Init(Hero *obj, float x1, float y1, float dx1, float dy1)
{
    obj->x=x1;
    obj->y=y1;
    obj->dx=dx1;
    obj->dy=dy1;
}

void Reflect (float *da, float *a, BOOL cond, float wall)
{
    if (!cond) return;
    *da*=-0;
    *a=wall;
}

void Hero_Move(Hero *obj)
{
    obj->y+=obj->dy;
    obj->x+=obj->dx;
    Reflect(&obj->dy, &obj->y, obj->y<0,0);
    Reflect(&obj->dx,&obj->x, obj->x<0, 0);
    Reflect(&obj->dx,&obj->x, obj->x>width-chSiX-0, width-chSiX-0);
    obj->dy-=gravity;
    if (GetKeyState(VK_LEFT)<0 && gameState==1)
    {
        currentframe+=0.15;
        obj->dx-=speed;
        obj->x+=obj->dx;
        obj->dx=0;
        if (currentframe>9) currentframe-=8;
        directional=1;

    }

    if (GetKeyState(VK_RIGHT)<0 && gameState==1)
    {
        currentframe+=0.15;
        obj->dx+=speed;
        obj->x+=obj->dx;
        obj->dx=0;
        if (currentframe>9) currentframe-=8;
        directional=0;


    }
    if (GetKeyState(VK_UP)<0 && (obj->y<410) && gameState==1)
    {
        obj->dy =speed*1.2;
        obj->y+=obj->dy;
    }
}



void Sprite_animation(GLuint texture, int n, Hero *obj)
{

    static float svertix[]= {0,0, 80,0, 80,80, 0,80};
    static float TexCord[]= {0,0, 0.12,0, 0.12,1, 0,1};
    static float line[]= {0,0, 80,0, 80,80, 0,80};

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.7);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2,GL_FLOAT,0,svertix);
    glTexCoordPointer(2,GL_FLOAT,0,TexCord);

    static float spriteXsize=800;
    static float spriteYsize=80;
    static float charsizey=26.0;
    static float charsizex=80.0;
    vrtcoord.left=(charsizex*n)/spriteXsize;
    vrtcoord.right=vrtcoord.left+(charsizex/spriteXsize);
    vrtcoord.top=(charsizey*0)/spriteYsize;
    vrtcoord.buttom=vrtcoord.top+(charsizey/spriteYsize);

    TexCord[1] = TexCord[3] = vrtcoord.buttom;
    TexCord[5] = TexCord[7] = vrtcoord.top;

    if (GetKeyState(VK_LEFT)<0 && gameState==1)
    {
        TexCord[2] = TexCord[4] = vrtcoord.left;
        TexCord[0] = TexCord[6] = vrtcoord.right;
    }
    if (GetKeyState(VK_RIGHT)<0 && gameState==1)
    {
        TexCord[0] = TexCord[6] = vrtcoord.left;
        TexCord[2] = TexCord[4] = vrtcoord.right;
    }




    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisable(GL_ALPHA_TEST);


    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();

    chSiX=charsizex;
    chSiY=charsizey;

}

void Load_Texture( char *filename, GLuint *textureID, int swarp, int twarp, int filter)
{
    int twidth, thight, tcnt;
    unsigned char *data=stbi_load(filename,&twidth,&thight,&tcnt,0);

    glGenTextures(1, textureID);
    glBindTexture(GL_TEXTURE_2D, *textureID);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,swarp);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,twarp);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,filter);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,filter);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,twidth,thight,
                 0, tcnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void Main_Init()
{
    Hero_Init(&pers, width/1.0,height/1.0,0,0);
    Load_Texture("Anim_cat.png", &sprite, GL_CLAMP, GL_CLAMP, GL_NEAREST);
    Load_Texture("podvorotna.png", &background, GL_REPEAT, GL_CLAMP, GL_NEAREST);
}



void Show_Background(GLuint texture)
{
    static float svertix[]= {-1024,0, -1024,678, 1024*2,678, 1024*2,0}; //Эти значения указывают на координаты вершин фона, учитывая его новое разрешение.
    static float TexCord[]= {-1,1, -1,0, 2,0, 2,1 };

    glClearColor(0,0,0,0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(2,GL_FLOAT,0,svertix);
    glTexCoordPointer(2,GL_FLOAT,0,TexCord);
    glDrawArrays(GL_TRIANGLE_FAN,0,4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


void Hero_Show(Hero *obj)
{
    glPushMatrix();
    glTranslatef(obj->x,obj->y,0);
    Sprite_animation(sprite, currentframe, &pers);
    glPopMatrix();

}

//-------------------------------------------------------------

typedef struct
{
    char name[nameLen];
    float vert[8];
    char isHover, isDown;
    //text
    float buffer[50*nameLen];
    int num_quads;
    float textPosX,textPosY,textS;
} Button;

Button *btn;
int btnCnt = 0;

float mouseX,mouseY;

int Menu_AddButton(char *name, float x, float y, float width, float height, float textS)
{
    btnCnt++;
    btn = (Button*)realloc(btn, sizeof(btn[0])*btnCnt);

    snprintf(btn[btnCnt-1].name, nameLen, "%s", name);
    float *vert = btn[btnCnt-1].vert;
    vert[0]=vert[6]=x;
    vert[2]=vert[4]=x+width;
    vert[1]=vert[3]=y;
    vert[5]=vert[7]=y+height;
    btn[btnCnt-1].isHover=0;
    btn[btnCnt-1].isDown=0;

    Button *b= btn + btnCnt - 1;
    b->num_quads = stb_easy_font_print(0, 0, name,0,b->buffer, sizeof(b->buffer));
    b->textPosX = x +(width-stb_easy_font_width(name)*textS)/2.0;
    b->textPosY = y +(height-stb_easy_font_height(name)*textS)/2.0;
    b->textPosY+= textS*2;
    b->textS = textS;

    return btnCnt-1;
}

void ShowButton(int buttonId)
{

    Button btn1 = btn[buttonId];
    glVertexPointer(2, GL_FLOAT, 0, btn1.vert);
    glEnableClientState(GL_VERTEX_ARRAY);
    if (btn1.isHover)glColor3f(0.0,0.0,0.0);
    else glColor3f(0.0,0.9,0.0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glColor3f(0.2, 0.2, 0.2);
    glLineWidth(1);
    glDrawArrays(GL_LINE_LOOP,0,4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(btn1.textPosX,btn1.textPosY,0);
    glScalef(btn1.textS,btn1.textS,0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, btn1.buffer);
    glDrawArrays(GL_QUADS, 0, btn1.num_quads*4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void Menu_ShowMenu()
{
    for(int i=0; i<btnCnt; i++)   ShowButton(i);
}

char isCordInButton(int buttonID, float x, float y)
{
    float *vert=btn[buttonID].vert;
    return (x>vert[0]) && (y>vert[1]) && (x<vert[4]) && (y<vert[5]);
}

int Menu_MouseMove (float x, float y)
{
    mouseX = x;
    mouseY = y;
    int movebtn=-1;
    for (int i=0; i<btnCnt; i++)
        if (isCordInButton(i, mouseX, mouseY))
        {
            btn[i].isHover =1;
            movebtn = i;
        }
        else
        {
            btn[i].isDown=0;
            btn[i].isHover=0;
        }
    return movebtn;
}

int Menu_MouseDown()
{
    int downbtn=-1;
    for (int i=0; i<btnCnt; i++)
        if (isCordInButton(i, mouseX, mouseY))
        {
            btn[i].isDown =1;
            downbtn = i;
        }
    return downbtn;
}

void Menu_MouseUp(){
for (int i=0; i < btnCnt; i++)
btn[i].isDown=0;
}

char *Menu_GetButtonName(int buttonID)
{
return btn[buttonID].name;
}

void Menu_Clear()
{
    btnCnt=0;
    free(btn);
    btn=0;
}

//-----------------------------------------------

void Menu_Init(){
    Menu_AddButton("Go", 5.0, 5.0, 111.0, 77.0, 2);
    Menu_AddButton("Stop", 150.0, 5.0, 111.0, 77.0, 2);
    Menu_AddButton("Close", 300.0, 5.0, 111.0, 77.0, 2);
}

void MouseDown()
{
    int buttonId = Menu_MouseDown();
    if(buttonId<0) return;
    char *name = Menu_GetButtonName(buttonId);
    printf("%s\n",name, &buttonId);
    switch (buttonId)
    {
    case 0:
        if (gameState==0) gameState=1;
        break;
    case 1:
        if (gameState==1) gameState=0;
        break;
    case 2:
        PostQuitMessage(0);
        break;

    }
}

#endif // MAIN_H_INCLUDED
