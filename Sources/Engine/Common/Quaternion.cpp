/*
 * Quaternion.cpp
 *
 *  Created on: 2 lip 2018
 *      Author: Xoliper
 */

#include "Quaternion.hpp"

Quaternion::Quaternion(){
	x = y = z = w = 0.0f;
}

Quaternion::Quaternion(const Quaternion & q){
	x = q.x;
	y = q.y;
	z = q.z;
	w = q.w;
}

Quaternion::Quaternion(glm::vec4 vector){
	x = vector.x;
	y = vector.y;
	z = vector.z;
	w = vector.w;
}

Quaternion::~Quaternion(){
}

Quaternion Quaternion::operator *(const Quaternion & q){
	Quaternion out;
	out.x = w*q.x + x*q.w + y*q.z - z*q.y;
	out.y = w*q.y + y*q.w + z*q.x - x*q.z;
	out.z = w*q.z + z*q.w + x*q.y - y*q.x;
	out.w = w*q.w - x*q.x - y*q.y - z*q.z;
	return out;
}

Quaternion Quaternion::operator !(){
	Quaternion out;
	out.x = -x;
	out.z = -z;
	out.y = -y;
	out.w = w;
	return out;
}

void Quaternion::ConvertFromRot(glm::vec3 vector, float angle){
	angle *= 0.0174533;
	angle *= 0.5f;
	float angleSin = sinf(angle);
	x = angleSin * vector.x;
	y = angleSin * vector.y;
	z = angleSin * vector.z;
	w = cosf(angle);
}

void Quaternion::ConvertFromXRot(float angle){
	angle *= 0.0174533;
	angle *= 0.5f;
	x = sinf(angle); y = 0.0f; z = 0.0f;
	w = cosf(angle);
}

void Quaternion::ConvertFromYRot(float angle){
	angle *= 0.0174533;
	angle *= 0.5f;
	x = 0.0f; y = sinf(angle); z = 0.0f;
	w = cosf(angle);
}

void Quaternion::ConvertFromZRot(float angle){
	angle *= 0.0174533;
	angle *= 0.5f;
	x = 0; y = 0.0f; z = sinf(angle);
	w = cosf(angle);
}

glm::vec3 Quaternion::GetRotationAxis(){
	glm::vec3 out;
	float sinThetaOver25q = 1.0f - w * w;
	if (sinThetaOver25q <= 0.0f){
		out.x = out.y = out.z = 0.0f;
	} else {
		float oneOverSinThetaOver2 = 1.0f / sqrtf(sinThetaOver25q);
		out.x = x * oneOverSinThetaOver2;
		out.y = y * oneOverSinThetaOver2;
		out.z = z * oneOverSinThetaOver2;
	}
	return out;
}

float Quaternion::GetRotationAngle(){
	float out = 0.0f;
	if (w <= -1.0f) out = 3.1415926535;
	if (w >= 1.0f) out = 0.0f;
	return acosf(out) * 2;
}

glm::mat4 Quaternion::GetRotationMatrix(){
	glm::mat4 out;

	float xx = x * x, yy = y * y, zz = z * z, xy = x * y, xz = x * z,
		  yz = y * z, wx = w * x, wy = w * y, wz = w * z;

	out[0][0] = 1.0f - 2.0f * ( yy + zz );
	out[0][1] = 2.0f * ( xy + wz );
	out[0][2] = 2.0f * ( xz - wy );
	out[0][3] = 0.0f;

	out[1][0] = 2.0f * ( xy - wz );
	out[1][1] = 1.0f - 2.0f * ( xx + zz );
	out[1][2] = 2.0f * ( yz + wx );
	out[1][3] = 0.0f;

	out[2][0] = 2.0f * ( xz + wy );
	out[2][1] = 2.0f * ( yz - wx );
	out[2][2] = 1.0f - 2.0f * ( xx + yy );
	out[2][3] = 0.0f;

	out[3][0] = 0.0f;
	out[3][1] = 0.0f;
	out[3][2] = 0.0f;
	out[3][3] = 1.0f;

	return out;
}

void Quaternion::Normalize(){
	float n = 1.0f / Length();
	x *= n;
	y *= n;
	z *= n;
	w *= n;
}

float Quaternion::Length(){
	return sqrtf(x*x + y*y + z*z + w*w);
}

void Quaternion::Clear(){
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}
