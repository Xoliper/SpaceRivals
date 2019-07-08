/*
 * TangentSpace.hpp
 *
 *  Created on: 27 cze 2018
 *      Author: Xoliper
 */

#ifndef ENGINE_COMMON_OBJECTS_TANGENTSPACE_HPP_
#define ENGINE_COMMON_OBJECTS_TANGENTSPACE_HPP_

#include <vector>
#include <glm/glm.hpp>

void ComputeTangentBasis(
	// inputs
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals,
	// outputs
	std::vector<glm::vec3> & tangents,
	std::vector<glm::vec3> & bitangents
);

#endif /* ENGINE_COMMON_OBJECTS_TANGENTSPACE_HPP_ */
