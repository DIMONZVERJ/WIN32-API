#pragma once
#include <tchar.h>
#include <process.h>
#include <fstream>
#include <windowsx.h>
#include <Windows.h>
#include <string>
static std::string win;
static HANDLE thread_background; //поток, отвечающий за смену фона
static HANDLE semaphore;   //семафор, ограничивающий количество запускаемых окон
static HANDLE semaphore_one_winner; //семафор, с помощью которого выводится только одно сообщение о победителе, и запрещающий ходить после победы(пока окно уведомления не закрыто)
static HANDLE hEvent;     //событие, которое ограничивает возможность хода всех игроков, пока игрок, нажавший два раза подряд по своему полю, не закроет уведомление
static HANDLE player1;     //семафор первого игрока. Чтобы первый игрок не мог два раза подряд сходить на своём поле
static HANDLE player2;     //семафор второго игрока. Чтобы второй игрок не мог два раза подряд сходить на своём поле
static BOOL thread_stop;   //проверка потока(запущен или приостановлен)
static UINT MESSAGE_EXIT;   //сообщение закрывающее все окна, при окончании игры
static RECT rect;           //размеры окна
static WNDCLASS wincl;       //описание окна
static HWND hwnd;            //хэндл окна
static HDC hdc;              //контекст устройства
static int sizeX;            //размер клетки по оси X
static int sizeY;            //размер клетки по оси Y
static HPEN hPen;            // перо для линий крестиков и элипсов, нарисованных при помощи WinAPI
static PAINTSTRUCT paint;
static HBRUSH hBrush;         //кисть для фона
static HBRUSH hBrushSection;  //кисть для внутренной части элипса, нарисованного с помощью WinAPI
int row_check(char**, int&);                //проверка стороки
int column_check(char**, int&);             //проверка столбца
int diagonal_check(char**, int&, int&);     //проверка главной диагонали
int antidiagonal_check(char**, int&, int&); //проверка побочной диагонали
bool fill_pole(char**);                     //проверка на заполненность поля
void EndGameCheck(int&, int&);              //проверка конца игры
void RunNotepad(void);                      //запуск блокнота
void SaveSetting();                         //сохранение настроек



