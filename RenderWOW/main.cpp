#include <thread>
#include <iostream>
#include <math.h>
#include "BMP/bmp.h"
#include "VEC/Vectors.h"

const size_t START_FRAME = 0; //начальный кадр (кратно ста т.е. 0 = 0, 1 = начать с 100 кадра, 2 = начать с 200 кадра)
const size_t COUNT_OF_FRAMES = 2; //количество кадров (кратно ста т.е. 0 = 0 кадров, 1 = 100 кадров, 2 = 200 кадров)
//обусловленно тем, что программа создает за раз 100 потоков, в которых рендерится картинка

void newPicture(int t) {
	int width = 1920;
	int height = 1080;
	string path = "pic";
	pixel** picture = new pixel * [width];
	for (int i = 0; i < width; i++)
		picture[i] = new pixel[height];
	vec3 spherePos = vec3(0, 3, 0);
	vec3 light = norm(vec3(-0.5, 0.5, -1.0));
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			vec2 uv = vec2(i, j) / vec2(width, height) - 0.5;
			uv.x *= (double)width / (double)height;
			vec3 ro = vec3(-5, 0, 0);
			vec3 rd = norm(vec3(1, uv + 0.1));
			ro = rotateZ(ro, t * 0.01);
			rd = rotateZ(rd, t * 0.01);
			ro = rotateY(ro, t * 0.01);
			rd = rotateY(rd, t * 0.01);
			double diff = 0;
			vec2 minIt = vec2(99999);
			vec3 n = 0;
			vec2 it = sphIntersect(ro, rd, spherePos, 1);
			if (it.x > 0 && it.x < minIt.x)
			{
				minIt = it;
				n = norm(ro - spherePos + rd * it.x);
			}
			vec3 boxN = vec3(0);
			vec2 it2 = boxIntersection(ro, rd, vec3(1), boxN);
			if (it2.x > 0 && it2.x < minIt.x)
			{
				minIt = it2;
				n = boxN;
			}
			diff = dot(n, light);
			int col = 0;
			if (minIt.x < 99999)
			{
				diff = diff * 0.5 + 0.5;
				col = (int)(diff * 255.0);
			}
			if (col < 0) col = 0;
			if (col > 255) col = 255;
			picture[i][j].R = col;
			picture[i][j].G = col;
			picture[i][j].B = col;
			picture[i][j].A = 255;
		}
	genBMP(width, height, path + to_string(t), picture);
	for (int i = 0; i < width; i++)
		delete[] picture[i];
	delete[] picture;
	cout << t << " done" << endl;
}

int main()
{
	setlocale(0, "");

	thread ta[100];

	for (size_t t = START_FRAME; t < START_FRAME + COUNT_OF_FRAMES; t++) {
		cout << "threads " << t * 100 << "-" << t * 100 + 99 << " started" << endl;
		for (int i = 0; i < 100; i++)
			ta[i] = thread(newPicture, t * 100 + i);
		for (int i = 0; i < 100; i++)
			ta[i].join();
		cout << "threads " << t * 100 << "-" << t * 100 + 99 << " done" << endl;
	}
	return 0;
}