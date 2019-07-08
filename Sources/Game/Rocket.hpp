
#ifndef GAME_ROCKET_HPP_
#define GAME_ROCKET_HPP_

#include <list>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../Engine/Common/World.hpp"
#include "../Engine/Common/Objects/ObjTemplate.hpp"
#include "../Engine/Common/Objects/Object.hpp"
#include "../Engine/Common/Obsolete/collisiondetector.hpp"

#include "Map.hpp"

class Rocket {
	friend class AdvancedWpn;
public:
	Rocket(World * w, ObjTemplate * rocketTemplate, ObjTemplate * thrusterTemplate, glm::vec3 pos, glm::vec3 addPos, glm::quat rotRa, glm::mat4 v);
	~Rocket();

	void Work(CollisionDetector * cd, Map * map);

	void Render();
	void RenderToBlur();

private:

	PhantomPointObject ppo;
	glm::mat4 rocket1Mat;
	glm::mat4 rocket2Mat;
	bool hit;

	ObjTemplate * rocketTemplate;
	ObjTemplate * thrusterTemplate;

	VisualObject * rocketVo1;
	VisualObject * rocketVo2;

	VisualObject * thrusterVo1;
	VisualObject * thrusterVo2;

	glm::mat4 v;
	glm::quat rotRa;
	glm::vec3 pos;
	glm::vec3 posAdd;
	float distance;

};

#endif /* GAME_ROCKET_HPP_ */
