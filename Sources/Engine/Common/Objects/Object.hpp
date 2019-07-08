#ifndef ENGINE_COMMON_OBJECTS_OBJECT_HPP_
#define ENGINE_COMMON_OBJECTS_OBJECT_HPP_

#include "ObjTemplate.hpp"
#include "../Drawable.hpp"
#include "../Shaderloader.hpp"
#include "../Graphics/Texture.hpp"
#include "../World.hpp"
#include "../Exceptions.hpp"

#include <math.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;


class PhantomPointObject : public Drawable {
public:
	PhantomPointObject();
	PhantomPointObject(float x, float y, float z, float s);
	virtual ~PhantomPointObject();

	void Render();

	float x, y, z;
	float diameter;

};


class VisualObject : public Drawable {

public:
	VisualObject(World * wrd, ObjTemplate * obj);
	virtual ~VisualObject();

	void SetLightViewMatrix(glm::mat4 newLv);
	void SetPosition(float x, float y, float z);
	void SetScale(float x, float  y, float z);
	void SetRotation(bool x, bool y, bool z, float angle);
	void ChangeRotation(bool x, bool y, bool z, float angle);
	void SetupAnimation(bool ifStart, bool repeat, bool ifReturn, bool recalculateBoxDimm);
	bool Animate(float deltaTime);
	void SetShader(std::string shaderName);
	void Render();


protected:

	void AnimationProc(int curFrame, float ratio);

	//Display mode
	std::string shaderName;

	//Obj Data
	ObjTemplate * objTemplate;

	//Lightning view-matrix
	glm::mat4 lv;
	bool otherLv;

	//Animation vars
	bool animRepeat;
	bool animPlayback;
	bool animReturn;
	float curAnimTimeStamp; //(2 sekundy, ale jesteœmy np w 1.45s)
	bool recalculateBoxDimm;

	//Additional matrixes
	mat4 mvp;

};


#endif /* ENGINE_COMMON_OBJECTS_OBJECT_HPP_ */
