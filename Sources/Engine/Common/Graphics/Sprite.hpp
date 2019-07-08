#ifndef ENGINE_COMMON_SPRITE_HPP_
#define ENGINE_COMMON_SPRITE_HPP_


#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../Drawable.hpp"
#include "../Macros.hpp"
#include "../Shaderloader.hpp"
#include "../World.hpp"
#include "../Timer.hpp"
#include "Texture.hpp"

using namespace glm;

#define AUTO -1

class Sprite : public Drawable {
public:
	Sprite(World * world, WorldRect wr, Texture * tex, int flags);
	virtual ~Sprite();

	void SetPosition(float x, float y, float z);
	void SetShader(std::string shaderName);
	void SetBlurDepthTex(GLuint id);
	void SetShaderParam(float first, float second);
	void Resize(int width, int height);
	void SetupAnimation(int tWidth, int tHeight, int frames, float animTime);
	bool Animate(Timer * timer);
	void SetMode(bool mode);
	void RotateToOrigin(float x, float y, float z, glm::vec3 upDirection);
	void SetScale(float x, float y, float z);

	void Render();

private:
	glm::mat4 mvp;
	glm::mat4 originMatrix;

	//Used shader params
	bool mode;
	std::string shaderName;
	float shpFirst, shpSecond;
	GLuint blurDepthTex;

	//Position handler
	WorldRect wr;

	//Sprite texture
	Texture * tex;

	//Buffers
	GLuint vertexArrayID;
	GLuint vertexBuffer;
	GLuint uvBuffer;

	std::vector<GLfloat> vertexBufferData;
	std::vector<GLfloat> uvBufferData;

	int tWidth, tHeight;
	int frames;
	float animTime;
	float curAnimTime;
	int curFrame;
	bool animState;
};

#endif /* ENGINE_COMMON_SPRITE_HPP_ */
