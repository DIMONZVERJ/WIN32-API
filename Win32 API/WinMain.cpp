#include "WinMain.h"
#include "WorkFile.h"
#pragma region  

#define KEY_SHIFTED     0x8000
#define KEY_TOGGLED     0x0001
using namespace std;
const TCHAR szWinClass[] = _T("Win32SampleApp");
const TCHAR szWinName[] = _T("Win32SampleWindow");
const TCHAR mapName[] = _T("map");
const TCHAR mess_name[] = _T("message");
const int countLine = 2;
const int sizeP = (countLine + 1) * (countLine + 1);
BOOL Flag = FALSE;
int sizeWindowX = CW_USEDEFAULT, sizeWindowY = CW_USEDEFAULT, wigth = 320, height = 240, R = 255, G = 255, B = 255;
struct Image
{
	BYTE* data = NULL;
	int width = 0;
	int height = 0;
	BOOL load = FALSE;
} png_my_image, jpeg_my_image;
struct Section
{
	BOOL Ellipse = FALSE;
	BOOL Cross = FALSE;
	RECT location;
};
Section** sections;
char* map_sections;
bool operator ==(RECT a1, RECT a2) //compare two struct's a RECT type
{
	return ((a1.left == a2.left) && (a1.top == a2.top) && (a1.right == a2.right) && (a1.bottom == a2.bottom));
}
void loadImage()
{
	HMODULE hModule = LoadLibrary(_T("Dll.dll"));
	BYTE* (*loadImage)(const char*, int*, int*);
	if (hModule != NULL) (FARPROC&)loadImage = GetProcAddress(hModule, "loadImage");
	else
	{
		MessageBox(hwnd,("Ошибка при загрузке библиотеки (код ошибки: " + to_string(GetLastError()) +")").c_str(),NULL,MB_ICONERROR);
		return;
	}
	if (loadImage != NULL)
	{
		if ((png_my_image.data = loadImage("D:\\VSProject\\Win32 API\\Dll\\Cross.png", &png_my_image.width, &png_my_image.height)) != NULL)
			png_my_image.load = TRUE;
		if ((jpeg_my_image.data = loadImage("D:\\VSProject\\Win32 API\\Dll\\nought.jpg", &jpeg_my_image.width, &jpeg_my_image.height)) != NULL)
			jpeg_my_image.load = TRUE;
	}
	FreeLibrary(hModule);
}
void DrawImage(BYTE* image, int& wigthImage, int& heightImage, RECT& sect)
{
	int wigth = sect.right - sect.left, height = sect.bottom - sect.top;
	HBITMAP hOldBitMap, hBitMap;
	HDC hMemDC = CreateCompatibleDC(hdc);
	hBitMap = CreateBitmap(wigthImage, heightImage, 1, 32, image);
	hOldBitMap = (HBITMAP)SelectObject(hMemDC, hBitMap);
	StretchBlt(hdc, sect.left, sect.top, wigth, height, hMemDC, 0, 0, wigthImage, heightImage, SRCCOPY);
	SelectObject(hMemDC, hOldBitMap);
	DeleteObject(hBitMap);
	DeleteDC(hMemDC);
}
void PaintLine(void)
{
	GetClientRect(hwnd, &clientArea);
	int currentPosX = 0, currentPosY = 0;
	for (int i = 0; i < countLine; i++)
	{
		currentPosX += (int)round(clientArea.right / ((double)(countLine) + 1));
		MoveToEx(hdc, currentPosX, 0, NULL);
		LineTo(hdc, currentPosX, clientArea.bottom);
	}
	for (int i = 0; i < countLine; i++)
	{
		currentPosY += (int)round(clientArea.bottom / (double(countLine) + 1));
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
	CloseHandle(pInfo.hThread);
	CloseHandle(pInfo.hProcess);
}
void Resize(int &newSizeX, int &newSizeY) //resize section's
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
void Redraw(HWND hwnd) //redraw section's
{
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrushSection);
	for (int i = 0; i < countLine + 1; i++)
		for (int j = 0; j < countLine + 1; j++)
			if (sections[i][j].Ellipse == 1 && map_sections[i*(countLine+1) + j] == 1)
			{
				if (jpeg_my_image.load == TRUE)
					DrawImage(jpeg_my_image.data, jpeg_my_image.width, jpeg_my_image.height, sections[i][j].location);
				else
				{
					Ellipse(hdc, sections[i][j].location.left, sections[i][j].location.top, sections[i][j].location.right, sections[i][j].location.bottom);
				}
			}
			else if (sections[i][j].Cross == 1 && map_sections[i*(countLine+1) + j] == 0)
			{
				if (png_my_image.load == TRUE)
					DrawImage(png_my_image.data, png_my_image.width, png_my_image.height, sections[i][j].location);
				else
				{
					MoveToEx(hdc, sections[i][j].location.left, sections[i][j].location.top, NULL);
					LineTo(hdc, sections[i][j].location.right, sections[i][j].location.bottom);
					MoveToEx(hdc, sections[i][j].location.right, sections[i][j].location.top, NULL);
					LineTo(hdc, sections[i][j].location.left, sections[i][j].location.bottom);
				}
			}
	ReleaseDC(hwnd, hdc);
}
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == MESSAGE)
	{
		hdc = GetDC(hwnd);
		sizeX = (int)round(clientArea.right / (double(countLine) + 1));
		sizeY = (int)round(clientArea.bottom / (double(countLine) + 1));
		int currentMouseX = GET_X_LPARAM(lParam);
		int currentMouseY = GET_Y_LPARAM(lParam);

		int i = currentMouseY / sizeY;
		int j = currentMouseX / sizeX;
		if (map_sections[i*(countLine+1)+j] == -1)
		{

			if (Flag)
			{
				sections[i][j].Ellipse = TRUE;
				map_sections[i*(countLine+1) + j] = 1;
			}
			else
			{
				sections[i][j].Cross = TRUE;
				map_sections[i * (countLine + 1) + j] = 0;
			}
		}

		for (int i = 0; i < countLine + 1; i++)
			for (int j = 0; j < countLine + 1; j++)
				if (map_sections[i * (countLine + 1) + j] == 0)
					sections[i][j].Cross = TRUE;
				else if (map_sections[i * (countLine + 1) + j] == 1)
					sections[i][j].Ellipse = TRUE;
		Flag = !Flag;
		Redraw(hwnd);
	}
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
		if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
		{
			GetWindowRect(hwnd, &window);
			sizeWindowX = window.left;
			sizeWindowY = window.top;
			wigth = window.right - window.left;
			height = window.bottom - window.top;
		}
		sizeX = (int)round(LOWORD(lParam) / (double)(countLine + 1));
		sizeY = (int)round(HIWORD(lParam) / (double)(countLine + 1));
		Resize(sizeX, sizeY);
		return 0;
	}
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &paint);
		PaintLine();
		Redraw(hwnd);
		EndPaint(hwnd, &paint);
		return 0;
	case WM_HOTKEY:
		if (wParam == id1) //ctrl+q
			PostQuitMessage(0);
		else if (wParam == id2) //shift+c
			RunNotepad();
		return 0;
	case WM_LBUTTONDOWN: 
	{
		SendMessage(HWND_BROADCAST, MESSAGE, 0, lParam);
		return 0;
	}
	case WM_KEYUP:
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
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	loadImage();
	srand((unsigned int)time(0));
	MSG message;            /* Here message to the application are saved */
	int a;
	wincl = { 0 };/* Data structure for the windowclass */
	auto res = CommandLineToArgvW(GetCommandLineW(), &a);
	if (res[1] != NULL)
	{
		int mode = atoi((const char*)res[1]);
		switch (mode)
		{
		case 1:
			fstream_in();
			break;
		case 2:
			FILE_var_in();
			break;
		case 3:
			funcWINAPI_in();
			break;
		case 4:
			MappingFile_in();
			break;
		default:
			fstream_in();
		}
	}
	wincl.style = CS_VREDRAW | CS_HREDRAW;
	wincl.hInstance = hInstance;
	wincl.lpszClassName = szWinClass;
	wincl.lpfnWndProc = WindowProcedure;
	hBrush = CreateSolidBrush(RGB(R, G, B)); //êèñòü äëÿ ôîíà
	hBrushSection = CreateSolidBrush(RGB(33, 189, 207)); //êèñòü äëÿ ðèñîâàíèÿ êðóãîâ
	hPen = CreatePen(PS_SOLID, 2, RGB(24, 240, 60)); //ïåðî äëÿ ðèñîâàíèÿ êðóãîâ
	wincl.hbrBackground = hBrush;
	/* Register the window class, and if it fails quit the program */
	if (!RegisterClass(&wincl))
		return 0;
	/* The class is registered, let's create the program*/
	hwnd = CreateWindow(
		szWinClass,            /* Classname */
		szWinName,             /* Title Text */
		WS_OVERLAPPEDWINDOW,   /* default window */
		sizeWindowX,           /* Windows decides the position */
		sizeWindowY,           /* where the window ends up on the screen */
		wigth,                 /* The programs width */
		height,                /* and height in pixels */
		HWND_DESKTOP,          /* The window is a child-window to desktop */
		NULL,                  /* No menu */
		hInstance,             /* Program Instance handler */
		NULL                   /* No Window Creation data */
	);
	/* Make the window visible on the screen */
	GetClientRect(hwnd, &clientArea);
	GetWindowRect(hwnd, &rect); sizeWindowX = rect.left; sizeWindowY = rect.top;
	hdc = GetDC(hwnd);
	sizeX = (int)round(clientArea.right / (double(countLine) + 1)); //size of a rectangle horizontally
	sizeY = (int)round(clientArea.bottom / (double(countLine) + 1)); //size of a rectangle vertically
	/*array rectangle initialization*/
	sections = new Section * [countLine + 1];
	for (int i = 0; i < countLine + 1; i++)
		sections[i] = new Section[countLine + 1];
	for (int i = 0; i < countLine + 1; i++)
		for (int j = 0; j < countLine + 1; j++)
			sections[i][j] = { FALSE,FALSE, {j * sizeX,i * sizeY,j * sizeX + sizeX,i * sizeY + sizeY} };
	ShowWindow(hwnd, nCmdShow);
	MESSAGE = RegisterWindowMessage(mess_name);
	hMapSections = CreateFileMapping(INVALID_HANDLE_VALUE, (LPSECURITY_ATTRIBUTES)NULL, PAGE_READWRITE, 0, sizeP, mapName);
	if (hMapSections != NULL)
	{
		map_sections = (char*)MapViewOfFile(hMapSections, FILE_MAP_ALL_ACCESS, 0, 0, sizeP);
		if (map_sections == NULL) exit(0);
		if (GetLastError() != ERROR_ALREADY_EXISTS)
			for (int i = 0; i < sizeP; i++)
				map_sections[i] = -1;
		else
			for (int i = 0; i < countLine + 1; i++)
				for (int j = 0; j < countLine + 1; j++)
					if (map_sections[i * (countLine + 1) + j] == 0)
						sections[i][j].Cross = TRUE;
					else
						sections[i][j].Ellipse = TRUE;
	}
	id1 = GlobalAddAtom(to_string(rand()%1001).c_str());
	id2 = GlobalAddAtom(to_string(1000 + rand() % 1001).c_str());
	BOOL res1 = RegisterHotKey(hwnd, id1, MOD_CONTROL | MOD_NOREPEAT, 0x51);
	BOOL res2 = RegisterHotKey(hwnd, id2, MOD_SHIFT | MOD_NOREPEAT, 0x43);
	//if (GetLastError() != 0)
	//	MessageBox(hwnd, ("Ошибка при ркгистрации горячей клавиши (код ощибки: " + std::to_string(GetLastError()) + ")").c_str(), NULL, MB_ICONERROR);
	/* Run the message loop. It will run until GetMessage() returns 0 */

	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		/* Send message to WindowProcedure */
		DispatchMessage(&message);
	}
	UnmapViewOfFile(map_sections);
	/* Cleanup stuff */
	for (int i = 0; i < countLine + 1; i++)
		delete[] sections[i];
	delete[] sections;
	if (res[1] != NULL)
	{
		int mode = atoi((const char*)res[1]);
		switch (mode)
		{
		case 1:
			fstream_out(); //ôàéëîâûå ïîòîêè
			break;
		case 2:
			FILE_var_out(); //ôàéëîâûå ïåðåìåííûå
			break;
		case 3:
			funcWINAPI_out(); //÷åðåç ôóíêöèè WINAPI
			break;
		case 4:
			MappingFile_out();
			break;
		default:
			fstream_out();
		}
	}
	delete[] png_my_image.data;
	delete[] jpeg_my_image.data;
	GlobalDeleteAtom(id1);
	GlobalDeleteAtom(id2);
	LocalFree(res);
	DeleteObject(hBrushSection);
	DeleteDC(hdc);
	DeleteObject(hPen);
	DestroyWindow(hwnd);
	DeleteObject(hBrush);
	UnregisterClass(szWinClass, hInstance);
	return NULL;
}