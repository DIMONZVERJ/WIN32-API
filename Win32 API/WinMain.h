#pragma once
#include <tchar.h>
#include <windowsx.h>
#include <cmath>
#include <time.h>
#include <fstream>
#include <Windows.h>
static HANDLE hMapSections; 
static UINT MESSAGE;
static RECT clientArea;
static RECT rect;
static std::fstream file;
static FILE* file_var;
static DWORD d;
static WNDCLASS wincl;       //�������
static HWND hwnd;            //�������
static HDC hdc;              //�������
static int sizeX;
static int sizeY;
static HPEN hPen;
static PAINTSTRUCT paint;
static HBRUSH hBrush;        //�������
static HBRUSH hBrushSection; //�������
bool operator ==(RECT, RECT);
void loadImage(void);
void PaintLine(void);
void DrawImage(BYTE*, int&, int&, RECT&);
void RunNotepad(void);
void Resize(int&, int&);
void Redraw(HWND);

