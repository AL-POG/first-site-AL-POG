#include <gl/gl.h>
#include <windows.h> // ��������� ������������� ����� ��� ������ � Windows API
#include <math.h>
#include <iostream>
#include <string>
#include <malloc.h> // ��������� ������������� ����� ��� ������������� ��������� ������
#include <stdio.h> // ��������� ������������� ����� ��� ������������ �����-������
#include <stdlib.h> // ��������� ������������� ����� ��� ����������� ���������� �������
#include "stb-master/stb_easy_font.h" // ��������� ������������� ����� ��� ������������� �������� ������ �� ���������� STB
#include "main.h"

using namespace std;

//------------------------------------------------------

Button *btn = NULL; // ���������� ��������� �� ������ ������ � ��� ������������� ��������� NULL
int btnCnt = 0; // ���������� ���������� ��� �������� ���������� ������ � �� ������������� ��������� 0

float mouseX, mouseY; // ���������� ���������� ��� �������� ��������� ����
int animationLine = 0; // ���������� ���������� ��� �������� ������ ������ ��������

int Menu_AddButton(char *name, float x, float y, float width, float height, float textS) { // ���������� ������� ��� ���������� ������ � ����
    btnCnt++; // ���������� �������� ������
    btn = (Button*)realloc(btn, sizeof(btn[0]) * btnCnt); // ������������� ������ ��� ������ ������

    snprintf(btn[btnCnt - 1].name, 50, "%s", name); // ����������� ����� ������ � ��������� ������
    float *vert = btn[btnCnt - 1].vert; // ��������� ��������� �� ������ ������ ������
    vert[0] = vert[6] = x; // ��������� ���������� X ��� ������ ������
    vert[2] = vert[4] = x + width; // ��������� ���������� X ��� ������ ������
    vert[1] = vert[3] = y; // ��������� ���������� Y ��� ������ ������
    vert[5] = vert[7] = y + height; // ��������� ���������� Y ��� ������ ������
    btn[btnCnt - 1].isHover = 0; // ��������� ����� ��������� ���� �� ������ � �������� 0
    btn[btnCnt - 1].isDown = 0; // ��������� ����� ������� ������ � �������� 0

    Button *b = &btn[btnCnt - 1]; // ��������� ��������� �� ������� ������
    b->num_quads = stb_easy_font_print(0, 0, name, 0, b->buffer, sizeof(b->buffer)); // ��������� �������� ��� ������ ����� ������
    b->textPosX = x + (width - stb_easy_font_width(name) * textS) / 2.0; // ���������� ������� ������ �� ��� X
    b->textPosY = y + (height - stb_easy_font_height(name) * textS) / 2.0 + textS * 2; // ���������� ������� ������ �� ��� Y
    b->textS = textS; // ��������� ������� ������

    return btnCnt - 1; // ������� ������� ����������� ������
}

void ShowButton(int buttonId) { // ���������� ������� ��� ����������� ������ �� �� ��������������
    Button btn1 = btn[buttonId]; // ��������� ����� ��������� ������ �� �� ��������������
    glVertexPointer(2, GL_FLOAT, 0, btn1.vert); // ��������� ��������� �� ������ ������ ������ ��� �����������
    glEnableClientState(GL_VERTEX_ARRAY); // ��������� ������� ������
    glColor3f(1, 1, 1); // ��������� ����� ��� ����������� ������
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // ����������� ������ ��� ����������������
    glPushMatrix(); // ���������� ������� �������
    glTranslatef(btn1.textPosX, btn1.textPosY, 0); // ��������� ������� ������ ������
    glScalef(btn1.textS, btn1.textS, 0); // ��������������� ������ ������
    glColor3f(0, 0, 0); // ��������� ����� ������
    glVertexPointer(2, GL_FLOAT, 16, btn1.buffer); // ��������� ��������� �� ������ ������ ������ ��� �����������
    glDrawArrays(GL_QUADS, 0, btn1.num_quads * 4); // ����������� ������ ��� �����������������
    glPopMatrix(); // �������������� ���������� �������
    glColor3f(1, 1, 1); // ��������� ����� �� ���������
}

void Menu_ShowMenu() { // ���������� ������� ��� ����������� ����
    for (int i = 0; i < btnCnt; i++) { // ���� �� ���� ������� � ����
        ShowButton(i); // ����������� ������� ������
    }
}

char isCordInButton(int buttonID, float x, float y) { // ���������� ������� ��� �����������, ��������� �� ���������� ������ ������
    float *vert = btn[buttonID].vert; // ��������� ��������� �� ������ ������ ������� ������
    return (x > vert[0]) && (y > vert[1]) && (x < vert[4]) && (y < vert[5]); // ��������, ��������� �� ���������� ������ ��������������, ������������� ��������� ������
}

int Menu_MouseMove(float x, float y) { // ���������� ������� ��� ��������� �������� ����
    mouseX = x; // ���������� ������� ���������� X ����
    mouseY = y; // ���������� ������� ���������� Y ����
    int movebtn = -1; // ������������� ���������� ��� �������� �������������� ������, ��� ������� ��������� ������ ����
    for (int i = 0; i < btnCnt; i++) { // ���� �� ���� ������� � ����
        if (isCordInButton(i, mouseX, mouseY)) { // ��������, ��������� �� ������ ���� ��� ������� �������
            btn[i].isHover = 1; // ��������� ����� ��������� ���� �� ������� ������
            movebtn = i; // ���������� �������������� ������� ������
        } else { // ���� ������ ���� �� ��������� ��� ������� �������
            btn[i].isDown = 0; // ����� ����� ������� ������
            btn[i].isHover = 0; // ����� ����� ��������� ����
        }
    }
    return movebtn; // ������� �������������� ������, ��� ������� ��������� ������ ����
}

int Menu_MouseDown() { // ���������� ������� ��� ��������� ������� ������ ����
    int downbtn = -1; // ������������� ���������� ��� �������� �������������� ������, �� ������� ���� ����������� �������
    for (int i = 0; i < btnCnt; i++) { // ���� �� ���� ������� � ����
        if (isCordInButton(i, mouseX, mouseY)) { // ��������, ��������� �� ������ ���� ��� ������� �������
            btn[i].isDown = 1; // ��������� ����� ������� ������
            downbtn = i; // ���������� �������������� ������� ������
        }
    }
    return downbtn; // ������� �������������� ������, �� ������� ���� ����������� �������
}

void Menu_changeState() { // ���������� ������� ��� ��������� ��������� ����
    menuProperties.isActive = !menuProperties.isActive; // �������������� �������� ����� ���������� ����
}

void Menu_Clear() { // ���������� ������� ��� ������� ����
    btnCnt = 0; // ����� �������� ������
    free(btn); // ������������ ������, ���������� ��� ������� ������
    btn = NULL; // ��������� ��������� �� ������ ������ � NULL
}

void MouseDown() { // ���������� ������� ��� ��������� ������� ������ ����
    if (int buttonId = Menu_MouseDown(); buttonId == 0) { // ��������, ���� �� ������ �� ������� ������ � ����
        Menu_Clear(); // ������� ����
        Menu_changeState(); // ��������� ��������� ����
    }
}

//------------------------------

GLuint anim; // ���������� ���������� ��� �������� �������������� �������� ��������
GLuint bg; // ���������� ���������� ��� �������� �������������� �������� ����
float frame; // ���������� ���������� ��� �������� ������ �������� ����� ��������
Menu menuProperties; // ���������� ���������� ��� �������� ������� ����
Player player; // ���������� ���������� ��� �������� ������ � ������

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // �������� ������� ��� ��������� ��������� ����

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC) { // ����������� ������� ��� ���������� OpenGL
    wglMakeCurrent(NULL, NULL); // ����� �������� ��������� OpenGL
    wglDeleteContext(hRC); // �������� ��������� OpenGL
    ReleaseDC(hwnd, hDC); // ������������ ��������� ����������
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC) { // ����������� ������� ��� ��������� OpenGL
    PIXELFORMATDESCRIPTOR pfd; // �������� ��������� ��� �������� ������� ��������
    int iFormat;

    *hDC = GetDC(hwnd); // ��������� ��������� ���������� ��� ����
    ZeroMemory(&pfd, sizeof(pfd)); // ������� ������ ��� ��������� pfd
    pfd.nSize = sizeof(pfd); // ������� ������� ��������� pfd
    pfd.nVersion = 1; // ������� ������ ��������� pfd
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // ������� ������ ��� ������� ��������
    pfd.iPixelType = PFD_TYPE_RGBA; // ������� ���� ��������
    pfd.cColorBits = 24; // ������� ���������� ��� ��� �����
    pfd.cDepthBits = 16; // ������� ���������� ��� ��� ������ �������
    pfd.iLayerType = PFD_MAIN_PLANE; // ������� ���� ����

    iFormat = ChoosePixelFormat(*hDC, &pfd); // ����� �������� ����������� ������� ��������
    SetPixelFormat(*hDC, iFormat, &pfd); // ��������� ���������� ������� ��������
    *hRC = wglCreateContext(*hDC); // �������� ��������� OpenGL
    wglMakeCurrent(*hDC, *hRC); // ��������� �������� ��������� OpenGL
}

void mEnu_init() { // ����������� ������� ��� ������������� ����
    Menu_AddButton("Go", 300, 300, 150, 200, 10); // ���������� ������ "Go" � ����
    Menu_changeState(); // ��������� ��������� ����
}

int WINAPI WinMain(HINSTANCE hInstance, // �������� ������� WinMain, ����� ����� � ���������� Windows
                   HINSTANCE hPrevInstance, // ���������� ��������� ���������� (�� ������������ � ����������� ������� Windows)
                   LPSTR lpCmdLine, // ������ ��������� ������
                   int nCmdShow) { // ����� ����������� ����

    WNDCLASSEX wcex; // ��������� ��� �������� ������ ����
    HWND hwnd; // ���������� ����
    HDC hDC; // �������� ����������
    HGLRC hRC; // �������� OpenGL
    MSG msg; // ��������� ��� �������� ���������
    BOOL bQuit = FALSE; // ���� ���������� ����������

    wcex.cbSize = sizeof(WNDCLASSEX); // ������ ��������� WNDCLASSEX
    wcex.style = CS_OWNDC; // ����� ������ ���� (����������� �������� ���������� ��� ������� ����)
    wcex.lpfnWndProc = WindowProc; // ��������� �� ������� ��������� ��������� ����
    wcex.cbClsExtra = 0; // �������������� ������ ������ ���� (�� ������������)
    wcex.cbWndExtra = 0; // �������������� ������ ���� (�� ������������)
    wcex.hInstance = hInstance; // ���������� �������� ���������� ����������
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); // ������ ����������
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // ������ ����
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // ����� ��� �������� ����
    wcex.lpszMenuName = NULL; // ��� ���� (�� ������������)
    wcex.lpszClassName = "CAT_level"; // ��� ������ ����
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // ��������� ������ ����������

    if (!RegisterClassEx(&wcex)) // ����������� ������ ����
        return 0; // � ������ ������ ����������� ���������� 0

    hwnd = CreateWindowEx(0, // ����������� ����� ���� (�� ������������)
                          "CAT_level", // ��� ������ ����
                          "LEVEL_CAT_JUMP", // ��������� ����
                          WS_OVERLAPPEDWINDOW, // ����� ���� (����������� ���� � ����������, ������, �������� ����������)
                          CW_USEDEFAULT, // ��������� ���� �� ��� X (������������ ��������� �������� �� ���������)
                          CW_USEDEFAULT, // ��������� ���� �� ��� Y (������������ ��������� �������� �� ���������)
                          1024, // ������ ����
                          678, // ������ ����
                          NULL, // ���������� ������������� ���� (�����������)
                          NULL, // ���������� ���� (�����������)
                          hInstance, // ���������� ���������� ����������
                          NULL); // �������������� ��������� (�����������)

    ShowWindow(hwnd, nCmdShow); // ����������� ����
    EnableOpenGL(hwnd, &hDC, &hRC); // ��������� OpenGL � �������� ��������� OpenGL ��� ����

    RECT rct; // ������������� ��� �������� �������� ���������� ������� ����
    GetClientRect(hwnd, &rct); // ��������� �������� ���������� ������� ����
    glOrtho(0, rct.right, rct.bottom, 0, 1, -1); // ��������� ��������������� �������� OpenGL
    glEnable(GL_TEXTURE_2D); // ��������� ������ ��������������� OpenGL
    mEnu_init(); // ������������� ����
    Load_Texture("podvorotna.jpg", &bg, GL_REPEAT, GL_REPEAT, GL_NEAREST); // �������� �������� ����
    Show_Background(bg); // ����������� ����
    Load_Texture("CAT.png", &anim, GL_REPEAT, GL_REPEAT, GL_NEAREST); // �������� �������� ��������

    while (!bQuit) { // ���� ��������� ��������� � ���������, ��������������, ���� �� ���������� ���� ������ �� ���������
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // �������� ������� ��������� � ������� ���������
            if (msg.message == WM_QUIT) { // ���� �������� ��������� � ������ �� ���������
                bQuit = TRUE; // ������������� ���� ������ �� ���������
            } else { // � ��������� ������
                TranslateMessage(&msg); // �������������� ��������� ������ � ���������� ���������
                DispatchMessage(&msg); // �������� ��������� �� ��������� ���������������� ����
            }
        } else { // ���� � ������� ��������� ���
            glClear(GL_COLOR_BUFFER_BIT); // ������� ������ ����� OpenGL
            glFlush(); // �������������� ���������� ���� ������ OpenGL � ������� ���������
            Show_Background(bg); // ����������� ����

            if (menuProperties.isActive) { // ���� ������� ����
                Menu_ShowMenu(); // ����������� ����
            }
            else { // ���� ���� �� �������
                Player_Move(); // ���������� �������� ������
                Sprite_animation(anim, frame, player.frameLine, player.posX, player.posY, player.lookRight); // ����������� �������� ������
                frame >= 9 ? frame = 0 : frame += 0.2; // ������������ ������ ��������
            }
            SwapBuffers(hDC); // ������������ ������� ��� ����������� ������ �����
            Sleep(1); // �������� ����� ��������� �����������
        }
    }

    DisableOpenGL(hwnd, hDC, hRC); // ���������� OpenGL � ������������ ��������� OpenGL
    DestroyWindow(hwnd); // ����������� ����
    return msg.wParam; // ������� ���� ������ �� ���������
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { // ����������� ��������� ��������� ��������� ����
    switch (uMsg) { // �������� ���� ���������
        case WM_CLOSE: // ��������� � �������� ����
            PostQuitMessage(0); // ���������� ��������� � ������� ��������� ��� ������ �� ����������
            break;

        case WM_MOUSEMOVE: // ��������� � ����������� ����
            Menu_MouseMove(LOWORD(lParam), HIWORD(lParam)); // ��������� �������� ���� � ����
            break;

        case WM_LBUTTONDOWN: // ��������� � ������� ����� ������ ����
            MouseDown(); // ��������� ������� ������ ����
            break;

        case WM_KEYDOWN: // ��������� � ������� ������� ����������
            if (wParam == VK_ESCAPE && !menuProperties.isActive) // ���� ������ ������� Escape � ���� �� �������
                mEnu_init(); // ������������� ����
            break;

        default: // ��� ���� ��������� ���������
            return DefWindowProc(hwnd, uMsg, wParam, lParam); // ����� ����������� ��������� ��������� ����
    }
    return 0; // ������� �������� �������� �� ���������
}


