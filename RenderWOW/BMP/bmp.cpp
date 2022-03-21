#include <cstdio>
#include <string>
#include "bmp.h"

using namespace std;

const uint32_t HEAD_SIZE = 54;
const uint32_t DIB_HEAD_SIZE = 40;
const uint16_t BIT_COUNT = 32;

string pathNorm(string path) {
	if (path.length() > 3 && path[path.length() - 4] == '.' && path[path.length() - 3] == 'b' && path[path.length() - 2] == 'm' && path[path.length() - 1] == 'p')
		return path;
	else
		return path + ".bmp";
}

void twoByteWrite(FILE& fbmp, uint16_t num) {
	byte arr[2];
	arr[0] = (num >> 8) & 0xFF;
	arr[1] = (num & 0xFF);
	fputc(arr[1], &fbmp);
	fputc(arr[0], &fbmp);
}
void fourByteWrite(FILE& fbmp, uint32_t num) {
	byte arr[4];
	arr[0] = (num >> 24) & 0xFF;
	arr[1] = (num >> 16) & 0xFF;
	arr[2] = (num >> 8) & 0xFF;
	arr[3] = (num & 0xFF);
	fputc(arr[3], &fbmp);
	fputc(arr[2], &fbmp);
	fputc(arr[1], &fbmp);
	fputc(arr[0], &fbmp);
}

bool genBMP(uint32_t width, uint32_t height, string path, pixel** image) {
	FILE* fbmp;
	path = pathNorm(path);
	fopen_s(&fbmp, path.c_str(), "wb");
	if (fbmp != 0)
	{
		fputc('B', fbmp);
		fputc('M', fbmp);
		uint32_t size = width * height * (BIT_COUNT / 8);
		fourByteWrite(*fbmp, size + HEAD_SIZE);
		fourByteWrite(*fbmp, 0);
		fourByteWrite(*fbmp, HEAD_SIZE);
		fourByteWrite(*fbmp, DIB_HEAD_SIZE);
		fourByteWrite(*fbmp, width);
		fourByteWrite(*fbmp, height);
		twoByteWrite(*fbmp, 1);
		twoByteWrite(*fbmp, BIT_COUNT);
		fourByteWrite(*fbmp, 0);
		fourByteWrite(*fbmp, size);
		fourByteWrite(*fbmp, 0);
		fourByteWrite(*fbmp, 0);
		fourByteWrite(*fbmp, 0);
		fourByteWrite(*fbmp, 0);
		for (int i = height - 1; i >= 0; i--) {
			for (uint32_t j = 0; j < width; j++) {
				fputc(image[j][i].B, fbmp);
				fputc(image[j][i].G, fbmp);
				fputc(image[j][i].R, fbmp);
				fputc(image[j][i].A, fbmp);
			}
		}
		fclose(fbmp);
		return true;
	}
	return false;
}