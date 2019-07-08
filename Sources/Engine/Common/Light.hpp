/*
 * Light.hpp
 *
 *  Created on: 26 cze 2018
 *      Author: Xoliper
 */

#ifndef ENGINE_COMMON_LIGHT_HPP_
#define ENGINE_COMMON_LIGHT_HPP_

#include <glm/glm.hpp>
#include "Graphics/Color.hpp"

class Light {
public:
	Light();
	~Light();

	void SetLightPower(float power);
	void SetLightPos(float x, float y, float z);

	glm::vec3 position;
	Color color;
	float power;
};

#endif /* ENGINE_COMMON_LIGHT_HPP_ */
