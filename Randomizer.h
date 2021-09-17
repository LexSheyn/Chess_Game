#pragma once
class Randomizer
{
public:

// Constructors and Destructor:

	Randomizer();
	~Randomizer();

// Functions:

	int32_t generate(const int32_t min, const int32_t max);
	double generate(const double min, const double max);
};

