#pragma once

#include "parameters.h"

#include "public.sdk/source/vst/vstparameters.h"
#include "pluginterfaces/base/ustring.h"

namespace Steinberg {
namespace DelayDoppler {

class ThresholdParameter : public Vst::Parameter
{
public:
	ThresholdParameter(const Vst::TChar* title, Vst::ParamID tag, const Vst::TChar* units = nullptr,
		Vst::ParamValue defaultValueNormalized = 0., int32 stepCount = 0,
		int32 flags = Vst::ParameterInfo::kCanAutomate, Vst::UnitID unitID = Vst::kRootUnitId,
		const Vst::TChar* shortTitle = nullptr)
		: Vst::Parameter(title, tag, units, defaultValueNormalized, stepCount, flags, unitID, shortTitle)
	{}

	/** Converts a normalized value to a string. */
	virtual void toString(Vst::ParamValue valueNormalized, Vst::String128 string) const override
	{
		UString wrapper(string, str16BufferSize(Vst::String128));

		if (valueNormalized <= kAutoStartThreshold)
		{
			wrapper.assign( STR16("MIDI") );
		}
		else
		{
			if (!wrapper.printFloat(valueNormalized, precision))
				string[0] = 0;
		}

	}
	///** Converts a string to a normalized value. */
	//virtual bool fromString(const Vst::TChar* string, Vst::ParamValue& valueNormalized) const override
	//{

	//}
};

}
}
