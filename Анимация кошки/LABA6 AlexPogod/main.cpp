#include <windows.h>
#include <gl/gl.h>
#include <math.h>

float vertices[] = {1,1,0, 1,-1,0, -1,-1,0, -1,1,0}; // ������ ��������� ������ �������� � ���������� ������������
float os_abcyss = 90; // ���� �������� ������ �� ��� X
float os_applicat = 180; // ���� �������� ������ �� ��� Z

float normal_vert[]={0,0,1, 0,0,1, 0,0,1, 0,0,1};

POINTFLOAT position = {0, 0}; // ������� ������� ������
int Size_desk = 50; // ������ �����

POINT lastMousePos; // ��������� ������� ����
BOOL mouseButtonDown = FALSE; // ���� ������� ������ ����


void Init_Light()
{
 glEnable(GL_LIGHTING); //����� ��������� ��� ����� ������������
 glShadeModel(GL_SMOOTH);
 GLfloat light_position[] = { 0.0f, 0.0f, 5.0f, 1.0f }; //������� ���������
 GLfloat light_spot_direction[] = {0.0, 0.0, -1.0, 1.0}; // ������� ����
 GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f }; //���������
 GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //���������
 GLfloat light_specular[] = { 0.2f, 0.2f, 0.2f, 32.0f }; //���������
 glLightfv(GL_LIGHT0, GL_POSITION, light_position);
glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15); // ����� ���
//������������� ���������
 glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_spot_direction);
 glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 8.0); // ����������
//�������� �������������
//�������������� �������� ��� ��������� LIGHT0
 glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
 glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
 glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
 glEnable(GL_LIGHT0); // �������� ����� LIGHT0
}


void Init_Material()
{
glEnable(GL_COLOR_MATERIAL); //���������� �������������
//���������
 glShadeModel(GL_SMOOTH); // ���������� �������
 GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
 GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
 GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 32.0f };
 GLfloat material_shininess[] = { 50.0f }; //����� ���������
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
    float angleRad = -os_applicat / 180 * M_PI; // ���� �������� � ��������
    float velosity = 0; // �������� ����������� ������

    if (GetKeyState('W') < 0)
    {
        velosity = 0.1; // �������� ������
    }

    if (GetKeyState('S') < 0)
    {
        velosity = -0.1; // �������� �����
    }

    if (GetKeyState('A') < 0)
    {
        velosity = 0.1;
        angleRad -= M_PI*0.5; // �������� �����
    }

    if (GetKeyState('D') < 0)
    {
        velosity = 0.1;
        angleRad += M_PI*0.5; // �������� ������
    }

    if (velosity != 0)  // ���� �������� �� ����� ����
    {
        position.x += sin(angleRad) * velosity; // ��������� ������� �� ��� X
        position.y += cos(angleRad) * velosity; // ��������� ������� �� ��� Y
    }

    glRotatef(-os_abcyss, 1, 0, 0); // ������� ������ �� ��� X
    glRotatef(-os_applicat, 0, 0, 1); // ������� ������ �� ��� Z
    glTranslatef(-position.x, -position.y, -3); // ����������� ������
}
float vertex[] = {-1,-1,0, 1,-1,0, 1,1,0, -1,1,0};
float normal[] = {0,0,1, 0,0,1, 0,0,1, 0,0,1};/*������� �������*/
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

        //��������� �����
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
float A = 5.0f; // ��������� ��� ���������� ��������

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM); // ���������� ��������� ������� ��������� ��������� ����
void SetupOpenGL(HWND, HDC*, HGLRC*); // ���������� ��������� ������� ��������� OpenGL
void TearDownOpenGL(HWND, HDC, HGLRC); // ���������� ��������� ������� ������������ �������� OpenGL

int WINAPI WinMain(HINSTANCE hInstance, // �������� ������� WinMain, ����� ����� � ���������� Windows
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX windowClass; // ��������� ��� �������� ������ ����
    HWND hwnd; // ���������� ����
    HDC hdc; // �������� ����������
    HGLRC hglrc; // �������� OpenGL
    MSG msg; // ��������� ��� �������� ���������

    windowClass.cbSize = sizeof(WNDCLASSEX); // ������ ���������
    windowClass.style = CS_OWNDC; // ����� ������ ����
    windowClass.lpfnWndProc = WindowProcedure; // ��������� �� ������� ��������� ��������� ����
    windowClass.cbClsExtra = 0; // �������������� ������ ������
    windowClass.cbWndExtra = 0; // �������������� ������ ����
    windowClass.hInstance = hInstance; // ���������� ���������� ����������
    windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // ������ ����������
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW); // ������ ����
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // ����� ��� ���� ����
    windowClass.lpszMenuName = NULL; // ��� ����
    windowClass.lpszClassName = "Chess_desk"; // ��� ������ ����
    windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // ��������� ������ ����������

    if (!RegisterClassEx(&windowClass)) // ����������� ������ ����
    {
        return 0;
    }

    hwnd = CreateWindowEx(0, // �������� ����
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

    ShowWindow(hwnd, nCmdShow); // ����������� ����

    SetupOpenGL(hwnd, &hdc, &hglrc); // ��������� OpenGL

    glFrustum(-1,1, -1,1, 2,80); // ��������� ������������� ��������
    glEnable(GL_DEPTH_TEST); // ��������� ������ ������� ��� ������ � ����������� ������������

    BOOL EXIT = FALSE; // ���������� ��� ���������� ������ ���������

    while (!EXIT) // �������� ���� ���������
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // �������� ������� ��������� � �������
        {
            if (msg.message == WM_QUIT) // �������� ������� ��������� � ���������� ���������
            {
                EXIT = TRUE;
            }
            else
            {
                TranslateMessage(&msg); // ������� ��������� � ������ ��� ���������
                DispatchMessage(&msg); // �������� ��������� �� ���������
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

                    glScalef(0.1f, 0.1f, 0.1f); // ��������� ������� ��������

                    glColor3f(1,1,1);
                    Init_Light();
                    Draw();
                glPopMatrix();

    Chess_Desk();
    glColor3f(1,0,1);
    glTranslatef(0,-2,0);
    Draw_Cube();

            glPopMatrix();


            theta += 0.01f; // ���������� ���� ��� �������� ��������

            SwapBuffers(hdc);

            Sleep (1);
        }
    }

    TearDownOpenGL(hwnd, hdc, hglrc); // ������������ �������� OpenGL ����� ��������� ����
    DestroyWindow(hwnd); // ����������� ����

    return msg.wParam; // ����������� ��������� ���������, ����� ��������� �������
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) // ��������� ��������� ����
    {
    case WM_CLOSE: // ��������� � �������� ����
        PostQuitMessage(0); // �������� ��������� � ������ �� ����������
        break;

    case WM_DESTROY: // ��������� � ���������� ����
        return 0; // ������� ����, ����� ��������� ��������� ���������

    case WM_KEYDOWN: // ��������� � ������� �������
        switch (wParam) // �������� ���� ������� �������
        {
        case VK_ESCAPE: // ���� ������ ������� Escape
            PostQuitMessage(0); // �������� ��������� � ������ �� ����������
            break;
        }
        break;

    case WM_MOUSEMOVE: // ��������� � ����������� ����
        if (mouseButtonDown) // ���� ������ ���� ������
        {
            POINT currentMousePos; // ������� ������� ����
            GetCursorPos(&currentMousePos); // ��������� ��������� ������� ������� ����
            ScreenToClient(hwnd, &currentMousePos); // �������������� ��������� �� �������� � ����������

            int dx = currentMousePos.x - lastMousePos.x; // ��������� ���������� �� ��� X
            int dy = currentMousePos.y - lastMousePos.y; // ��������� ���������� �� ��� Y

            os_applicat += dx; // ������� ������ ������ ��� Z
            os_abcyss += dy; // ������� ������ ������ ��� X

            lastMousePos = currentMousePos; // ���������� ��������� ������� ����
        }
        break;

    case WM_MOUSEWHEEL: // ��������� � �������� ������ ����
        os_applicat += GET_WHEEL_DELTA_WPARAM(wParam) / 40; // ��������� ���� �������� ������ �� ��� Z
        break;

    case WM_LBUTTONDOWN: // ��������� � ������� ����� ������ ����
        mouseButtonDown = TRUE; // ��������� ����� ������� ������ ���� � TRUE
        GetCursorPos(&lastMousePos); // ��������� ������� ������� ����
        ScreenToClient(hwnd, &lastMousePos); // �������������� ��������� �� �������� � ����������
        SetCapture(hwnd); // ��������� ������� ���� �����
        break;

    case WM_LBUTTONUP: // ��������� � ���������� ����� ������ ����
        mouseButtonDown = FALSE; // ��������� ����� ������� ������ ���� � FALSE
        ReleaseCapture(); // ������������ ������� ����
        break;

    default: // ��� ��������� ���������
        return DefWindowProc(hwnd, uMsg, wParam, lParam); // ����� ����������� ��������� ��������� ����
    }

    return 0; // ������� ����, ����� �������, ��� ��������� ����������
}


void SetupOpenGL(HWND hwnd, HDC* hdc, HGLRC* hglrc)
{
    PIXELFORMATDESCRIPTOR pfd; // ���������, ����������� ������ ��������
    int pixelFormat;

    *hdc = GetDC(hwnd); // ��������� ��������� ���������� ��� ����

    ZeroMemory(&pfd, sizeof(pfd)); // ���������� ��������� ������
    pfd.nSize = sizeof(pfd); // ������ ��������� � ������
    pfd.nVersion = 1; // ������ ���������
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // ����� ������� ��������
    pfd.iPixelType = PFD_TYPE_RGBA; // ��� ��������: RGBA
    pfd.cColorBits = 24; // ������� �����
    pfd.cDepthBits = 16; // ������� ������ �������
    pfd.iLayerType = PFD_MAIN_PLANE; // ������� ����

    pixelFormat = ChoosePixelFormat(*hdc, &pfd); // ����� �������� ����������� ������� ��������
    SetPixelFormat(*hdc, pixelFormat, &pfd); // ��������� ���������� ������� ��������

    *hglrc = wglCreateContext(*hdc); // �������� ��������� OpenGL
    wglMakeCurrent(*hdc, *hglrc); // ��������� �������� ��������� OpenGL ��� ����������
}


void TearDownOpenGL(HWND hwnd, HDC hdc, HGLRC hglrc)
{
    wglMakeCurrent(NULL, NULL); // ������������ �������� ��������� OpenGL
    wglDeleteContext(hglrc); // �������� ��������� OpenGL
    ReleaseDC(hwnd, hdc); // ������������ ��������� ���������� ��� ����
}

/*
������������� ����: � ������� WinMain ��������� ���� Windows, ������� ����� �������������� ��� ����������� ������� OpenGL.

��������� OpenGL: � ������� SetupOpenGL ��������� �������� OpenGL � ��������������� ������ ��������.

������� ���� ���������: �������� ���� ��������� (while (!EXIT)) ������������ ��������� Windows, ����� ��� ����������� ���� � ������� ������, � ����� ������������ ����� OpenGL.

��������� ��������� ����: � ������� WindowProcedure �������������� ��������� ����, ����� ��� �������� ���� ��� ����������� ����.

��������� �����: ������� Camera_View ������������ �������� ������, � ����� ������������ ��������� �����, ��������� ������� OpenGL ��� ���������� ���������, ��������� ������ � ��������� ������.

������������ �������� OpenGL: �� ���������� ������ ��������� ����������� ������� OpenGL � ������� ������� TearDownOpenGL.

*/
