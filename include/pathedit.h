#pragma once

#include "point.h"
#include "parameters.h"
#include "cubichermitespline.h"

#include "vstgui/vstgui.h"

namespace VSTGUI {

using namespace Steinberg::DelayDoppler;

class PathEdit : public CControl
{

public:
	PathEdit(const CRect& size);
	~PathEdit();

	//void draw(CDrawContext* pContext) override;

	void getPoints(Point* out, uint32_t& num)
	{
		num = mNumPoints;
		memcpy(out, mPoints, kMaxNumPoints * sizeof(Point));
	}

	void setPoints(Point* in, uint32_t num)
	{
		if (num > kMaxNumPoints) num = kMaxNumPoints;

		if (in && mPoints)
		{
			mNumPoints = num;
			memcpy(mPoints, in, kMaxNumPoints * sizeof(Point));
		}
	}

	CLASS_METHODS(PathEdit, CControl)

	CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons) override
	{
		if (buttons.isLeftButton() && mClickedPoint == nullptr)
		{
			//invalidMouseWheelEditTimer(this);
			//mouseStartValue = getValue();
			//mouseChangeStartPoint = where;
			//mouseChangeStartPoint.offset(-getViewSize().left - getRoundRectRadius() / 2., -getViewSize().top - getRoundRectRadius() / 2.);

			CCoord width = getWidth();
			CCoord height = getHeight();
			CPoint click = where;
			click.offset(-getViewSize().left, -getViewSize().top);

			for (int i = 0; i < kMaxNumPoints; ++i)
			{
				CRect r(mPoints[i].x * width, mPoints[i].y * height, mPoints[i].x * width, mPoints[i].y * height);
				r.extend(kPointRadius, kPointRadius);

				if (r.pointInside(click))
				{
					mClickedPoint = &mPoints[i];// +i * sizeof(Point);
					beginEdit();
					return onMouseMoved(where, buttons);
				}
			}		
		}
		return kMouseEventNotHandled;

		//valueChanged(); // This way the control listener gets notified and can call getPoints()
		
		//return kMouseEventHandled;
		//kMouseEventNotHandled,
	}

	CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& buttons)
	{
		if (buttons.isLeftButton() && mClickedPoint != nullptr)
		{
			/*if (stopTrackingOnMouseExit)
			{
				if (!hitTest(where, buttons))
				{
					endEdit();
					return kMouseMoveEventHandledButDontNeedMoreEvents;
				}
			}*/
			CCoord width = getWidth();
			CCoord height = getHeight();
			where.offset(-getViewSize().left, -getViewSize().top);

			float x, y;
			x = (float)(where.x / width);
			y = (float)(where.y / height);
			mClickedPoint->x = x;
			mClickedPoint->y = y;

			//setValue(x);

			//setValue(calculateValue(x, y));
			//if (isDirty())
			//{
			valueChanged();
			setDirty(true);//invalid();
			//}
			//lastMouseChangePoint = where;
			return kMouseEventHandled;
		}
		return kMouseEventNotHandled;
	}

	CMouseEventResult onMouseUp(CPoint& where, const CButtonState& buttons)
	{
		if (isEditing())
		{
			mClickedPoint = nullptr;
			endEdit();
			return kMouseEventHandled;
		}
		return kMouseEventNotHandled;
	}

	//CMouseEventResult onMouseCancel()
	//{
	//	if (isEditing())
	//	{
	//		//value = mouseStartValue;
	//		//if (isDirty())
	//		//{
	//		clickedPoint = nullptr;
	//		valueChanged();
	//		invalid();
	//		//}
	//		endEdit();
	//	}
	//	return kMouseEventHandled;
	//}

	void draw(CDrawContext* context) override;

	inline void setProgress(double p)
	{
		mProgress = p;
		setDirty(true); // triggers redraw
	}

	inline void setLoop(bool l)
	{
		mLoop = l;
		setDirty(true); // triggers redraw
	}

	void addPoint()
	{
		if (mNumPoints >= kMaxNumPoints)
		{
			return;
		}

		//mPoints[mNumPoints].x = 0.5f;
		//mPoints[mNumPoints].y = 0.5f;
		mNumPoints++;

		setDirty(true);
		valueChanged();
	}

	void removePoint()
	{
		if (mNumPoints <= 2)
		{
			return;
		}

		//mPoints[mNumPoints].x = -1.0f;
		//mPoints[mNumPoints].y = -1.0f;
		mNumPoints--;

		setDirty(true);
		valueChanged();
	}

	inline CBitmap* getEmitter() const { return mEmitterBitmap; }
	inline void setEmitter(CBitmap* newEmitter)
	{
		if (!newEmitter || newEmitter == mEmitterBitmap)
		{
			return;
		}

		mEmitterBitmap = newEmitter;
	}

	inline CColor getPointColor() const { return mPointColor; }
	inline void setPointColor(CColor newColor) { mPointColor = newColor; }

	inline CColor getPathColor() const { return mPathColor; }
	inline void setPathColor(CColor newColor) { mPathColor = newColor; }

	//static float calculateValue(Point p1, Point p2, Point p3)
	//{
	//	float v1 = std::floor(p1.x * 100.f + 0.5f) * 0.01f;
	//	float v2 = std::floor(p1.y * 100.f + 0.5f) * 0.0001f;
	//	float v3 = std::floor(p2.x * 100.f + 0.5f) * 0.000001f;
	//	float v4 = std::floor(p2.y * 100.f + 0.5f) * 0.00000001f;
	//	float v5 = std::floor(p3.x * 100.f + 0.5f) * 0.0000000001f;
	//	float v6 = std::floor(p3.y * 100.f + 0.5f) * 0.000000000001f;
	//	
	//	return v1 + v2 + v3 + v4 + v5 + v6;
	//}

	//static void calculatePoints(float value, Point& p1, Point& p2, Point& p3)
	//{
	//	float sum = 0.f;
	//	p1.x = std::floor( value        * 100.f + 0.5f) * 0.01f;
	//	sum += p1.x;
	//	p1.y = std::floor((value - sum) * 10000.f + 0.5f) * 0.01f;
	//	sum += p1.y;
	//	p2.x = std::floor((value - sum) * 1000000.f + 0.5f) * 0.01f;
	//	sum += p2.x;
	//	p2.y = std::floor((value - sum) * 100000000.f + 0.5f) * 0.01f;
	//	sum += p2.y;
	//	p3.x = std::floor((value - sum) * 10000000000.f + 0.5f) * 0.01f;
	//	sum += p3.x;
	//	p3.y = std::floor((value - sum) * 1000000000000.f + 0.5f) * 0.01f;
	//	//sum += p3.y;
	//}

private:
	//CubicHermiteSpline mSpline;
	//Point pointA, pointB;
	Point* mPoints;
	int32_t mNumPoints;
	Point* mClickedPoint = nullptr;

	double mProgress;
	bool mLoop;

	const CCoord kPointRadius = 10.;
	const CCoord kListenerRadius = 20.;

	CBitmap* mEmitterBitmap = nullptr;
	CColor mPointColor;
	CColor mPathColor;
};

}