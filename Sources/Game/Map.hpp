/*
 * Map.hpp
 *
 *  Created on: 31 sie 2018
 *      Author: Xoliper
 */

#ifndef GAME_MAP_HPP_
#define GAME_MAP_HPP_

#include <vector>
#include <string>
#include <list>

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>

#include "../Engine/Common/World.hpp"
#include "../Engine/Common/Graphics/Texture.hpp"
#include "../Engine/Common/Objects/SkyBox.hpp"
#include "../Engine/Common/Objects/MeshInstances.hpp"
#include "../Engine/Common/Objects/Object.hpp"
#include "../Engine/Common/RnGenerator.hpp"


class Map {
public:
	Map(World * world);
	~Map();

	std::list<Drawable *> * GetCollisionObjects();
	void LoadSkyBox(std::string path);
	void LoadSun(ObjTemplate * objTemplate, float scale, float x, float y, float z);
	void GenerateAsteroids(ObjTemplate * objTemplate, int amount, float radiusBase, int radiusDeviation, float sizeBase, int sizeDeviation);
	void Render();
	void RenderToBlur();

private:
	//General data
	World * world;
	RnGenerator rng;

	//Skybox
	std::vector<std::string> faces;
	CubeMapTexture * cmt;
	SkyBox * sk;

	//Sun
	VisualObject * sun;

	//Particles (asteroids)
	std::list<Drawable * > lPpo;
	std::list<MeshInstances *> asteroids;

};

#endif /* GAME_MAP_HPP_ */
