// 9.cpp : Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "9.h"
#include <fstream>
#include <cmath>

#include "atlstr.h"
#include <tchar.h>


#define _T(x) __T(x)
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY9, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY9));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY9));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY9);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
static int sx, sy;
const int SCALE = 1000;
const int MARK = 4;
void DcInLp(POINT& point)
{
	point.x = point.x* SCALE / sx;
	point.y = SCALE - point.y* SCALE / sy;
}
void transform(HDC& hdc)
{
	SetMapMode(hdc, MM_ANISOTROPIC);
	SetWindowExtEx(hdc, SCALE, -SCALE, NULL);
	SetViewportExtEx(hdc, sx, sy, NULL);
	SetViewportOrgEx(hdc, 0, sy, NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	TCHAR buf[255] = { 0 };
	HDC hdc;
	static HPEN hDash, hBezier;
	static HBRUSH hRect, hSel;
	static POINT pt[101];
	static POINT kt[101];
	static  POINT  point;
	RECT rt;
	static int count, index;
	static bool capture;
	int i,change,change1;
	std::ifstream in;
	std::ofstream out;
	bool ctrl = false;

	switch (message)
	{
	case WM_CREATE:
		in.open("dat.txt");
		if (in.fail())
		{
			MessageBox(hWnd, _T("Файл dat.txt не найден"),
				_T("Открытие файла"), MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(0);
			return 1;
		}
		for (count = 0; in >> pt[count].x; count++) in >> pt[count].y;
		in.close();  //В переменной count сохранится размер массива точек
		hDash = CreatePen(PS_DASH, 1, 0);
		hBezier = CreatePen(PS_SOLID, 4, RGB(25, 18, 60));
		hRect = CreateSolidBrush(RGB(255, 52, 128));
		hSel = CreateSolidBrush(RGB(255, 255, 10));
		break;
	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		break;
	case WM_LBUTTONDOWN:
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);


		//Преобразование экранных координат мыши в логические 
		DcInLp(point);
		for (i = 0; i < count; i++)
		{
			SetRect(&rt, pt[i].x - MARK, pt[i].y - MARK, pt[i].x + MARK, pt[i].y + MARK);
			if (PtInRect(&rt, point))
			{  //Курсор мыши попал в точку 
				if (wParam & MK_CONTROL)
				{
					_stprintf_s(buf, TEXT("X = %d\nY = %d"), pt[i].x,pt[i].y);
					::MessageBox(NULL, buf, TEXT("Координаты"), MB_OK); break;
				}
				
				index = i;
				capture = true;
				for (int j = 0; j < count; j++)
				{

					kt[j].x = pt[j].x;
					kt[j].y = pt[j].y;

				}
				for (int j = 0; j < count-1; j++)
				{
					if (j >= index)
					{
						pt[j].x = kt[j + 1].x;
						pt[j].y = kt[j + 1].y;
					}
					else
					{
						pt[j].x = kt[j].x;
						pt[j].y = kt[j].y;
					}
				}
				count--;
				
			}
		
			
		}
		break;
	
		
	case WM_LBUTTONUP:
		if (capture)
		{
			ReleaseCapture();    //Освобождение мыши 
			capture = false;
			hdc = GetDC(hWnd);
			InvalidateRect(hWnd, NULL, TRUE);

			
		}
		break;
	case WM_RBUTTONUP:
		if (capture)
		{
			ReleaseCapture();    //Освобождение мыши 
			capture = false;
		
		}
		break;
	case WM_RBUTTONDOWN:


		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		DcInLp(point);

		if (count < 100)
		{
			pt[count].x = point.x;
			pt[count].y = point.y;

			out.open("dat.txt");

			for (i = 0; i < count; i++) {

				
				out << pt[i].x << '\t' << pt[i].y << '\n';
			}
			out.close();
			count++;

			InvalidateRect(hWnd, NULL, TRUE);


		}

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		transform(hdc);      //Переход в логические координаты
		SelectObject(hdc, hDash);
		Polyline(hdc, pt, count);  //Строим ломаную линию 
		SelectObject(hdc, hBezier);
		PolyBezier(hdc, pt, count);  //Строим кривую Безье 
		for (i = 0; i < count; i++)
		{  //Закрашиваем точки графика прямоугольниками 
			SetRect(&rt, pt[i].x - MARK, pt[i].y - MARK, pt[i].
				x + MARK, pt[i].y + MARK);
			FillRect(hdc, &rt, hRect);
		}
		EndPaint(hWnd, &ps);
		break;
	
	case WM_DESTROY:
		DeleteObject(hDash);
		DeleteObject(hBezier);
		DeleteObject(hRect);
		DeleteObject(hSel);
		out.open("dat.txt");
		for (i = 0; i < count; i++) out << pt[i].x << '\t' << pt[i].y << '\n';
		out.close();
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
