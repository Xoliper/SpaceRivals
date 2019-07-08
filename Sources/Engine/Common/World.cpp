/*
 * world.cpp
 *
 *  Created on: 8 paü 2017
 *      Author: Arkadiusz Jarlaczyk
 */

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "World.hpp"

WorldRect::WorldRect(){
	x = y = w = h = 0;
}

WorldRect::WorldRect(int x, int y, int w, int h){
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
};

WorldRect::WorldRect(const WorldRect & c){
	this->x = c.x;
	this->y = c.y;
	this->w = c.w;
	this->h = c.h;
};

WorldRect::~WorldRect(){
};

WorldPt::WorldPt(){
	x = y = 0;
}

WorldPt::WorldPt(int x, int y){
	this->x = x;
	this->y = y;
};

WorldPt::WorldPt(const WorldPt & c){
	this->x = c.x;
	this->y = c.y;
};

WorldPt::~WorldPt(){
};

Translation::Translation(){
	x = y = z = 0.0f;
}
Translation::Translation(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}
Translation::Translation(World * world, int x, int y){
	this->x = (float)x/world->width;
	this->y = (float)y/world->height;
	z = 0.0f;
}
Translation::~Translation(){
}

Translation GetTranslation(glm::mat4 & matrix){
	Translation pt;
	pt.x = matrix[3].x;
	pt.y = matrix[3].y;
	pt.z = matrix[3].z;
	return pt;
}

WorldData::WorldData(){
	lookToX = lookToY = lookToZ = fov = 0.0f;
	CameraX = CameraY = CameraZ = 0.0f;
}

WorldData::WorldData(const WorldData & wd){
	cameraDirection = wd.cameraDirection;
	projection = wd.projection;
	view = wd.view;
	fov = wd.fov;
	lookToX = wd.lookToX;
	lookToY = wd.lookToY;
	lookToZ = wd.lookToZ;
	CameraX = wd.CameraX;
	CameraY = wd.CameraY;
	CameraZ = wd.CameraZ;
}


WorldData::~WorldData(){
}


World::World() {
	deltaTime = 1.0f;

	width = 0;
	height = 0;

	staticContext.fov = 60.f;
	staticContext.projection = glm::perspective(glm::radians(staticContext.fov), 1.0f, 0.1f, 100.0f);
	staticContext.view       = glm::lookAt
									(
									glm::vec3(0,0,1), // Camera is at (4,3,3), in World Space
									glm::vec3(0,0,0), // and looks at the origin
									glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
									);


	dynamicContext.fov = 3.14159 * 2000. / 180;
	dynamicContext.projection = glm::perspective(glm::radians(dynamicContext.fov), 1.0f, 0.1f, 1000.0f);
	dynamicContext.view       = glm::lookAt(
								glm::vec3(0,0,0), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
}

World::World(const World & w){
	dynamicContext = w.dynamicContext;
	staticContext = w.staticContext;
	light = w.light;
	width = w.width;
	height = w.height;
	deltaTime = w.deltaTime;
}

World::~World() {
}

void World::SetupFov(float fov, float width, float height){
	this->width = width;
	this->height = height;
	//dynamicContext.fov = fov;
	float testFov = 3.14159 * 2000. / 180;
	dynamicContext.projection = glm::perspective(testFov, width/height, 0.1f, 2500.0f);
	dynamicContext.view       = glm::lookAt(
								glm::vec3(0,0,0), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
}

void World::Change3DLookPosition(float x_temp, float y_temp, float z_temp){
	dynamicContext.lookToX += x_temp*deltaTime;
	dynamicContext.lookToY += y_temp*deltaTime;
	dynamicContext.lookToZ += z_temp*deltaTime;

	dynamicContext.view = glm::lookAt(
			glm::vec3(dynamicContext.CameraX,dynamicContext.CameraY,dynamicContext.CameraZ), // Camera is at (0,0,z), in World Space
			glm::vec3(dynamicContext.lookToX,dynamicContext.lookToY, dynamicContext.lookToZ) + dynamicContext.cameraDirection, // and looks at the origin //x y 0 + camera direction
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	   );
}

void World::Change3DCameraPosition(float x_temp, float y_temp, float z_temp){
	dynamicContext.CameraX += x_temp*deltaTime;
	dynamicContext.CameraY += y_temp*deltaTime;
	dynamicContext.CameraZ += z_temp*deltaTime;

	dynamicContext.view = glm::lookAt(
			glm::vec3(dynamicContext.CameraX,dynamicContext.CameraY,dynamicContext.CameraZ), // Camera is at (0,0,z), in World Space
			glm::vec3(dynamicContext.lookToX,dynamicContext.lookToY, dynamicContext.lookToZ) + dynamicContext.cameraDirection, // and looks at the origin  + cameradirection
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	   );
}

void World::Rotate3DCamera(glm::mat4 rotationMatrix){

		dynamicContext.view = glm::lookAt(
				glm::vec3(dynamicContext.CameraX,dynamicContext.CameraY,dynamicContext.CameraZ), // Camera is at (0,0,z), in World Space
				glm::vec3(dynamicContext.lookToX,dynamicContext.lookToY, dynamicContext.lookToZ), // and looks at the origin  + cameradirection
				glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
		 );

		dynamicContext.view *= rotationMatrix;
}

glm::mat4 World::Get(){
	return glm::lookAt(
			glm::vec3(dynamicContext.CameraX,dynamicContext.CameraY,dynamicContext.CameraZ), // Camera is at (0,0,z), in World Space
			glm::vec3(dynamicContext.lookToX,dynamicContext.lookToY, dynamicContext.lookToZ), // and looks at the origin  + cameradirection
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	 );
}

void World::VecTest(glm::vec3 direction){
	dynamicContext.view = glm::lookAt(
			glm::vec3(dynamicContext.CameraX,dynamicContext.CameraY,dynamicContext.CameraZ), // Camera is at (0,0,z), in World Space
			glm::vec3(dynamicContext.lookToX,dynamicContext.lookToY, dynamicContext.lookToZ) + direction, // and looks at the origin  + cameradirection
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	 );
}

glm::vec3 World::GetCross(){
	return glm::vec3(dynamicContext.CameraX,dynamicContext.CameraY,dynamicContext.CameraZ) - glm::vec3(dynamicContext.lookToX,dynamicContext.lookToY, dynamicContext.lookToZ);
}

void World::Change2DPosition(float x_temp, float y_temp){
	staticContext.CameraX -= x_temp;
	staticContext.CameraY -= y_temp;
	staticContext.lookToX -= x_temp;
	staticContext.lookToY -= y_temp;
	staticContext.view = glm::lookAt(
			glm::vec3(staticContext.CameraX,staticContext.CameraY, 1.0f), // Camera is at (x,y,1.0f), in World Space
			glm::vec3(staticContext.lookToX, staticContext.lookToY, 0.0f), // and looks at the origin //x y 0
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	   );
}
