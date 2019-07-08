/*
 * Engine.hpp
 *
 *  Created on: 1 wrz 2018
 *      Author: Xoliper
 */

#ifndef GAME_ENGINE_HPP_
#define GAME_ENGINE_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../Engine/Common/World.hpp"
#include "../Engine/Common/Objects/ObjTemplate.hpp"
#include "../Engine/Common/Objects/Object.hpp"

class Engine {
public:
	Engine(World * world, ObjTemplate * engineObjTemplate);
	~Engine();

	float GetCurSpeed();
	float GetTurnSpeed();
	float GetHorizontalSpeed();

	float Work();
	void UpdateVisuals(glm::vec3 pos, glm::vec3 addPos, glm::quat rot, glm::quat addRot);
	void Render();

	VisualObject * thruster;


private:
	float horizontalSpeed;
	float turnSpeed;
	float maxSpeed;
	float minSpeed;
	float curSpeed;
	bool booster;
	int boosterSkip;
};

#endif /* GAME_ENGINE_HPP_ */
