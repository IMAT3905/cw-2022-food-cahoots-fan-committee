#pragma once

#include "system.h"
#include <random>
#include <memory>

namespace Engine
{
	enum class RandomTypes { None, Uniform, Normal };

	class Randomiser : public System
	{
	public:
		virtual void start(SystemSignal init = SystemSignal::None, ...) override;
		virtual void stop(SystemSignal close = SystemSignal::None, ...) override;

		static int32_t uniformIntBetween(int32_t lower, int32_t upper);
		static float uniformFloatBetween(float lower, float upper);
		static float normalInt(float c, float sigma);
		static float normalFloat(float c, float sigma);
	private:
		static std::shared_ptr<std::mt19937> s_generator;
		static std::uniform_int_distribution<int32_t> s_uniformInt;
		static std::uniform_real_distribution<float> s_uniformFloat;
		static float s_intRange;
		static float s_floatRange;
	};
}