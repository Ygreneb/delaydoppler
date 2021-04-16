#pragma once

#include "point.h"
#include "parameters.h"

//#include <algorithm>
//#include <vector>
//#include <cmath>

namespace Steinberg {
namespace DelayDoppler {

//struct SplinePoint
//{
//	Point pos;
//	Point tan;
//	float time;
//};

/*
 * Use with a.time = 0.0 and b.time = 1.0 and 0.0 <= t <= 1.0
 * Tangents are vectors, i.e. they are relative to points
 */
static Point CubicHermiteSplineSegment(const Point& a_pos, const Point& a_tan, const Point& b_pos, const Point& b_tan, float t)
{
	// calculate position at time t
	// from https://de.wikipedia.org/wiki/Kubisch_Hermitescher_Spline
	// d always equals 1.0
	//float d = (b.time - a.time);
	//float tt = (t - a.time) / d;
	//float tt2 = tt * tt;
	//float tt3 = tt2 * tt;

	float t2 = t * t;
	float t3 = t2 * t;

	return
		(2 * t3 - 3 * t2 + 1) * a_pos +
		(-2 * t3 + 3 * t2) * b_pos +
		(t3 - 2 * t2 + t) /** d*/ * a_tan +
		(t3 - t2) /** d*/ * b_tan;
}

// Get position at time 0 <= t <= 1, with points[0] at t = 0 and
// points[numPoints-1] at t = 1. Points are equally spaced along
// time scale
static Point CubicHermiteSpline(const Point* points, int numPoints, float t, bool loop = false)
{
	t = Clip(t, 0.f, 1.f);

	float t_scaled = loop ? t * numPoints : t * (numPoints - 1);
	int t_round = std::round(t_scaled);

	//Point res;
	if (abs(t_scaled - t_round) < 0.0001)
	{
		// No need for interpolation, just take the current point
		return points[t_round % numPoints];
	}
	
	int i_floor = std::floor(t_scaled);
	int i_ceil = std::ceil(t_scaled);
		
	Point a_tan, b_tan;
	
	if (loop)
	{
		a_tan = points[i_ceil % numPoints]		 - points[(i_floor - 1 + numPoints) % numPoints];
		b_tan = points[(i_ceil + 1) % numPoints] - points[i_floor];
	}
	else
	{
		a_tan = (i_floor == 0			 ? Point(0, 0) : points[i_ceil] - points[i_floor - 1]);
		b_tan = (i_ceil == numPoints - 1 ? Point(0, 0) : points[i_ceil + 1] - points[i_floor]);
	}

	// change influence of tangents here
	/*a_tan *= 1.f;
	b_tan *= 1.f;*/
		
	return CubicHermiteSplineSegment(points[i_floor], a_tan, points[i_ceil % numPoints], b_tan, t_scaled - i_floor);

	//return res;
}

/*
 * 0 <= t <= 1
 */
static Point LinearInterpolationSegment(const Point& a, const Point& b, float t)
{
	return a + t * (b - a);
}

// Get position at time 0 <= t <= 1, with points[0] at t = 0 and
// points[numPoints-1] at t = 1. Points are equally spaced along
// time scale
static Point LinearInterpolation(const Point* points, int numPoints, float t)
{
	//if (!points) return Point;

	t = Clip(t, 0.f, 1.f);

	float t_scaled = t * (numPoints - 1);
	int t_round = std::round(t_scaled);
	
	Point res;
	if (abs(t_scaled - t_round) < 0.0001)
	{
		// No need for interpolation, just take the current point
		res = points[t_round];
	}
	else
	{
		int i_floor = std::floor(t_scaled);
		int i_ceil = std::ceil(t_scaled);
		res = LinearInterpolationSegment(points[i_floor], points[i_ceil], t_scaled - i_floor);
	}
	
	return res;
}



//class CubicHermiteSpline
//{
//public:
//	//struct SplinePoint
//	//{
//	//	Point<float> pos;
//	//	Point<float> tan;
//	//	float time;
//	//};
//
//	//CubicHermiteSpline()
//	//	: Component()
//	//	, handleDiameter(10.f)
//	//	, halfDiameter(.5f * handleDiameter, .5f * handleDiameter)
//	//	, clickedPoint(nullptr)
//	//	, isTangentClicked(false)
//	//{};
//
//	Point getPositionAtTime(float t) const;
//
//	void addSplinePoint(const SplinePoint& point)
//	{
//		points.push_back(point);
//		std::sort(points.begin(), points.end(), [](const SplinePoint& a, const SplinePoint& b) { return a.time < b.time; });
//		//updatePath();
//	}
//
//	inline float getDuration() const
//	{
//		if (!points.empty())
//		{
//			return points.back().time;
//		}
//		return 0.f;
//	}
//
//	inline SplinePoint& operator[](size_t index) { return points[index]; }
//
//	// return true if pos is within point or tangent handles
//	// if isClick == true, sets corresponding variables
//	//bool click(const Point<float>& pos, bool isClick = false);
//
//	//inline const std::vector< Point >& getPoints() const { return points; }
//
//	// from Component
//	//virtual void paint(Graphics& g) override;
//	//virtual bool hitTest(int x, int y) override;
//
//	//void updatePath();
//
//	//std::function<void()> onPointDragged;
//private:
//
//	std::vector<SplinePoint> points;
//
//
//	//SplinePoint* clickedPoint;
//	//bool isTangentClicked;
//
//
//	//JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CubicHermiteSpline)
//};

}
}