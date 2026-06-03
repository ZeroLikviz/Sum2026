/* FILE NAME: t01fwin.c
 * PROGRAMMER: TM5
 * DATE: 01.06.2026
 * PURPOSE: ---
 */
#include <windows.h>
#include <math.h>
#define WND_CLASS_NAME "CSID:002959"
#define RAND_NUM 7626
#define MAXIMIZE_BUTTON_ID 6110
#define BALLS_COUNT 10
struct tagBALL
{
  INT X, Y, R;
  DOUBLE VX, VY;
};
typedef struct tagBALL BALL;

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  MSG msg;
  HWND hWnd;

  // Class Registration //
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_APPSTARTING);
  wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  wc.hInstance = hInstance;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = WND_CLASS_NAME;
  wc.lpfnWndProc = MyWindowFunc;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Couldn't register window class", WND_CLASS_NAME, MB_OK | MB_ICONASTERISK);
    return -1;
  }
  // Create Window //
  hWnd = CreateWindow(WND_CLASS_NAME, "HacHKED!!", WS_OVERLAPPEDWINDOW | WS_BORDER, 0, 0, 720, 480, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd, ShowCmd);

  CreateWindow("BUTTON", "maximise", WS_CHILD | WS_VISIBLE, 10, 10, 120, 40, hWnd, (HMENU)MAXIMIZE_BUTTON_ID, hInstance, NULL);
  
  // Main Loop //
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

VOID InitBall( BALL *ball, INT W, INT H )
{
  ball->X = W / 2;
  ball->Y = H / 2;
  ball->VX = rand() % 40 - 20;
  ball->VY = rand() % 40 - 20;
  ball->R = 20 + rand() % 40;
}

INT Sqr( INT x )
{
  return x * x;
}

VOID ProcessEllipse( HWND hWnd, BALL *ball, INT W, INT H )
{
  DOUBLE Hyp;
  POINT ps;

  GetCursorPos(&ps);
  ScreenToClient(hWnd, &ps);
  
  Hyp = Sqr(ball->X - ps.x) + Sqr(ball->Y - ps.y);

  ball->X += (INT)((ball->X - ps.x) / Hyp * 4209);
  ball->Y += (INT)((ball->Y - ps.y) / Hyp * 4209);

  ball->Y += (INT)ball->VY;
  ball->X += (INT)ball->VX;
  ball->VY += 1.5;
  ball->VX *= 0.998;
  ball->VY *= 0.997;

  if (ball->X - ball->R < 0)
  {
    ball->X = ball->R;
    ball->VX *= -0.9;
  }
  if (ball->X + ball->R > W)
  {
    ball->X = W - ball->R;
    ball->VX *= -0.9;
  }
  if (ball->Y - ball->R < 0)
  {
    ball->Y = ball->R;
    ball->VY *= -1;
  }
  if (ball->Y + ball->R > H)
  {
    ball->Y = H - ball->R;
    ball->VY *= -0.9;
  }
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  static BALL balls[BALLS_COUNT];
  static INT i = 0;
  static INT count = 0;
  static INT W, H;
  PAINTSTRUCT ps;
  HDC hDC;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, RAND_NUM, 30, NULL);
    return 0;
  case WM_TIMER:
    count += 1;
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);
    for (i = 0; i < BALLS_COUNT; i++)
      InitBall(balls + i, W, H);
    return 0;  
  case WM_DESTROY:
    PostMessage(NULL, WM_QUIT, 0, 0);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);

    if(count / 500 % 2)
      SelectObject(hDC, GetStockObject(NULL_BRUSH));
    else
      SelectObject(hDC, GetStockObject(BLACK_BRUSH));
    Ellipse(hDC, 0, 0, W / 2, H);
    if(!(count / 500 % 2))
      SelectObject(hDC, GetStockObject(NULL_BRUSH));
    else
      SelectObject(hDC, GetStockObject(BLACK_BRUSH));
    Ellipse(hDC, W / 2, 0, W, H);
    
    SelectObject(hDC, GetStockObject(GRAY_BRUSH));
    for (i = 0; i < BALLS_COUNT; i++)
    {
      Ellipse(hDC, balls[i].X - balls[i].R, balls[i].Y - balls[i].R, balls[i].X + balls[i].R, balls[i].Y + balls[i].R);
      ProcessEllipse(hWnd, balls + i, W, H);
    }
    


    EndPaint(hWnd, &ps);
    return 0;                         
  case WM_COMMAND:
    if (LOWORD(wParam) == MAXIMIZE_BUTTON_ID)
      ShowWindow(hWnd, SW_MAXIMIZE);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}