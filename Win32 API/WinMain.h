#pragma once
#include <tchar.h>
#include <process.h>
#include <fstream>
#include <windowsx.h>
#include <Windows.h>
#include <string>
static std::string win;
static HANDLE thread_background; //�����, ���������� �� ����� ����
static HANDLE semaphore;   //�������, �������������� ���������� ����������� ����
static HANDLE semaphore_one_winner; //�������, � ������� �������� ��������� ������ ���� ��������� � ����������, � ����������� ������ ����� ������(���� ���� ����������� �� �������)
static HANDLE hEvent;     //�������, ������� ������������ ����������� ���� ���� �������, ���� �����, �������� ��� ���� ������ �� ������ ����, �� ������� �����������
static HANDLE player1;     //������� ������� ������. ����� ������ ����� �� ��� ��� ���� ������ ������� �� ���� ����
static HANDLE player2;     //������� ������� ������. ����� ������ ����� �� ��� ��� ���� ������ ������� �� ���� ����
static BOOL thread_stop;   //�������� ������(������� ��� �������������)
static UINT MESSAGE_EXIT;   //��������� ����������� ��� ����, ��� ��������� ����
static RECT rect;           //������� ����
static WNDCLASS wincl;       //�������� ����
static HWND hwnd;            //����� ����
static HDC hdc;              //�������� ����������
static int sizeX;            //������ ������ �� ��� X
static int sizeY;            //������ ������ �� ��� Y
static HPEN hPen;            // ���� ��� ����� ��������� � �������, ������������ ��� ������ WinAPI
static PAINTSTRUCT paint;
static HBRUSH hBrush;         //����� ��� ����
static HBRUSH hBrushSection;  //����� ��� ���������� ����� ������, ������������� � ������� WinAPI
int row_check(char**, int&);                //�������� �������
int column_check(char**, int&);             //�������� �������
int diagonal_check(char**, int&, int&);     //�������� ������� ���������
int antidiagonal_check(char**, int&, int&); //�������� �������� ���������
bool fill_pole(char**);                     //�������� �� ������������� ����
void EndGameCheck(int&, int&);              //�������� ����� ����
void RunNotepad(void);                      //������ ��������
void SaveSetting();                         //���������� ��������



