#pragma once
#include <tchar.h>
#include <png.h>
#include <windowsx.h>
#include <cmath>
#include <time.h>
#include <fstream>
#include <Windows.h>
#include "jpeglib.h"
static RECT clientArea;
static RECT rect;
static std::fstream file;
static FILE* file_var;
static DWORD d;
static WNDCLASS wincl;
static HWND hwnd;
static HDC hdc;
static int sizeX;
static int sizeY;
static HPEN hPen;
static PAINTSTRUCT paint;
static HBRUSH hBrush;
static HBRUSH hBrushSection;
bool operator ==(RECT, RECT);
void FindSectAndDraw(RECT);
void PaintLine(void);
void DrawBitmap(BYTE* image, int& wigthImage, int& heightImage, RECT& sect);
void RunNotepad(void);
void Resize(int, int);
void Redraw(void);

