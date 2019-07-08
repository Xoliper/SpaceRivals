/*
 * world.hpp
 *
 *  Created on: 8 paü 2017
 *      Author: Arkadiusz Jarlaczyk
 */

#ifndef ENGINE_COMMON_WORLD_HPP_
#define ENGINE_COMMON_WORLD_HPP_

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <windows.h>
#include <math.h>
#include <iostream>
#include "Config.hpp"
#include "Light.hpp"

class World;

class WorldRect {
public:
	WorldRect();
	WorldRect(int x, int y, int w, int h);
	WorldRect(const WorldRect & c);
	~WorldRect();

	int x,y,w,h;
};

class WorldPt {
public:
	WorldPt();
	WorldPt(int x, int y);
	WorldPt(const WorldPt &c);
	~WorldPt();

	int x,y;
};

class Translation {
public:
	Translation();
	Translation(float x, float y, float z);
	Translation(World * world, int x, int y);
	~Translation();
	float x, y, z;
};

Translation GetTranslation(glm::mat4 & matrix);


class WorldData {
public:
	WorldData();
	WorldData(const WorldData & wd);
	~WorldData();

	float CameraX,CameraY,CameraZ;
	float lookToX,lookToY,lookToZ;
	glm::vec3 cameraDirection;
	float fov;
	glm::mat4 projection;
	glm::mat4 view;
};

class World {

public:
	World();
	World(const World & w);
	virtual ~World();

	void SetupFov(float fov, float width, float height);
	void Change3DLookPosition(float x, float y, float z);
	void Change3DCameraPosition(float x, float y, float z);
	void Rotate3DCamera(glm::mat4 rotationMatrix);
	void Update3DCamera();
	void Change2DPosition(float x, float y);
	glm::mat4 Get();
	void VecTest(glm::vec3 vec);
	glm::vec3 GetCross();

	WorldData dynamicContext;
	WorldData staticContext;
	Light light;
	int width, height;
	float deltaTime;



};

#endif /* ENGINE_COMMON_WORLD_HPP_ */
