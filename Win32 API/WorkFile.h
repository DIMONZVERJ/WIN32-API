#pragma once
#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include <tchar.h>
#include <string>
static char* buffer;            //����� � ������� ������������ ���������
static int temp[8];             //������ � �����������
static HANDLE handle;           //����� ����� 
static HANDLE handle_file;      //����� ������������� �����
static HANDLE handle_map;       //����� �����������
static size_t size_file;        //������ �����
static FILE* file_var;
static std::fstream file;
static DWORD d;                 //���������� ����������� ����
int MaxSizeBuf();               //������������ ���������� �������� ����� � �����������
void funcWINAPI_in(void);       //�������� ��� ������ ������� WinAPI
void funcWINAPI_out(void);      //���������� ��� ������ ������� WinAPI
void FILE_var_in(void);         //�������� ��� ������ �������� ����������
void FILE_var_out(void);        //���������� ��� ������ �������� ���������� 
void fstream_in(void);          //�������� ��� ������ �������� �������
void fstream_out(void);         //���������� ��� ������ �������� �������
void MappingFile_in(void);      //�������� �������� ��� ������ ����������� �� ������
void MappingFile_out(void);     //���������� ��� ������ ����������� �� ������
void ParseToInt(char[], int[]); //�������������� ������� �������� � �����