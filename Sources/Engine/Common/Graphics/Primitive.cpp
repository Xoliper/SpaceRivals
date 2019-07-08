#include "Primitive.hpp"

RectanglePrim::RectanglePrim(World * world, WorldRect wr, Color color, int flags) {

	params = ShaderLoader::GetProgram("primitive");

	this->wr = wr;
	this->world = world;
	this->color = color;
	this->flags = flags;

	model = glm::mat4(1.0f);
	rotation = glm::rotate(0.0f, glm::vec3( 1.0f, 1.0f, 1.0f));
	translation = glm::translate(glm::mat4(), glm::vec3((float)(wr.x*2)/world->width, (float)(wr.y*2)/world->height, 0.0f));
	scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

	float w_ver = (float)wr.w/world->width;
	float h_ver =  (float)wr.h/world->height;
	float w_ver_start = -w_ver;
	float h_ver_start = -h_ver;


	if(flags){ //If true -> wr position is measured in center of rectangle
		w_ver_start = 0.0f;
		h_ver_start = 0.0f;
		w_ver *= 2;
		h_ver *=2;
	}

	size = 18;
	bufferData.resize(size, 0.0f);

	bufferData = {w_ver_start, h_ver_start, 0.0f,
				  w_ver_start, h_ver, 0.0f,
				  w_ver, h_ver_start, 0.0f,
				  w_ver, h_ver_start, 0.0f,
				  w_ver_start, h_ver, 0.0f,
				  w_ver, h_ver, 0.0f};

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, size*sizeof(GLfloat), &bufferData[0], GL_STATIC_DRAW);
}

RectanglePrim::~RectanglePrim() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void RectanglePrim::SetPosition(WorldRect wr){
	this->wr = wr;
	translation = glm::translate(glm::mat4(), glm::vec3((float)(wr.x*2)/world->width, (float)(wr.y*2)/world->height, 0.0f));
}

void RectanglePrim::Resize(int newWidth, int newHeight){
	if(newWidth >= 0) wr.w = newWidth;
	if(newHeight >= 0) wr.h = newHeight;

	float w_ver = (float)wr.w/world->width;
	float h_ver =  (float)wr.h/world->height;
	float w_ver_start = -w_ver;
	float h_ver_start = -h_ver;

	if(flags){
		w_ver_start = 0.0f;
		h_ver_start = 0.0f;
		w_ver *= 2;
		h_ver *=2;
	}

	bufferData = {w_ver_start, h_ver_start, 0.0f,
				  w_ver_start, h_ver, 0.0f,
				  w_ver, h_ver_start, 0.0f,
				  w_ver, h_ver_start, 0.0f,
				  w_ver_start, h_ver, 0.0f,
				  w_ver, h_ver, 0.0f};

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(GLfloat), &bufferData[0]);
}

int RectanglePrim::GetWidth(){
	return wr.w;
}

int RectanglePrim::GetHeight(){
	return wr.h;
}

WorldRect RectanglePrim::GetBoundary(){
	return wr;
}

void RectanglePrim::SetColor(Color color){
	this->color = color;
}

void RectanglePrim::Render(){
	glDisable(GL_CULL_FACE);

	model = translation  * rotation * scale * glm::mat4(1.0f);

	glUseProgram(params->at(0));
	glUniformMatrix4fv(params->at(1), 1, GL_FALSE, &world->staticContext.view[0][0]);
	glUniformMatrix4fv(params->at(2), 1, GL_FALSE, &model[0][0]);
	glUniform4f(params->at(3), color.r, color.g, color.b, color.a);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);


	// Draw the triangles !
	//glDisable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLES, 0, size/3); // 12*3 indices starting at 0 -> 12 triangles
	//glEnable(GL_CULL_FACE);


	glDisableVertexAttribArray(0);
	glEnable(GL_CULL_FACE);

}

