#pragma once

#include <cmath>
#include <vector>

namespace Steinberg {
namespace DelayDoppler {

class DelayLine
{
public:
	DelayLine()
		: mWritePos(0)
		, mDelaySamples(1)
		, mSampleRate(48000.0)
	{}

	inline void setSizeSamples(int64_t sizeInSamples)
	{
		mBufferL.resize(sizeInSamples, 0.f);
		mBufferR.resize(sizeInSamples, 0.f);
	}

	inline void setSampleRate(double sampleRate)
	{
		mSampleRate = sampleRate;
	}

	inline bool empty()
	{
		return mBufferL.empty(); // L & R have always identical size
	}

	inline void clear()
	{
		mBufferL.clear();
		mBufferR.clear();
	}

	/// Call *after* you have set the size (setSizeSamples())
	void setDelay(double delayInSeconds)
	{
		//mDelaySamples = std::round(delayInSeconds * mSampleRate);
		mDelaySamples = delayInSeconds * mSampleRate;
		if (mDelaySamples >= mBufferL.size() - 1)
		{
			mDelaySamples = mBufferL.size() - 2;
		}
		else if (mDelaySamples <= 1.0)
		{
			mDelaySamples = 1.0;
		}
	}

	// TODO: Implement linear interpolation and keep mDelaySamples double type
	void process(float inL, float inR, float& outL, float& outR)
	{
		//int64_t readPos = mWritePos - mDelaySamples;
		double readPos = mWritePos - mDelaySamples;
		while (readPos < 0) { readPos += mBufferL.size(); }

		mBufferL[mWritePos] = inL;
		mBufferR[mWritePos++] = inR;
		if (mWritePos >= mBufferL.size()) { mWritePos -= mBufferL.size(); }

		if (abs(readPos - round(readPos)) <= 0.0001)
		{
			const int64 readPosInt = round(readPos);
			outL = mBufferL[readPosInt];
			outR = mBufferR[readPosInt];
		}
		else
		{
			int64 readPosCeil = ceil(readPos);
			int64 readPosFloor = floor(readPos);
			while (readPosCeil >= mBufferL.size())	readPosCeil -= mBufferL.size();
			// can never happen:
			// while (readPosFloor < 0)				readPosFloor += mBufferL.size();
			float readPosRemainder = readPos - readPosFloor;
			outL = readPosRemainder * (mBufferL[readPosCeil] - mBufferL[readPosFloor]) + mBufferL[readPosFloor];
			outR = readPosRemainder * (mBufferR[readPosCeil] - mBufferR[readPosFloor]) + mBufferR[readPosFloor];
		}
	}

	void reset()
	{
		std::fill(mBufferL.begin(), mBufferL.end(), 0.f);
		std::fill(mBufferR.begin(), mBufferR.end(), 0.f);
		mWritePos = 0;
	}

private:
	double mSampleRate;
	//int64_t mDelaySamples;
	double mDelaySamples;
	std::vector<float> mBufferL, mBufferR;
	int64_t mWritePos;
};

}
}
