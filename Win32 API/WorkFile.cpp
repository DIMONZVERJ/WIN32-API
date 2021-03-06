#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "WinMain.h"
#include "WorkFile.h"
using namespace std;
extern int sizeWindowX;
extern int sizeWindowY;
extern int wigth;
extern int height;
extern int R;
extern int G;
extern int B;
extern int size_field;
int MaxSizeBuf()
{
	int heightMonitor = GetSystemMetrics(SM_CYSCREEN);
	int wightMonitor = GetSystemMetrics(SM_CXSCREEN);
	int MaxWight = wightMonitor;
	int MaxHeight = heightMonitor;
	int MaxR = 255;
	int MaxG = 255;
	int MaxB = 255;
	string s = to_string(heightMonitor) + " " + to_string(wightMonitor) + " " + to_string(MaxWight)
		+ " " + to_string(MaxHeight) + " " + to_string(MaxR) + " " + to_string(MaxG) + " " + to_string(MaxB) + to_string(wightMonitor);
	return s.length();
}
void ParseToInt(char buffer[], int temp[])
{
	char s[10]; int k = 0, m = 0;
	memset(s, ' ', 10);
	for (int i = 0; i < (int)size_file; i++)
	{
		if ((buffer[i] >= '0' && buffer[i] <= '9') || buffer[i] == '-')
		{
			s[k] = buffer[i];
			k++;
		} //���������� �����
		else if (buffer[i] == ' ')
		{
			k = 0;
			if (s[0] >= '0' && s[0] <= '9') temp[m] = atoi(s);
			memset(s, char(-52), 10);
			m++;
		} //��������� � temp
	}
	if (s[0] >= '0' && s[0] <='9') temp[m] = atoi(s);
}
void funcWINAPI_in()
{
	handle = CreateFile(_T("Text.txt"), GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	size_file=GetFileSize(handle, NULL);
	if (size_file == 0 || handle == INVALID_HANDLE_VALUE || handle == NULL || GetLastError() == 0)
		MessageBox(NULL, _T("���� ������ ���� �� �� ����������. ����� ������������ ��������� �� ���������"), NULL, MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);
	else
	{
		buffer = new char[MaxSizeBuf()];
		bool result = ReadFile(handle, buffer, MaxSizeBuf(), &d, NULL);
		if (result == false)
		{
			MessageBox(
				NULL,
				("������ ��� ������ ������ (��� ������: " + to_string(GetLastError()) + "). ����� ������������ ��������� �� ���������").c_str(),
				NULL,
				MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY
			);
			CloseHandle(handle);
			delete[] buffer;
			return;
		}
		ParseToInt(buffer, temp);
		sizeWindowX = temp[0]; sizeWindowY = temp[1]; wigth = temp[2]; height = temp[3]; R = temp[4]; G = temp[5]; B = temp[6]; size_field = temp[7];
		delete[] buffer;
	}
	if (handle != NULL) CloseHandle(handle);
}
void funcWINAPI_out()
{
	buffer = new char[MaxSizeBuf()];
	temp[0] = sizeWindowX; temp[1] = sizeWindowY; temp[2] = wigth; temp[3] = height; temp[4] = R; temp[5] = G; temp[6] = B; temp[7] = size_field;
	sprintf(buffer, "%d %d %d %d %d %d %d %d", temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);
	handle = CreateFile(_T("Text.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle == INVALID_HANDLE_VALUE)
		MessageBox(
			NULL,
			("������ ��� ���������� �������� (��� ������: " + to_string(GetLastError()) + "). ���� ��������� ���������, ��� �� ����������").c_str(),
			NULL,
			MB_OK |MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY
		);
	else 
		WriteFile(handle, buffer, strlen(buffer), &d, NULL);
	CloseHandle(handle);
	delete[] buffer;
}
void FILE_var_in()
{
	if ((file_var = fopen("Text.txt", "r")) == NULL)	
		MessageBox(
			NULL,
			("������ ��� ������ ������ (��� ������: " + to_string(GetLastError()) + "). ����� ������������ ��������� �� ���������").c_str(),
			NULL,
			MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY
		);
	else
	{
		int numsucc = fscanf(file_var, "%d %d %d %d %d %d %d %d", &sizeWindowX, &sizeWindowY, &wigth, &height, &R, &G, &B, &size_field);
		fclose(file_var); //�������� ����������
	}
}
void FILE_var_out()
{
	if ((file_var = fopen("Text.txt", "w")) == NULL) 
		MessageBox(
			NULL,
			("������ ��� �������� ����� (��� ������: " + to_string(GetLastError()) +")").c_str(),
			NULL,
			MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY
		);
	else 
	{
		int numsucc = fprintf(file_var, "%d %d %d %d %d %d %d %d", sizeWindowX, sizeWindowY, wigth, height, R, G, B, size_field);
		fclose(file_var); //������� ���������� 
	}
}
void fstream_in()
{
	file.open("Text.txt", ios_base::in);
	std::string s;
	getline(file, s);
	if (!s.empty())
	{
		file.seekg(0, ios::beg);
		file >> sizeWindowX >> sizeWindowY >> wigth >> height >> R >> G >> B >> size_field;
	}
	file.close(); //�������� ������
}
void fstream_out()
{
	file.open("Text.txt", ios_base::out | ios_base::trunc);
	file << sizeWindowX << " " << sizeWindowY << " " << wigth << " " << height << " " << R << " " << G << " " << B << " " << size_field;
	file.close(); //�������� ������ 
}
void MappingFile_in()
{
	handle_file = CreateFile(_T("Text.txt"), GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
	size_file = GetFileSize(handle_file, NULL);
	if (size_file == 0 || handle_file == INVALID_HANDLE_VALUE || handle_file == NULL || GetLastError() == 0)
		MessageBox(NULL, _T("���� ������ ���� �� �� ����������. ����� ������������ ��������� �� ���������"), NULL, MB_OK | MB_ICONINFORMATION | MB_DEFAULT_DESKTOP_ONLY);
	else
	{
		handle_map = CreateFileMapping(handle_file, NULL, PAGE_READONLY, 0, 0, NULL);
		if (handle_map != NULL)
		{
			buffer = (char*)MapViewOfFile(handle_map, FILE_MAP_READ, 0, 0, 0);
			if (buffer != NULL)
			{
				ParseToInt(buffer, temp);
				sizeWindowX = temp[0]; sizeWindowY = temp[1]; wigth = temp[2]; height = temp[3]; R = temp[4]; G = temp[5]; B = temp[6]; size_field = temp[7];
				UnmapViewOfFile(buffer);
			}
			CloseHandle(handle_map);
		}
	}
	if (handle_file != NULL) CloseHandle(handle_file);
}
void MappingFile_out()
{
	int max = MaxSizeBuf();
	temp[0] = sizeWindowX; temp[1] = sizeWindowY; temp[2] = wigth; temp[3] = height; temp[4] = R; temp[5] = G; temp[6] = B; temp[7] = size_field;
	handle_file = CreateFile(_T("Text.txt"), GENERIC_WRITE|GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle_file == INVALID_HANDLE_VALUE || handle_file == NULL)
		MessageBox(
			NULL, 
			("������ ��� ���������� �������� (��� ������: " + to_string(GetLastError()) + "). ���� ��������� ���������, ��� �� ����������").c_str(),
			NULL,
			MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY
		);
	else
	{
		handle_map = CreateFileMapping(handle_file, NULL, PAGE_READWRITE, 0, max + 1, NULL);
		if (handle_map != NULL)
		{
			buffer = (char*)MapViewOfFile(handle_map, FILE_MAP_WRITE, 0, 0, max);
			if (buffer != NULL)
			{
				sprintf(buffer, "%d %d %d %d %d %d %d %d", temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);
				UnmapViewOfFile(buffer);
			}
			CloseHandle(handle_map);
		}
	}
	if (handle_file != NULL) CloseHandle(handle_file);
}