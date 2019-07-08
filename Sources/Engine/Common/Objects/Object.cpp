/*
 * Object.cpp
 *
 *  Created on: 12 maj 2018
 *      Author: Xoliper
 */

#include "Object.hpp"

PhantomPointObject::PhantomPointObject(){
	x = y = z = diameter = 0.0f;
	objType = STATIC;
}

PhantomPointObject::PhantomPointObject(float x, float y, float z, float s){
	this->x = x;
	this->y = y;
	this->z = z;
	this->diameter = s;
	objType = STATIC;
}

PhantomPointObject::~PhantomPointObject(){
}

void PhantomPointObject::Render(){
}


VisualObject::VisualObject(World * wrd, ObjTemplate * obj) {

	//Init variables
	shaderName = "objectAdv";
	this->world = wrd;
	this->objTemplate = obj;
	params = ShaderLoader::GetProgram(shaderName);
	lv = world->dynamicContext.view;
	otherLv = false;

	animReturn = false;
	animRepeat = false;
	animPlayback = false;
	recalculateBoxDimm = false;
	curAnimTimeStamp = 0.0f;

	model = glm::mat4(1.0f);
	rotation = glm::rotate(0.0f, glm::vec3( 1.0f, 1.0f, 1.0f));
	translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
	scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

	if(objTemplate == nullptr || objTemplate->GetFramesAmount() <= 0){
		throw Exception("VisualObject", "Constructor", "Failed to load ObjTemplate (maybe it is not valid?)");
	}


	//Init meshes
	for(unsigned int i = 0; i<objTemplate->frames[0]->meshesTemplate.size(); i++){
		SubObject so;

		so.tangents = objTemplate->frames[0]->meshesTemplate[i]->otTangents;
		so.bitangents = objTemplate->frames[0]->meshesTemplate[i]->otBitangents;

		so.indices = objTemplate->frames[0]->meshesTemplate[i]->otIndices;
		so.vertices =  objTemplate->frames[0]->meshesTemplate[i]->otVertices;
		so.uvs = objTemplate->frames[0]->meshesTemplate[i]->otUvs;
		so.normals = objTemplate->frames[0]->meshesTemplate[i]->otNormals;

		so.dimm = objTemplate->frames[0]->meshesTemplate[i]->dimm;
		so.materialId = objTemplate->frames[0]->meshesTemplate[i]->materialId;
		meshes.push_back(so);
	}

	if(objTemplate->GetFramesAmount() == 1){
		objType = STATIC;
	} else if(objTemplate->GetFramesAmount() > 1) {
		objType = DYNAMIC;
	} else {
		objType = DK;
	}

	for(unsigned int i = 0; i<meshes.size();i++){
	    meshes[i].CreateBuffers(objType);
	}

}

VisualObject::~VisualObject(){
}

void VisualObject::SetPosition(float x, float y, float z){
	translation = glm::translate(glm::mat4(), glm::vec3(x, y, z));
}

void VisualObject::SetScale(float x, float y, float z){
	scale = glm::scale(glm::vec3(x,y,z));
}

void VisualObject::SetRotation(bool x, bool y, bool z, float angle){
	rotation = glm::rotate(angle, glm::vec3( x, y, z));
}

void VisualObject::ChangeRotation(bool x, bool y, bool z, float angle){
	rotation *= glm::rotate(angle, glm::vec3( x, y, z));
}

void VisualObject::SetupAnimation(bool ifStart, bool repeat, bool ifReturn, bool recalculateBoxDimm){
	animPlayback = ifStart;
	animRepeat = repeat;
	animReturn = ifReturn;
	this->recalculateBoxDimm = recalculateBoxDimm;
}

void VisualObject::AnimationProc(int curFrame, float ratio){
	//Update meshes
	for(unsigned int i = 0; i<objTemplate->frames[curFrame]->meshesTemplate.size(); i++){ //lecimy po sub objektach i dokomujemy interpolacji dla odpowiadaj¹cych sobie punktów
		meshes[i].vertices.clear();
		for(unsigned int j = 0; j<objTemplate->frames[curFrame]->meshesTemplate[i]->otVertices.size(); j++){
			glm::vec3 tempVertex;
			//cc = (c+(n-c)*ratio)
			tempVertex.x = (objTemplate->frames[curFrame]->meshesTemplate[i]->otVertices[j].x + (objTemplate->frames[curFrame+1]->meshesTemplate[i]->otVertices[j].x-objTemplate->frames[curFrame]->meshesTemplate[i]->otVertices[j].x)*ratio);
			tempVertex.y = (objTemplate->frames[curFrame]->meshesTemplate[i]->otVertices[j].y + (objTemplate->frames[curFrame+1]->meshesTemplate[i]->otVertices[j].y-objTemplate->frames[curFrame]->meshesTemplate[i]->otVertices[j].y)*ratio);
			tempVertex.z = (objTemplate->frames[curFrame]->meshesTemplate[i]->otVertices[j].z + (objTemplate->frames[curFrame+1]->meshesTemplate[i]->otVertices[j].z-objTemplate->frames[curFrame]->meshesTemplate[i]->otVertices[j].z)*ratio);
			meshes[i].vertices.push_back(tempVertex);


			if(recalculateBoxDimm){
				if(j == 0){
					meshes[i].dimm.x_max = meshes[i].dimm.x_min = tempVertex.x;
					meshes[i].dimm.y_max = meshes[i].dimm.y_min = tempVertex.y;
					meshes[i].dimm.z_max = meshes[i].dimm.z_min = tempVertex.z;
				} else {
					if(meshes[i].dimm.x_max < tempVertex.x) meshes[i].dimm.x_max = tempVertex.x;
					if(meshes[i].dimm.x_min > tempVertex.x) meshes[i].dimm.x_min = tempVertex.x;
					if(meshes[i].dimm.y_max < tempVertex.y) meshes[i].dimm.y_max = tempVertex.y;
					if(meshes[i].dimm.y_min > tempVertex.y) meshes[i].dimm.y_min = tempVertex.y;
					if(meshes[i].dimm.z_max < tempVertex.z) meshes[i].dimm.z_max = tempVertex.z;
					if(meshes[i].dimm.z_min > tempVertex.z) meshes[i].dimm.z_min = tempVertex.z;
				}
			}
		}
		if(recalculateBoxDimm){
			meshes[i].dimm.width = abs(meshes[i].dimm.x_max-meshes[i].dimm.x_min);
			meshes[i].dimm.height = abs(meshes[i].dimm.y_max-meshes[i].dimm.y_min);
			meshes[i].dimm.depth = abs(meshes[i].dimm.z_max-meshes[i].dimm.z_min);
		}
		meshes[i].UpdateVertexBuffers(objType);

		//curFrame instread of frames[0]
		meshes[i].materialId = objTemplate->frames[0]->meshesTemplate[i]->materialId;
	}
}

bool VisualObject::Animate(float deltaTime){

	if(!animPlayback or objTemplate->GetFramesAmount() < 2){
		return true;
	}

	if(deltaTime+curAnimTimeStamp > objTemplate->GetAnimationDuration()){
		curAnimTimeStamp = objTemplate->GetAnimationDuration();
	}

	//Search for actual frame
	int curFrame = 0;
	for(unsigned int i = 0; i<objTemplate->GetFramesAmount()-1; i++){
		if(objTemplate->frames[i+1]->sTime >= curAnimTimeStamp and objTemplate->frames[i]->sTime <= curAnimTimeStamp){
			curFrame = i;
			break;
		}
	}

	//Calculate frame-transition ratio
	float frameDuration = objTemplate->frames[curFrame+1]->sTime - objTemplate->frames[curFrame]->sTime;
	float normalizedTime = abs(objTemplate->frames[curFrame]->sTime - curAnimTimeStamp);
	float ratio = normalizedTime/frameDuration;

	AnimationProc(curFrame, ratio);

	if(curAnimTimeStamp == objTemplate->GetAnimationDuration()){
		curAnimTimeStamp = 0.0f;
		if(!animRepeat) animPlayback = false;
		if(animReturn){
			AnimationProc(curFrame, 0.0f);
		}
	} else {
		curAnimTimeStamp += deltaTime;
	}

	if(animPlayback == false){
		return true;
	} else {
		return false;
	}
}


void VisualObject::SetLightViewMatrix(glm::mat4 newLv){
	lv = newLv;
	otherLv = true;
}

void VisualObject::SetShader(std::string shaderName){
	params = ShaderLoader::GetProgram(shaderName);
	this->shaderName = shaderName;
}

void VisualObject::Render(){


	for(unsigned int i = 0; i<meshes.size(); i++){

		if(otherLv == false){
			model = translation * rotation * scale * glm::mat4(1.0f);
			mvp = world->dynamicContext.projection * world->dynamicContext.view * model;
		} else {
		//	model = translation  * lv * rotation * scale * glm::mat4(1.0f); // *rotation
		//	mvp = world->dynamicContext.projection * world->dynamicContext.view * (translation  * rotation * scale * glm::mat4(1.0f));
		}

		//Use "object" shader
		glUseProgram(params->at(0));

		// Send our transformation to the currently bound shader, in the "MVP" uniform
		glUniformMatrix4fv(params->at(1), 1, GL_FALSE, &mvp[0][0]);
		glUniformMatrix4fv(params->at(2), 1, GL_FALSE, &world->dynamicContext.view[0][0]);			//view for light?
		glUniformMatrix4fv(params->at(3), 1, GL_FALSE, &model[0][0]);

		//Light uniforms
		glUniform3f(params->at(5), world->light.position.x, world->light.position.y, world->light.position.z);	//Light position
		glUniform3f(params->at(7), world->light.color.r, world->light.color.g, world->light.color.b); 			//Light color
		glUniform1f(params->at(6), world->light.power); 														//Light power

		// Bind our texture in Texture Unit 0 & Texture unit 0 uniform
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, objTemplate->materials[meshes[i].materialId]->diffuseTex.texture_id);
		glUniform1i(params->at(4), 0);

		if(shaderName == "objectAdv"){

			glm::mat3 ModelView3x3Matrix = transpose(inverse(mat3(model * world->dynamicContext.view)));//glm::mat3(model);
			glUniformMatrix3fv(params->at(12), 1, GL_FALSE, &ModelView3x3Matrix[0][0]); //mv3x3

			//Pass object color props
			glUniform3f(params->at(8),
				objTemplate->materials[meshes[i].materialId]->mcd.Ka.r,
				objTemplate->materials[meshes[i].materialId]->mcd.Ka.g,
				objTemplate->materials[meshes[i].materialId]->mcd.Ka.b
			);

			glUniform3f(params->at(9),
				objTemplate->materials[meshes[i].materialId]->mcd.Kd.r,
				objTemplate->materials[meshes[i].materialId]->mcd.Kd.g,
				objTemplate->materials[meshes[i].materialId]->mcd.Kd.b
			);
			glUniform3f(params->at(10),
				objTemplate->materials[meshes[i].materialId]->mcd.Ks.r,
				objTemplate->materials[meshes[i].materialId]->mcd.Ks.g,
				objTemplate->materials[meshes[i].materialId]->mcd.Ks.b
			);

			// Bind our normal texture in Texture Unit 1
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, objTemplate->materials[meshes[i].materialId]->normalTex.texture_id);
			// Set our "NormalTextureSampler" sampler to use Texture Unit 1
			glUniform1i(params->at(11), 1);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, objTemplate->materials[meshes[i].materialId]->specularTex.texture_id);
			// Set our "NormalTextureSampler" sampler to use Texture Unit 1
			glUniform1i(params->at(13), 2);
		}

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].vertexBuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].uvBuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		 // 3rd attribute buffer : normals
		 glEnableVertexAttribArray(2);
		 glBindBuffer(GL_ARRAY_BUFFER, meshes[i].normalBuffer);
		 glVertexAttribPointer(
			 2,                                // attribute
			 3,                                // size
			 GL_FLOAT,                         // type
			 GL_FALSE,                         // normalized?
			 0,                                // stride
			 (void*)0                          // array buffer offset
		 );

		 if(shaderName == "objectAdv"){
			 // 4th attribute buffer : tangents
			 glEnableVertexAttribArray(3);
			 glBindBuffer(GL_ARRAY_BUFFER, meshes[i].tangentBuffer);
			 glVertexAttribPointer(
				 3,                                // attribute
				 3,                                // size
				 GL_FLOAT,                         // type
				 GL_FALSE,                         // normalized?
				 0,                                // stride
				 (void*)0                          // array buffer offset
			 );

			 // 5th attribute buffer : bitangents
			 glEnableVertexAttribArray(4);
			 glBindBuffer(GL_ARRAY_BUFFER, meshes[i].bitangentBuffer);
			 glVertexAttribPointer(
				 4,                                // attribute
				 3,                                // size
				 GL_FLOAT,                         // type
				 GL_FALSE,                         // normalized?
				 0,                                // stride
				 (void*)0                          // array buffer offset
			 );
		 }


		 // Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i].elementBuffer);

		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,     			 // mode
			meshes[i].indices.size(),    // count
			GL_UNSIGNED_SHORT,  		 // type
			(void*)0					 // element array buffer offset
		);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		if(shaderName == "objectAdv"){
			glDisableVertexAttribArray(3);
			glDisableVertexAttribArray(4);
		}

	}
}
