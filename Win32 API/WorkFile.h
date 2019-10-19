#pragma once
#include <Windows.h>
#include <stdio.h>
#include <fstream>
#include <tchar.h>
#include <string>
static HANDLE handle_file;
static HANDLE handle_map;
static size_t size_file;
static PBYTE pbite_map;
static INT MaxSizeText;
void funcWINAPI_in(void);
void funcWINAPI_out(void);
void FILE_var_in(void);
void FILE_var_out(void);
void fstream_in(void);
void fstream_out(void);
void MappingFile_in(void);
void MappingFile_out(void);
void ParseToInt(char[], int[]);