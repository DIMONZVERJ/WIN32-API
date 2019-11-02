#include "WinMain.h"
#include "WorkFile.h"
#pragma region  

#define KEY_SHIFTED     0x8000
#define KEY_TOGGLED     0x0001
const TCHAR szWinClass[] = _T("Win32SampleApp");
const TCHAR szWinName[] = _T("Win32SampleWindow");
const int countLine = 2;
BOOL Flag = FALSE;
int sizeWindowX = CW_USEDEFAULT, sizeWindowY = CW_USEDEFAULT, wigth = 320, height = 240, R = 255, G = 255, B = 255;
struct Image
{
	BYTE* data;
	int width;
	int height;
} png_my_image, jpeg_my_image;
struct Section
{
	BOOL Ellipse = FALSE;
	BOOL Cross = FALSE;
	RECT location;
};
Section** sections;

bool operator ==(RECT a1, RECT a2) //compare two struct's a RECT type
{
	return ((a1.left == a2.left) && (a1.top == a2.top) && (a1.right == a2.right) && (a1.bottom == a2.bottom));
}
void DrawImage(BYTE* image, int& wigthImage, int& heightImage, RECT& sect)
{
	int wigth = sect.right - sect.left, height = sect.bottom - sect.top;
	HBITMAP hOldBitMap, hBitMap;
	HDC hMemDC = CreateCompatibleDC(hdc);
	hBitMap = CreateBitmap(wigthImage, heightImage, 1, 32, image);  //������
	hOldBitMap = (HBITMAP)SelectObject(hMemDC, hBitMap);
	StretchBlt(hdc, sect.left, sect.top, wigth, height, hMemDC, 0, 0, wigthImage, heightImage, SRCCOPY);
	SelectObject(hMemDC, hOldBitMap);
	DeleteObject(hBitMap);
	DeleteDC(hMemDC);
	ReleaseDC(hwnd, hdc);
}
void FindSectAndDraw(RECT sect)  //find a section which user to press
{
	for (int i = 0; i < countLine + 1; i++)
		for (int j = 0; j < countLine + 1; j++)
			if (sections[i][j].location == sect)
			{
				sections[i][j].location = sect;
				Section current_sect = sections[i][j];
				if (current_sect.Cross == TRUE || current_sect.Ellipse == TRUE) return;
				else if (Flag == 0) //������ �������
				{
					DrawImage(png_my_image.data, png_my_image.width, png_my_image.height, current_sect.location);
					current_sect.Cross = TRUE;
					Flag = 1;
				}
				else if (Flag == 1) //������ �����
				{
					DrawImage(jpeg_my_image.data, jpeg_my_image.width, jpeg_my_image.height, current_sect.location);
					current_sect.Ellipse = TRUE;
					Flag = 0;
				}
			}
}
void loadImage()
{
	HMODULE hModule = LoadLibrary(_T("Dll.dll"));
	BYTE* (*loadImage)(const char*, int*, int*);
	if (hModule != NULL) (FARPROC&)loadImage = GetProcAddress(hModule, "loadImage");
	else
	{
		printf("hModule is NULL");
		return;
	}
	if (loadImage != NULL)
	{
		int wigthImage, heightImage;
		png_my_image.data = loadImage("D:\\VSProject\\Win32 API\\Dll\\Cross.png", &png_my_image.width, &png_my_image.height);
		jpeg_my_image.data = loadImage("D:\\VSProject\\Win32 API\\Dll\\nought.jpg", &jpeg_my_image.width, &jpeg_my_image.height);
	}
	FreeLibrary(hModule);
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

	CreateProcess(_T("cmd.exe"),
		NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
	CloseHandle(pInfo.hThread);
	CloseHandle(pInfo.hProcess);
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
		sizeWindowX = rect->left;
		sizeWindowY = rect->top;
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
	case WM_LBUTTONDOWN: //��� ������� �� ����� ������ ���� �������� ������ � ��������� ������
	{
		sizeX = clientArea.right / (countLine + 1);
		sizeY = clientArea.bottom / (countLine + 1);
		hdc = GetDC(hwnd);
		int currentMouseX = GET_X_LPARAM(lParam); //������� ���������� x ����
		int currentMouseY = GET_Y_LPARAM(lParam); //������� ���������� y ����
		SelectObject(hdc, hBrushSection);
		SelectObject(hdc, hPen);
		RECT section = { (LONG)floor(currentMouseX / sizeX) * sizeX, (LONG)floor(currentMouseY / sizeY) * sizeY, ((LONG)floor(currentMouseX / sizeX) + 1) * sizeX, ((LONG)floor(currentMouseY / sizeY) + 1) * sizeY };
		//����� ����������
		FindSectAndDraw(section);
		ReleaseDC(hwnd, hdc);
		return 0;
	}
	case WM_KEYUP: //��������� ������������, ����� ���������� ����������� �������
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

#pragma endregion
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand(time(0));
	MSG message;            /* Here message to the application are saved */
	wincl = { 0 };/* Data structure for the windowclass */
	int a, mode;
	auto res = CommandLineToArgvW(GetCommandLine(), &a);
	mode = atoi((const char*)res[1]);
	loadImage(); //�������� �����������
	switch (mode)
	{
	case 1:
		fstream_in(); //�������� ������
		break;
	case 2:
		FILE_var_in(); //�������� ����������
		break;
	case 3:
		funcWINAPI_in(); //����� ������� WINAPI
		break;
	case 4:
		MappingFile_in();
		break;
	default:
		break;
	}
	wincl.style = CS_VREDRAW | CS_HREDRAW;
	wincl.hInstance = hInstance;
	wincl.lpszClassName = szWinClass;
	wincl.lpfnWndProc = WindowProcedure;
	hBrush = CreateSolidBrush(RGB(R, G, B)); //����� ��� ����
	hBrushSection = CreateSolidBrush(RGB(33, 189, 207)); //����� ��� ��������� ������
	hPen = CreatePen(PS_SOLID, 2, RGB(24, 240, 60)); //���� ��� ��������� ������
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
	GetWindowRect(hwnd, &rect); sizeWindowX = rect.left; sizeWindowY = rect.top;
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
	//����������� ������� ������
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
	switch (mode)
	{
	case 1:
		fstream_out(); //�������� ������
		break;
	case 2:
		FILE_var_out(); //�������� ����������
		break;
	case 3:
		funcWINAPI_out(); //����� ������� WINAPI
		break;
	case 4:
		MappingFile_out();
		break;
	default:
		break;
	}
	DeleteObject(hBrushSection);
	DeleteObject(hPen);
	DestroyWindow(hwnd);
	DeleteObject(hBrush);
	UnregisterClass(szWinClass, hInstance);
	return NULL;

}