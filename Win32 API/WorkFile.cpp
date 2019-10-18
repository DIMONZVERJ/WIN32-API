#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include "WinMain.h"
#include <tchar.h>
extern const int size_buffer;
extern int sizeWindowX;
extern int sizeWindowY;
extern int wigth;
extern int height;
extern int R;
extern int G;
extern int B;
void ParseToInt(char buffer[], int temp[])
{
	char s[10]; int k = 0, m = 0;
	memset(s, ' ', 10);
	for (int i = 0; i < size_buffer; i++)
	{
		if ((buffer[i] >= '0' && buffer[i] <= '9') || buffer[i] == '-')
		{
			s[k] = buffer[i];
			k++;
		} //записываем число
		else if (buffer[i] == ' ')
		{
			k = 0;
			temp[m] = atoi(s);
			memset(s, char(-52), 10);
			m++;
		} //добавляем в temp
		else
		{
			temp[m] = atoi(s);
			break;
		}

	}
}
void funcWINAPI_in()
{
	buffer = new char[size_buffer];
	handle = CreateFile(_T("Text.txt"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	bool result = ReadFile(handle, buffer, size_buffer, &d, NULL);
	CloseHandle(handle);
	ParseToInt(buffer, temp);
	sizeWindowX = temp[0]; sizeWindowY = temp[1]; wigth = temp[2]; height = temp[3]; R = temp[4]; G = temp[5]; B = temp[6];
}
void funcWINAPI_out()
{
	temp[0] = sizeWindowX; temp[1] = sizeWindowY; temp[2] = wigth; temp[3] = height; temp[4] = R; temp[5] = G; temp[6] = B;
	memset(buffer, ' ', strlen(buffer));
	sprintf(buffer, "%d %d %d %d %d %d %d", temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6]);
	handle = CreateFile(_T("Text.txt"), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(handle, buffer, sizeof(int) * 7, &d, NULL);
	CloseHandle(handle);
	delete[] buffer;
}
void FILE_var_in()
{
	if ((file_var = fopen("Text.txt", "a+")) == NULL)	printf("Error");
	fscanf(file_var, "%d %d %d %d %d %d %d", &sizeWindowX, &sizeWindowY, &wigth, &height, &R, &G, &B);
	fclose(file_var); //файловые переменные
}
void FILE_var_out()
{
	if ((file_var = fopen("Text.txt", "w")) == NULL) printf("Error");
	fprintf(file_var, "%d %d %d %d %d %d %d", sizeWindowX, sizeWindowY, wigth, height, R, G, B);
	fclose(file_var); //фаловые переменные 
}
void fstream_in()
{
	file.open("Text.txt", std::ios_base::in);
	if (!file.fail())	file >> sizeWindowX >> sizeWindowY >> wigth >> height >> R >> G >> B;
	file.close(); //файловые потоки
}
void fstream_out()
{
	file.open("Text.txt", std::ios_base::out | std::ios_base::trunc);
	if (!file.fail())	file << sizeWindowX << " " << sizeWindowY << " " << wigth << " " << height << " " << R << " " << G << " " << B;
	file.close(); //файловые потоки 
}