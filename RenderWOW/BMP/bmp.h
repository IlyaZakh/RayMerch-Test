#pragma once
#include <string>

using namespace std;

typedef uint8_t byte;

struct pixel
{
	byte R, G, B, A;
};

bool genBMP(uint32_t, uint32_t, string, pixel**);