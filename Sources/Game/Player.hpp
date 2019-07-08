/*
 * Player.hpp
 *
 *  Created on: 10 lip 2018
 *      Author: Xoliper
 */

#ifndef GAME_PLAYER_HPP_
#define GAME_PLAYER_HPP_

#define PI 3.14159265

#include <list>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>
#include <math.h>

#include "../Engine/Common/World.hpp"
#include "../Engine/Common/Objects/ObjTemplate.hpp"
#include "../Engine/Common/Objects/Object.hpp"
#include "../Engine/Common/Light.hpp"
#include "../Engine/Common/Timer.hpp"
#include "../Engine/Common/Obsolete/collisiondetector.hpp"

#include "Ship.hpp"
#include "Engine.hpp"
#include "Weapons.hpp"
#include "Map.hpp"

using namespace glm;

extern Light * playerLight;

typedef struct {
	float x;
	float y;
} Force;

class Player {
public:
	Player(World * world,  Timer * timer, ObjTemplate * obj, ObjTemplate * obj2);
	~Player();

	void FixMouse(int x, int y);
	Force GetForce();
	void UpdateLocRot(Force & force, bool collision);
	void UpdateCameraBehaviour(Force & force);
	void EquipAdwWpn(ObjTemplate * rocketToShip, ObjTemplate * obj, ObjTemplate * obj2, float reload);
	void EquipStdWpn(ObjTemplate * obj, float reload);

	void Update(bool ifUpdate, Map * map);	//Move & rotate function
	void Render();
	void RenderToBlur();

	glm::vec3 upDirection;

	//Equipment
	Ship * ship;
	Engine * engine;
	AdvancedWpn * advWpn;
	StandardWpn * stdWpn;

	//For Rockets/Lasers
	glm::vec3 addPos;
	glm::quat addRotRa;

	//Collision
	CollisionDetector cd;

private:
	World * globalWorld;
	Timer * timer;

	//Rotation vars
	glm::quat prevRot;
	glm::quat toRot;
	glm::quat rotAngle;

	//Translation vars
	glm::vec3 oldPos;
	glm::vec3 toPos;

	//View vars
	int fixX, fixY;
	glm::vec3 camPosition;
	float slerpTime;

	//Ship and camera transformation data
	float hPeakTrans;
	float vPeakTrans;
	float zPeakTrans;
	glm::quat ra;

	//Handling collision
	int bounceSkip;
	float bounceBonus;
	float bounceAngle;
	bool bounceZ;
	bool bounceX;

};

#endif /* GAME_PLAYER_HPP_ */
