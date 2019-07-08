/*
 * MeshInstances.hpp
 *
 *  Created on: 30 cze 2018
 *      Author: Xoliper
 */

#ifndef ENGINE_COMMON_OBJECTS_MESHINSTANCES_HPP_
#define ENGINE_COMMON_OBJECTS_MESHINSTANCES_HPP_

#include <string>
#include "..\Drawable.hpp"
#include "MeshData.hpp"
#include "..\World.hpp"
#include "..\Graphics\Texture.hpp"
#include "../Shaderloader.hpp"

//Dodac funkcje manipulujaca (particles_position_buffer_data)
//ale to w sumie w particles managerze

class MeshInstances : public Drawable {
public:
	MeshInstances(World * wrd, MeshData md, Texture * tex, Texture * texNormal, Texture * texSpec);
	~MeshInstances();

	void UpdateBuffers();

	void Render();

	std::vector<GLfloat> particles_position_buffer_data;


private:

	std::string shaderName;
	MeshData meshData;
	Texture * tex;
	Texture * texNormal;
	Texture * texSpec;

	GLuint VertexArrayID;
	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;
	GLuint particles_normals_buffer;
	GLuint particles_tangent_buffer;
	GLuint particles_bitangent_buffer;
	GLuint elementBuffer;

	std::vector<GLfloat> particles_normals_data;
	std::vector<GLfloat> billboard_vertex_buffer_data;
	std::vector<GLfloat> particles_color_buffer_data;
	std::vector<GLfloat> particles_tangent_buffer_data;
	std::vector<GLfloat> particles_bitangent_buffer_data;

};

#endif /* ENGINE_COMMON_OBJECTS_MESHINSTANCES_HPP_ */
