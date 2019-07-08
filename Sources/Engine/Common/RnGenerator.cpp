/*
 * RnGenerator.cpp
 *
 *  Created on: 30 lip 2018
 *      Author: Xoliper
 */

#include "RnGenerator.hpp"

RnGenerator::RnGenerator(){
	uniform01 = std::uniform_real_distribution<double>(0.0, 1.0);
	SetRandomSeed();
}

RnGenerator::~RnGenerator(){
}

void RnGenerator::SetRandomSeed(){
	seed = std::chrono::system_clock::now().time_since_epoch().count();
}

void RnGenerator::SetSeed(int seed){
	this->seed = seed;
}

double RnGenerator::GetRandom(){
	return uniform01(generator);
}
