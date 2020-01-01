#include "Animation.h"
extern int size_field;
extern int R;
extern int G; 
extern int B;
const int time_sleep = 1000;
listArg listArgChangeBackground;
RECT** location;
char* map_sections;
void ColorChange(int& R, int& G, int& B)
{
	int max_color = max(max(R, G), B); int min_color = min(min(R, G), B);
	int R_i = max_color, G_i = min_color, B_i = min_color; //140,21,21
	list_color.push_back(RGB(R_i, G_i, B_i));
	for (; G_i < max_color; G_i++)
		list_color.push_back(RGB(R_i, G_i, B_i));
	//140,140,21
	for (; R_i > min_color; R_i--)
		list_color.push_back(RGB(R_i, G_i, B_i));
	//21,140,21
	for (; B_i < max_color; B_i++)
		list_color.push_back(RGB(R_i, G_i, B_i));
	//21,140,140
	for (; G_i > min_color; G_i--)
		list_color.push_back(RGB(R_i, G_i, B_i));
	//21,21,140
	for (; R_i < max_color; R_i++)
		list_color.push_back(RGB(R_i, G_i, B_i));
	//140,21,140
	for (; B_i > min_color; B_i--)
		list_color.push_back(RGB(R_i, G_i, B_i));
	//140,21,21
	it = std::find(list_color.begin(), list_color.end(), RGB(R, G, B));
}
DWORD WINAPI ChangeBackground(LPVOID arglist)
{
	listArgChangeBackground = *(listArg*)arglist;
	HDC hdc = listArgChangeBackground.hdc;
	HWND hwnd = listArgChangeBackground.hwnd;
	HBRUSH hBrush = listArgChangeBackground.hBrush;
	while (true)
	{
		hdc = GetDC(hwnd);
		DeleteObject(hBrush);
		hBrush = CreateSolidBrush(*it);
		R = GetRValue(*it); G = GetBValue(*it); B = GetBValue(*it);
		if (it != --list_color.end())
			it++;
		else
			it = list_color.begin();
		SetClassLongPtr(hwnd, GCL_HBRBACKGROUND, (LONG)hBrush);
		InvalidateRect(hwnd, NULL, TRUE);
		ReleaseDC(hwnd, hdc);
		Sleep(time_sleep);
	}
	return 0;
}
void loadImage()
{
	HMODULE hModule = LoadLibrary(_T("Dll.dll"));
	BYTE* (*loadImage)(const char*, int*, int*);
	if (hModule != NULL) (FARPROC&)loadImage = GetProcAddress(hModule, "loadImage");
	else
	{
		MessageBox(NULL, ("Ошибка при загрузке библиотеки (код ошибки: " + std::to_string(GetLastError()) + ")").c_str(), NULL, MB_ICONERROR);
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
void DrawImage(HDC& hdc,BYTE* image, int& wigthImage, int& heightImage, RECT& sect)
{
	int wigth = sect.right - sect.left - 2, height = sect.bottom - sect.top - 2; //уменьшим ширину и высоту конечного изобржания, чтобы оно не перекрывало линии
	HBITMAP hOldBitMap, hBitMap;
	HDC hMemDC = CreateCompatibleDC(hdc);
	hBitMap = CreateBitmap(wigthImage, heightImage, 1, 32, image);
	hOldBitMap = (HBITMAP)SelectObject(hMemDC, hBitMap);
	StretchBlt(hdc, sect.left + 1, sect.top + 1, wigth, height, hMemDC, 0, 0, wigthImage, heightImage, SRCCOPY);
	SelectObject(hMemDC, hOldBitMap);
	DeleteObject(hBitMap);
	DeleteDC(hMemDC);
}
void PaintLine(HWND& hwnd, HDC& hdc)
{
	GetClientRect(hwnd, &clientArea);
	int currentPosX = 0, currentPosY = 0;
	for (int i = 0; i < size_field-1; i++)
	{
		currentPosX += (int)round(clientArea.right / ((double)(size_field)));
		MoveToEx(hdc, currentPosX, 0, NULL);
		LineTo(hdc, currentPosX, clientArea.bottom);
	}
	for (int i = 0; i < size_field-1; i++)
	{
		currentPosY += (int)round(clientArea.bottom / (double(size_field)));
		MoveToEx(hdc, 0, currentPosY, NULL);
		LineTo(hdc, clientArea.right, currentPosY);
	}
}
void Resize(int& newSizeX, int& newSizeY) //resize section's
{
	for (int i = 0; i < size_field; i++)
		for (int j = 0; j < size_field; j++)
		{
			location[i][j].left = j * newSizeX;
			location[i][j].top = i * newSizeY;
			location[i][j].right = j * newSizeX + newSizeX;
			location[i][j].bottom = i * newSizeY + newSizeY;
		}
}
void Redraw(HWND& hwnd, HDC& hdc, HPEN& hPen, HBRUSH& hBrushSection) //redraw section's
{
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrushSection);
	for (int i = 0; i < size_field; i++)
		for (int j = 0; j < size_field; j++)
			if (map_sections[i * size_field + j] == 1)
			{
				if (jpeg_my_image.load == TRUE)
					DrawImage(hdc,jpeg_my_image.data, jpeg_my_image.width, jpeg_my_image.height, location[i][j]);
				else
				{
					Ellipse(hdc, location[i][j].left, location[i][j].top, location[i][j].right, location[i][j].bottom);
				}
			}
			else if (map_sections[i * size_field + j] == 0)
			{
				if (png_my_image.load == TRUE)
					DrawImage(hdc,png_my_image.data, png_my_image.width, png_my_image.height, location[i][j]);
				else
				{
					MoveToEx(hdc, location[i][j].left, location[i][j].top, NULL);
					LineTo(hdc, location[i][j].right, location[i][j].bottom);
					MoveToEx(hdc, location[i][j].right, location[i][j].top, NULL);
					LineTo(hdc, location[i][j].left, location[i][j].bottom);
				}
			}
	ReleaseDC(hwnd, hdc);
}
void DestroyImage()
{
	delete[] png_my_image.data;
	delete[] jpeg_my_image.data;
}

void InitializeLocation(int& sizeX, int& sizeY)
{
	location = new RECT * [size_field];
	for (int i = 0; i < size_field; i++)
		location[i] = new RECT[size_field];
	for (int i = 0; i < size_field; i++)
		for (int j = 0; j < size_field; j++)
			location[i][j] = { j * sizeX,i * sizeY,j * sizeX + sizeX,i * sizeY + sizeY };
}
RECT** GetLocation()
{
	return location;
}
void DestroyLocation()
{
	for (int i = 0; i < size_field; i++)
		delete[] location[i];
	delete[] location;
}

void DestroyMapSection()
{
	UnmapViewOfFile(map_sections);
	if (hMapSections != NULL) CloseHandle(hMapSections);
}

char* GetMapSection()
{
	return map_sections;
}

void InitializeMapSection()
{
	hMapSections = CreateFileMapping(INVALID_HANDLE_VALUE, (LPSECURITY_ATTRIBUTES)NULL, PAGE_READWRITE, 0, size_field*size_field, _T("map"));
	if (hMapSections != NULL)
	{
		map_sections = (char*)MapViewOfFile(hMapSections, FILE_MAP_ALL_ACCESS, 0, 0, size_field*size_field);
		if (map_sections == NULL) exit(0);
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			for (int i = 0; i < size_field*size_field; i++)
				map_sections[i] = -1;
		}
	}
}
