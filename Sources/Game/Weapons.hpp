/*
 * Weapons.hpp
 *
 *  Created on: 1 wrz 2018
 *      Author: Xoliper
 */

#ifndef GAME_WEAPONS_HPP_
#define GAME_WEAPONS_HPP_

#include <list>

#include "../Engine/Common/World.hpp"
#include "../Engine/Common/Objects/ObjTemplate.hpp"
#include "../Engine/Common/Objects/Object.hpp"
#include "../Engine/Common/Timer.hpp"
#include "../Engine/Common/Obsolete/collisiondetector.hpp"
#include "../Engine/Common/Graphics/Sprite.hpp"

#include "Rocket.hpp"
#include "Laser.hpp"
#include "Map.hpp"
#include "ResContainer.hpp"

extern ResContainer * rc;

class Player;

class AdvancedWpn {
	friend class Gui;
public:
	AdvancedWpn(World * world, Player * player, ObjTemplate * rocketToShip, ObjTemplate * rocketTemplate, ObjTemplate * thrusterTemplate, Timer * timer, float reload);
	~AdvancedWpn();

	void Add( glm::vec3 pos, glm::vec3 addPos, glm::quat rotRa, glm::mat4 v);
	void Work(glm::vec3 pos, glm::vec3 addPos, glm::quat rot, glm::quat addRot, CollisionDetector * cd, Map * map);

	void Render();
	void RenderToBlur();

	float actualReload;
	float reload;

private:
	World * world;
	Timer * timer;
	Player * player;

	ObjTemplate * rocketTemplate;
	ObjTemplate * thrusterTemplate;
	VisualObject * attachedRockets;
	std::list<Rocket*> rocketsList;
	std::list<Sprite*> explosionsList;
};


class StandardWpn {
	friend class Gui;
public:
	StandardWpn(World * world, Player * player, ObjTemplate * laserTemplate, Timer * timer, float reload);
	~StandardWpn();

	void Add( glm::vec3 pos, glm::vec3 addPos, glm::quat rotRa, glm::mat4 v);
	void Work( CollisionDetector * cd, Map * map);

	void Render();
	void RenderToBlur();

	float actualReload;
	float reload;

private:
	World * world;
	Timer * timer;
	Player * player;

	ObjTemplate * laserTemplate;
	std::list<Laser*> lasersList;
	std::list<Sprite*> lImpactList;

};

#endif /* GAME_WEAPONS_HPP_ */
