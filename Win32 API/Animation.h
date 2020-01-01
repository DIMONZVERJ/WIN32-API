#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <list>
struct listArg
{
	HDC hdc;
	HBRUSH hBrush;
	HWND hwnd;
}; //список аргументов функции CreateThread
struct Image
{
	BYTE* data;
	int width;
	int height;
	BOOL load = FALSE;
}; //структура изображения
static Image png_my_image;
static Image jpeg_my_image;
static std::list<COLORREF> list_color;    //лист в котором хранятся цвета
static std::list<COLORREF>::iterator it;  //итератор листа, в котором хранятся цвета
static RECT clientArea;                   //прямоугольник, который задаёт размеры клиентской области
static HANDLE hMapSections;               //хэндл отобрения
void Resize(int&, int&);                  //изменение размеров клеток
void Redraw(HWND&, HDC&, HPEN&, HBRUSH&); //перерисовка окна
void ColorChange(int&, int&, int&);       //инициализация цветов фона
DWORD WINAPI ChangeBackground(LPVOID);    //изменение фона
void loadImage(void);      //загрузка изображений
void PaintLine(HWND&,HDC&); //отрисовка линий
void DrawImage(HDC&,BYTE*, int&, int&, RECT&); //рисование изображения заданной длины и ширины в прямоугольнике
void DestroyImage();                           //уничтожение изображений
void InitializeLocation(int&, int&); //ининциализацич массива с координатами клеток
void InitializeMapSection();//инициализация отображаемого массива
void DestroyLocation();     //уничтожить массива с координатами клеток
void DestroyMapSection();   //уничтожить отображаемый массив
char* GetMapSection();      //получить отображаемый массив(поле)
RECT** GetLocation();