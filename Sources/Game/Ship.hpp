/*
 * Ship.hpp
 *
 *  Created on: 1 wrz 2018
 *      Author: Xoliper
 */

#ifndef GAME_SHIP_HPP_
#define GAME_SHIP_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../Engine/Common/World.hpp"
#include "../Engine/Common/Objects/ObjTemplate.hpp"
#include "../Engine/Common/Objects/Object.hpp"


class Ship {
public:
	Ship(World * world, ObjTemplate * shipObjTemplate);
	~Ship();

	glm::vec3 GetShipTranslation();
	glm::vec3 GetShipCameraOffset();

	void UpdateVisuals(glm::vec3 pos, glm::vec3 addPos, glm::quat rot, glm::quat addRot);
	void Render();

	VisualObject * shipVo;

private:
	World * world;
	glm::vec3 offset;

};

#endif /* GAME_SHIP_HPP_ */
