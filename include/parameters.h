#pragma once

#include <algorithm>

namespace Steinberg {
namespace DelayDoppler {

	const double kDurationMin		= 0.01; 
	const double kDurationMax		= 20.0;
	const double kDurationDefault	= 5.0;

	const double kDistMin			= 1.0;
	const double kDistMax			= 500.0;
	const double kDistDefault		= 30.0;

	const double kPosDefault		= 0.5;

	const double kSpeedOfSoundMin		= 10.0;
	const double kSpeedOfSoundMax		= 343.0;
	const double kSpeedOfSoundDefault	= 343.0;

	const double kGainMin		= -60.0;
	const double kGainMax		= 60.0;
	const double kGainDefault	= 0.0;

	const int32_t kMaxNumPoints = 10;

	const double kAutoStartThreshold = 0.01;

	//const double kThresholdDefault  = 0.5;
	//const double kAngleMin			= -45.0;
	//const double kAngleMax			= 45.0;
	//const double kAngleDefault		= 0.0;

	inline double ParameterToPlain(double normalized, double min, double max)
	{
		return normalized * (max - min) + min;
	}

	inline double ParameterToNormalized(double plain, double min, double max)
	{
		return (plain - min) / (max - min);
	}

	template<class T>
	inline T Clip(T val, T min, T max)
	{
		return std::min<T>(std::max<T>(val, min), max);
	}
}
}
