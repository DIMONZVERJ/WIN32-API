#include <tchar.h>
#include <windowsx.h>
#include <cmath>
#include <time.h>
#include <fstream>
#include "WinMain.h"
#include "WorkFile.h"
#include <Windows.h>
#define KEY_SHIFTED     0x8000
#define KEY_TOGGLED     0x0001
const TCHAR szWinClass[] = _T("Win32SampleApp");
const TCHAR szWinName[] = _T("Win32SampleWindow");
const int countLine = 2;
BOOL Flag = FALSE;
int sizeWindowX = CW_USEDEFAULT, sizeWindowY = CW_USEDEFAULT, wigth = 320, height = 240, R = 255, G = 255, B = 255;
struct Section
{
	BOOL Ellipse;
	BOOL Cross;
	RECT location;
};
Section** sections;

bool operator ==(RECT a1, RECT a2) //compare two struct's a RECT type
{
	return ((a1.left == a2.left) && (a1.top == a2.top) && (a1.right == a2.right) && (a1.bottom == a2.bottom));
}
void FindSectAndDraw(RECT sect)  //find a section which user to press
{
	for (int i = 0; i < countLine + 1; i++)
		for (int j = 0; j < countLine + 1; j++)
			if (sections[i][j].location == sect)
			{
				sections[i][j].location = sect;
				if (sections[i][j].Ellipse == TRUE || sections[i][j].Cross == TRUE) return;
				if (Flag == 0)
				{
					sections[i][j].Ellipse = TRUE;
					Ellipse(hdc, sect.left, sect.top, sect.right, sect.bottom);
					Flag = 1;
				}
				else if (Flag == 1)
				{
					sections[i][j].Cross = TRUE;
					MoveToEx(hdc, sect.left, sect.top, NULL);
					LineTo(hdc, sect.right, sect.bottom);
					MoveToEx(hdc, sect.right, sect.top, NULL);
					LineTo(hdc, sect.left, sect.bottom);
					Flag = 0;
				}
			}
}
void PaintLine(void)
{
	GetClientRect(hwnd, &clientArea);
	int currentPosX = 0, currentPosY = 0;
	for (int i = 0; i < countLine; i++)
	{
		currentPosX += clientArea.right / (countLine + 1);
		MoveToEx(hdc, currentPosX, 0, NULL);
		LineTo(hdc, currentPosX, clientArea.bottom);
	}
	for (int i = 0; i < countLine; i++)
	{
		currentPosY += clientArea.bottom / (countLine + 1);
		MoveToEx(hdc, 0, currentPosY, NULL);
		LineTo(hdc, clientArea.right, currentPosY);
	}
}
void RunNotepad(void) //start the Notepad.exe 
{
	STARTUPINFO sInfo = { 0 };
	PROCESS_INFORMATION pInfo = { 0 };

	CreateProcess(_T("C:\\Windows\\Notepad.exe"),
		NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
}
void Resize(int newSizeX, int newSizeY) //resize section's
{
	for (int i = 0; i < countLine + 1; i++)
		for (int j = 0; j < countLine + 1; j++)
		{
			sections[i][j].location.left = j * newSizeX;
			sections[i][j].location.top = i * newSizeY;
			sections[i][j].location.right = j * newSizeX + newSizeX;
			sections[i][j].location.bottom = i * newSizeY + newSizeY;
		}
}
void Redraw(void) //redraw section's
{
	for (int i = 0; i < countLine + 1; i++)
		for (int j = 0; j < countLine + 1; j++)
			if (sections[i][j].Ellipse == 1)
				Ellipse(hdc, sections[i][j].location.left, sections[i][j].location.top, sections[i][j].location.right, sections[i][j].location.bottom);
			else if (sections[i][j].Cross == 1)
			{
				MoveToEx(hdc, sections[i][j].location.left, sections[i][j].location.top, NULL);
				LineTo(hdc, sections[i][j].location.right, sections[i][j].location.bottom);
				MoveToEx(hdc, sections[i][j].location.right, sections[i][j].location.top, NULL);
				LineTo(hdc, sections[i][j].location.left, sections[i][j].location.bottom);
			}
}
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_MOVING:
	{
		LPRECT rect = (LPRECT)lParam;
		sizeWindowX = rect->left;
		sizeWindowY = rect->top;
	}
	case WM_SIZING:
	{
		LPRECT rect = (LPRECT)lParam;
		wigth = rect->right - rect->left;
		height = rect->bottom - rect->top;
		break;
	}
	case WM_SIZE:
	{
		RECT window;
		hdc = GetDC(hwnd);
		if (wParam == SIZE_MAXIMIZED)
		{
			GetWindowRect(hwnd, &window);
			sizeWindowX = window.left;
			sizeWindowY = window.top;
			wigth = window.right - window.left;
			height = window.bottom - window.top;
		}
		sizeX = LOWORD(lParam) / (countLine + 1);
		sizeY = HIWORD(lParam) / (countLine + 1);
		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrushSection);
		Resize(sizeX, sizeY);
		Redraw();
		ReleaseDC(hwnd, hdc);
	}
	return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &paint);
		PaintLine();
		SelectObject(hdc, hBrushSection);
		SelectObject(hdc, hPen);
		Redraw();
		EndPaint(hwnd, &paint);
		return 0;
	case WM_HOTKEY:
		switch (wParam)
		{
		case 1: //ctrl+q
			PostQuitMessage(0);
			break;
		case 2: //shift+c
			RunNotepad();
			break;
		}
		return 0;
	case WM_LBUTTONDOWN: //при нажатии на левую кнопку мыши рисуетс€ эллипс в выбранной секции
	{
		sizeX = clientArea.right / (countLine + 1);
		sizeY = clientArea.bottom / (countLine + 1);
		hdc = GetDC(hwnd);
		int currentMouseX = GET_X_LPARAM(lParam); //текуща€ координата x мыши
		int currentMouseY = GET_Y_LPARAM(lParam); //текуща€ координата y мыши
		SelectObject(hdc, hBrushSection);
		SelectObject(hdc, hPen);
		RECT section = { (LONG)floor(currentMouseX / sizeX) * sizeX, (LONG)floor(currentMouseY / sizeY) * sizeY, ((LONG)floor(currentMouseX / sizeX) + 1) * sizeX, ((LONG)floor(currentMouseY / sizeY) + 1) * sizeY };
		//нашли координаты
		FindSectAndDraw(section);
		ReleaseDC(hwnd, hdc);
		return 0;
	}
	case WM_KEYUP: //сообщение отправл€етс€, когда отжимаетс€ определЄнна€ клавиша
		switch (wParam)
		{
		case VK_ESCAPE: //esc
			PostQuitMessage(0);
			return 0;
		case VK_RETURN: //enter
			hdc = GetDC(hwnd);
			DeleteObject(hBrush);
			R = rand() % 256; G = rand() % 256; B = rand() % 256;
			COLORREF bkcolor = RGB(R, G, B);
			hBrush = CreateSolidBrush(bkcolor);
			SetClassLongPtr(hwnd, GCL_HBRBACKGROUND, (LONG)hBrush);
			InvalidateRect(hwnd, NULL, TRUE);
			ReleaseDC(hwnd, hdc);
			return 0;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand(time(0));
	MSG message;            /* Here message to the application are saved */
	wincl = { 0 };         /* Data structure for the windowclass */
	//fstream_in(); //файловые потоки
	//FILE_var_in(); //файловые переменные
	funcWINAPI_in(); //через функции WINAPI
	//MappinigFile_in();
	wincl.style = CS_VREDRAW | CS_HREDRAW;
	wincl.hInstance = hInstance;
	wincl.lpszClassName = szWinClass;
	wincl.lpfnWndProc = WindowProcedure;
	hBrush = CreateSolidBrush(RGB(R, G, B)); //кисть дл€ фона
	hBrushSection = CreateSolidBrush(RGB(33, 189, 207)); //кисть дл€ рисовани€ кругов
	hPen = CreatePen(PS_SOLID, 2, RGB(24, 240, 60)); //перо дл€ рисовани€ кругов
	wincl.hbrBackground = hBrush;
	/* Register the window class, and if it fails quit the program */
	if (!RegisterClass(&wincl))
		return 0;
	/* The class is registered, let's create the program*/
	hwnd = CreateWindow(
		szWinClass,          /* Classname */
		szWinName,       /* Title Text */
		WS_OVERLAPPEDWINDOW, /* default window */
		sizeWindowX,       /* Windows decides the position */
		sizeWindowY,       /* where the window ends up on the screen */
		wigth,                 /* The programs width */
		height,                 /* and height in pixels */
		HWND_DESKTOP,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
	);
	/* Make the window visible on the screen */
	GetClientRect(hwnd, &clientArea);
	sizeX = clientArea.right / (countLine + 1); //size of a rectangle horizontally
	sizeY = clientArea.bottom / (countLine + 1); //size of a rectangle vertically
	/*array rectangle initialization*/
	sections = new Section * [countLine + 1];
	for (int i = 0; i < countLine + 1; i++)
		sections[i] = new Section[countLine + 1];
	for (int i = 0; i < countLine + 1; i++)
		for (int j = 0; j < countLine + 1; j++)
			sections[i][j] = { FALSE,FALSE, {j * sizeX,i * sizeY,j * sizeX + sizeX,i * sizeY + sizeY} };
	ShowWindow(hwnd, nCmdShow);
#pragma endregion
	//регистраци€ гор€чих клавиш
	RegisterHotKey(hwnd, 1, MOD_CONTROL, 0x51);
	RegisterHotKey(hwnd, 2, MOD_SHIFT, 0x43);
	/* Run the message loop. It will run until GetMessage() returns 0 */

	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		/* Send message to WindowProcedure */
		DispatchMessage(&message);
	}

	/* Cleanup stuff */
	for (int i = 0; i < countLine + 1; i++)
		delete[] sections[i];
	delete[] sections;
	DeleteObject(hBrushSection);
	DeleteObject(hPen);
	DestroyWindow(hwnd);
	DeleteObject(hBrush);
	UnregisterClass(szWinClass, hInstance);
	//fstream_out(); //файловые потоки
	//FILE_var_out(); //файловые переменные
	funcWINAPI_out(); //через функции WINAPI
	//MappingFile_out();
	return NULL;
}