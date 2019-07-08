/*
 * SkyBox.cpp
 *
 *  Created on: 28 cze 5018
 *      Author: Xoliper
 */

#include "SkyBox.hpp"

float points[] = {
  -1000.0f,  1000.0f, -1000.0f,
  -1000.0f, -1000.0f, -1000.0f,
   1000.0f, -1000.0f, -1000.0f,
   1000.0f, -1000.0f, -1000.0f,
   1000.0f,  1000.0f, -1000.0f,
  -1000.0f,  1000.0f, -1000.0f,

  -1000.0f, -1000.0f,  1000.0f,
  -1000.0f, -1000.0f, -1000.0f,
  -1000.0f,  1000.0f, -1000.0f,
  -1000.0f,  1000.0f, -1000.0f,
  -1000.0f,  1000.0f,  1000.0f,
  -1000.0f, -1000.0f,  1000.0f,

   1000.0f, -1000.0f, -1000.0f,
   1000.0f, -1000.0f,  1000.0f,
   1000.0f,  1000.0f,  1000.0f,
   1000.0f,  1000.0f,  1000.0f,
   1000.0f,  1000.0f, -1000.0f,
   1000.0f, -1000.0f, -1000.0f,

  -1000.0f, -1000.0f,  1000.0f,
  -1000.0f,  1000.0f,  1000.0f,
   1000.0f,  1000.0f,  1000.0f,
   1000.0f,  1000.0f,  1000.0f,
   1000.0f, -1000.0f,  1000.0f,
  -1000.0f, -1000.0f,  1000.0f,

  -1000.0f,  1000.0f, -1000.0f,
   1000.0f,  1000.0f, -1000.0f,
   1000.0f,  1000.0f,  1000.0f,
   1000.0f,  1000.0f,  1000.0f,
  -1000.0f,  1000.0f,  1000.0f,
  -1000.0f,  1000.0f, -1000.0f,

  -1000.0f, -1000.0f, -1000.0f,
  -1000.0f, -1000.0f,  1000.0f,
   1000.0f, -1000.0f, -1000.0f,
   1000.0f, -1000.0f, -1000.0f,
  -1000.0f, -1000.0f,  1000.0f,
   1000.0f, -1000.0f,  1000.0f
};

SkyBox::SkyBox(World * wrd, CubeMapTexture * cmt){
	this->world = wrd;
	this->cmt = cmt;
	params = ShaderLoader::GetProgram("skybox");

	model = glm::mat4(1.0f);
	rotation = glm::rotate(0.0f, glm::vec3( 1.0f, 1.0f, 1.0f));
	translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
	scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points, GL_STATIC_DRAW);

}

SkyBox::~SkyBox(){

}

void SkyBox::SetPosition(float x, float y, float z){
	translation = glm::translate(glm::mat4(), glm::vec3(x, y, z));
}

void SkyBox::SetScale(float x, float y, float z){
	scale = glm::scale(glm::vec3(x,y,z));
}

void SkyBox::SetRotation(bool x, bool y, bool z, float angle){
	rotation = glm::rotate(angle, glm::vec3( x, y, z));
}

void SkyBox::ChangeRotation(bool x, bool y, bool z, float angle){
	rotation *= glm::rotate(angle, glm::vec3( x, y, z));
}

void SkyBox::Render(){

	model = translation  * rotation * scale * glm::mat4(1.0f); // *rotation
	mvp = world->dynamicContext.projection * world->dynamicContext.view * model;

	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);

	glUseProgram(params->at(0));

	glUniformMatrix4fv(params->at(1), 1, GL_FALSE, &world->dynamicContext.view[0][0]);
	glUniformMatrix4fv(params->at(2), 1, GL_FALSE, &world->dynamicContext.projection[0][0]);
	glUniformMatrix4fv(params->at(4), 1, GL_FALSE, &model[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cmt->texture_id);
	glUniform1i(params->at(3), 0);

	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);

}
