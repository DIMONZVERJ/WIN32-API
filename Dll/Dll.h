#pragma once
#include <Windows.h>
#include <png.h>
#include <zlib.h>
#include <stdio.h>
#include "jpeglib.h"

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
extern "C" DLL_API BYTE* loadImage(const char*, int*, int*);
//конвертация изображения в формате png в массив байт
png_bytep ReadImagePNG(const char*, int*, int*);
//проверка что это png файл
bool check_if_png(const char*);
//конвертация jpeg изображения в массив байт
BYTE* read_JPEG_file(const char*,int*,int*);