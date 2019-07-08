/*
 * MeshInstances.cpp
 *
 *  Created on: 30 cze 2018
 *      Author: Xoliper
 */


//TODO: DODAÆ INDEKSOWANIE BUFORÓW!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//DONE :D
#include "MeshInstances.hpp"

MeshInstances::MeshInstances(World * wrd, MeshData md, Texture * tex, Texture * texNorm, Texture * texSpec){
	this->world = wrd;
	this->meshData = md;
	this->tex = tex;
	this->texNormal = texNorm;
	this->texSpec = texSpec;
	params = ShaderLoader::GetProgram("meshInstances");
	shaderName = "meshInstances";

	for(unsigned int i = 0; i<meshData.vertices.size(); i++){
		billboard_vertex_buffer_data.push_back(meshData.vertices[i].x);
		billboard_vertex_buffer_data.push_back(meshData.vertices[i].y);
		billboard_vertex_buffer_data.push_back(meshData.vertices[i].z);
	}

	for(unsigned int i = 0; i<meshData.uvs.size(); i++){
		particles_color_buffer_data.push_back(meshData.uvs[i].x);
		particles_color_buffer_data.push_back(meshData.uvs[i].y);
	}

	for(unsigned int i = 0; i<meshData.normals.size(); i++){
		particles_normals_data.push_back(meshData.normals[i].x);
		particles_normals_data.push_back(meshData.normals[i].y);
		particles_normals_data.push_back(meshData.normals[i].z);
	}

	for(unsigned int i = 0; i<meshData.tangents.size(); i++){
		particles_tangent_buffer_data.push_back(meshData.tangents[i].x);
		particles_tangent_buffer_data.push_back(meshData.tangents[i].y);
		particles_tangent_buffer_data.push_back(meshData.tangents[i].z);
	}

	for(unsigned int i = 0; i<meshData.bitangents.size(); i++){
		particles_bitangent_buffer_data.push_back(meshData.bitangents[i].x);
		particles_bitangent_buffer_data.push_back(meshData.bitangents[i].y);
		particles_bitangent_buffer_data.push_back(meshData.bitangents[i].z);
	}

	glGenVertexArrays(1, &VertexArrayID);

	glGenBuffers(1, &billboard_vertex_buffer);	//Store vertexs
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * billboard_vertex_buffer_data.size() , &billboard_vertex_buffer_data[0], GL_STATIC_DRAW);


	glGenBuffers(1, &particles_position_buffer);	//Store position.xyz + scale.xyz
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * particles_position_buffer_data.size(), NULL, GL_STREAM_DRAW);


	glGenBuffers(1, &particles_color_buffer);	//Store UVs
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER,  sizeof(GLfloat) * particles_color_buffer_data.size(), &particles_color_buffer_data[0], GL_STATIC_DRAW);


	//Indices
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indices.size() * sizeof(unsigned short), &meshData.indices[0], GL_STATIC_DRAW);

	//Normals
	glGenBuffers(1, &particles_normals_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particles_normals_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, particles_normals_data.size() * sizeof(GLfloat), &particles_normals_data[0], GL_STATIC_DRAW);

	//tangents
	glGenBuffers(1, &particles_tangent_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particles_tangent_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, particles_tangent_buffer_data.size() * sizeof(GLfloat), &particles_tangent_buffer_data[0], GL_STATIC_DRAW);

	//bitangents
	glGenBuffers(1, &particles_bitangent_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particles_bitangent_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, particles_bitangent_buffer_data.size() * sizeof(GLfloat), &particles_bitangent_buffer_data[0], GL_STATIC_DRAW);


};

MeshInstances::~MeshInstances(){
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteBuffers(1, &billboard_vertex_buffer);
	glDeleteBuffers(1, &particles_position_buffer);
	glDeleteBuffers(1, &particles_color_buffer);
	glDeleteBuffers(1, &elementBuffer);
	glDeleteBuffers(1, &particles_normals_buffer);
	glDeleteBuffers(1, &particles_tangent_buffer);
	glDeleteBuffers(1, &particles_bitangent_buffer);

}

void MeshInstances::UpdateBuffers(){
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * particles_position_buffer_data.size(), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * particles_position_buffer_data.size(), &particles_position_buffer_data[0]);
}


void MeshInstances::Render(){

	glUseProgram(params->at(0));

	//Pass uniforms
	glUniformMatrix4fv(params->at(1), 1, GL_FALSE, &world->dynamicContext.view[0][0]);
	glUniformMatrix4fv(params->at(2), 1, GL_FALSE, &world->dynamicContext.projection[0][0]);

	glUniform3f(params->at(6), world->light.position.x, world->light.position.y, world->light.position.z);	//Light position
	glUniform3f(params->at(5), world->light.color.r, world->light.color.g, world->light.color.b); 			//Light color
	glUniform1f(params->at(4), world->light.power);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->texture_id);
	glUniform1i(params->at(3), 0);


	if(shaderName == "meshInstancesAdv"){
		glm::mat3 ModelView3x3Matrix = glm::transpose(glm::inverse(glm::mat3(world->dynamicContext.view)));//glm::transpose(glm::inverse(glm::mat3(world->dynamicContext.view)));//glm::mat3(model);
		//glm::mat3 ModelView3x3Matrix = glm::mat3((lv)*model);				//Bump map
		glUniformMatrix3fv(params->at(9), 1, GL_FALSE, &ModelView3x3Matrix[0][0]); //mv3x3

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texNormal->texture_id);
		glUniform1i(params->at(7), 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texSpec->texture_id);
		glUniform1i(params->at(8), 2);
	}



	//glBindVertexArray(VertexArrayID);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
	 0, // attribute. No particular reason for 0, but must match the layout in the shader.
	 3, // size
	 GL_FLOAT, // type
	 GL_FALSE, // normalized?
	 0, // stride
	 (void*)0 // array buffer offset
	);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
	 1, // attribute. No particular reason for 1, but must match the layout in the shader.
	 4, // size : x + y + z + size
	 GL_FLOAT, // type
	 GL_FALSE, // normalized?
	 0, // stride
	 (void*)0 // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(
	 2, // attribute. No particular reason for 1, but must match the layout in the shader.
	 2, // size : U+V
	 GL_FLOAT, // type
	 GL_FALSE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
	 0, // stride
	 (void*)0 // array buffer offset
	);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, particles_normals_buffer);
	glVertexAttribPointer(
	 3, // attribute. No particular reason for 1, but must match the layout in the shader.
	 3, // size : xyz
	 GL_FLOAT, // type
	 GL_FALSE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
	 0, // stride
	 (void*)0 // array buffer offset
	);

	if(shaderName == "meshInstancesAdv"){
		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, particles_tangent_buffer);
		glVertexAttribPointer(
		 4, // attribute. No particular reason for 1, but must match the layout in the shader.
		 3, // size : xyz
		 GL_FLOAT, // type
		 GL_FALSE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		 0, // stride
		 (void*)0 // array buffer offset
		);

		glEnableVertexAttribArray(5);
		glBindBuffer(GL_ARRAY_BUFFER, particles_bitangent_buffer);
		glVertexAttribPointer(
		 5, // attribute. No particular reason for 1, but must match the layout in the shader.
		 3, // size : xyz
		 GL_FLOAT, // type
		 GL_FALSE, // normalized? *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		 0, // stride
		 (void*)0 // array buffer offset
		);
	}

	//std::cerr<<"test?"<<std::endl;


	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same mesh vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per vec3 (its center) -> 1
	glVertexAttribDivisor(2, 0); // color : one per UV positions -> 1
	glVertexAttribDivisor(3, 0); // color : one per UV positions -> 1
	if(shaderName == "meshInstancesAdv"){
		glVertexAttribDivisor(4, 0); // color : one per UV positions -> 1
		glVertexAttribDivisor(5, 0); // color : one per UV positions -> 1
	}
	//glDrawArraysInstanced(GL_TRIANGLES, 0, billboard_vertex_buffer_data.size(), particles_position_buffer_data.size()/3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);


	glDrawElementsInstanced (
		GL_TRIANGLES,     			 // mode
		meshData.indices.size(),    // count
		GL_UNSIGNED_SHORT,  		 // type
		(void*)0,	//Where are indices stored
		particles_position_buffer_data.size()/3					 // how many
	);

	//glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, meshData.indices.size(), particles_position_buffer_data.size()/3);


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	if(shaderName == "meshInstancesAdv"){
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
	}

	//Disable divisor
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 0);
	glVertexAttribDivisor(3, 0);
	if(shaderName == "meshInstancesAdv"){
		glVertexAttribDivisor(4, 0);
		glVertexAttribDivisor(5, 0);
	}

}
