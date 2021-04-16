//------------------------------------------------------------------------


#include "../include/cubichermitespline.h"
#include "../include/parameters.h"
#include "../include/plugprocessor.h"
#include "../include/plugids.h"
#include "../include/point.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Steinberg {
namespace DelayDoppler {

//-----------------------------------------------------------------------------
PlugProcessor::PlugProcessor ()
{
	// register its editor class
	setControllerClass (MyControllerUID);

	mPoints = new Point[kMaxNumPoints];
	// default init for brand new plug-in insert
	for (int i = 0; i < kMaxNumPoints; ++i)
	{
		mPoints[i].x = (float)(i / (kMaxNumPoints - 1)) * 0.8f + 0.1f;
		mPoints[i].y = (float)(i / (kMaxNumPoints - 1)) * 0.8f + 0.1f;
	}
	mNumPoints = 2;
}

//-----------------------------------------------------------------------------
PlugProcessor::~PlugProcessor()
{
	delete[] mPoints;
}


//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::initialize (FUnknown* context)
{
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	if (result != kResultTrue)
		return kResultFalse;

	//---create Audio In/Out buses------
	// we want a stereo Input and a Stereo Output
	addAudioInput (STR16 ("AudioInput"), Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("AudioOutput"), Vst::SpeakerArr::kStereo);

	addEventInput (STR16 ("MIDIInput"));

	// Init range parameters with useful default values, will probably be overwritten by setState()
	mDist = ParameterToNormalized(kDistDefault, kDistMin, kDistMax);
	mDuration = ParameterToNormalized(kDurationDefault, kDurationMin, kDurationMax);
	mSpeedOfSound = ParameterToNormalized(kSpeedOfSoundDefault, kSpeedOfSoundMin, kSpeedOfSoundMax);
	mGain = ParameterToNormalized(kGainDefault, kGainMin, kGainMax);

	// Add the two spline points now, they will be edited later
	//mSpline.addSplinePoint(SplinePoint());
	//mSpline.addSplinePoint(SplinePoint());

	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setBusArrangements (Vst::SpeakerArrangement* inputs,
                                                            int32 numIns,
                                                            Vst::SpeakerArrangement* outputs,
                                                            int32 numOuts)
{
	// we only support one in and output bus, both stereo
	if (numIns == 1 && numOuts == 1 && inputs[0] == Vst::SpeakerArr::kStereo && outputs[0] == Vst::SpeakerArr::kStereo)
	{
		return AudioEffect::setBusArrangements (inputs, numIns, outputs, numOuts);
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setupProcessing (Vst::ProcessSetup& setup)
{
	// here you get, with setup, information about:
	// sampleRate, processMode, maximum number of samples per audio block
	mSampleRate = setup.sampleRate;
	//mProcessedSamples = std::ceil(ParameterToPlain(mDuration, kDurationMin, kDurationMax) * mSampleRate);

	return AudioEffect::setupProcessing (setup);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setActive (TBool state)
{
	if (state) // Initialize
	{
		// Allocate Memory Here
		// Ex: algo.create ();
		mDelayLine.setSizeSamples(mSampleRate * kDurationMax);
		mDelayLine.setSampleRate(mSampleRate);
		mDelayLine.setDelay(0.0);
	}
	else // Release
	{
		// Free Memory if still allocated
		// Ex: if(algo.isCreated ()) { algo.destroy (); }
		mDelayLine.clear();
	}
	return AudioEffect::setActive (state);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::process (Vst::ProcessData& data)
{
	//--- Read inputs parameter changes-----------
	if (data.inputParameterChanges)
	{
		int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
		for (int32 index = 0; index < numParamsChanged; index++)
		{
			Vst::IParamValueQueue* paramQueue =
			    data.inputParameterChanges->getParameterData (index);
			if (paramQueue)
			{
				Vst::ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount ();
				switch (paramQueue->getParameterId ())
				{
					case DelayDopplerParams::kBypassId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							mBypass = (value > 0.5f);
						break;
					//case DelayDopplerParams::kPosStartXId:
					//	if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
					//		mPosStartX = value;
					//	break;
					//case DelayDopplerParams::kPosStartYId:
					//	if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
					//		mPosStartY = value;
					//	break;
					//case DelayDopplerParams::kPosEndXId:
					//	if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
					//		mPosEndX = value;
					//	break;
					//case DelayDopplerParams::kPosEndYId:
					//	if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
					//		mPosEndY = value;
					//	break;
					case DelayDopplerParams::kDistId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							mDist = value;
						break;
					case DelayDopplerParams::kDurationId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							mDuration = value;
						break;
					case DelayDopplerParams::kSpeedOfSoundId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							mSpeedOfSound = value;
						break;
					case DelayDopplerParams::kAttenMixId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							mAttenMix = value;
						break;
					case DelayDopplerParams::kGainId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							mGain = value;
						break;
					case DelayDopplerParams::kThresholdId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
						{
							mThreshold = value;
							mAutoRun = mThreshold >= kAutoStartThreshold;
						}
						break;
					//case DelayDopplerParams::kAutoStartId:
					//	if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
					//		mAutoRun = (value > 0.5);
					//	break;
					case DelayDopplerParams::kLoopId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							mLoop = (value > 0.5);
						break;
					
				}
			}
		}
	}

	//--- Read MIDI events
	if (data.inputEvents && !mAutoRun)
	{
		for (int32 i = 0; i < data.inputEvents->getEventCount(); ++i)
		{
			Vst::Event e;
			data.inputEvents->getEvent(i, e);

			if (e.type == Vst::Event::kNoteOnEvent/* && mNotePitch == 0*/)
			{
				mStartSample = data.processContext->projectTimeSamples + e.sampleOffset;
				mNotePitch = e.noteOn.pitch;
				mRun = true;
				mDelayLine.reset();
				mProcessedSamples = -e.sampleOffset;
				mCanReset = false;
			}
			/*else if (e.type == Vst::Event::kNoteOffEvent && mNotePitch == e.noteOff.pitch)
			{
				mNotePitch = 0;
			}*/
		}
		
	}

	//--- Process Audio---------------------
	//--- ----------------------------------
	if (data.numInputs == 0 || data.numOutputs == 0)
	{
		// when here, host probably just flushes parameter changes and
		// playback is not running => nothing to do, except for stopping doppler progress
		//mRun = false;
		return kResultOk;
	}

	// TODO silence flags
	//if (data.inputs[0].silenceFlags > 0)
	//{
	//	mRun = false;
	//}

	//if (mNextProjectTimeSamples < 0)
	//{
	//	// when here it's the first sample block
	//	mNextProjectTimeSamples = data.processContext->projectTimeSamples;
	//}
	// mNextProjectTimeSamples == projectTimeSamples is not always true,
	// even in normal play mode. Cubase internal rounding error get 1-2 sample
	// off from time to time
	if (abs(mNextProjectTimeSamples - data.processContext->projectTimeSamples) > 2)
	{
		// when here cursor has changed unpredictably (e.g. loop, manual cursor change)
		mRun = false;
		mProgress = 0.;
		//mNextProjectTimeSamples = -1;
	}
	mNextProjectTimeSamples = data.processContext->projectTimeSamples + data.numSamples; 

	if (mBypass || mDelayLine.empty()/* || (!mAutoRun && !mRun)*/)
	{
		memcpy(data.outputs[0].channelBuffers32[0], data.inputs[0].channelBuffers32[0], data.numSamples * sizeof(Vst::Sample32));
		memcpy(data.outputs[0].channelBuffers32[1], data.inputs[0].channelBuffers32[1], data.numSamples * sizeof(Vst::Sample32));
		return kResultOk;
	}

	if (data.numSamples > 0)
	{
		double duration = ParameterToPlain(mDuration, kDurationMin, kDurationMax);

		Vst::TSamples projectTimeSamples = data.processContext->projectTimeSamples;
		Vst::TSamples dopplerDurationSamples = std::floor(duration * data.processContext->sampleRate);

		double dist = ParameterToPlain(mDist, kDistMin, kDistMax);
		float gain = pow(10.0, ParameterToPlain(mGain, kGainMin, kGainMax) / 20.0);

		for (int32 i = 0; i < data.numSamples; ++i)
		{
			if (mAutoRun && !mRun)
			{
				// TODO enter abs()
				if (data.inputs[0].channelBuffers32[0][i] >= mThreshold || data.inputs[0].channelBuffers32[1][i] >= mThreshold)
				{
					mRun = true;
					//mStartSample = currentTimeSamples;
					mProcessedSamples = 0;
				}
			}

			Vst::TSamples currentTimeSamples = projectTimeSamples + i;
			//double t = 0.0;
			double t = static_cast<double>(mProcessedSamples) / dopplerDurationSamples;
			if (t >= 1.0) mRun = false;
			t = Clip<double>(t, 0.0, 1.0);

			//if (currentTimeSamples >= mStartSample &&
			//	currentTimeSamples <= mStartSample + dopplerDurationSamples)
			//{
			//	t = static_cast<double>(currentTimeSamples - mStartSample) / dopplerDurationSamples;
			//}
			//else if (currentTimeSamples > mStartSample + dopplerDurationSamples)
			//{
			//	t = 1.0;
			//	mRun = false;
			//}

			//Point p = LinearInterpolation(mPoints, mNumPoints, t);
			Point p = CubicHermiteSpline(mPoints, mNumPoints, t, mLoop);
			p = 2.0 * (p - 0.5) * dist;

			float p_len = p.len();
			double delay = p_len / ParameterToPlain(mSpeedOfSound, kSpeedOfSoundMin, kSpeedOfSoundMax);
			mDelayLine.setDelay(delay);
						
			// equal power panning
			float factorR = (p.x + dist) / (2.0 * dist); // 0 to 1, left to right
			float factorL = std::cosf(factorR * M_PI_2);
			factorR = std::sinf(factorR * M_PI_2);

			// physically correct attenuation, tweakable
			float atten = p_len < 1.f ? 1.f : 1.f / p_len; // asumption: original sound level at distance = 1 m
			atten = static_cast<float>(1.0 - mAttenMix) * (1.f - atten) + atten;

			float outL, outR;
			const float mono = 0.5f * (data.inputs[0].channelBuffers32[0][i] + data.inputs[0].channelBuffers32[1][i]);
			mDelayLine.process(atten * factorL * mono, atten * factorR * mono, outL, outR);
			data.outputs[0].channelBuffers32[0][i] = outL * gain;
			data.outputs[0].channelBuffers32[1][i] = outR * gain;

			// for debugging / convenience
			mProgress = t;
			
			if (mRun) ++mProcessedSamples;
		}
	}

	if (mProgress > 0.001 && mProgress < 0.999)
	{
		// update progress value for generic editor ..
		if (Vst::IParameterChanges* outParamChanges = data.outputParameterChanges)
		{
			int32 indexQueue = 0;
			int32 indexData = 0;
			Vst::IParamValueQueue* paramQueue;

			paramQueue = outParamChanges->addParameterData(kProgressId, indexQueue);
			if (paramQueue)
			{
				paramQueue->addPoint(0, mProgress, indexData);
			}
		}

		// .. and now for custom GUI
		//if (IPtr<Vst::IMessage> message = owned(allocateMessage()))
		//{
		//	message->setMessageID("FloatMessage");
		//	message->getAttributes()->setFloat("Progress", mProgress);
		//	sendMessage(message);
		//}
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setState (IBStream* state)
{
	if (!state)
		return kResultFalse;

	// called when we load a preset or project, the model has to be reloaded

	IBStreamer streamer (state, kLittleEndian);

	if (streamer.readBool(mBypass) == false)
		return kResultFalse;

	//if (streamer.readDouble(mPosStartX) == false)
	//	return kResultFalse;

	//if (streamer.readDouble(mPosStartY) == false)
	//	return kResultFalse;

	//if (streamer.readDouble(mPosEndX) == false)
	//	return kResultFalse;

	//if (streamer.readDouble(mPosEndY) == false)
	//	return kResultFalse;

	if (streamer.readDouble(mDist) == false)
		return kResultFalse;

	if (streamer.readDouble(mDuration) == false)
		return kResultFalse;

	if (streamer.readDouble(mSpeedOfSound) == false)
		return kResultFalse;

	if (streamer.readDouble(mAttenMix) == false)
		return kResultFalse;	

	if (streamer.readDouble(mGain) == false)
		return kResultFalse;

	if (streamer.readDouble(mThreshold) == false)
		return kResultFalse;

	if (streamer.readBool(mAutoRun) == false)
		return kResultFalse;

	if (streamer.readBool(mLoop) == false)
		return kResultFalse;

	if (streamer.readInt32(mNumPoints) == false)
		return kResultFalse;
	mNumPoints = Clip<int>(mNumPoints, 2, kMaxNumPoints);

	// Always store the whole array if user wants to get old deleted points back
	if (streamer.readFloatArray((float*)mPoints, kMaxNumPoints * sizeof(Point) / sizeof(float)) == false)
		return kResultFalse;

	// update mNumPoints: invalid points have -1.0 coordinate
	//mNumPoints = kMaxNumPoints;
	//for (uint32 i = 0; i < kMaxNumPoints; ++i)
	//{
	//	if (mPoints[i].x < -0.5 || mPoints[i].y < -0.5)
	//	{
	//		mNumPoints = i;
	//		break;
	//	}
	//}

	// TODO send message to controller to update path view?

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::getState (IBStream* state)
{
	// here we need to save the model (preset or project)

	IBStreamer streamer (state, kLittleEndian);

	streamer.writeBool(mBypass);
	//streamer.writeDouble(mPosStartX);
	//streamer.writeDouble(mPosStartY);
	//streamer.writeDouble(mPosEndX);
	//streamer.writeDouble(mPosEndY);
	streamer.writeDouble(mDist);
	streamer.writeDouble(mDuration);
	streamer.writeDouble(mSpeedOfSound);
	streamer.writeDouble(mAttenMix);
	streamer.writeDouble(mGain);
	streamer.writeDouble(mThreshold);
	streamer.writeBool(mAutoRun);
	streamer.writeBool(mLoop);
	streamer.writeInt32(mNumPoints);
	streamer.writeFloatArray((float*)mPoints, kMaxNumPoints * sizeof(Point) / sizeof(float));

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::notify(Vst::IMessage* message)
{
	if (!message)
		return kInvalidArgument;

	if (!strcmp(message->getMessageID(), "BinaryMessage"))
	{
		const void* data;
		uint32 size;
		if (message->getAttributes()->getBinary("PointData", data, size) == kResultOk)
		{
			const Point* pointdata = (const Point*)data;
			//if (pointsize != mNumPoints)
			//	return kInvalidArgument;

			if (pointdata)
			{
				mNumPoints = size / sizeof(Point);
				memcpy(mPoints, pointdata, size);
				return kResultOk;
			}
		}
	}

	return AudioEffect::notify(message);
}

//------------------------------------------------------------------------
//void PlugProcessor::updateSpline()
//{
//	float startAngle = ParameterToPlain(mPosStartAngle, kPosAngleMin, kPosAngleMax); // -45° to 45°
//	float midAngle = ParameterToPlain(mPosMidAngle, kPosAngleMin, kPosAngleMax); // -45° to 45°
//	float endAngle = ParameterToPlain(mPosEndAngle, kPosAngleMin, kPosAngleMax); // -45° to 45°
//
//	Utilities::Point startPoint, midPoint, endPoint;
//	const double distFactor = 1000.0; // in m
//	startPoint.x	= distFactor * mPosStartDist * sin(startAngle);
//	startPoint.y	= distFactor * mPosStartDist * cos(startAngle);
//	midPoint.x		= distFactor * mPosMidDist * sin(midAngle);
//	midPoint.y		= distFactor * mPosMidDist * cos(midAngle);
//	endPoint.y		= distFactor * mPosEndDist * cos(endAngle);
//	endPoint.x		= distFactor * mPosEndDist * sin(endAngle);
//	
//	Utilities::Point test = startPoint + midPoint;
//
//	mSpline[0].pos = startPoint;
//	mSpline[0].tan = midPoint -startPoint;
//	mSpline[0].time = 0.f;
//
//
//	mSpline[1].pos = endPoint;
//	mSpline[1].tan = endPoint -midPoint;
//	mSpline[1].time = 1.f;
//}

//------------------------------------------------------------------------
} // namespace
} // namespace Steinberg
