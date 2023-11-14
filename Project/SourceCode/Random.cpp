#include "Random.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <random>

void Random::Init()
{
	srand((uint32_t)time(nullptr));
}

int32_t Random::Range(const int32_t min, const int32_t max)
{
	return rand() % ((max - min) + 1) + min;
}

float Random::RangeF(const float min, const float max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(min, max);
	return float(dist(gen));
}

bool Random::RangeB(const float rate)
{
	float num = RangeF(0, 100);
	if (num < rate) return true;
	return false;
}

float Random::RangeAngle(const float min, const float max)
{
	return RangeF(min, max);
}

float Random::RangeRadian(const float min, const float max)
{
	return RangeF(min, max);
}