/*
 * MeshData.hpp
 *
 *  Created on: 30 cze 2018
 *      Author: Xoliper
 */

#ifndef ENGINE_COMMON_OBJECTS_MESHDATA_HPP_
#define ENGINE_COMMON_OBJECTS_MESHDATA_HPP_

#include <vector>
#include <glm/glm.hpp>

class MeshData {
public:
	MeshData();
	MeshData(const MeshData & obj);
	~MeshData();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
};

#endif /* ENGINE_COMMON_OBJECTS_MESHDATA_HPP_ */
