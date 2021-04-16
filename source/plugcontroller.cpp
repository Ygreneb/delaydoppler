//------------------------------------------------------------------------

#include "../include/plugcontroller.h"

#include "../include/thresholdparam.h"
#include "../include/parameters.h"
#include "../include/plugids.h"
#include "../include/PathSubcontroller.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"

using namespace VSTGUI;

namespace Steinberg {
namespace DelayDoppler {

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugController::initialize (FUnknown* context)
{
	tresult result = EditController::initialize (context);
	if (result == kResultTrue)
	{
		//---Create Parameters------------
		parameters.addParameter (STR16 ("Bypass"), nullptr, 1, 0,
		                         Vst::ParameterInfo::kCanAutomate | Vst::ParameterInfo::kIsBypass,
		                         DelayDopplerParams::kBypassId);

		//parameters.addParameter(STR16("Pos Start X"), nullptr, 0, kPosDefault, Vst::ParameterInfo::kNoFlags,
		//						DelayDopplerParams::kPosStartXId, 0, STR16("PStartX"));
		//parameters.addParameter(STR16("Pos Start Y"), nullptr, 0, kPosDefault, Vst::ParameterInfo::kNoFlags,
		//						DelayDopplerParams::kPosStartYId, 0, STR16("PStartY"));
		//
		//parameters.addParameter(STR16("Pos End X"), nullptr, 0, kPosDefault, Vst::ParameterInfo::kNoFlags,
		//						DelayDopplerParams::kPosEndXId, 0, STR16("PEndX"));
		//parameters.addParameter(STR16("Pos End Y"), nullptr, 0, kPosDefault, Vst::ParameterInfo::kNoFlags,
		//						DelayDopplerParams::kPosEndYId, 0, STR16("PEndY"));

		parameters.addParameter(new Vst::RangeParameter(
								STR16("Distance"), DelayDopplerParams::kDistId, STR16("m"),
								kDistMin, kDistMax, kDistDefault, 0, Vst::ParameterInfo::kNoFlags, 0, STR16("Dist")));
		parameters.addParameter(new Vst::RangeParameter(
								STR16("Duration"), DelayDopplerParams::kDurationId, STR16("s"),
								kDurationMin, kDurationMax, kDurationDefault, 0, Vst::ParameterInfo::kNoFlags, 0, STR16("Dur")));
		parameters.addParameter(new Vst::RangeParameter(
								STR16("Speed of Sound"), DelayDopplerParams::kSpeedOfSoundId, STR16("m/s"),
								kSpeedOfSoundMin, kSpeedOfSoundMax, kSpeedOfSoundDefault, 0, Vst::ParameterInfo::kNoFlags, 0, STR16("SoundSpeed")));
		parameters.addParameter(STR16("Attenuation Mix"), nullptr, 0, 1.0, Vst::ParameterInfo::kNoFlags,
								DelayDopplerParams::kAttenMixId, 0, STR16("AttenMix"));
		parameters.addParameter(new Vst::RangeParameter(
								STR16("Gain"), DelayDopplerParams::kGainId, STR16("dB"),
								kGainMin, kGainMax, kGainDefault, 0, Vst::ParameterInfo::kNoFlags, 0, STR16("Gain")));
		parameters.addParameter(new ThresholdParameter(
								STR16("Threshold"), DelayDopplerParams::kThresholdId, nullptr, 0.5, 0,
								Vst::ParameterInfo::kNoFlags, 0, STR16("Thresh")));
		parameters.addParameter(STR16("Auto Start"), nullptr, 1, 0, Vst::ParameterInfo::kNoFlags,
								DelayDopplerParams::kAutoStartId, 0, STR16("AutoS"));
		parameters.addParameter(STR16("Loop"), nullptr, 1, 0, Vst::ParameterInfo::kNoFlags,
								DelayDopplerParams::kLoopId, 0, STR16("Loop"));
		
		parameters.addParameter(STR16("Progress"), nullptr, 0, 0.0, Vst::ParameterInfo::kIsReadOnly,
								DelayDopplerParams::kProgressId, 0, STR16("Prog"));

		Vst::Parameter* p;
		if (p = parameters.getParameter(DelayDopplerParams::kDistId)) p->setPrecision(1);
		if (p = parameters.getParameter(DelayDopplerParams::kDurationId)) p->setPrecision(1);
		if (p = parameters.getParameter(DelayDopplerParams::kSpeedOfSoundId)) p->setPrecision(1);
		if (p = parameters.getParameter(DelayDopplerParams::kAttenMixId)) p->setPrecision(2);
		if (p = parameters.getParameter(DelayDopplerParams::kGainId)) p->setPrecision(1);
		if (p = parameters.getParameter(DelayDopplerParams::kThresholdId)) p->setPrecision(2);

		//parameters.addParameter(STR16("Add Point"), nullptr, 1, 0.0, Vst::ParameterInfo::kNoFlags,
		//						DelayDopplerParams::kAddPointId, 0, STR16("AddPt"));
		//parameters.addParameter(STR16("Remove Point"), nullptr, 1, 0.0, Vst::ParameterInfo::kNoFlags,
		//						DelayDopplerParams::kRemovePointId, 0, STR16("RmvPt"));

	}
	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API PlugController::createView (const char* name)
{
	// someone wants my editor
	if (name && strcmp (name, "editor") == 0)
	{
		auto* view = new VST3Editor (this, "view", "plug.uidesc");
		return view;
	}
	return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugController::setComponentState (IBStream* state)
{
	// we receive the current state of the component (processor part)
	// we read our parameters and bypass value...
	if (!state)
		return kResultFalse;

	IBStreamer streamer (state, kLittleEndian);

	bool valbool = false;
	double valdouble = 0.0;

	//Point a, b;

	if (streamer.readBool(valbool) == false)
		return kResultFalse;
	setParamNormalized(DelayDopplerParams::kBypassId, valbool ? 1.0 : 0.0);

	//if (streamer.readDouble(valdouble) == false)
	//	return kResultFalse;
	//setParamNormalized(DelayDopplerParams::kPosStartXId, valdouble);
	////a.x = valdouble;

	//if (streamer.readDouble(valdouble) == false)
	//	return kResultFalse;
	//setParamNormalized(DelayDopplerParams::kPosStartYId, valdouble);
	////a.y = valdouble;

	//if (streamer.readDouble(valdouble) == false)
	//	return kResultFalse;
	//setParamNormalized(DelayDopplerParams::kPosEndXId, valdouble);
	////b.x = valdouble;

	//if (streamer.readDouble(valdouble) == false)
	//	return kResultFalse;
	//setParamNormalized(DelayDopplerParams::kPosEndYId, valdouble);
	////b.y = valdouble;

	if (streamer.readDouble(valdouble) == false)
		return kResultFalse;
	setParamNormalized(DelayDopplerParams::kDistId, valdouble);

	if (streamer.readDouble(valdouble) == false)
		return kResultFalse;
	setParamNormalized(DelayDopplerParams::kDurationId, valdouble);

	if (streamer.readDouble(valdouble) == false)
		return kResultFalse;
	setParamNormalized(DelayDopplerParams::kSpeedOfSoundId, valdouble);

	if (streamer.readDouble(valdouble) == false)
		return kResultFalse;
	setParamNormalized(DelayDopplerParams::kAttenMixId, valdouble);

	if (streamer.readDouble(valdouble) == false)
		return kResultFalse;
	setParamNormalized(DelayDopplerParams::kGainId, valdouble);

	if (streamer.readDouble(valdouble) == false)
		return kResultFalse;
	setParamNormalized(DelayDopplerParams::kThresholdId, valdouble);

	if (streamer.readBool(valbool) == false)
		return kResultFalse;
	setParamNormalized(DelayDopplerParams::kAutoStartId, valbool);

	if (streamer.readBool(valbool) == false)
		return kResultFalse;
	setParamNormalized(DelayDopplerParams::kLoopId, valbool);

	if (streamer.readInt32(mNumPoints) == false)
		return kResultFalse;
	
	if (streamer.readFloatArray((float*)mPoints, kMaxNumPoints * sizeof(Point) / sizeof(float)) == false)
		return kResultFalse;

	// TODO change this to notify and let processor send message
	//if (mPathSubcontroller != nullptr)
	//{
	//	mPathSubcontroller->updatePathPoints(mPoints, numPoints);
	//}

	return kResultOk;
}

//------------------------------------------------------------------------
IController* PlugController::createSubController(UTF8StringPtr name, const IUIDescription* description, VST3Editor* editor)
{
	if (UTF8StringView(name) == "PathController")
	{
		auto* controller = new PathSubcontroller(this);
		//addPathSubcontroller(controller);
		mPathSubcontroller = controller;
		return controller;
	}
	return nullptr;
}


//------------------------------------------------------------------------
//tresult PLUGIN_API PlugController::notify(Vst::IMessage* message)
//{
//	if (!message)
//		return kInvalidArgument;
//
//	if (!strcmp(message->getMessageID(), "FloatMessage"))
//	{
//		double val;
//		if (message->getAttributes()->getFloat("Progress", val) == kResultOk)
//		{
//			if (mPathSubcontroller != nullptr)
//			{
//				mPathSubcontroller->setProgress(val);
//			}
//			return kResultOk;
//		}
//	}
//
//	return EditController::notify(message);
//}

tresult PLUGIN_API PlugController::setParamNormalized(Vst::ParamID tag, Vst::ParamValue value)
{
	if (mPathSubcontroller)
	{
		if (tag == kProgressId)
		{
			mPathSubcontroller->setProgress(value);
		}
		else if (tag == kLoopId)
		{
			mPathSubcontroller->setLoop(value > 0.5);
		}
	}
	return EditController::setParamNormalized(tag, value);
}

} // namespace
} // namespace Steinberg
