//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

//#include "../include/cubichermitespline.h"
#include "../include/delayline.h"
#include "../include/parameters.h"

namespace Steinberg {
namespace DelayDoppler {

struct Point;

//-----------------------------------------------------------------------------
class PlugProcessor : public Vst::AudioEffect
{
public:
	PlugProcessor();
	~PlugProcessor();

	tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;
	tresult PLUGIN_API setBusArrangements (Vst::SpeakerArrangement* inputs, int32 numIns,
	                                       Vst::SpeakerArrangement* outputs,
	                                       int32 numOuts) SMTG_OVERRIDE;

	tresult PLUGIN_API setupProcessing (Vst::ProcessSetup& setup) SMTG_OVERRIDE;
	tresult PLUGIN_API setActive (TBool state) SMTG_OVERRIDE;
	tresult PLUGIN_API process (Vst::ProcessData& data) SMTG_OVERRIDE;

//------------------------------------------------------------------------
	tresult PLUGIN_API setState (IBStream* state) SMTG_OVERRIDE;
	tresult PLUGIN_API getState (IBStream* state) SMTG_OVERRIDE;

	static FUnknown* createInstance (void*) { return (Vst::IAudioProcessor*)new PlugProcessor (); }

	tresult PLUGIN_API notify(Vst::IMessage* message) SMTG_OVERRIDE;

protected:
	//void updateSpline();

	bool mBypass = false;

	//Vst::ParamValue mPosStartX = kPosDefault;
	//Vst::ParamValue mPosStartY = kPosDefault;
	//Vst::ParamValue mPosEndX = kPosDefault;
	//Vst::ParamValue mPosEndY = kPosDefault;

	Point* mPoints = nullptr;
	int32 mNumPoints = 0;

	Vst::ParamValue mDist = kDistDefault;	
	Vst::ParamValue mDuration = kDurationDefault;
	Vst::ParamValue mSpeedOfSound = kSpeedOfSoundDefault;
	Vst::ParamValue mAttenMix = 1.0;
	Vst::ParamValue mGain = kGainDefault;

	Vst::ParamValue mProgress = 0.0;

	DelayLine mDelayLine;

	Vst::TSamples mStartSample = 0;
	Vst::TSamples mProcessedSamples = 0;
	Vst::TSamples mNextProjectTimeSamples = 0;
	int16 mNotePitch = 0;
	bool mRun = false;
	bool mCanReset = true;
	bool mAutoRun = true; // true if should use amplitude threshold to trigger
	bool mLoop = false;
	Vst::ParamValue mThreshold = 0.5;

	Vst::SampleRate mSampleRate = 0.;
	
};

//------------------------------------------------------------------------
} // namespace DelayDoppler
} // namespace Steinberg
