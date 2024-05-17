#include <windows.h>
#include <gl/gl.h>
#include <math.h>

float vertices[] = {1,1,0, 1,-1,0, -1,-1,0, -1,1,0};
float os_abcyss = 90;
float os_applicat = 180;

float normal_vert[]={0,0,1, 0,0,1, 0,0,1, 0,0,1};

POINTFLOAT position = {0, 0};
int Size_desk = 50;

POINT lastMousePos;
BOOL mouseButtonDown = FALSE;


void Init_Light()
{
 glEnable(GL_LIGHTING);
 glShadeModel(GL_SMOOTH);
 GLfloat light_position[] = { 0.0f, 0.0f, 5.0f, 1.0f };
 GLfloat light_spot_direction[] = {0.0, 0.0, -1.0, 1.0};
 GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
 GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
 GLfloat light_specular[] = { 0.2f, 0.2f, 0.2f, 32.0f };
 glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15);

 glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_spot_direction);
 glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 8.0);
 glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
 glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
 glEnable(GL_LIGHT0);
}


void Init_Material()
{
glEnable(GL_COLOR_MATERIAL);

 glShadeModel(GL_SMOOTH);
 GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
 GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
 GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 32.0f };
 GLfloat material_shininess[] = { 50.0f };
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
    float angleRad = -os_applicat / 180 * M_PI;
    float velosity = 0;

    if (GetKeyState('W') < 0)
    {
        velosity = 0.1;
    }

    if (GetKeyState('S') < 0)
    {
        velosity = -0.1;
    }

    if (GetKeyState('A') < 0)
    {
        velosity = 0.1;
        angleRad -= M_PI*0.5;
    }

    if (GetKeyState('D') < 0)
    {
        velosity = 0.1;
        angleRad += M_PI*0.5;
    }

    if (velosity != 0)
    {
        position.x += sin(angleRad) * velosity;
        position.y += cos(angleRad) * velosity;
    }

    glRotatef(-os_abcyss, 1, 0, 0);
    glRotatef(-os_applicat, 0, 0, 1);
    glTranslatef(-position.x, -position.y, -3);
}
float vertex[] = {-1,-1,0, 1,-1,0, 1,1,0, -1,1,0};
float normal[] = {0,0,1, 0,0,1, 0,0,1, 0,0,1};
void Light_Source()
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




void Prisms_Draw() {
    int num = 5;
    GLfloat transparency[] = {1.0f, 0.8f, 0.5f, 0.3f, 0.1f};
    GLfloat positions[5][3] = {{-5, 0, 0}, {-2, -4, 0}, {2, -4, 0}, {5, 0, 0}, {2, 4, 0}}; // Положения уменьшены в 2 раза

    for(int i = 0; i < num; ++i) {
        glPushMatrix();
        glTranslatef(positions[i][0], positions[i][1], positions[i][2]);
        glColor4f(1.0f, 0.0f, 1.0f, transparency[i]);

        //-------------------------------------------------
        const int N = 18;
        GLfloat vertices_top[N * 3];
        GLfloat vertices_bottom[N * 3];

        for(int j = 0; j < N; ++j) {
            float angle = 2.0f * M_PI * j / N;
            float x = 1.5f * cos(angle) * 0.5f; // Уменьшаем размер в 2 раза
            float y = 1.5f * sin(angle) * 0.5f; // Уменьшаем размер в 2 раза

            vertices_top[j * 3] = x;
            vertices_top[j * 3 + 1] = y;
            vertices_top[j * 3 + 2] = 1.0f;

            vertices_bottom[j * 3] = x;
            vertices_bottom[j * 3 + 1] = y;
            vertices_bottom[j * 3 + 2] = 0.0f;
        }

        glEnableClientState(GL_VERTEX_ARRAY);

        // Отображение крыши
        glVertexPointer(3, GL_FLOAT, 0, vertices_top);
        glDrawArrays(GL_TRIANGLE_FAN, 0, N);

        // Отображение основания
        glVertexPointer(3, GL_FLOAT, 0, vertices_bottom);
        glDrawArrays(GL_TRIANGLE_FAN, 0, N);

        // Отображение боковых граней
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j < N; ++j) {
            glVertex3f(vertices_top[j * 3], vertices_top[j * 3 + 1], vertices_top[j * 3 + 2]);
            glVertex3f(vertices_bottom[j * 3], vertices_bottom[j * 3 + 1], vertices_bottom[j * 3 + 2]);
        }
        // Соединяем последнюю вершину с первой, чтобы закрыть фигуру
        glVertex3f(vertices_top[0], vertices_top[1], vertices_top[2]);
        glVertex3f(vertices_bottom[0], vertices_bottom[1], vertices_bottom[2]);
        glEnd();

        // Отображение непрозрачных рёбер
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f); // Непрозрачный синий цвет
        glBegin(GL_LINE_LOOP);
        for(int j = 0; j < N; ++j) {
            glVertex3f(vertices_top[j * 3], vertices_top[j * 3 + 1], vertices_top[j * 3 + 2]);
        }
        glEnd();

        glBegin(GL_LINE_LOOP);
        for(int j = 0; j < N; ++j) {
            glVertex3f(vertices_bottom[j * 3], vertices_bottom[j * 3 + 1], vertices_bottom[j * 3 + 2]);
        }
        glEnd();

        glBegin(GL_LINES);
        for(int j = 0; j < N; ++j) {
            glVertex3f(vertices_top[j * 3], vertices_top[j * 3 + 1], vertices_top[j * 3 + 2]);
            glVertex3f(vertices_bottom[j * 3], vertices_bottom[j * 3 + 1], vertices_bottom[j * 3 + 2]);
        }
        glEnd();
        //--------------------------------------------------

        glPopMatrix();
    }
}




float theta = 0.0f;
float x, y;
float A = 5.0f;

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void SetupOpenGL(HWND, HDC*, HGLRC*);
void TearDownOpenGL(HWND, HDC, HGLRC);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX windowClass;
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
    MSG msg;

    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_OWNDC;
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = "Chess_desk";
    windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&windowClass))
    {
        return 0;
    }

    hwnd = CreateWindowEx(0,
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

    ShowWindow(hwnd, nCmdShow);
    SetupOpenGL(hwnd, &hdc, &hglrc);

    glFrustum(-1,1, -1,1, 2,80);
    glEnable(GL_DEPTH_TEST);

    BOOL EXIT = FALSE;

    while (!EXIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                EXIT = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
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

                    glScalef(0.1f, 0.1f, 0.1f);

                    glColor3f(1,1,1);
                    Init_Light();
                    Light_Source();
                glPopMatrix();

    Chess_Desk();
    glColor3f(1,0,1);
    glTranslatef(0,-2,0);
    Draw_Cube();


glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
Prisms_Draw();
glDisable(GL_BLEND);




            glPopMatrix();


            theta += 0.01f;

            SwapBuffers(hdc);

            Sleep (1);
        }
    }

    TearDownOpenGL(hwnd, hdc, hglrc);
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
        break;

    case WM_MOUSEMOVE:
        if (mouseButtonDown)
        {
            POINT currentMousePos;
            GetCursorPos(&currentMousePos);
            ScreenToClient(hwnd, &currentMousePos);

            int dx = currentMousePos.x - lastMousePos.x;
            int dy = currentMousePos.y - lastMousePos.y;

            os_applicat += dx;
            os_abcyss += dy;

            lastMousePos = currentMousePos;
        }
        break;

    case WM_MOUSEWHEEL:
        os_applicat += GET_WHEEL_DELTA_WPARAM(wParam) / 40;
        break;

    case WM_LBUTTONDOWN:
        mouseButtonDown = TRUE;
        GetCursorPos(&lastMousePos);
        ScreenToClient(hwnd, &lastMousePos);
        SetCapture(hwnd);
        break;

    case WM_LBUTTONUP:
        mouseButtonDown = FALSE;
        ReleaseCapture();
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}


void SetupOpenGL(HWND hwnd, HDC* hdc, HGLRC* hglrc)
{
    PIXELFORMATDESCRIPTOR pfd;
    int pixelFormat;

    *hdc = GetDC(hwnd);

    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    pixelFormat = ChoosePixelFormat(*hdc, &pfd);
    SetPixelFormat(*hdc, pixelFormat, &pfd);

    *hglrc = wglCreateContext(*hdc);
    wglMakeCurrent(*hdc, *hglrc);
}


void TearDownOpenGL(HWND hwnd, HDC hdc, HGLRC hglrc)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd, hdc);
}

/*
Инициализация окна: В функции WinMain создается окно Windows, которое будет использоваться для отображения графики OpenGL.

Настройка OpenGL: В функции SetupOpenGL создается контекст OpenGL и устанавливается формат пикселей.

Главный цикл программы: Основной цикл программы (while (!EXIT)) обрабатывает сообщения Windows, такие как перемещение мыши и нажатие клавиш, а также отрисовывает сцену OpenGL.

Обработка сообщений окна: В функции WindowProcedure обрабатываются сообщения окна, такие как закрытие окна или перемещение мыши.

Отрисовка сцены: Функция Camera_View обрабатывает движение камеры, а затем отрисовывает шахматную доску, используя функции OpenGL для управления матрицами, массивами вершин и настройки цветов.

Освобождение ресурсов OpenGL: По завершении работы программа освобождает ресурсы OpenGL с помощью функции TearDownOpenGL.

*/

