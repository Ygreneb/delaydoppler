///------------------------------------------------------------------------

#pragma once

//#include "PathSubcontroller.h"
#include "point.h"
#include "parameters.h"

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "vstgui/plugin-bindings/vst3editor.h"
//#include "vstgui/uidescription/icontroller.h"

//#include "plugids.h"

namespace Steinberg {
namespace DelayDoppler {

class PathSubcontroller;

//-----------------------------------------------------------------------------
class PlugController : public Vst::EditController, public VSTGUI::VST3EditorDelegate
{
	using IController = VSTGUI::IController;
	using UTF8StringPtr = VSTGUI::UTF8StringPtr;
	using IUIDescription = VSTGUI::IUIDescription;
	using VST3Editor = VSTGUI::VST3Editor;

public:
//------------------------------------------------------------------------
	// create function required for Plug-in factory,
	// it will be called to create new instances of this controller
//------------------------------------------------------------------------
	static FUnknown* createInstance (void*)
	{
		return (Vst::IEditController*)new PlugController ();
	}

	PlugController()
		: mPoints(new Point[kMaxNumPoints])
		, mNumPoints(2)
	{
		// these values are overwritten anyway
		for (int i = 0; i < kMaxNumPoints; ++i)
		{
			//mPoints[i].x = (float)(i / (kMaxNumPoints - 1)) * 0.8f + 0.1f;
			//mPoints[i].y = (float)(i / (kMaxNumPoints - 1)) * 0.8f + 0.1f;
			mPoints[i].x = 0.5f;
			mPoints[i].y = 0.5f;
		}
	}

	~PlugController()
	{
		delete[] mPoints;
	}

	//---from IPluginBase--------
	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;

	//---from EditController-----
	IPlugView* PLUGIN_API createView (const char* name) SMTG_OVERRIDE;
	tresult PLUGIN_API setComponentState (IBStream* state) SMTG_OVERRIDE;
	
	//---from VST3EditorDelegate-
	IController* createSubController(UTF8StringPtr name, const IUIDescription* description, VST3Editor* editor) SMTG_OVERRIDE;

	//tresult PlugController::notify(Vst::IMessage* message) SMTG_OVERRIDE;
	void removePathSubcontroller(PathSubcontroller* mc)
	{
		if (mPathSubcontroller && mc == mPathSubcontroller)
		{
			mPathSubcontroller = nullptr;
		}
	}

	tresult getPoints(Point* points, int32& numPoints) const
	{
		if (!mPoints)
			return kResultFalse;

		if (!points)
			points = new Point[kMaxNumPoints];

		memcpy(points, mPoints, kMaxNumPoints * sizeof(Point));
		numPoints = mNumPoints;
		return kResultOk;
	}

	tresult PLUGIN_API setParamNormalized(Vst::ParamID tag, Vst::ParamValue value) SMTG_OVERRIDE;

private:
	PathSubcontroller* mPathSubcontroller = nullptr;

	Point* mPoints;
	int32 mNumPoints;
};

//------------------------------------------------------------------------
} // namespace DelayDoppler
} // namespace Steinberg
