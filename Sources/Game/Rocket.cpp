/*
 * Rocket.cpp
 *
 *  Created on: 30 sie 2018
 *      Author: Xoliper
 */

#include "Rocket.hpp"

Rocket::Rocket(World * w, ObjTemplate * rocketTemplate, ObjTemplate * thrusterTemplate, glm::vec3 pos, glm::vec3 addPos, glm::quat rotRa, glm::mat4 v){
	this->rocketTemplate = rocketTemplate;
	this->thrusterTemplate = thrusterTemplate;
	this->v = v;

	rocketVo1 = new VisualObject(w, rocketTemplate);
	rocketVo1->SetScale(0.01, 0.01, 0.01);
	//rocketVo1->SetShader("object");

	rocketVo2 = new VisualObject(w, rocketTemplate);
	rocketVo2->SetScale(0.01, 0.01, 0.01);
	//rocketVo2->SetShader("object");

	thrusterVo1 = new VisualObject(w, thrusterTemplate);
	thrusterVo1->SetScale(0.01, 0.01, 0.01);

	thrusterVo2 = new VisualObject(w, thrusterTemplate);
	thrusterVo2->SetScale(0.01, 0.01, 0.01);

	this->pos = pos;
	this->posAdd = addPos;
	this->rotRa = rotRa;
	distance = 0.0f;
	hit = false;
}

Rocket::~Rocket(){
	delete rocketVo1;
	delete rocketVo2;
	delete thrusterVo1;
	delete thrusterVo2;
}

void Rocket::Work(CollisionDetector * cd, Map * map){
	glm::vec3 test = glm::vec3(-0.0707974, -0.0430439, -0.0396656);
	glm::vec3 test2 = glm::vec3(0.0707974, -0.0430439, -0.0396656);

	rocketVo1->rotation = glm::mat4_cast(conjugate(toQuat(v)))*glm::translate(posAdd+test*glm::inverse(rotRa));
	rocketVo1->translation = glm::translate(pos + vec3(0.0f,0.0f,-distance) * glm::quat(v));///glm::mat4(1.0f);//glm::translate(vec3(0.0f,distance,0.0f)*toQuat(v));//glm::translate((posAdd+test)*conjugate(toQuat(v)));//glm::translate(glm::inverse(v)[3][0], glm::inverse(v)[3][1], glm::inverse(v)[3][2]);

	rocketVo2->rotation = glm::mat4_cast(conjugate(toQuat(v)))*glm::translate(posAdd+test2*glm::inverse(rotRa));
	rocketVo2->translation = glm::translate(pos +  vec3(0.0f,0.0f,-distance) * glm::quat(v));///glm::mat4(1.0f);//glm::translate(vec3(0.0f,distance,0.0f)*toQuat(v));//glm::translate((posAdd+test)*conjugate(toQuat(v)));//glm::translate(glm::inverse(v)[3][0], glm::inverse(v)[3][1], glm::inverse(v)[3][2]);

	thrusterVo1->rotation = glm::mat4_cast(conjugate(toQuat(v)))*glm::translate(posAdd+test*glm::inverse(rotRa));
	thrusterVo1->translation = glm::translate(pos +  vec3(0.0f,0.0f,-distance) * glm::quat(v));///glm::mat4(1.0f);//glm::translate(vec3(0.0f,distance,0.0f)*toQuat(v));//glm::translate((posAdd+test)*conjugate(toQuat(v)));//glm::translate(glm::inverse(v)[3][0], glm::inverse(v)[3][1], glm::inverse(v)[3][2]);

	thrusterVo2->rotation = glm::mat4_cast(conjugate(toQuat(v)))*glm::translate(posAdd+test2*glm::inverse(rotRa));
	thrusterVo2->translation = glm::translate(pos + vec3(0.0f,0.0f,-distance) * glm::quat(v));///glm::mat4(1.0f);//glm::translate(vec3(0.0f,distance,0.0f)*toQuat(v));//glm::translate((posAdd+test)*conjugate(toQuat(v)));//glm::translate(glm::inverse(v)[3][0], glm::inverse(v)[3][1], glm::inverse(v)[3][2]);


	rocket1Mat = rocketVo1->translation * rocketVo1->rotation;
	rocket2Mat = rocketVo2->translation * rocketVo2->rotation;

	float r3 = 0.9 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.1-0.9)));
	thrusterVo1->SetScale(1.0f*0.01f, 1.0f*0.01f, r3*0.01f);
	thrusterVo2->SetScale(1.0f*0.01f, 1.0f*0.01f, r3*0.01f);

	if(cd != nullptr or map != nullptr){
		CollisionResult cr;
		ppo.x = rocket1Mat[3][0];
		ppo.y = rocket1Mat[3][1];
		ppo.z = rocket1Mat[3][2];
		cr = cd->CheckCollision(&ppo, map->GetCollisionObjects());
		if(cr.state == true) hit = true;
		ppo.x = rocket2Mat[3][0];
		ppo.y = rocket2Mat[3][1];
		ppo.z = rocket2Mat[3][2];
		cr = cd->CheckCollision(&ppo, map->GetCollisionObjects());
		if(cr.state == true) hit = true;
	}

	distance += 0.05;
}


void Rocket::Render(){
	rocketVo1->Render();
	rocketVo2->Render();
}

void Rocket::RenderToBlur(){
	thrusterVo1->Render();
	thrusterVo2->Render();
}
