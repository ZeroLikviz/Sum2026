/* FILE NAME: t03clock.c
 * PROGRAMMER: TM5
 * DATE: 03.06.2026
 * PURPOSE: Get to know the basics of WinAPI.
 */
#include <windows.h>
#include <math.h>
#define WND_CLASS_NAME "CSID:002959"
#define RAND_NUM 7626
#define POLYGON_SIZE 40
#define POLYGON_MARGIN 50
#define TIMER_MS 14
#define RAND_BATCH 256
#define MAX_PTS 24
#define HALF_PI 1.5708
#define PI 3.1416
#define DPI 6.2831
struct TagPOLYGON
{
  int size;
  POINT pts[MAX_PTS];
};
typedef struct TagPOLYGON POLYGON;

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

  // Main Loop //
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}

VOID SetColor( HDC hDC, INT r, INT g, INT b )
{
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(r, g, b));
}

VOID RotatePolygon( POLYGON *NewPolygon, DOUBLE Rads )
{
  INT i;
  DOUBLE X, Y;
  for (i = 0; i < NewPolygon->size; i++)
  {
    X = NewPolygon->pts[i].x;
    Y = NewPolygon->pts[i].y;
    NewPolygon->pts[i].x = (LONG)(X * cos(Rads) + Y * sin(Rads));
    NewPolygon->pts[i].y = (LONG)(X * sin(Rads) - Y * cos(Rads));
  }
}

VOID ResizePolygon( POLYGON *NewPolygon, DOUBLE M )
{
  INT i;
  for (i = 0; i < NewPolygon->size; i++)
  {
    NewPolygon->pts[i].x = (LONG)(NewPolygon->pts[i].x * M);
    NewPolygon->pts[i].y = (LONG)(NewPolygon->pts[i].y * M);
  }
}

VOID InitPolygon( POLYGON *NewPolygon, POINT *Pts)
{
  INT i;

  for (i = 0; i < NewPolygon->size; i++)
    NewPolygon->pts[i] = Pts[i];
}

POLYGON CreatePolygon( INT X, INT Y, DOUBLE M, DOUBLE Rads, POINT *Pts, int PtsSize )
{
  POLYGON NewPolygon;
  INT i;

  NewPolygon.size = PtsSize;
  InitPolygon(&NewPolygon, Pts);
  ResizePolygon(&NewPolygon, M);
  RotatePolygon(&NewPolygon, Rads);
  for (i = 0; i < PtsSize; i++)
  {
    NewPolygon.pts[i].x += X;
    NewPolygon.pts[i].y += Y;
  }

  return NewPolygon;
}

VOID DrawPolygon( HDC hDC, INT X, INT Y, DOUBLE Rad, POINT *Pts, int PtsSize )
{
  POLYGON NewPolygon = CreatePolygon(X, Y, 4, Rad, Pts, PtsSize);

  Polygon(hDC, NewPolygon.pts, PtsSize);
}

DOUBLE SmoothSecToMin( INT Sec, INT Mil )
{
  if (Sec < 59 || Mil < 200)
    return 0;
  else
    return pow(Mil / 800.0, 8) / DPI;
}

DWORD FontColor( INT r, INT g, INT b)
{
  DWORD Color = 0;

  Color |= ((b & 255) << 16);
  Color |= ((g & 255) << 8);
  Color |= (r & 255);

  return Color;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  static INT Clocks = 0;
  static CHAR TextStr[256];
  static HFONT hFont;
  static INT W, H, X, Y;
  static BOOL Random = FALSE;
  static HDC hMemDC, hClockDC;
  static HBITMAP hBM, hClockBM;
  static BITMAP ClockBM;
  static POINT SecondHandPts[] = {{0, 65}, {-5, 55}, {-6, 50}, {-7, 40}, {-7, 0}, {-3, 0}, {-3, -1}, {-4, -1}, {-4, -15}, {0, -15}};
  static POINT MinuteHandPts[] = {{0, 45}, {-5, 38}, {-6, 34}, {-7, 28}, {-7, 0}, {-3, 0}, {-3, -1}, {-4, -1}, {-4, -15}, {0, -15}};
  static POINT HourHandPts[] = {{0, 35}, {-5, 27}, {-6, 23}, {-7, 14}, {-7, 0}, {-3, 0}, {-3, -1}, {-4, -1}, {-4, -15}, {0, -15}};
  PAINTSTRUCT ps;
  HDC hDC;
  SYSTEMTIME tm;

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hClockDC = CreateCompatibleDC(hDC);
    
    hClockBM = LoadImage(NULL, "Clock.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(hClockDC, hClockBM);
    GetObject(hClockBM, sizeof(BITMAP), &ClockBM);

    hFont = CreateFont(80, 0, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DFA_DISABLE, FALSE, FF_DONTCARE, "");
    SelectObject(hMemDC, hFont);

    ReleaseDC(hWnd, hDC);
    SetTimer(hWnd, RAND_NUM, TIMER_MS, NULL);
    return 0;
  case WM_SIZE:
    W = LOWORD(lParam);
    H = HIWORD(lParam);

    if (hBM != NULL)
      DeleteObject(hBM);
    hDC = GetDC(hWnd);
    hBM = CreateCompatibleBitmap(hDC, W, H);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBM);
    return 0;  
  case WM_DESTROY:
    DeleteObject(hBM);
    DeleteObject(hClockBM);
    DeleteObject(&ClockBM);
    DeleteObject(hFont);
    ReleaseDC(hWnd, hClockDC);
    ReleaseDC(hWnd, hMemDC);
    PostMessage(NULL, WM_QUIT, 0, 0);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_TIMER:
    // Clean up
    SetColor(hMemDC, 255, 255, 255);
    Rectangle(hMemDC, 0, 0, W, H);
    StretchBlt(hMemDC, (W - ClockBM.bmWidth * 3) / 2, (H - ClockBM.bmHeight * 3) / 2, ClockBM.bmWidth * 3, ClockBM.bmHeight * 3, hClockDC, 0, 0, ClockBM.bmWidth, ClockBM.bmHeight, SRCCOPY);
   
    // Adding hands
    GetLocalTime(&tm);
    SetColor(hMemDC, 170, 170, 170);
    DrawPolygon(hMemDC, W / 2, H / 2, ((DOUBLE)tm.wSecond + pow(tm.wMilliseconds / 800.0, 8) / DPI) / 60 * DPI, SecondHandPts, sizeof(SecondHandPts) / sizeof(POINT));
    SetColor(hMemDC, 200, 200, 200);
    DrawPolygon(hMemDC, W / 2, H / 2, (DOUBLE)(tm.wMinute + SmoothSecToMin(tm.wSecond, tm.wMilliseconds)) / 60 * DPI, MinuteHandPts, sizeof(MinuteHandPts) / sizeof(POINT));
    SetColor(hMemDC, 240, 240, 240);
    DrawPolygon(hMemDC, W / 2, H / 2, (DOUBLE)(tm.wHour + tm.wMinute / 60.0) / 12 * DPI, HourHandPts, sizeof(HourHandPts) / sizeof(POINT));

    // Adding text
    SetBkMode(hMemDC, TRANSPARENT);
    for (Y = -3; Y <= 3; Y++)
      for (X = -3; X <= 3; X++)
      {
        SetTextColor(hMemDC, FontColor(rand() % 120 + 90, rand() % 120, rand() % 120));
        TextOut(hMemDC, 400 + X + rand() % 8 - 2 + (INT)(10 * sin(Clocks / 20.0)), 900 + Y + rand() % 8 - 2 + (INT)(10 * sin(Clocks / 10.0)), TextStr, wsprintf(TextStr, "DOOMSDAY TIMER: %.2d:%.2d:%.2d", rand() % 24, rand() % 60, rand() % 60));
      }
    SetTextColor(hMemDC, FontColor(0, 0, 0));
    TextOut(hMemDC, 400 + (INT)(5 * sin(Clocks / 20.0)), 900 + (INT)(5 * sin(Clocks / 10.0)), TextStr, wsprintf(TextStr, "DOOMSDAY TIMER: %.2d:%.2d:%.2d", 23 - tm.wHour, 59 - tm.wMinute, 59 - tm.wSecond));

    // Finishing
    Clocks++;
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  }

  return DefWindowProc(hWnd, Msg, wParam, lParam);
}