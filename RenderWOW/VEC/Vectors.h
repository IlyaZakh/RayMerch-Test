#pragma once
#include <math.h>

struct vec2
{
	double x, y;

    vec2(double _value) : x(_value), y(_value) {};
    vec2(double _x, double _y) : x(_x), y(_y) {};

    vec2 operator+(vec2 other) { return vec2(x + other.x, y + other.y); }
    vec2 operator-(vec2 other) { return vec2(x - other.x, y - other.y); }
    vec2 operator/(vec2 other) { return vec2(x / other.x, y / other.y); }
};

struct vec3
{
	double x, y, z;

	vec3(double _value) : x(_value), y(_value), z(_value) {};
	vec3(vec2 v, double _z) : x(v.x), y(v.y), z(_z) {};
	vec3(double _x, vec2 v) : x(_x), y(v.x), z(v.y) {};
	vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};

	vec3 operator+(vec3 other) { return vec3(x + other.x, y + other.y, z + other.z); }
	vec3 operator-(vec3 other) { return vec3(x - other.x, y - other.y, z - other.z); }
	vec3 operator*(vec3 other) { return vec3(x * other.x, y * other.y, z * other.z); }
	vec3 operator/(vec3 other) { return vec3(x / other.x, y / other.y, z / other.z); }
	vec3 operator-() { return vec3(-x, -y, -z); }

	vec3 xxx() { return vec3(x, x, x); }
	vec3 xxy() { return vec3(x, x, y); }
	vec3 xxz() { return vec3(x, x, z); }
	vec3 xyx() { return vec3(x, y, x); }
	vec3 xyy() { return vec3(x, y, y); }
	vec3 xyz() { return vec3(x, y, z); }
	vec3 xzx() { return vec3(x, z, x); }
	vec3 xzy() { return vec3(x, z, y); }
	vec3 xzz() { return vec3(x, z, z); }
	vec3 yxx() { return vec3(y, x, x); }
	vec3 yxy() { return vec3(y, x, y); }
	vec3 yxz() { return vec3(y, x, z); }
	vec3 yyx() { return vec3(y, y, x); }
	vec3 yyy() { return vec3(y, y, y); }
	vec3 yyz() { return vec3(y, y, z); }
	vec3 yzx() { return vec3(y, z, x); }
	vec3 yzy() { return vec3(y, z, y); }
	vec3 yzz() { return vec3(y, z, z); }
	vec3 zxx() { return vec3(z, x, x); }
	vec3 zxy() { return vec3(z, x, y); }
	vec3 zxz() { return vec3(z, x, z); }
	vec3 zyx() { return vec3(z, y, x); }
	vec3 zyy() { return vec3(z, y, y); }
	vec3 zyz() { return vec3(z, y, z); }
	vec3 zzx() { return vec3(z, z, x); }
	vec3 zzy() { return vec3(z, z, y); }
	vec3 zzz() { return vec3(z, z, z); }

};

double sign(double a) { return (double)(0 < a) - (double)(a < 0); }
double step(double edge, double x) { return x > edge; }
double length(vec2 v) { return sqrt(v.x * v.x + v.y * v.y); }
double length(vec3 v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }
vec2 norm(vec2 v) { return v / length(v); }
vec3 norm(vec3 v) { return v / length(v); }
double dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
vec3 abs(vec3 v) { return vec3(fabs(v.x), fabs(v.y), fabs(v.z)); }
vec3 sign(vec3 v) { return vec3(sign(v.x), sign(v.y), sign(v.z)); }
vec3 step(vec3 edge, vec3 v) { return vec3(step(edge.x, v.x), step(edge.y, v.y), step(edge.z, v.z)); }

vec3 rotateX(vec3 a, double angle)
{
	vec3 b = a;
	b.z = a.z * cos(angle) - a.y * sin(angle);
	b.y = a.z * sin(angle) + a.y * cos(angle);
	return b;
}

vec3 rotateY(vec3 a, double angle)
{
	vec3 b = a;
	b.x = a.x * cos(angle) - a.z * sin(angle);
	b.z = a.x * sin(angle) + a.z * cos(angle);
	return b;
}

vec3 rotateZ(vec3 a, double angle)
{
	vec3 b = a;
	b.x = a.x * cos(angle) - a.y * sin(angle);
	b.y = a.x * sin(angle) + a.y * cos(angle);
	return b;
}

vec2 sphIntersect(vec3 ro, vec3 rd, vec3 ce, double ra)
{
	vec3 oc = ro - ce;
	double b = dot(oc, rd);
	double c = dot(oc, oc) - ra * ra;
	double h = b * b - c;
	if (h < 0.0) return vec2(-1.0);
	h = sqrt(h);
	return vec2(-b - h, -b + h);
}

vec2 boxIntersection(vec3 ro, vec3 rd, vec3 boxSize, vec3& outNormal)
{
	vec3 m = vec3(1.0) / rd;
	vec3 n = m * ro;
	vec3 k = abs(m) * boxSize;
	vec3 t1 = -n - k;
	vec3 t2 = -n + k;
	double tN = fmax(fmax(t1.x, t1.y), t1.z);
	double tF = fmin(fmin(t2.x, t2.y), t2.z);
	if (tN > tF || tF < 0.0) return vec2(-1.0);
	vec3 yzx = vec3(t1.y, t1.z, t1.x);
	vec3 zxy = vec3(t1.z, t1.x, t1.y);
	outNormal = -sign(rd) * step(yzx, t1) * step(zxy, t1);
	return vec2(tN, tF);
}