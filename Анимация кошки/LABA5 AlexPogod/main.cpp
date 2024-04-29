#include <windows.h>
#include <gl/gl.h>
#include <math.h>

float vertices[] = {1,1,0, 1,-1,0, -1,-1,0, -1,1,0};
float os_abcyss = 90;
float os_applicat = 180;
POINTFLOAT position = {0, 0};
int Size_desk = 50;

POINT lastMousePos; // Последняя позиция мыши
BOOL mouseButtonDown = FALSE; // Флаг нажатия кнопки мыши

void Camera_View()
{

    float angleRad = -os_applicat / 180 * M_PI; // Перевод угла поворота в радианы

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

            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, 0, &vertices);

            for (int i = -Size_desk; i < Size_desk; i++)
            {
                for (int j = -Size_desk; j < Size_desk; j++)
                {
                    glPushMatrix();
                    if ((i+j) % 2 == 0) glColor3f(0, 0, 0);
                    else glColor3f(1, 1, 1);
                    glTranslatef(i*2, j*2, 0);
                    glDrawArrays(GL_TRIANGLE_FAN, 0,4);
                    glPopMatrix();
                }
            }

            glLineWidth(5);
            glBegin(GL_LINES);
            glColor3f(1,0,0);
            glVertex3f(0, 0, 1);
            glVertex3f(1, 0, 1);

            glColor3f(0,1,0);
            glVertex3f(0, 0, 1);
            glVertex3f(0, 1, 1);

            glColor3f(0,0,1);
            glVertex3f(0, 0, 1);
            glVertex3f(0, 0, 2);
            glEnd();

            glDisableClientState(GL_VERTEX_ARRAY);
            glPopMatrix();

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

            os_applicat += dx; // Поворот камеры вокруг оси Z
            os_abcyss += dy; // Поворот камеры вокруг оси X

            lastMousePos = currentMousePos;
        }
        break;

    case WM_MOUSEWHEEL:
        os_applicat += GET_WHEEL_DELTA_WPARAM(wParam) / 40; // Изменение угла поворота камеры по оси Z
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

