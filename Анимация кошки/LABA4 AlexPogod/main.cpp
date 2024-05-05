#include <gl/gl.h> // Подключение заголовочного файла OpenGL
#include <GL/glu.h> // Подключение заголовочного файла OpenGL Utility Library
#include <windows.h> // Подключение заголовочного файла Windows
#include <math.h> // Подключение заголовочного файла для математических функций
#include <iostream> // Подключение заголовочного файла для ввода-вывода в стандартный поток
#include <string> // Подключение заголовочного файла для работы со строками
#include <malloc.h> // Подключение заголовочного файла для работы с динамической памятью
#include <stdio.h> // Подключение заголовочного файла для стандартных ввода-вывода
#include <stdlib.h> // Подключение заголовочного файла для стандартной библиотеки
#include "stb-master/stb_easy_font.h" // Подключение заголовочного файла для вывода текста с помощью stb_easy_font
#include "main.h" // Подключение заголовочного файла с объявлением структур и функций

using namespace std; // Использование пространства имен std

//------------------------------------------------------

Button *btn = NULL; // Объявление указателя на массив кнопок и инициализация его значением NULL.
int btnCnt = 0; // Объявление переменной для хранения количества кнопок и инициализация ее нулем.

float mouseX, mouseY; // Объявление переменных для хранения координат курсора мыши.
int animationLine = 0; // Объявление переменной для анимации линии и инициализация ее нулем.

int Menu_AddButton(char *name, float x, float y, float width, float height, float textS)   // Определение функции для добавления кнопки в меню.
{
    btnCnt++; // Увеличение счетчика кнопок.

    // Перевыделение памяти под массив кнопок с учетом новой кнопки.
    btn = (Button*)realloc(btn, sizeof(btn[0]) * btnCnt);

    // Копирование имени кнопки в соответствующее поле структуры кнопки.
    snprintf(btn[btnCnt - 1].name, 50, "%s", name);

    // Получение указателя на вершины прямоугольника кнопки.
    float *vert = btn[btnCnt - 1].vert;

    // Установка координат вершин прямоугольника кнопки.
    vert[0] = vert[6] = x;
    vert[2] = vert[4] = x + width;
    vert[1] = vert[3] = y;
    vert[5] = vert[7] = y + height;

    // Установка флагов наведения и нажатия кнопки в исходное состояние (0 - не наведена, не нажата).
    btn[btnCnt - 1].isHover = 0;
    btn[btnCnt - 1].isDown = 0;

    // Получение указателя на добавленную кнопку.
    Button *b = &btn[btnCnt - 1];

    // Вычисление количества квадратов для отображения текста кнопки и сохранение результата в структуру кнопки.
    b->num_quads = stb_easy_font_print(0, 0, name, 0, b->buffer, sizeof(b->buffer));

    // Вычисление позиции для отображения текста на кнопке и сохранение координат в структуру кнопки.
    b->textPosX = x + (width - stb_easy_font_width(name) * textS) / 2.0;
    b->textPosY = y + (height - stb_easy_font_height(name) * textS) / 2.0 + textS * 2;

    // Сохранение размера текста в структуру кнопки.
    b->textS = textS;

    // Возвращение индекса добавленной кнопки.
    return btnCnt - 1;
}

void ShowButton(int buttonId)   // Определение функции для отображения кнопки по ее идентификатору.
{
    Button btn1 = btn[buttonId]; // Получение копии структуры кнопки с помощью ее идентификатора.

    // Установка указателя на массив вершин прямоугольника кнопки и включение использования массива вершин.
    glVertexPointer(2, GL_FLOAT, 0, btn1.vert);
    glEnableClientState(GL_VERTEX_ARRAY);

    // Установка цвета для отрисовки прямоугольника кнопки.
    glColor3f(1, 1, 1);

    // Отрисовка прямоугольника кнопки с помощью массива вершин.
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // Сохранение текущей матрицы и перемещение в указанную позицию для отображения текста.
    glPushMatrix();
    glTranslatef(btn1.textPosX, btn1.textPosY, 0);
    glScalef(btn1.textS, btn1.textS, 0);

    // Установка цвета для отображения текста.
    glColor3f(0, 0, 0);

    // Установка указателя на массив вершин текста и отрисовка текста кнопки с помощью массива вершин.
    glVertexPointer(2, GL_FLOAT, 16, btn1.buffer);
    glDrawArrays(GL_QUADS, 0, btn1.num_quads * 4);

    // Восстановление предыдущей матрицы.
    glPopMatrix();

    // Восстановление цвета для последующей отрисовки.
    glColor3f(1, 1, 1);
}

void Menu_ShowMenu()   // Функция для отображения всех кнопок в меню.
{
    for (int i = 0; i < btnCnt; i++)   // Цикл по всем кнопкам в меню.
    {
        ShowButton(i); // Вызов функции отображения кнопки.
    }
}

char isCordInButton(int buttonID, float x, float y)   // Функция для проверки, находятся ли координаты внутри кнопки.
{
    float *vert = btn[buttonID].vert; // Получение массива вершин кнопки по ее идентификатору.
    return (x > vert[0]) && (y > vert[1]) && (x < vert[4]) && (y < vert[5]); // Возвращение значения, указывающего на наличие координат внутри кнопки.
}

int Menu_MouseMove(float x, float y)   // Функция для обработки движения мыши над кнопками в меню.
{
    mouseX = x; // Обновление координаты x мыши.
    mouseY = y; // Обновление координаты y мыши.
    int movebtn = -1; // Переменная для хранения индекса кнопки, над которой произведено движение мыши.
    for (int i = 0; i < btnCnt; i++)   // Цикл по всем кнопкам в меню.
    {
        if (isCordInButton(i, mouseX, mouseY))   // Проверка, находятся ли текущие координаты мыши внутри кнопки.
        {
            btn[i].isHover = 1; // Установка флага наведения на кнопку.
            movebtn = i; // Сохранение индекса кнопки, над которой произведено движение мыши.
        }
        else
        {
            btn[i].isDown = 0; // Сброс флага нажатия кнопки.
            btn[i].isHover = 0; // Сброс флага наведения на кнопку.
        }
    }
    return movebtn; // Возврат индекса кнопки, над которой произведено движение мыши (-1, если движения не было).
}

int Menu_MouseDown()   // Функция для обработки нажатия кнопок мыши в меню.
{
    int downbtn = -1; // Переменная для хранения индекса кнопки, нажатой мышью.
    for (int i = 0; i < btnCnt; i++)   // Цикл по всем кнопкам в меню.
    {
        if (isCordInButton(i, mouseX, mouseY))   // Проверка, находятся ли текущие координаты мыши внутри кнопки.
        {
            btn[i].isDown = 1; // Установка флага нажатия кнопки.
            downbtn = i; // Сохранение индекса нажатой кнопки.
        }
    }
    return downbtn; // Возврат индекса нажатой кнопки (-1, если ни одна кнопка не была нажата).
}

void Menu_changeState()   // Функция для изменения состояния меню (активное/неактивное).
{
    menuProperties.isActive = !menuProperties.isActive; // Инвертирование состояния активности меню.
}

void Menu_Clear()   // Функция для очистки меню.
{
    btnCnt = 0; // Сброс счетчика количества кнопок в меню.
    free(btn); // Освобождение памяти, выделенной под массив кнопок.
    btn = NULL; // Установка указателя на массив кнопок в NULL.
}

void MouseDown()   // Функция для обработки нажатия кнопок мыши.
{
    if (int buttonId = Menu_MouseDown(); buttonId == 0)   // Проверка, если нажата кнопка с индексом 0.
    {
        Menu_Clear(); // Очистка меню.
        Menu_changeState(); // Изменение состояния меню (активное/неактивное).
    }
}

//------------------------------

GLuint anim; // Идентификатор текстуры для анимации.
GLuint bg; // Идентификатор текстуры для заднего фона.
float frame; // Текущий кадр анимации.
Menu menuProperties; // Свойства меню.
Player player; // Объект игрока.
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // Прототип функции обработки сообщений окна.

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)   // Функция для отключения OpenGL.
{
    wglMakeCurrent(NULL, NULL); // Снятие текущего контекста OpenGL.
    wglDeleteContext(hRC); // Удаление контекста OpenGL.
    ReleaseDC(hwnd, hDC); // Освобождение контекста устройства для окна.
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)   // Определение функции для включения OpenGL
{
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



void Menu_Init()
{
    Menu_AddButton("Go", 300, 300, 150, 200, 10); // Добавление кнопки "Go" в меню с указанными параметрами.
    Menu_changeState(); // Изменение состояния меню.
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    // Определение переменных и структур для работы с окном
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;

    // Заполнение структуры WNDCLASSEX для регистрации оконного класса
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC; // Установка стиля окна для собственного контекста устройства отображения
    wcex.lpfnWndProc = WindowProc; // Установка функции обработки сообщений окна
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance; // Установка дескриптора экземпляра приложения
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Загрузка значка окна
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // Загрузка курсора мыши
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // Установка цвета фона окна
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "CAT_level"; // Уникальное имя класса окна
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // Загрузка маленького значка окна

    // Регистрация оконного класса
    if (!RegisterClassEx(&wcex))
        return 0;

    // Создание окна
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

    // Отображение окна
    ShowWindow(hwnd, nCmdShow);

    // Включение OpenGL
    EnableOpenGL(hwnd, &hDC, &hRC);

    RECT rct;
    // Получение размеров клиентской области окна
    GetClientRect(hwnd, &rct);
    // Установка ортографической проекции
    glOrtho(0, rct.right, rct.bottom, 0, 1, -1);
    // Включение текстур
    glEnable(GL_TEXTURE_2D);
    // Инициализация меню
    Menu_Init();
    // Загрузка и отображение заднего фона
    Load_Texture("podvorotna.jpg", &bg, GL_REPEAT, GL_REPEAT, GL_NEAREST);
    Background(bg);
    // Загрузка и отображение анимации
    Load_Texture("CAT.png", &anim, GL_REPEAT, GL_REPEAT, GL_NEAREST);

    // Основной цикл обработки сообщений окна
    while (!bQuit)
    {
        // Проверка наличия сообщений в очереди
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            // Обработка сообщений из очереди
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE; // Установка флага выхода из цикла
            }
            else
            {
                TranslateMessage(&msg); // Перевод сообщения в клавиатурные сообщения
                DispatchMessage(&msg); // Отправка сообщения на обработку
            }
        }
        else
        {
            // Отрисовка сцены
            glClear(GL_COLOR_BUFFER_BIT); // Очистка буфера цвета
            glFlush(); // Очистка буфера отрисовки
            Background(bg); // Отображение заднего фона

            // Проверка активности меню
            if (menuProperties.isActive)
            {
                Menu_ShowMenu(); // Отображение меню
            }
            else
            {
                Player_Move(); // Перемещение игрока
                // Отображение анимации
                Sprite_animation(anim, frame, player.frameLine, player.posX, player.posY, player.lookRight);
                frame >= 9 ? frame = 0 : frame += 0.2; // Обновление кадра анимации

                Draw_CollisionBlocks(); // Отображение столкновений
            }
            SwapBuffers(hDC); // Обмен передним и задним буферами
            Sleep(1); // Пауза для управления скоростью обновления кадров
        }
    }

    // Выключение OpenGL
    DisableOpenGL(hwnd, hDC, hRC);
    // Уничтожение окна
    DestroyWindow(hwnd);
    return msg.wParam; // Возвращение параметра сообщения
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // Обработка сообщений окна
    switch (uMsg)
    {
    // Сообщение о закрытии окна
    case WM_CLOSE:
        PostQuitMessage(0); // Отправка сообщения о выходе из приложения в главный цикл сообщений
        break;

    // Сообщение о перемещении мыши
    case WM_MOUSEMOVE:
        Menu_MouseMove(LOWORD(lParam), HIWORD(lParam)); // Вызов функции обработки движения мыши в меню
        break;

    // Сообщение о нажатии кнопки мыши
    case WM_LBUTTONDOWN:
        MouseDown(); // Вызов функции обработки нажатия кнопки мыши
        break;

    // Сообщение о нажатии клавиши
    case WM_KEYDOWN:
        // Если нажата клавиша Escape и меню не активно
        if (wParam == VK_ESCAPE && !menuProperties.isActive)
            Menu_Init(); // Инициализация меню
        break;

    // Обработка остальных сообщений
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam); // Передача сообщения по умолчанию
    }
    return 0; // Возврат значения по умолчанию
}


