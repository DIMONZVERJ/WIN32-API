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
void ParseToInt(char buffer[], int temp[])
{
	char s[10]; int k = 0, m = 0;
	memset(s, ' ', 10);
	for (int i = 0; i < size_file; i++)
	{
		if ((buffer[i] >= '0' && buffer[i] <= '9') || buffer[i] == '-')
		{
			s[k] = buffer[i];
			k++;
		} //���������� �����
		else if (buffer[i] == ' ')
		{
			k = 0;
			temp[m] = atoi(s);
			memset(s, char(-52), 10);
			m++;
		} //��������� � temp
	}
	temp[m] = atoi(s);
}
void funcWINAPI_in()
{
	handle = CreateFile(_T("Text.txt"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	size_file=GetFileSize(handle, NULL);
	buffer = new char[size_file];
	bool result = ReadFile(handle, buffer, size_file, &d, NULL);
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
	fclose(file_var); //�������� ����������
}
void FILE_var_out()
{
	if ((file_var = fopen("Text.txt", "w")) == NULL) printf("Error");
	fprintf(file_var, "%d %d %d %d %d %d %d", sizeWindowX, sizeWindowY, wigth, height, R, G, B);
	fclose(file_var); //������� ���������� 
}
void fstream_in()
{
	file.open("Text.txt", std::ios_base::in);
	if (!file.fail())	file >> sizeWindowX >> sizeWindowY >> wigth >> height >> R >> G >> B;
	file.close(); //�������� ������
}
void fstream_out()
{
	file.open("Text.txt", std::ios_base::out | std::ios_base::trunc);
	if (!file.fail())	file << sizeWindowX << " " << sizeWindowY << " " << wigth << " " << height << " " << R << " " << G << " " << B;
	file.close(); //�������� ������ 
}
void MappingFile_in()
{
	handle_file = CreateFile(_T("Text.txt"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
	size_file = GetFileSize(handle_file, NULL);
	handle_map = CreateFileMapping(handle_file, NULL, PAGE_READONLY, 0, 0, NULL);
	buffer = (char*)MapViewOfFile(handle_map, FILE_MAP_READ, 0, 0, 0);
	ParseToInt(buffer, temp);
	UnmapViewOfFile(buffer);
	CloseHandle(handle_map);
	CloseHandle(handle_file);
	sizeWindowX = temp[0]; sizeWindowY = temp[1]; wigth = temp[2]; height = temp[3]; R = temp[4]; G = temp[5]; B = temp[6];
}
void MappingFile_out()
{
	int heightMonitor = GetSystemMetrics(SM_CYSCREEN);
	int wightMonitor = GetSystemMetrics(SM_CXSCREEN);
	int MaxWight = wightMonitor;
	int MaxHeight = heightMonitor;
	int MaxR = 255;
	int MaxG = 255;
	int MaxB = 255;
	string s = to_string(heightMonitor) + " " + to_string(wightMonitor) + " " + to_string(MaxWight)
		+ " " + to_string(MaxHeight) + " " + to_string(MaxR) + " " + to_string(MaxG) + " " + to_string(MaxB);
	MaxSizeText = s.length();
	temp[0] = sizeWindowX; temp[1] = sizeWindowY; temp[2] = wigth; temp[3] = height; temp[4] = R; temp[5] = G; temp[6] = B;
	handle_file = CreateFile(_T("Text.txt"), GENERIC_WRITE|GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	handle_map = CreateFileMapping(handle_file, NULL, PAGE_READWRITE, 0, MaxSizeText+1, NULL);
	buffer = (char*)MapViewOfFile(handle_map, FILE_MAP_WRITE, 0, 0, MaxSizeText);
	sprintf(buffer, "%d %d %d %d %d %d %d", temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6]);
	UnmapViewOfFile(buffer);
	CloseHandle(handle_map);
	CloseHandle(handle_file);
}