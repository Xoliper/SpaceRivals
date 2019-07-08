#include "Laser.hpp"

Laser::Laser(World * w, ObjTemplate * laserTemplate, glm::vec3 pos, glm::vec3 addPos, glm::quat rotRa, glm::mat4 v){
	this->laserTemplate = laserTemplate;
	this->v = v;

	laserParticle0 = new VisualObject(w, laserTemplate);
	laserParticle0->SetScale(0.01, 0.01, 0.01);
	laserParticle0->SetShader("object");

	laserParticle1 = new VisualObject(w, laserTemplate);
	laserParticle1->SetScale(0.01, 0.01, 0.01);
	laserParticle1->SetShader("object");

	this->pos = pos;
	this->posAdd = addPos;
	this->rotRa = rotRa;
	distance = 0.0f;
	hit = false;

}

Laser::~Laser(){
	delete laserParticle0;
	delete laserParticle1;
}

void Laser::Work(CollisionDetector * cd, Map * map){
	glm::vec3 test = glm::vec3(-0.0257974, -0.0230439, -0.0396656);
	glm::vec3 test2 = glm::vec3(0.0257974, -0.0230439, -0.0396656);

	laserParticle0->rotation = glm::mat4_cast(conjugate(toQuat(v)))*glm::translate(posAdd+test*glm::inverse(rotRa));
	laserParticle0->translation = glm::translate(pos +  vec3(0.0f,0.0f,-distance) * glm::quat(v));///glm::mat4(1.0f);//glm::translate(vec3(0.0f,distance,0.0f)*toQuat(v));//glm::translate((posAdd+test)*conjugate(toQuat(v)));//glm::translate(glm::inverse(v)[3][0], glm::inverse(v)[3][1], glm::inverse(v)[3][2]);

	laserParticle1->rotation = glm::mat4_cast(conjugate(toQuat(v)))*glm::translate(posAdd+test2*glm::inverse(rotRa));
	laserParticle1->translation = glm::translate(pos +   vec3(0.0f,0.0f,-distance) * glm::quat(v));///glm::mat4(1.0f);//glm::translate(vec3(0.0f,distance,0.0f)*toQuat(v));//glm::translate((posAdd+test)*conjugate(toQuat(v)));//glm::translate(glm::inverse(v)[3][0], glm::inverse(v)[3][1], glm::inverse(v)[3][2]);

	distance += 0.23;

	laser1Mat = laserParticle0->translation * laserParticle0->rotation;
	laser2Mat = laserParticle1->translation * laserParticle1->rotation;

	if(cd != nullptr or map != nullptr){
		CollisionResult cr;
		ppo.x = laser1Mat[3][0];
		ppo.y = laser1Mat[3][1];
		ppo.z = laser1Mat[3][2];
		cr = cd->CheckCollision(&ppo, map->GetCollisionObjects());
		if(cr.state == true) hit = true;
		ppo.x = laser2Mat[3][0];
		ppo.y = laser2Mat[3][1];
		ppo.z = laser2Mat[3][2];
		cr = cd->CheckCollision(&ppo, map->GetCollisionObjects());
		if(cr.state == true) hit = true;
	}
}


void Laser::Render(){
	laserParticle0->Render();
	laserParticle1->Render();
}

