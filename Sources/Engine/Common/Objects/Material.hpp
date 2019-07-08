/*
 * Material.hpp
 *
 *  Created on: 29 maj 2018
 *      Author: Xoliper
 */

#ifndef ENGINE_COMMON_OBJECTS_MATERIAL_HPP_
#define ENGINE_COMMON_OBJECTS_MATERIAL_HPP_

#include <string>
#include <glm/glm.hpp>
#include "../Graphics/Texture.hpp"

#define MATERIAL_UNDEFINED	0xFFFF
#define MATERIAL_DIFFUSE	0x0000
#define MATERIAL_NORMAL		0x0001
#define MATERIAL_SPECULAR	0x0002

typedef struct {
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
} MaterialColorData;

class Material {
public:
	Material();
	virtual ~Material();

	std::string materialName;
	Texture diffuseTex;
	Texture normalTex;
	Texture specularTex;
	MaterialColorData mcd;

};

#endif /* ENGINE_COMMON_OBJECTS_MATERIAL_HPP_ */
