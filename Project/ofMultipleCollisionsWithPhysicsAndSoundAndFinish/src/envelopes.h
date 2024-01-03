#pragma once

#include "ofMain.h"
#include <iostream>

class evelopeGenerator
{
public:
	evelopeGenerator(const size_t& pCycleCount) : index(0), phase(0), lifeTime(true), cycleCount(pCycleCount) {}
	~evelopeGenerator() {}

	inline float play(const std::vector<float>& pBreakpoints)
	{
		if (pBreakpoints[index] < pBreakpoints[index + 1])
		{
			phase += (pBreakpoints[index + 1] - pBreakpoints[index]) / 44100 / (1 / pBreakpoints[index + 2]);

			if (phase >= pBreakpoints[index + 1])
			{
				index += 3;
			}
		}

		if (pBreakpoints[index] > pBreakpoints[index + 1])
		{
			phase += (pBreakpoints[index + 1] - pBreakpoints[index]) / 44100 / (1 / pBreakpoints[index + 2]);

			if (phase <= pBreakpoints[index + 1])
			{
				index += 3;
			}
		}

		if (index >= pBreakpoints.size() - 1)
		{
			phase = pBreakpoints[pBreakpoints.size() - 2];
		}

		cycleCount--;
		if (cycleCount <= 0)
		{
			lifeTime = false;
		}

		return phase;
	}

	inline bool isAlive()
	{
		return lifeTime;
	}

	bool lifeTime;

private:
	float index, phase;
	size_t cycleCount;
};