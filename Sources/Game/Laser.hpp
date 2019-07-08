
#ifndef GAME_LASER_HPP_
#define GAME_LASER_HPP_


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

class Laser {
	friend class StandardWpn;
public:
	Laser(World * w, ObjTemplate * laserTemplate, glm::vec3 pos, glm::vec3 addPos, glm::quat rotRa, glm::mat4 v);
	~Laser();

	void Work(CollisionDetector * cd, Map * map);

	void Render();

private:

	PhantomPointObject ppo;
	glm::mat4 laser1Mat;
	glm::mat4 laser2Mat;
	bool hit;

	ObjTemplate * laserTemplate;
	VisualObject * laserParticle0;
	VisualObject * laserParticle1;

	glm::mat4 v;
	glm::quat rotRa;
	glm::vec3 pos;
	glm::vec3 posAdd;
	float distance;
};

#endif /* GAME_LASER_HPP_ */
