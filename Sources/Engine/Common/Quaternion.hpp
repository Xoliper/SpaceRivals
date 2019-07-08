/*
 * Quaternion.hpp
 *
 *  Created on: 2 lip 2018
 *      Author: Xoliper
 */

#ifndef ENGINE_COMMON_QUATERNION_HPP_
#define ENGINE_COMMON_QUATERNION_HPP_

#include <math.h>
#include <glm/glm.hpp>

class Quaternion {

public:
	Quaternion();
	Quaternion(glm::vec4 vector);
	Quaternion(const Quaternion & q);
	~Quaternion();

	//Operators overload
	Quaternion operator*(const Quaternion & q);
	Quaternion operator!();

	//From rotation to quaternion
	void ConvertFromRot(glm::vec3 vector, float angle);
	void ConvertFromXRot(float angle);
	void ConvertFromYRot(float angle);
	void ConvertFromZRot(float angle);

	//From quaternion to rotation
	glm::vec3 GetRotationAxis();
	float GetRotationAngle();

	//Get rotation matrix
	glm::mat4 GetRotationMatrix();

	//Usefull funcs
	void Normalize();
	float Length();
	void Clear();
	float x, y, z, w;

private:

};

#endif /* ENGINE_COMMON_QUATERNION_HPP_ */
