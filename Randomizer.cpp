#include "stdafx.h"
#include "Randomizer.h"

// Constructors and Destructor:

Randomizer::Randomizer()
{

}

Randomizer::~Randomizer()
{
}

int32_t Randomizer::generate(const int32_t min, const int32_t max)
{
	std::random_device seed;
	std::mt19937 random(seed());
	std::uniform_int_distribution<int32_t> generated(min, max);

	return generated(random);
}

double Randomizer::generate(const double min, const double max)
{
	std::random_device seed;
	std::mt19937 random(seed());
	std::uniform_real_distribution<double> generated(min, max);

	return generated(random);
}
