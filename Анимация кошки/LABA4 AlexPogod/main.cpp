#include <gl/gl.h>
#include <GL/glu.h>
#include <windows.h>
#include <math.h>
#include <iostream>
#include <string>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "stb-master/stb_easy_font.h"
#include "main.h"

using namespace std;

//------------------------------------------------------
//buffer
Button *btn = NULL;
int btnCnt = 0;

float mouseX, mouseY;
int animationLine = 0;

int Menu_AddButton(char *name, float x, float y, float width, float height, float textS) {
    btnCnt++;
    btn = (Button*)realloc(btn, sizeof(btn[0]) * btnCnt);

    snprintf(btn[btnCnt - 1].name, 50, "%s", name);
    float *vert = btn[btnCnt - 1].vert;
    vert[0] = vert[6] = x;
    vert[2] = vert[4] = x + width;
    vert[1] = vert[3] = y;
    vert[5] = vert[7] = y + height;
    btn[btnCnt - 1].isHover = 0;
    btn[btnCnt - 1].isDown = 0;

    Button *b = &btn[btnCnt - 1];
    b->num_quads = stb_easy_font_print(0, 0, name, 0, b->buffer, sizeof(b->buffer));
    b->textPosX = x + (width - stb_easy_font_width(name) * textS) / 2.0;
    b->textPosY = y + (height - stb_easy_font_height(name) * textS) / 2.0 + textS * 2;
    b->textS = textS;

    return btnCnt - 1;
}

void ShowButton(int buttonId) {
    Button btn1 = btn[buttonId];
    glVertexPointer(2, GL_FLOAT, 0, btn1.vert);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor3f(1, 1, 1);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glPushMatrix();
    glTranslatef(btn1.textPosX, btn1.textPosY, 0);
    glScalef(btn1.textS, btn1.textS, 0);
    glColor3f(0, 0, 0);
    glVertexPointer(2, GL_FLOAT, 16, btn1.buffer);
    glDrawArrays(GL_QUADS, 0, btn1.num_quads * 4);
    glPopMatrix();
    glColor3f(1, 1, 1);
}

void Menu_ShowMenu() {
    for (int i = 0; i < btnCnt; i++) {
        ShowButton(i);
    }
}

char isCordInButton(int buttonID, float x, float y) {
    float *vert = btn[buttonID].vert;
    return (x > vert[0]) && (y > vert[1]) && (x < vert[4]) && (y < vert[5]);
}

int Menu_MouseMove(float x, float y) {
    mouseX = x;
    mouseY = y;
    int movebtn = -1;
    for (int i = 0; i < btnCnt; i++) {
        if (isCordInButton(i, mouseX, mouseY)) {
            btn[i].isHover = 1;
            movebtn = i;
        } else {
            btn[i].isDown = 0;
            btn[i].isHover = 0;
        }
    }
    return movebtn;
}

int Menu_MouseDown() {
    int downbtn = -1;
    for (int i = 0; i < btnCnt; i++) {
        if (isCordInButton(i, mouseX, mouseY)) {
            btn[i].isDown = 1;
            downbtn = i;
        }
    }
    return downbtn;
}

void Menu_changeState() {
    menuProperties.isActive = !menuProperties.isActive;
}

void Menu_Clear() {
    btnCnt = 0;
    free(btn);
    btn = NULL;
}

void MouseDown() {
    if (int buttonId = Menu_MouseDown(); buttonId == 0) {
        Menu_Clear();
        Menu_changeState();
    }
}

//------------------------------

GLuint anim;
GLuint bg;
float frame;
Menu menuProperties;
Player player;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC) { // Определение функции для включения OpenGL
    PIXELFORMATDESCRIPTOR pfd; // Создание структуры для описания формата пикселей
    int iFormat;

    *hDC = GetDC(hwnd); // Получение контекста устройства для окна
    ZeroMemory(&pfd, sizeof(pfd)); // Очистка памяти для структуры pfd
    pfd.nSize = sizeof(pfd); // Задание размера структуры pfd
    pfd.nVersion = 1; // Задание версии структуры pfd
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // Задание флагов для формата пикселей
    pfd.iPixelType = PFD_TYPE_RGBA; // Задание типа пикселей
    pfd.cColorBits = 24; // Задание количества бит для цвета
    pfd.cDepthBits = 16; // Задание количества бит для буфера глубины
    pfd.iLayerType = PFD_MAIN_PLANE; // Задание типа слоя

    iFormat = ChoosePixelFormat(*hDC, &pfd); // Выбор наиболее подходящего формата пикселей
    SetPixelFormat(*hDC, iFormat, &pfd); // Установка выбранного формата пикселей
    *hRC = wglCreateContext(*hDC); // Создание контекста OpenGL
    wglMakeCurrent(*hDC, *hRC); // Установка текущего контекста OpenGL
}



void mEnu_init() {
    Menu_AddButton("Go", 300, 300, 150, 200, 10);
    Menu_changeState();
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow) {

    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "CAT_level";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
        return 0;

    hwnd = CreateWindowEx(0,
                          "CAT_level",
                          "LEVEL_CAT_JUMP",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1024,
                          678,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    EnableOpenGL(hwnd, &hDC, &hRC);

    RECT rct;
    GetClientRect(hwnd, &rct);
    glOrtho(0, rct.right, rct.bottom, 0, 1, -1);
    glEnable(GL_TEXTURE_2D);
    mEnu_init();
    Load_Texture("podvorotna.jpg", &bg, GL_REPEAT, GL_REPEAT, GL_NEAREST);
    Show_Background(bg);
    Load_Texture("CAT.png", &anim, GL_REPEAT, GL_REPEAT, GL_NEAREST);


    while (!bQuit) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                bQuit = TRUE;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {
            glClear(GL_COLOR_BUFFER_BIT);
            glFlush();
            Show_Background(bg);

            if (menuProperties.isActive) {
                Menu_ShowMenu();
            }
            else {
                Player_Move();
                Sprite_animation(anim, frame, player.frameLine, player.posX, player.posY, player.lookRight);
                frame >= 9 ? frame = 0 : frame += 0.2;

                Draw_CollisionBlocks();
            }
            SwapBuffers(hDC);
            Sleep(1);
        }
    }

    DisableOpenGL(hwnd, hDC, hRC);
    DestroyWindow(hwnd);
    return msg.wParam;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_MOUSEMOVE:
            Menu_MouseMove(LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_LBUTTONDOWN:
            MouseDown();
            break;

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE && !menuProperties.isActive)
                mEnu_init();
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}


