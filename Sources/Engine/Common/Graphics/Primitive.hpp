#ifndef ENGINE_COMMON_RECTANGLEPRIM_HPP_
#define ENGINE_COMMON_RECTANGLEPRIM_HPP_

#include "color.hpp"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "../Drawable.hpp"
#include "../Macros.hpp"
#include "../Shaderloader.hpp"
#include "../World.hpp"
using namespace glm;

class RectanglePrim : public Drawable {
public:
	RectanglePrim(World * world, WorldRect wr, Color color, int flags); //Rectangle
	virtual ~RectanglePrim();

	void SetColor(Color color);
	void SetPosition(WorldRect wr);
	void Resize(int newWidth, int newHeight);

	int GetWidth();
	int GetHeight();
	WorldRect GetBoundary();

	void Render();

private:

	//Position handler
	WorldRect wr;

	//Pointer for program
	std::vector<GLuint> * params;

	//Color
	Color color;
	int flags;
	int size;

	//Matrixes
	World * world;
	mat4 model;
	mat4 scale;
	mat4 rotation;
	mat4 translation;

	//Buffers
	GLuint vertexArrayID;
	GLuint vertexBuffer;
	std::vector<GLfloat> bufferData;

};

#endif /* ENGINE_COMMON_RectanglePrim_HPP_ */
