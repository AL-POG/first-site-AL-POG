#include <windows.h>
#include <gl/gl.h>
#include <math.h>

float vertices[] = {1,1,0, 1,-1,0, -1,-1,0, -1,1,0}; // Массив координат вершин квадрата в трехмерном пространстве
float os_abcyss = 90; // Угол поворота камеры по оси X
float os_applicat = 180; // Угол поворота камеры по оси Z

float normal_vert[]={0,0,1, 0,0,1, 0,0,1, 0,0,1};

POINTFLOAT position = {0, 0}; // Текущая позиция камеры
int Size_desk = 50; // Размер доски

POINT lastMousePos; // Последняя позиция мыши
BOOL mouseButtonDown = FALSE; // Флаг нажатия кнопки мыши


void Init_Light()
{
 glEnable(GL_LIGHTING); //общее освещения для всего пространства
 glShadeModel(GL_SMOOTH);
 GLfloat light_position[] = { 0.0f, 0.0f, 5.0f, 1.0f }; //позиция источника
 GLfloat light_spot_direction[] = {0.0, 0.0, -1.0, 1.0}; // позиция цели
 GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f }; //параметры
 GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //параметры
 GLfloat light_specular[] = { 0.2f, 0.2f, 0.2f, 32.0f }; //параметры
 glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15); // конус для
//направленного источника
 glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_spot_direction);
 glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 8.0); // экспонента
//убывания интенсивности
//задействование настроек для источника LIGHT0
 glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
 glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
 glEnable(GL_LIGHT0); // источник света LIGHT0
}


void Init_Material()
{
glEnable(GL_COLOR_MATERIAL); //разрешения использования
//материала
 glShadeModel(GL_SMOOTH); // сглаживает границы
 GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
 GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
 GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 32.0f };
 GLfloat material_shininess[] = { 50.0f }; //блеск материала
 glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
 glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
 glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
 glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);
}



void Draw_Cube(){
GLfloat vertices[] = {
 -0.5f, -0.5f, -0.5f,
 0.5f, -0.5f, -0.5f,
 0.5f, 0.5f, -0.5f,
 -0.5f, 0.5f, -0.5f,
 -0.5f, -0.5f, 0.5f,
 0.5f, -0.5f, 0.5f,
 0.5f, 0.5f, 0.5f,
 -0.5f, 0.5f, 0.5f
};
GLuint indices[] = {
 0, 1, 2,
 2, 3, 0,
 1, 5, 6,
 6, 2, 1,
 7, 6, 5,
 5, 4, 7,
 4, 0, 3,
 3, 7, 4,
 4, 5, 1,
 1, 0, 4,
 3, 2, 6,
 6, 7, 3
};
GLfloat normals[] = {
 0.0f, 0.0f, -1.0f,
 0.0f, 0.0f, -1.0f,
 0.0f, 0.0f, -1.0f,
 0.0f, 0.0f, -1.0f,
 0.0f, 0.0f, 1.0f,
 0.0f, 0.0f, 1.0f,
 0.0f, 0.0f, 1.0f,
 0.0f, 0.0f, 1.0f,
 -1.0f, 0.0f, 0.0f,
 -1.0f, 0.0f, 0.0f,
 -1.0f, 0.0f, 0.0f,
 -1.0f, 0.0f, 0.0f,
 1.0f, 0.0f, 0.0f,
 1.0f, 0.0f, 0.0f,
 1.0f, 0.0f, 0.0f,
 1.0f, 0.0f, 0.0f,
 0.0f, -1.0f, 0.0f,
 0.0f, -1.0f, 0.0f,
 0.0f, -1.0f, 0.0f,
 0.0f, -1.0f, 0.0f,
 0.0f, 1.0f, 0.0f,
 0.0f, 1.0f, 0.0f,
 0.0f, 1.0f, 0.0f,
 0.0f, 1.0f, 0.0f
};
glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, vertices);
glEnableClientState(GL_NORMAL_ARRAY);
glNormalPointer(GL_FLOAT, 0, normals);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_NORMAL_ARRAY);
}



void Camera_View()
{
    float angleRad = -os_applicat / 180 * M_PI; // Угол поворота в радианах
    float velosity = 0; // Скорость перемещения камеры

    if (GetKeyState('W') < 0)
    {
        velosity = 0.1; // Движение вперед
    }

    if (GetKeyState('S') < 0)
    {
        velosity = -0.1; // Движение назад
    }

    if (GetKeyState('A') < 0)
    {
        velosity = 0.1;
        angleRad -= M_PI*0.5; // Движение влево
    }

    if (GetKeyState('D') < 0)
    {
        velosity = 0.1;
        angleRad += M_PI*0.5; // Движение вправо
    }

    if (velosity != 0)  // Если скорость не равна нулю
    {
        position.x += sin(angleRad) * velosity; // Изменение позиции по оси X
        position.y += cos(angleRad) * velosity; // Изменение позиции по оси Y
    }

    glRotatef(-os_abcyss, 1, 0, 0); // Поворот камеры по оси X
    glRotatef(-os_applicat, 0, 0, 1); // Поворот камеры по оси Z
    glTranslatef(-position.x, -position.y, -3); // Перемещение камеры
}
float vertex[] = {-1,-1,0, 1,-1,0, 1,1,0, -1,1,0};
float normal[] = {0,0,1, 0,0,1, 0,0,1, 0,0,1};/*векторы нормали*/
void Draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glNormalPointer(GL_FLOAT, 0, normal);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}
float vert[] = {1,1,0, 1,-1,0, -1,-1,0, -1,1,0};
int Desk_size = 30;

void Chess_Desk()
{
    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vert);

        for (int i = -Desk_size; i < Desk_size; i++)
        {
            for (int j = -Desk_size; j < Desk_size; j++)
            {
                glPushMatrix();
                    if ((i+j) % 2 == 0) glColor3f(0.0, 0.0, 0.0);
                    else glColor3f(1, 1, 1);
                    glTranslatef(i*2, j*2, 0);
                    glDrawArrays(GL_TRIANGLE_FAN, 0 ,4);
                glPopMatrix();
            }
        }

        //Отрисовка линий
        glLineWidth(5);
        glBegin(GL_LINES);

            glColor3f (0, 1, 0);
            glVertex3f(0, 0, 1);
            glVertex3f(0, 1, 1);

            glColor3f (1, 0, 0);
            glVertex3f(0, 0, 1);
            glVertex3f(1, 0, 1);

            glColor3f (0, 0, 1);
            glVertex3f(0, 0, 1);
            glVertex3f(0, 0, 2);
        glEnd();

        glTranslatef(2,2,1);
        glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex2f(0.0f,   1.0f);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex2f(0.87f,  -0.5f);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex2f(-0.87f, -0.5f);
        glEnd();

    glDisableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT,0,&normal_vert);
    glDisable(GL_NORMAL_ARRAY);
}


float theta = 0.0f;
float x, y;
float A = 5.0f; // Амплитуда для траектории восьмёрки

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM); // Объявление прототипа функции обработки сообщений окна
void SetupOpenGL(HWND, HDC*, HGLRC*); // Объявление прототипа функции настройки OpenGL
void TearDownOpenGL(HWND, HDC, HGLRC); // Объявление прототипа функции освобождения ресурсов OpenGL

int WINAPI WinMain(HINSTANCE hInstance, // Основная функция WinMain, точка входа в приложение Windows
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX windowClass; // Структура для описания класса окна
    HWND hwnd; // Дескриптор окна
    HDC hdc; // Контекст устройства
    HGLRC hglrc; // Контекст OpenGL
    MSG msg; // Структура для хранения сообщений

    windowClass.cbSize = sizeof(WNDCLASSEX); // Размер структуры
    windowClass.style = CS_OWNDC; // Стиль класса окна
    windowClass.lpfnWndProc = WindowProcedure; // Указатель на функцию обработки сообщений окна
    windowClass.cbClsExtra = 0; // Дополнительные данные класса
    windowClass.cbWndExtra = 0; // Дополнительные данные окна
    windowClass.hInstance = hInstance; // Дескриптор экземпляра приложения
    windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Иконка приложения
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW); // Курсор мыши
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // Кисть для фона окна
    windowClass.lpszMenuName = NULL; // Имя меню
    windowClass.lpszClassName = "Chess_desk"; // Имя класса окна
    windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // Маленькая иконка приложения

    if (!RegisterClassEx(&windowClass)) // Регистрация класса окна
    {
        return 0;
    }

    hwnd = CreateWindowEx(0, // Создание окна
                          "Chess_desk",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1000,
                          600,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow); // Отображение окна

    SetupOpenGL(hwnd, &hdc, &hglrc); // Настройка OpenGL

    glFrustum(-1,1, -1,1, 2,80); // Установка перспективной проекции
    glEnable(GL_DEPTH_TEST); // Включение буфера глубины для работы с трехмерными координатами

    BOOL EXIT = FALSE; // Переменная для управления циклом программы

    while (!EXIT) // Основной цикл программы
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Проверка наличия сообщений в очереди
        {
            if (msg.message == WM_QUIT) // Проверка наличия сообщения о завершении программы
            {
                EXIT = TRUE;
            }
            else
            {
                TranslateMessage(&msg); // Перевод сообщения в формат для обработки
                DispatchMessage(&msg); // Отправка сообщения на обработку
            }
        }
        else
       {


            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPushMatrix();
                Camera_View();

                Init_Material();
                glPushMatrix();

                    x = A * sin(theta);
                    y = A * sin(theta) * cos(theta);
                    glTranslatef(x, y, 5);

                    glScalef(0.1f, 0.1f, 0.1f); // Изменение размера квадрата

                    glColor3f(1,1,1);
                    Init_Light();
                    Draw();
                glPopMatrix();

    Chess_Desk();
    glColor3f(1,0,1);
    glTranslatef(0,-2,0);
    Draw_Cube();

            glPopMatrix();


            theta += 0.01f; // Увеличение угла для плавного движения

            SwapBuffers(hdc);

            Sleep (1);
        }
    }

    TearDownOpenGL(hwnd, hdc, hglrc); // Освобождение ресурсов OpenGL перед закрытием окна
    DestroyWindow(hwnd); // Уничтожение окна

    return msg.wParam; // Возвращение параметра сообщения, чтобы завершить процесс
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) // Обработка сообщений окна
    {
    case WM_CLOSE: // Сообщение о закрытии окна
        PostQuitMessage(0); // Отправка сообщения о выходе из приложения
        break;

    case WM_DESTROY: // Сообщение о разрушении окна
        return 0; // Возврат нуля, чтобы завершить обработку сообщения

    case WM_KEYDOWN: // Сообщение о нажатии клавиши
        switch (wParam) // Проверка кода нажатой клавиши
        {
        case VK_ESCAPE: // Если нажата клавиша Escape
            PostQuitMessage(0); // Отправка сообщения о выходе из приложения
            break;
        }
        break;

    case WM_MOUSEMOVE: // Сообщение о перемещении мыши
        if (mouseButtonDown) // Если кнопка мыши нажата
        {
            POINT currentMousePos; // Текущая позиция мыши
            GetCursorPos(&currentMousePos); // Получение координат текущей позиции мыши
            ScreenToClient(hwnd, &currentMousePos); // Преобразование координат из экранных в клиентские

            int dx = currentMousePos.x - lastMousePos.x; // Изменение координаты по оси X
            int dy = currentMousePos.y - lastMousePos.y; // Изменение координаты по оси Y

            os_applicat += dx; // Поворот камеры вокруг оси Z
            os_abcyss += dy; // Поворот камеры вокруг оси X

            lastMousePos = currentMousePos; // Обновление последней позиции мыши
        }
        break;

    case WM_MOUSEWHEEL: // Сообщение о вращении колеса мыши
        os_applicat += GET_WHEEL_DELTA_WPARAM(wParam) / 40; // Изменение угла поворота камеры по оси Z
        break;

    case WM_LBUTTONDOWN: // Сообщение о нажатии левой кнопки мыши
        mouseButtonDown = TRUE; // Установка флага нажатия кнопки мыши в TRUE
        GetCursorPos(&lastMousePos); // Получение текущей позиции мыши
        ScreenToClient(hwnd, &lastMousePos); // Преобразование координат из экранных в клиентские
        SetCapture(hwnd); // Установка захвата мыши окном
        break;

    case WM_LBUTTONUP: // Сообщение о отпускании левой кнопки мыши
        mouseButtonDown = FALSE; // Установка флага нажатия кнопки мыши в FALSE
        ReleaseCapture(); // Освобождение захвата мыши
        break;

    default: // Для остальных сообщений
        return DefWindowProc(hwnd, uMsg, wParam, lParam); // Вызов стандартной обработки сообщений окна
    }

    return 0; // Возврат нуля, чтобы указать, что сообщение обработано
}


void SetupOpenGL(HWND hwnd, HDC* hdc, HGLRC* hglrc)
{
    PIXELFORMATDESCRIPTOR pfd; // Структура, описывающая формат пикселей
    int pixelFormat;

    *hdc = GetDC(hwnd); // Получение контекста устройства для окна

    ZeroMemory(&pfd, sizeof(pfd)); // Заполнение структуры нулями
    pfd.nSize = sizeof(pfd); // Размер структуры в байтах
    pfd.nVersion = 1; // Версия структуры
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // Флаги формата пикселей
    pfd.iPixelType = PFD_TYPE_RGBA; // Тип пикселей: RGBA
    pfd.cColorBits = 24; // Глубина цвета
    pfd.cDepthBits = 16; // Глубина буфера глубины
    pfd.iLayerType = PFD_MAIN_PLANE; // Главный слой

    pixelFormat = ChoosePixelFormat(*hdc, &pfd); // Выбор наиболее подходящего формата пикселей
    SetPixelFormat(*hdc, pixelFormat, &pfd); // Установка выбранного формата пикселей

    *hglrc = wglCreateContext(*hdc); // Создание контекста OpenGL
    wglMakeCurrent(*hdc, *hglrc); // Установка текущего контекста OpenGL для устройства
}


void TearDownOpenGL(HWND hwnd, HDC hdc, HGLRC hglrc)
{
    wglMakeCurrent(NULL, NULL); // Освобождение текущего контекста OpenGL
    wglDeleteContext(hglrc); // Удаление контекста OpenGL
    ReleaseDC(hwnd, hdc); // Освобождение контекста устройства для окна
}

/*
Инициализация окна: В функции WinMain создается окно Windows, которое будет использоваться для отображения графики OpenGL.

Настройка OpenGL: В функции SetupOpenGL создается контекст OpenGL и устанавливается формат пикселей.

Главный цикл программы: Основной цикл программы (while (!EXIT)) обрабатывает сообщения Windows, такие как перемещение мыши и нажатие клавиш, а также отрисовывает сцену OpenGL.

Обработка сообщений окна: В функции WindowProcedure обрабатываются сообщения окна, такие как закрытие окна или перемещение мыши.

Отрисовка сцены: Функция Camera_View обрабатывает движение камеры, а затем отрисовывает шахматную доску, используя функции OpenGL для управления матрицами, массивами вершин и настройки цветов.

Освобождение ресурсов OpenGL: По завершении работы программа освобождает ресурсы OpenGL с помощью функции TearDownOpenGL.

*/
