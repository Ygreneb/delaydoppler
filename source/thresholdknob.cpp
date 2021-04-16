
#include "../include/thresholdknob.h"
#include "../include/parameters.h"

namespace VSTGUI
{

void ThresholdKnob::draw(CDrawContext* pContext)
{
	float v = getValueNormalized();
	if (v < Steinberg::DelayDoppler::kAutoStartThreshold && mExtraBitmap != nullptr)
		mExtraBitmap->draw(pContext, getViewSize());
	else
		CAnimKnob::draw(pContext);
}

}