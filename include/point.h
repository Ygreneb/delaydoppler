#pragma once

#include <cmath>

namespace Steinberg {
namespace DelayDoppler {

struct Point
{
	float x;
	float y;

	Point()
		: x(0.f), y(0.f) {}

	Point(float _x, float _y)
		: x(_x), y(_y) {}

	inline float lenSq() const
	{
		return (x * x + y * y);
	}

	inline float len() const
	{
		return std::sqrtf(x * x + y * y);
	}

	inline Point& operator*=(float f)
	{
		x *= f;
		y *= f;
		return *this;
	}
};

inline Point operator*(Point p, const float& v)
{
	p.x *= v;
	p.y *= v;
	return p;
}

inline Point operator*(const float& v, Point p)
{
	p.x *= v;
	p.y *= v;
	return p;
}

inline Point operator+(Point p1, const Point& p2)
{
	p1.x += p2.x;
	p1.y += p2.y;
	return p1;
}

inline Point operator-(Point p1, const Point& p2)
{
	p1.x -= p2.x;
	p1.y -= p2.y;
	return p1;
}

inline Point operator-(Point p, float f)
{
	p.x -= f;
	p.y -= f;
	return p;
}

/*
 * 0 <= t <= 1
 */
//static Point LinearInterpolation(const Point& a, const Point& b, float t)
//{
//	return a + t * (b - a);
//}


}
}
