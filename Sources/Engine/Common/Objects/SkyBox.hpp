/*
 * SkyBox.hpp
 *
 *  Created on: 28 cze 2018
 *      Author: Xoliper
 */

#ifndef ENGINE_COMMON_OBJECTS_SKYBOX_HPP_
#define ENGINE_COMMON_OBJECTS_SKYBOX_HPP_

#include "Object.hpp"

class SkyBox : public Drawable {

public:
	SkyBox(World * wrd, CubeMapTexture * cmt);
	virtual ~SkyBox();

	void SetPosition(float x, float y, float z);
	void SetScale(float x, float  y, float z);
	void SetRotation(bool x, bool y, bool z, float angle);
	void ChangeRotation(bool x, bool y, bool z, float angle);
	void Render();

	GLuint vbo;
	GLuint vao;

	CubeMapTexture * cmt;

protected:

	//Pointer for program
	std::vector<GLuint> * params;

	//Additional Matrixes
	mat4 mvp;

};

#endif /* ENGINE_COMMON_OBJECTS_SKYBOX_HPP_ */
