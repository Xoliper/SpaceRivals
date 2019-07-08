/*
 * RnGenerator.hpp
 *
 *  Created on: 30 lip 2018
 *      Author: Xoliper
 */

#ifndef ENGINE_COMMON_RNGENERATOR_HPP_
#define ENGINE_COMMON_RNGENERATOR_HPP_

#include <random>
#include <cmath>
#include <chrono>

class RnGenerator {
public:
	RnGenerator();
	~RnGenerator();

	void SetRandomSeed();
	void SetSeed(int seed);
	double GetRandom();
private:
	unsigned int seed;
	std::mt19937 generator;
	std::uniform_real_distribution<double> uniform01;
};

#endif /* ENGINE_COMMON_RNGENERATOR_HPP_ */
