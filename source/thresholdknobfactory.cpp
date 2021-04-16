
#include "../include/thresholdknob.h"

#include "vstgui/vstgui.h"
#include "vstgui/vstgui_uidescription.h"
#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"
#include "vstgui/uidescription/viewcreator/knobcreator.h"

namespace VSTGUI {
namespace UIViewCreator {

static const IdStringPtr kThresholdKnob = "ThresholdKnob";

static const std::string kAttrExtraBitmap = "extra-bitmap";

class ThresholdKnobFactory : public KnobBaseCreator
{
public:
	ThresholdKnobFactory() { UIViewFactory::registerViewCreator(*this); }

	IdStringPtr getViewName() const override { return kThresholdKnob; }
	IdStringPtr getDisplayName() const override { return "Threshold Knob"; }
	IdStringPtr getBaseViewName() const override { return UIViewCreator::kCAnimKnob; }

	CView* create(const UIAttributes& attributes, const IUIDescription* description) const override
	{
		return new ThresholdKnob(CRect(0, 0, 0, 0), nullptr, -1, nullptr);
	}

	//------------------------------------------------------------------------
	bool apply(CView* view, const UIAttributes& attributes, const IUIDescription* description) const override
	{
		auto* threshKnob = dynamic_cast<ThresholdKnob*> (view);
		if (!threshKnob)
			return false;

		CBitmap* bitmap;
		if (stringToBitmap(attributes.getAttributeValue(kThresholdKnob), bitmap, description))
			threshKnob->setExtraBitmap(bitmap);

		//CColor color;
		//if (stringToColor(attributes.getAttributeValue(kAttrPointColor), color, description))
		//	pathEdit->setPointColor(color);
		//if (stringToColor(attributes.getAttributeValue(kAttrPathColor), color, description))
		//	pathEdit->setPathColor(color);

		//CPoint p;
		//if (attributes.getPointAttribute(kAttrBitmapOffset, p))
		//	slider->setBackgroundOffset(p);

		//int32_t drawStyle = slider->getDrawStyle();
		//applyStyleMask(attributes.getAttributeValue(kAttrDrawFrame), CSlider::kDrawFrame, drawStyle);
		//applyStyleMask(attributes.getAttributeValue(kAttrDrawBack), CSlider::kDrawBack, drawStyle);
		//applyStyleMask(attributes.getAttributeValue(kAttrDrawValue), CSlider::kDrawValue, drawStyle);
		//applyStyleMask(attributes.getAttributeValue(kAttrDrawValueFromCenter),
		//	CSlider::kDrawValueFromCenter, drawStyle);
		//applyStyleMask(attributes.getAttributeValue(kAttrDrawValueInverted), CSlider::kDrawInverted,
		//	drawStyle);
		//slider->setDrawStyle(drawStyle);

		//CCoord lineWidth;
		//if (attributes.getDoubleAttribute(kAttrFrameWidth, lineWidth))
		//	slider->setFrameWidth(lineWidth);

		//CColor color;
		//if (stringToColor(attributes.getAttributeValue(kAttrDrawFrameColor), color, description))
		//	slider->setFrameColor(color);
		//if (stringToColor(attributes.getAttributeValue(kAttrDrawBackColor), color, description))
		//	slider->setBackColor(color);
		//if (stringToColor(attributes.getAttributeValue(kAttrDrawValueColor), color, description))
		//	slider->setValueColor(color);
		return KnobBaseCreator::apply(view, attributes, description);
	}

	//------------------------------------------------------------------------
	bool getAttributeNames(StringList& attributeNames) const override
	{
		attributeNames.emplace_back(kThresholdKnob);
		//attributeNames.emplace_back(kAttrPointColor);
		//attributeNames.emplace_back(kAttrPathColor);
		//attributeNames.emplace_back(kAttrBitmapOffset);
		//attributeNames.emplace_back(kAttrDrawFrame);
		//attributeNames.emplace_back(kAttrDrawBack);
		//attributeNames.emplace_back(kAttrDrawValue);
		//attributeNames.emplace_back(kAttrDrawValueFromCenter);
		//attributeNames.emplace_back(kAttrDrawValueInverted);
		//attributeNames.emplace_back(kAttrFrameWidth);
		//attributeNames.emplace_back(kAttrDrawFrameColor);
		//attributeNames.emplace_back(kAttrDrawBackColor);
		//attributeNames.emplace_back(kAttrDrawValueColor);
		return KnobBaseCreator::getAttributeNames(attributeNames);
	}

	//------------------------------------------------------------------------
	AttrType getAttributeType(const string& attributeName) const override
	{
		if (attributeName == kThresholdKnob)
			return kBitmapType;
		//if (attributeName == kAttrPointColor)
		//	return kColorType;
		//if (attributeName == kAttrPathColor)
		//	return kColorType;
		//if (attributeName == kAttrBitmapOffset)
		//	return kPointType;
		//if (attributeName == kAttrDrawFrame)
		//	return kBooleanType;
		//if (attributeName == kAttrDrawBack)
		//	return kBooleanType;
		//if (attributeName == kAttrDrawValue)
		//	return kBooleanType;
		//if (attributeName == kAttrDrawValueFromCenter)
		//	return kBooleanType;
		//if (attributeName == kAttrDrawValueInverted)
		//	return kBooleanType;
		//if (attributeName == kAttrFrameWidth)
		//	return kFloatType;
		//if (attributeName == kAttrDrawFrameColor)
		//	return kColorType;
		//if (attributeName == kAttrDrawBackColor)
		//	return kColorType;
		//if (attributeName == kAttrDrawValueColor)
		//	return kColorType;
		return KnobBaseCreator::getAttributeType(attributeName);
	}

	//------------------------------------------------------------------------
	bool getAttributeValue(CView* view, const string& attributeName, string& stringValue, const IUIDescription* desc) const override
	{
		auto* threshKnob = dynamic_cast<ThresholdKnob*> (view);
		if (!threshKnob)
			return false;
		if (attributeName == kThresholdKnob)
		{
			CBitmap* bitmap = threshKnob->getExtraBitmap();
			if (bitmap)
			{
				bitmapToString(bitmap, stringValue, desc);
			}
			return true;
		}
		//if (attributeName == kAttrPointColor)
		//{
		//	colorToString(pathEdit->getPointColor(), stringValue, desc);
		//	return true;
		//}
		//if (attributeName == kAttrPathColor)
		//{
		//	colorToString(pathEdit->getPathColor(), stringValue, desc);
		//	return true;
		//}
		//if (attributeName == kAttrBitmapOffset)
		//{
		//	stringValue = UIAttributes::pointToString(slider->getBackgroundOffset());
		//	return true;
		//}
		//if (attributeName == kAttrDrawFrame)
		//{
		//	if (slider->getDrawStyle() & CSlider::kDrawFrame)
		//		stringValue = strTrue;
		//	else
		//		stringValue = strFalse;
		//	return true;
		//}
		//if (attributeName == kAttrDrawBack)
		//{
		//	if (slider->getDrawStyle() & CSlider::kDrawBack)
		//		stringValue = strTrue;
		//	else
		//		stringValue = strFalse;
		//	return true;
		//}
		//if (attributeName == kAttrDrawValue)
		//{
		//	if (slider->getDrawStyle() & CSlider::kDrawValue)
		//		stringValue = strTrue;
		//	else
		//		stringValue = strFalse;
		//	return true;
		//}
		//if (attributeName == kAttrDrawValueFromCenter)
		//{
		//	if (slider->getDrawStyle() & CSlider::kDrawValueFromCenter)
		//		stringValue = strTrue;
		//	else
		//		stringValue = strFalse;
		//	return true;
		//}
		//if (attributeName == kAttrDrawValueInverted)
		//{
		//	if (slider->getDrawStyle() & CSlider::kDrawInverted)
		//		stringValue = strTrue;
		//	else
		//		stringValue = strFalse;
		//	return true;
		//}
		//if (attributeName == kAttrDrawFrameColor)
		//{
		//	colorToString(slider->getFrameColor(), stringValue, desc);
		//	return true;
		//}
		//if (attributeName == kAttrDrawBackColor)
		//{
		//	colorToString(slider->getBackColor(), stringValue, desc);
		//	return true;
		//}
		//if (attributeName == kAttrDrawValueColor)
		//{
		//	colorToString(slider->getValueColor(), stringValue, desc);
		//	return true;
		//}
		//if (attributeName == kAttrFrameWidth)
		//{
		//	stringValue = UIAttributes::doubleToString(slider->getFrameWidth());
		//	return true;
		//}
		return KnobBaseCreator::getAttributeValue(view, attributeName, stringValue, desc);
	}
};

ThresholdKnobFactory __thresholdKnobFactory;
}
}