
#include "../include/PathEdit.h"
//#include "../include/parameters.h"

namespace VSTGUI {

PathEdit::PathEdit(const CRect& size)
	: CControl(size)
{
	//Utilities::SplinePoint s1, s2;
	//s1.time = 0.f;
	//s2.time = 1.f;
	//mSpline.addSplinePoint(s1);
	//mSpline.addSplinePoint(s2);
	//pointA.x = 0.1f;
	//pointA.y = .33f;
	//pointB.x = .66f;
	//pointB.y = 0.9f;
	mPoints = new Point[kMaxNumPoints];
	mNumPoints = 0;
}

PathEdit::~PathEdit()
{
	delete[] mPoints;
}

void PathEdit::draw(CDrawContext* context)
{
	// --- Draw background
	//drawBack(context);
	//context->setLineWidth(1);
	//context->setFillColor(getBackground);
	//context->setFrameColor(CColor(0, 0, 0, 255));
	//context->drawRect(getViewSize(), kDrawFilled);

	// --- Draw listener
	CCoord width = getWidth();
	CCoord height = getHeight();
	//CRect l(0.5 * width, 0.5 * height, 0.5 * width, 0.5 * height);
	//l.extend(kListenerRadius, kListenerRadius);
	//l.offset(getViewSize().left, getViewSize().top);
	//context->setFillColor(CColor(200, 200, 200, 150));
	//context->setDrawMode(kAntiAliasing);
	//context->drawEllipse(l, kDrawFilled);

	// --- Draw line of movement
	if (mNumPoints >= 2)
	{
		CLineStyle ls;
		//ls.setDashPhase(10.0);
		context->setLineStyle(ls);
		context->setLineWidth(2.0);
		context->setFrameColor(mPathColor);

		const int numDashes = 30 * (mNumPoints - 1); // 10 dashes for every segment
		float dt = 0.5f / static_cast<float>(numDashes);

		for (int i = 0; i < 2 * numDashes; i += 2)
		{
			Point start = CubicHermiteSpline(mPoints, mNumPoints, i * dt, mLoop);
			Point end = CubicHermiteSpline(mPoints, mNumPoints, (i + 1) * dt, mLoop);
			CPoint p_start(start.x * width, start.y * height);
			CPoint p_end(end.x * width, end.y * height);
			p_start.offset(getViewSize().left, getViewSize().top);
			p_end.offset(getViewSize().left, getViewSize().top);
			context->drawLine(std::make_pair(p_start, p_end));
		}

		/*for (int i = 0; i < mNumPoints - 1; ++i)
		{
			CPoint lstart(mPoints[i].x * width, mPoints[i].y * height);
			CPoint lend(mPoints[i+1].x * width, mPoints[i+1].y * height);
			lstart.offset(getViewSize().left, getViewSize().top);
			lend.offset(getViewSize().left, getViewSize().top);

			context->drawLine(std::make_pair(lstart, lend));
		}*/
	}

	// --- Draw progress point / sound emitter
	if (mProgress > 0.001 && mProgress < 0.999 && mNumPoints >= 2 && mEmitterBitmap != nullptr)
	{
		//Point p_point = LinearInterpolation(mPoints, mNumPoints, mProgress);
		Point p_point = CubicHermiteSpline(mPoints, mNumPoints, mProgress, mLoop);
		CPoint p_center;
		p_center.x = p_point.x * width;
		p_center.y = p_point.y * height;

		CRect p(p_center, CPoint());
		p.extend(0.5 * mEmitterBitmap->getWidth(), 0.5 * mEmitterBitmap->getHeight());
		//p.extend(kPointRadius, kPointRadius);
		p.offset(getViewSize().left, getViewSize().top);
		//context->setFillColor(CColor(255, 0, 0, 100));
		context->setDrawMode(kAntiAliasing);
		//context->drawEllipse(p, kDrawFilled);

		context->drawBitmap(mEmitterBitmap, p);
	}

	// --- Draw points
	//float x, y;
	//calculateXY(getValue(), x, y);
	for (int i = 0; i < mNumPoints; ++i)
	{
		CRect r(mPoints[i].x * width, mPoints[i].y * height, mPoints[i].x * width, mPoints[i].y * height);
		r.extend(kPointRadius, kPointRadius);
		r.offset(getViewSize().left, getViewSize().top);

		context->setFillColor(mPointColor);
		context->setDrawMode(kAntiAliasing);
		context->drawEllipse(r, kDrawFilled);
	}

	setDirty(false);
}



}