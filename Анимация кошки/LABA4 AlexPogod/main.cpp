#include <gl/gl.h>
#include <windows.h> // Включение заголовочного файла для работы с Windows API
#include <math.h>
#include <iostream>
#include <string>
#include <malloc.h> // Включение заголовочного файла для динамического выделения памяти
#include <stdio.h> // Включение заголовочного файла для стандартного ввода-вывода
#include <stdlib.h> // Включение заголовочного файла для стандартной библиотеки функций
#include "stb-master/stb_easy_font.h" // Включение заголовочного файла для использования простого шрифта из библиотеки STB
#include "main.h"

using namespace std;

//------------------------------------------------------

Button *btn = NULL; // Объявление указателя на массив кнопок и его инициализация значением NULL
int btnCnt = 0; // Объявление переменной для хранения количества кнопок и ее инициализация значением 0

float mouseX, mouseY; // Объявление переменных для хранения координат мыши
int animationLine = 0; // Объявление переменной для хранения номера строки анимации

int Menu_AddButton(char *name, float x, float y, float width, float height, float textS) { // Объявление функции для добавления кнопки в меню
    btnCnt++; // Увеличение счетчика кнопок
    btn = (Button*)realloc(btn, sizeof(btn[0]) * btnCnt); // Перевыделение памяти под массив кнопок

    snprintf(btn[btnCnt - 1].name, 50, "%s", name); // Копирование имени кнопки в структуру кнопки
    float *vert = btn[btnCnt - 1].vert; // Получение указателя на массив вершин кнопки
    vert[0] = vert[6] = x; // Установка координаты X для вершин кнопки
    vert[2] = vert[4] = x + width; // Установка координаты X для вершин кнопки
    vert[1] = vert[3] = y; // Установка координаты Y для вершин кнопки
    vert[5] = vert[7] = y + height; // Установка координаты Y для вершин кнопки
    btn[btnCnt - 1].isHover = 0; // Установка флага наведения мыши на кнопку в значение 0
    btn[btnCnt - 1].isDown = 0; // Установка флага нажатия кнопки в значение 0

    Button *b = &btn[btnCnt - 1]; // Получение указателя на текущую кнопку
    b->num_quads = stb_easy_font_print(0, 0, name, 0, b->buffer, sizeof(b->buffer)); // Генерация текстуры для вывода имени кнопки
    b->textPosX = x + (width - stb_easy_font_width(name) * textS) / 2.0; // Вычисление позиции текста по оси X
    b->textPosY = y + (height - stb_easy_font_height(name) * textS) / 2.0 + textS * 2; // Вычисление позиции текста по оси Y
    b->textS = textS; // Установка размера текста

    return btnCnt - 1; // Возврат индекса добавленной кнопки
}

void ShowButton(int buttonId) { // Объявление функции для отображения кнопки по ее идентификатору
    Button btn1 = btn[buttonId]; // Получение копии структуры кнопки по ее идентификатору
    glVertexPointer(2, GL_FLOAT, 0, btn1.vert); // Установка указателя на массив вершин кнопки для отображения
    glEnableClientState(GL_VERTEX_ARRAY); // Включение массива вершин
    glColor3f(1, 1, 1); // Установка цвета для отображения кнопки
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Отображение кнопки как четырехугольника
    glPushMatrix(); // Сохранение текущей матрицы
    glTranslatef(btn1.textPosX, btn1.textPosY, 0); // Установка позиции текста кнопки
    glScalef(btn1.textS, btn1.textS, 0); // Масштабирование текста кнопки
    glColor3f(0, 0, 0); // Установка цвета текста
    glVertexPointer(2, GL_FLOAT, 16, btn1.buffer); // Установка указателя на массив вершин текста для отображения
    glDrawArrays(GL_QUADS, 0, btn1.num_quads * 4); // Отображение текста как четырехугольников
    glPopMatrix(); // Восстановление предыдущей матрицы
    glColor3f(1, 1, 1); // Установка цвета по умолчанию
}

void Menu_ShowMenu() { // Объявление функции для отображения меню
    for (int i = 0; i < btnCnt; i++) { // Цикл по всем кнопкам в меню
        ShowButton(i); // Отображение текущей кнопки
    }
}

char isCordInButton(int buttonID, float x, float y) { // Объявление функции для определения, находятся ли координаты внутри кнопки
    float *vert = btn[buttonID].vert; // Получение указателя на массив вершин текущей кнопки
    return (x > vert[0]) && (y > vert[1]) && (x < vert[4]) && (y < vert[5]); // Проверка, находятся ли координаты внутри прямоугольника, образованного вершинами кнопки
}

int Menu_MouseMove(float x, float y) { // Объявление функции для обработки движения мыши
    mouseX = x; // Сохранение текущей координаты X мыши
    mouseY = y; // Сохранение текущей координаты Y мыши
    int movebtn = -1; // Инициализация переменной для хранения идентификатора кнопки, над которой находится курсор мыши
    for (int i = 0; i < btnCnt; i++) { // Цикл по всем кнопкам в меню
        if (isCordInButton(i, mouseX, mouseY)) { // Проверка, находится ли курсор мыши над текущей кнопкой
            btn[i].isHover = 1; // Установка флага наведения мыши на текущую кнопку
            movebtn = i; // Сохранение идентификатора текущей кнопки
        } else { // Если курсор мыши не находится над текущей кнопкой
            btn[i].isDown = 0; // Сброс флага нажатия кнопки
            btn[i].isHover = 0; // Сброс флага наведения мыши
        }
    }
    return movebtn; // Возврат идентификатора кнопки, над которой находится курсор мыши
}

int Menu_MouseDown() { // Объявление функции для обработки нажатия кнопок мыши
    int downbtn = -1; // Инициализация переменной для хранения идентификатора кнопки, на которую было произведено нажатие
    for (int i = 0; i < btnCnt; i++) { // Цикл по всем кнопкам в меню
        if (isCordInButton(i, mouseX, mouseY)) { // Проверка, находится ли курсор мыши над текущей кнопкой
            btn[i].isDown = 1; // Установка флага нажатия кнопки
            downbtn = i; // Сохранение идентификатора текущей кнопки
        }
    }
    return downbtn; // Возврат идентификатора кнопки, на которую было произведено нажатие
}

void Menu_changeState() { // Объявление функции для изменения состояния меню
    menuProperties.isActive = !menuProperties.isActive; // Инвертирование значения флага активности меню
}

void Menu_Clear() { // Объявление функции для очистки меню
    btnCnt = 0; // Сброс счетчика кнопок
    free(btn); // Освобождение памяти, выделенной для массива кнопок
    btn = NULL; // Установка указателя на массив кнопок в NULL
}

void MouseDown() { // Объявление функции для обработки нажатия кнопки мыши
    if (int buttonId = Menu_MouseDown(); buttonId == 0) { // Проверка, было ли нажато на нулевую кнопку в меню
        Menu_Clear(); // Очистка меню
        Menu_changeState(); // Изменение состояния меню
    }
}

//------------------------------

GLuint anim; // Объявление переменной для хранения идентификатора текстуры анимации
GLuint bg; // Объявление переменной для хранения идентификатора текстуры фона
float frame; // Объявление переменной для хранения номера текущего кадра анимации
Menu menuProperties; // Объявление переменной для хранения свойств меню
Player player; // Объявление переменной для хранения данных о игроке

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM); // Прототип функции для обработки сообщений окна

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC) { // Определение функции для выключения OpenGL
    wglMakeCurrent(NULL, NULL); // Сброс текущего контекста OpenGL
    wglDeleteContext(hRC); // Удаление контекста OpenGL
    ReleaseDC(hwnd, hDC); // Освобождение контекста устройства
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

void mEnu_init() { // Определение функции для инициализации меню
    Menu_AddButton("Go", 300, 300, 150, 200, 10); // Добавление кнопки "Go" в меню
    Menu_changeState(); // Изменение состояния меню
}

int WINAPI WinMain(HINSTANCE hInstance, // Основная функция WinMain, точка входа в приложение Windows
                   HINSTANCE hPrevInstance, // Предыдущий экземпляр приложения (не используется в современных версиях Windows)
                   LPSTR lpCmdLine, // Строка командной строки
                   int nCmdShow) { // Флаги отображения окна

    WNDCLASSEX wcex; // Структура для описания класса окна
    HWND hwnd; // Дескриптор окна
    HDC hDC; // Контекст устройства
    HGLRC hRC; // Контекст OpenGL
    MSG msg; // Структура для хранения сообщений
    BOOL bQuit = FALSE; // Флаг завершения приложения

    wcex.cbSize = sizeof(WNDCLASSEX); // Размер структуры WNDCLASSEX
    wcex.style = CS_OWNDC; // Стиль класса окна (собственный контекст устройства для каждого окна)
    wcex.lpfnWndProc = WindowProc; // Указатель на функцию обработки сообщений окна
    wcex.cbClsExtra = 0; // Дополнительные данные класса окна (не используются)
    wcex.cbWndExtra = 0; // Дополнительные данные окна (не используются)
    wcex.hInstance = hInstance; // Дескриптор текущего экземпляра приложения
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Иконка приложения
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW); // Курсор мыши
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // Кисть для закраски фона
    wcex.lpszMenuName = NULL; // Имя меню (не используется)
    wcex.lpszClassName = "CAT_level"; // Имя класса окна
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // Маленькая иконка приложения

    if (!RegisterClassEx(&wcex)) // Регистрация класса окна
        return 0; // В случае ошибки регистрации возвращаем 0

    hwnd = CreateWindowEx(0, // Расширенные стили окна (не используются)
                          "CAT_level", // Имя класса окна
                          "LEVEL_CAT_JUMP", // Заголовок окна
                          WS_OVERLAPPEDWINDOW, // Стиль окна (стандартное окно с заголовком, рамкой, кнопками управления)
                          CW_USEDEFAULT, // Положение окна по оси X (используется системное значение по умолчанию)
                          CW_USEDEFAULT, // Положение окна по оси Y (используется системное значение по умолчанию)
                          1024, // Ширина окна
                          678, // Высота окна
                          NULL, // Дескриптор родительского окна (отсутствует)
                          NULL, // Дескриптор меню (отсутствует)
                          hInstance, // Дескриптор экземпляра приложения
                          NULL); // Дополнительные параметры (отсутствуют)

    ShowWindow(hwnd, nCmdShow); // Отображение окна
    EnableOpenGL(hwnd, &hDC, &hRC); // Включение OpenGL и создание контекста OpenGL для окна

    RECT rct; // Прямоугольник для хранения размеров клиентской области окна
    GetClientRect(hwnd, &rct); // Получение размеров клиентской области окна
    glOrtho(0, rct.right, rct.bottom, 0, 1, -1); // Установка ортографической проекции OpenGL
    glEnable(GL_TEXTURE_2D); // Включение режима текстурирования OpenGL
    mEnu_init(); // Инициализация меню
    Load_Texture("podvorotna.jpg", &bg, GL_REPEAT, GL_REPEAT, GL_NEAREST); // Загрузка текстуры фона
    Show_Background(bg); // Отображение фона
    Load_Texture("CAT.png", &anim, GL_REPEAT, GL_REPEAT, GL_NEAREST); // Загрузка текстуры анимации

    while (!bQuit) { // Цикл обработки сообщений и отрисовки, продолжающийся, пока не установлен флаг выхода из программы
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // Проверка наличия сообщений в очереди сообщений
            if (msg.message == WM_QUIT) { // Если получено сообщение о выходе из программы
                bQuit = TRUE; // Устанавливаем флаг выхода из программы
            } else { // В противном случае
                TranslateMessage(&msg); // Преобразование сообщения клавиш в символьные сообщения
                DispatchMessage(&msg); // Отправка сообщения на обработку соответствующему окну
            }
        } else { // Если в очереди сообщений нет
            glClear(GL_COLOR_BUFFER_BIT); // Очистка буфера кадра OpenGL
            glFlush(); // Принудительное выполнение всех команд OpenGL в текущем контексте
            Show_Background(bg); // Отображение фона

            if (menuProperties.isActive) { // Если активно меню
                Menu_ShowMenu(); // Отображение меню
            }
            else { // Если меню не активно
                Player_Move(); // Выполнение движения игрока
                Sprite_animation(anim, frame, player.frameLine, player.posX, player.posY, player.lookRight); // Отображение анимации игрока
                frame >= 9 ? frame = 0 : frame += 0.2; // Переключение кадров анимации
            }
            SwapBuffers(hDC); // Переключение буферов для отображения нового кадра
            Sleep(1); // Задержка перед следующим обновлением
        }
    }

    DisableOpenGL(hwnd, hDC, hRC); // Выключение OpenGL и освобождение контекста OpenGL
    DestroyWindow(hwnd); // Уничтожение окна
    return msg.wParam; // Возврат кода выхода из программы
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { // Определение процедуры обработки сообщений окна
    switch (uMsg) { // Проверка типа сообщения
        case WM_CLOSE: // Сообщение о закрытии окна
            PostQuitMessage(0); // Постановка сообщения в очередь сообщений для выхода из приложения
            break;

        case WM_MOUSEMOVE: // Сообщение о перемещении мыши
            Menu_MouseMove(LOWORD(lParam), HIWORD(lParam)); // Обработка движения мыши в меню
            break;

        case WM_LBUTTONDOWN: // Сообщение о нажатии левой кнопки мыши
            MouseDown(); // Обработка нажатия кнопки мыши
            break;

        case WM_KEYDOWN: // Сообщение о нажатии клавиши клавиатуры
            if (wParam == VK_ESCAPE && !menuProperties.isActive) // Если нажата клавиша Escape и меню не активно
                mEnu_init(); // Инициализация меню
            break;

        default: // Для всех остальных сообщений
            return DefWindowProc(hwnd, uMsg, wParam, lParam); // Вызов стандартной обработки сообщений окна
    }
    return 0; // Возврат нулевого значения по умолчанию
}


