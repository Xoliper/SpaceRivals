#include "Sprite.hpp"

Sprite::Sprite(World * world, WorldRect wr, Texture * tex,   int flags) {

	params = ShaderLoader::GetProgram("sprite");
	this->shaderName = shaderName;
	this->mode = 0;

	frames = 0;
	tWidth = 0;
	tHeight = 0;
	animTime = 0.0f;
	curAnimTime = 0.0f;

	this->wr = wr;
	this->tex = tex;
	this->world = world;

	model = glm::mat4(1.0f);
	rotation = glm::rotate(0.0f, glm::vec3( 1.0f, 1.0f, 1.0f));
	//translation = glm::translate(glm::mat4(), glm::vec3((float)(wr.x*2)/world->width, (float)(wr.y*2)/world->height, 0.0f));
	translation = glm::translate(glm::mat4(), 0.0f, 0.0f, 0.0f);
	scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

	blurDepthTex = (GLuint)-1;
	shpFirst = 1.0f/800.0f;
	shpSecond = 1.0f/800.0f;

	animState = true;

	float w_ver;
	float h_ver;

	if(wr.w == -1){
		w_ver = (float)tex->dimm.width/world->width;
	} else {
		w_ver = (float)wr.w/world->width;
	}
	if(wr.h == -1){
		h_ver = (float)tex->dimm.height/world->height;
	} else {
		h_ver = (float)wr.h/world->height;
	}
	float w_ver_start = -w_ver;
	float h_ver_start = -h_ver;

	if(flags){
		w_ver_start = 0.0f;
		h_ver_start = 0.0f;
		w_ver *= 2;
		h_ver *=2;
	}

	vertexBufferData.resize(18, 0.0f);
	uvBufferData.resize(12, 0.0f);

	vertexBufferData = {w_ver_start, h_ver_start, 0.0f,
						w_ver_start,h_ver, 0.f,
						w_ver, h_ver_start, 0.0f,
						w_ver, h_ver_start, 0.0f,
						w_ver_start, h_ver, 0.0f,
						w_ver, h_ver, 0.0f};

	uvBufferData = {0.0f, 0.0f,
					0.0f, 1.0f,
					1.0f, 0.0f,
					1.0f, 0.0f,
					0.0f, 1.0f,
					1.0f, 1.0f};


	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), &vertexBufferData[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), &uvBufferData[0], GL_STATIC_DRAW);
}

Sprite::~Sprite() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Sprite::SetPosition(float x, float y, float z){
	translation = glm::translate(glm::mat4(), x, y, z);
}

void Sprite::RotateToOrigin(float x, float y, float z, glm::vec3 upDirection){
	//rotation = glm::rotation(, );
	originMatrix = glm::inverse(glm::lookAt(glm::vec3(translation[3][0],translation[3][1],translation[3][2]), glm::vec3(x,y,z), glm::vec3(1,0,0)));
}

void Sprite::SetScale(float x, float y, float z){
	if(mode == 0){
		scale = glm::scale(glm::vec3(x,y,z));
	} else {
		scale = glm::scale(glm::vec3(x, y*(float)world->height/world->width, z));
	}
}

void Sprite::SetShader(std::string shaderName){
	params = ShaderLoader::GetProgram(shaderName);
	this->shaderName = shaderName;
}

void Sprite::SetBlurDepthTex(GLuint id){
	blurDepthTex = id;
}

void Sprite::SetShaderParam(float first, float second){
	shpFirst = first;
	shpSecond = second;
}

void Sprite::Resize(int width, int height){
	wr.w = width;
	wr.h = height;
	std::cout<<wr.h/world->height<<std::endl;
	scale = glm::scale(glm::vec3(wr.w/world->width, 1.0f, 1.0f));
}


void Sprite::SetupAnimation(int tWidth, int tHeight, int frames, float animTime){
	this->tWidth = tWidth;
	this->tHeight = tHeight;
	this->frames = frames;
	this->animTime = animTime;
	this->animState = false;
	this->curFrame = 0;
}

void Sprite::SetMode(bool mode){
	this->mode = mode;
	if(mode == 1){
		scale = glm::scale(glm::vec3(1.0f, 1.0f*(float)world->height/world->width, 1.0f));
	}
}

bool Sprite::Animate(Timer * timer){
	if(timer->GetPhysicsUpdateState()){

		if(curFrame == frames-1){
			return true;
		}

		//Calculate frames (0-63)
		float frameTime = animTime/frames;
		curFrame = curAnimTime/frameTime;

		curAnimTime += timer->GetDelta();

		if(curAnimTime > animTime){
			curAnimTime = 0.0f;
		}

		float tileW = (float)tWidth/tex->dimm.width;
		float tileH = (float)tHeight/tex->dimm.height;

		int xAmount = tex->dimm.width/tWidth;
		int yAmount = tex->dimm.height/tHeight;

		float x =  tileW * (curFrame%xAmount);
		float y = 1.0f-(tileH * (curFrame/yAmount)-tileH);
		float w = x + tileW;
		float h = (y + tileH);


		//Set UV coords
		uvBufferData = {x, y,
						w, y,
						x, h,
						x, h,
						w, y,
						w, h};


		glBindVertexArray(vertexArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(glm::vec3), &uvBufferData[0], GL_DYNAMIC_DRAW);
	}
	return false;
}

void Sprite::Render(){

	if(mode == 0){
		model = translation  * rotation * scale * glm::mat4(1.0f);
	} else {
		model = originMatrix * scale;
	}

	glUseProgram(params->at(0));
	if(mode == 0){
		glUniformMatrix4fv(params->at(1), 1, GL_FALSE, &world->staticContext.view[0][0]);
		mvp = world->staticContext.view * model;
	} else {
		glUniformMatrix4fv(params->at(1), 1, GL_FALSE, &world->dynamicContext.view[0][0]);
		mvp = world->dynamicContext.projection * world->dynamicContext.view * model;
	}
	glUniformMatrix4fv(params->at(4), 1, GL_FALSE, &mvp[0][0]);

	glUniformMatrix4fv(params->at(2), 1, GL_FALSE, &model[0][0]);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->texture_id);
	glUniform1i(params->at(3), 0);

	if(shaderName == "spriteBlur"){
        glUniform2f(params->at(5), shpFirst, shpSecond);
        //if(blurDepthTex != (GLuint)-1){
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, blurDepthTex);
			glUniform1i(params->at(6), 1);
        //}
	}

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

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size : U+V => 2
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDisable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

