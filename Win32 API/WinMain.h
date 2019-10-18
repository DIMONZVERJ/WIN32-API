#pragma once
#include <Windows.h>
extern const int size_buffer;
static RECT clientArea;
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
static HANDLE handle;
static int temp[7];
static char* buffer;
bool operator ==(RECT, RECT);
void FindSectAndDraw(RECT);
void PaintLine(void);
void RunNotepad(void);
void Resize(int, int);
void Redraw(void);

