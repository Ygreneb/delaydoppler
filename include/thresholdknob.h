#pragma once

#include "vstgui/vstgui.h"

namespace VSTGUI
{

class ThresholdKnob : public CAnimKnob
{
public:
	ThresholdKnob(const CRect& size, IControlListener* listener, int32_t tag, CBitmap* background, const CPoint& offset = CPoint(0, 0))
		: CAnimKnob(size, listener, tag, background, offset)
	{}

	CLASS_METHODS(ThresholdKnob, CAnimKnob)

	void draw(CDrawContext* pContext) override;

	inline CBitmap* getExtraBitmap() const { return mExtraBitmap; }
	inline void setExtraBitmap(CBitmap* newBitmap) 
	{ 
		if (!newBitmap || newBitmap == mExtraBitmap)
		{
			return;
		}
		mExtraBitmap = newBitmap;
	}

protected:
	CBitmap* mExtraBitmap = nullptr;
};

}