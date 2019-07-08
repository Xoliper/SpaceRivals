/*
 * Light.cpp
 *
 *  Created on: 26 cze 2018
 *      Author: Xoliper
 */

#include "Light.hpp"

Light::Light(){
	power = 0.0f;
}

Light::~Light(){
}

void Light::SetLightPower(float power){
	this->power = power;
}

void Light::SetLightPos(float x, float y, float z){
	position.x = x;
	position.y = y;
	position.z = z;
}
