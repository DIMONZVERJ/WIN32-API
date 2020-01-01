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
}; //������ ���������� ������� CreateThread
struct Image
{
	BYTE* data;
	int width;
	int height;
	BOOL load = FALSE;
}; //��������� �����������
static Image png_my_image;
static Image jpeg_my_image;
static std::list<COLORREF> list_color;    //���� � ������� �������� �����
static std::list<COLORREF>::iterator it;  //�������� �����, � ������� �������� �����
static RECT clientArea;                   //�������������, ������� ����� ������� ���������� �������
static HANDLE hMapSections;               //����� ���������
void Resize(int&, int&);                  //��������� �������� ������
void Redraw(HWND&, HDC&, HPEN&, HBRUSH&); //����������� ����
void ColorChange(int&, int&, int&);       //������������� ������ ����
DWORD WINAPI ChangeBackground(LPVOID);    //��������� ����
void loadImage(void);      //�������� �����������
void PaintLine(HWND&,HDC&); //��������� �����
void DrawImage(HDC&,BYTE*, int&, int&, RECT&); //��������� ����������� �������� ����� � ������ � ��������������
void DestroyImage();                           //����������� �����������
void InitializeLocation(int&, int&); //�������������� ������� � ������������ ������
void InitializeMapSection();//������������� ������������� �������
void DestroyLocation();     //���������� ������� � ������������ ������
void DestroyMapSection();   //���������� ������������ ������
char* GetMapSection();      //�������� ������������ ������(����)
RECT** GetLocation();