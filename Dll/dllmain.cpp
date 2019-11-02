#define _CRT_SECURE_NO_WARNINGS
#define PNG_BYTES_TO_CHECK 100
// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "Dll.h"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

BYTE* loadImage(const char* fileName, int* width, int* height)
{
	if (check_if_png(fileName) == true)
	{
		BYTE* res = (BYTE*)ReadImagePNG(fileName, width, height);
		return res;
	}
	else
	{
		 return read_JPEG_file(fileName, width, height);
	}
	return NULL;
}
bool check_if_png(const char* file_name)
{
	char buf[PNG_BYTES_TO_CHECK];
	FILE* file;
	bool res = 1;
	if ((file = fopen(file_name, "rb")) == NULL)
		res = 0;
	else if (fread(buf, 1, PNG_BYTES_TO_CHECK, file) != PNG_BYTES_TO_CHECK)
	{
		res = 0;
		fclose(file);
	}
	if (res == 0) return res;
	return(!png_sig_cmp((png_bytep)buf, (png_size_t)0, PNG_BYTES_TO_CHECK));
}
png_bytep ReadImagePNG(const char* fileName, int* width, int* height)
{
	png_bytep buffer;
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info_ptr = png_create_info_struct(png_ptr);
	png_image image;
	memset(&image, 0, (sizeof image));
	image.version = PNG_IMAGE_VERSION;
	if (png_image_begin_read_from_file(&image, fileName) != 0)
	{
		image.format = PNG_FORMAT_BGRA;
		buffer = (png_bytep)malloc(PNG_IMAGE_SIZE(image));
		if (buffer != NULL)
		{
			png_image_finish_read(&image, NULL, buffer, 0, NULL);
		}
		*width = image.width;
		*height = image.height;
		return buffer;
	}
	else
	{
		printf("Error in png_image_begin_read_from_file");
		return NULL;
	}
}

BYTE* read_JPEG_file(const char* filename, int* width, int* height)
{

	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE* infile;		/* source file */
	BYTE* buffer;		/* Output row buffer */
	int size_image;		/* physical row width in output buffer */
	if ((infile = fopen(filename, "rb")) == NULL) {
		return NULL;
	}
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	(void)jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);
	cinfo.out_color_space = JCS_EXT_BGRA;
	cinfo.output_components = 4;
	*width = cinfo.output_width;
	*height = cinfo.output_height;
	size_image = cinfo.output_height * cinfo.output_width * cinfo.output_components;
	BYTE* temp_buffer = new BYTE[size_image];
	buffer = new BYTE[size_image];
	int step = cinfo.output_width * cinfo.output_components;
	int count_step = 0;
	while (cinfo.output_scanline < cinfo.output_height) {
		jpeg_read_scanlines(&cinfo, &temp_buffer, 1);
		memmove(buffer, temp_buffer,step);
		buffer+=step;
		count_step++;
	}
	buffer = buffer - count_step * step;
	(void)jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
	delete[] temp_buffer;
	return buffer;
}