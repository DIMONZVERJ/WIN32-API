#pragma once
#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include <tchar.h>
#include <string>
static char* buffer;            //буфер в который записываются настройки
static int temp[8];             //массив с настройками
static HANDLE handle;           //хэндл файла 
static HANDLE handle_file;      //хендл отображаемого файла
static HANDLE handle_map;       //хэндл отображения
static size_t size_file;        //размер файла
static FILE* file_var;
static std::fstream file;
static DWORD d;                 //количество прочитанных байт
int MaxSizeBuf();               //максимальное количество символов файла с настройками
void funcWINAPI_in(void);       //загрузка при помощи функций WinAPI
void funcWINAPI_out(void);      //сохранение при помощи функций WinAPI
void FILE_var_in(void);         //загрузка при помощи файловых переменных
void FILE_var_out(void);        //сохранение при помощи файловых переменных 
void fstream_in(void);          //загрузка при помощи файловых потоков
void fstream_out(void);         //сохранение при помощи файловых потоков
void MappingFile_in(void);      //загрузка настроек при помощи отображения на память
void MappingFile_out(void);     //сохранение при помощи отображения на память
void ParseToInt(char[], int[]); //преобразование массива символов к числу