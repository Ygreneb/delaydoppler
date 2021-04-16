
#include "../include/cubichermitespline.h"

namespace Steinberg {
namespace DelayDoppler {

//Point CubicHermiteSpline::getPositionAtTime(float t) const
//{
//	if (points.empty())
//	{
//		return Point();
//	}
//	
//	const SplinePoint* p0 = nullptr;
//	const SplinePoint* p1 = nullptr;
//
//	// find points around time t
//	for (size_t i = 0; i < points.size() - 1; ++i)
//	{
//		if (points[i].time <= t && t <= points[i + 1].time)
//		{
//			p0 = &points[i];
//			p1 = &points[i + 1];
//			break;
//		}
//	}
//
//	// t not covered? Return first or last point
//	if (!p0 || !p1)
//	{
//		if (t < points[0].time) { return points[0].pos; }
//		else /* t > points[-1].time */ { return points.back().pos; }
//	}
//
//	// calculate position at time t
//	// from https://de.wikipedia.org/wiki/Kubisch_Hermitescher_Spline
//	float d = (p1->time - p0->time);
//	float tt = (t - p0->time) / d;
//	float tt2 = tt * tt;
//	float tt3 = tt2 * tt;
//
//	return
//		(2 * tt3 - 3 * tt2 + 1) * p0->pos +
//		(-2 * tt3 + 3 * tt2) * p1->pos +
//		(tt3 - 2 * tt2 + tt) * d * p0->tan +
//		(tt3 - tt2) * d * p1->tan;
//}

}
}



//bool CubicHermiteSpline::click(const Point<float>& pos, bool isClick)
//{
//	for (auto& p : points)
//	{
//		// point handle = circle
//		Point<float> dist = p.pos - pos;
//		const float radius = .5f * handleDiameter;
//		if (dist.x * dist.x + dist.y * dist.y < radius * radius)
//		{
//			if (isClick)
//			{
//				clickedPoint = &p;
//				isTangentClicked = false;
//			}
//			return true;
//		}
//
//		// tangent handle = square
//		if (Rectangle<float>(p.pos + p.tan - halfDiameter, p.pos + p.tan + halfDiameter).contains(pos))
//		{
//			if (isClick)
//			{
//				clickedPoint = &p;
//				isTangentClicked = true;
//			}
//			return true;
//		}
//	}
//
//	return false;
//}

//void CubicHermiteSpline::paint(Graphics& g)
//{
//	g.setColour(getLookAndFeel().findColour(Slider::rotarySliderFillColourId));
//
//	for (const auto& p : points)
//	{
//		// path/spline
//		g.strokePath(path, PathStrokeType(3));
//
//		// point handle
//		g.fillEllipse(Rectangle<float>(p.pos - halfDiameter, p.pos + halfDiameter));
//
//		// tangent handle
//		g.fillRect(Rectangle<float>(p.pos + p.tan - halfDiameter, p.pos + p.tan + halfDiameter));
//
//		// connection line between handles
//		const float dashLengths[] = { 1,3 };
//		g.drawDashedLine(Line<float>(p.pos, p.pos + p.tan), dashLengths, 2);
//	}
//}

//bool CubicHermiteSpline::hitTest(int x, int y)
//{
//	//if (onMouseDown) onMouseDown(e);
//	return click(Point<float>(x, y), false);
//}
//
//void CubicHermiteSpline::mouseDown(const MouseEvent& e)
//{
//	//if (onMouseDown) onMouseDown(e);
//	click(e.mouseDownPosition, true);
//}
//
//void CubicHermiteSpline::mouseUp(const MouseEvent& e)
//{
//	//if (onMouseUp) onMouseUp(e);
//	clickedPoint = nullptr;
//}
//
//void CubicHermiteSpline::mouseDrag(const MouseEvent& e)
//{
//	//if (onMouseDrag) onMouseDrag(e);
//	if (clickedPoint && getBounds().contains(e.getPosition()))
//	{
//		//Point<float> pos(e.getPosition().toFloat());
//		if (isTangentClicked)
//		{
//			clickedPoint->tan = e.position - clickedPoint->pos;
//		}
//		else
//		{
//			clickedPoint->pos = e.position;
//		}
//
//		//if (onPointDragged) { onPointDragged(); }
//		//MovementPathComponent* parent = static_cast<MovementPathComponent*>(getParentComponent());
//		//parent->updatePath();
//		updatePath();
//	}
//}

//void CubicHermiteSpline::updatePath()
//{
//	float tMax = points.back().time;
//	float dt = tMax / 100.f;
//
//	path.clear();
//
//	for (float t = 0.f; t < tMax; t += dt)
//	{
//		Point<float> p = getPositionAtTime(t);
//		if (t < dt)
//			path.startNewSubPath(p);
//		else
//			path.lineTo(p);
//	}
//}