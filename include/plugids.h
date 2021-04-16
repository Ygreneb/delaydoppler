//------------------------------------------------------------------------

#pragma once

namespace Steinberg {
namespace DelayDoppler {

// HERE are defined the parameter Ids which are exported to the host
enum DelayDopplerParams : Vst::ParamID
{
	kBypassId = 100,

	kPosStartXId = 200,
	kPosStartYId,
	kPosEndXId,
	kPosEndYId,
	kPathPointChange,

	kDistId = 300,
	kDurationId,
	kSpeedOfSoundId,
	kAttenMixId,
	kGainId,
	kThresholdId,
	kAutoStartId,
	kLoopId,

	kProgressId = 400,

	kAddPointId = 500,
	kRemovePointId

};


// HERE you have to define new unique class ids: for processor and for controller
// you can use GUID creator tools like https://www.guidgenerator.com/
static const FUID MyProcessorUID (0x335ABD1D, 0x7E3B421E, 0xB49CB61C, 0xB051FD0D);
static const FUID MyControllerUID (0xCDA46E24, 0x700C4F50, 0x9DFB0D6A, 0xC444D41C);

//------------------------------------------------------------------------
} // namespace DelayDoppler
} // namespace Steinberg
