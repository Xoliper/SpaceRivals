/*
 * text.hpp
 *
 *  Created on: 21 lis 2017
 *      Author: Arkadiusz Jarlaczyk
 */

#ifndef ENGINE_COMMON_TEXT_HPP_
#define ENGINE_COMMON_TEXT_HPP_

#include "Color.hpp"
#include "../Config.hpp"
#include "Font.hpp"
#include "../Shaderloader.hpp"
#include "../World.hpp"

using namespace glm;

class Text {
public:
	Text(World * world, Font * fnt, std::string text, int x, int y, int flags);
	virtual ~Text();

	void SetPosition(int x, int y);
	void SetScale(float x, float y, float z);
	void SetText(std::string newText);

	void Colorize(Color color);
	void Colorize(int r, int g, int b, int a);
	void Colorize(float r, float g, float b, float a);

	void Render();

private:

	void SetPosition(float new_x, float new_y);

	//Pointer for program
	std::vector<GLuint> * params;

	//Matrixes
	World * world;
	mat4 model;
	mat4 scale;
	mat4 rotation;
	mat4 translation;

	//Buffers
	GLuint vertexArrayID;
	GLuint vertexBuffer;
	GLuint uvBuffer;

	std::vector<GLfloat> vertexBufferData;//[6*3*2];
	std::vector<GLfloat> uvBufferData;//[6*2*2];

	//Other vars
	Color color;
	int charsAmount;
	Font * fnt;
	float x, y;
	int flags;

};

#endif /* ENGINE_COMMON_TEXT_HPP_ */
