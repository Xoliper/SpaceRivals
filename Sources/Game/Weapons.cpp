/*
 * Weapons.cpp
 *
 *  Created on: 1 wrz 2018
 *      Author: Xoliper
 */

#include "Weapons.hpp"
#include "Player.hpp"

//-----------------------------------------------------------------------
//ADV WPN
//-----------------------------------------------------------------------

AdvancedWpn::AdvancedWpn(World * world, Player * player, ObjTemplate * rocketToShip, ObjTemplate * rocketTemplate, ObjTemplate * thrusterTemplate, Timer * timer, float reload){
	this->world = world;
	this->timer = timer;
	this->rocketTemplate = rocketTemplate;
	this->thrusterTemplate = thrusterTemplate;
	this->reload = reload;
	this->actualReload = 0.0f;
	this->player = player;

	attachedRockets = new VisualObject(world, rocketToShip);
	attachedRockets->SetScale(0.009, 0.01, 0.01);
	attachedRockets->SetRotation(1,0,0,90);
}

AdvancedWpn::~AdvancedWpn(){
	for(auto it = rocketsList.begin(); it!=rocketsList.end(); it++){
		delete (*it);
	}
}

void AdvancedWpn::Render(){
	attachedRockets->Render();
	for(auto it = rocketsList.begin(); it!=rocketsList.end(); it++){
		(*it)->Render();
	}
	glDepthMask(false);
	for(auto it = explosionsList.begin(); it!=explosionsList.end(); it++){
		(*it)->Render();
	}
	glDepthMask(true);
}

void AdvancedWpn::RenderToBlur(){
	for(auto it = rocketsList.begin(); it!=rocketsList.end(); it++){
		(*it)->RenderToBlur();
	}
}


void AdvancedWpn::Add( glm::vec3 pos, glm::vec3 addPos, glm::quat rotRa, glm::mat4 v){
	if(actualReload > reload){
		rocketsList.push_back(new Rocket(world, rocketTemplate, thrusterTemplate, pos, addPos, rotRa, v));
		rocketsList.back()->Work(nullptr, nullptr);
		actualReload = 0.0f;
	}
}

void AdvancedWpn::Work(glm::vec3 pos, glm::vec3 addPos, glm::quat rot, glm::quat addRot, CollisionDetector * cd, Map * map){
	//Update Reload time
	actualReload += timer->GetDelta();

	//Update attachments position
	attachedRockets->translation = glm::translate(pos);
	attachedRockets->rotation = glm::mat4_cast(rot) * glm::translate(addPos)  * glm::mat4_cast(addRot);

	//Update Rockets position
	for(auto it = rocketsList.begin(); it!=rocketsList.end(); it++){
		(*it)->Work(cd, map);

		if((*it)->distance > 7.0f or (*it)->hit == true){
			int texId1 = ( std::rand() % 4 ) + 1;
			int texId2 = ( std::rand() % 4 ) + 1;

			//Add animations
			Sprite * explosionSprite;
			explosionSprite = new Sprite(world, {0,0,256,256}, rc->GetTexture("EXPLOSION"+std::to_string(texId1)), 0);
			explosionSprite->SetupAnimation(256,256,64,2.0f);
			explosionSprite->SetMode(true);
			explosionSprite->SetScale(0.8f, 0.8f, 1.0f);
			explosionSprite->SetPosition((*it)->rocket1Mat[3][0], (*it)->rocket1Mat[3][1], (*it)->rocket1Mat[3][2]);
			explosionsList.push_back(explosionSprite);

			explosionSprite = new Sprite(world, {0,0,256,256}, rc->GetTexture("EXPLOSION"+std::to_string(texId2)), 0);
			explosionSprite->SetupAnimation(256,256,64,2.0f);
			explosionSprite->SetMode(true);
			explosionSprite->SetScale(0.8f, 0.8f, 1.0f);
			explosionSprite->SetPosition((*it)->rocket2Mat[3][0], (*it)->rocket2Mat[3][1], (*it)->rocket2Mat[3][2]);
			explosionsList.push_back(explosionSprite);

			//Delete rockets
			std::list<Rocket*>::iterator itBack= it;
			std::advance(itBack, -1);
			delete (*it);
			rocketsList.erase(it);
			it = itBack;
		}
	}

	//Update explosion animation
	for(auto it = explosionsList.begin(); it != explosionsList.end(); it++){
		bool state = (*it)->Animate(timer);
		if(state == true){
			//Delete sprite
			std::list<Sprite*>::iterator itBack = it;
			std::advance(itBack, -1);
			delete (*it);
			explosionsList.erase(it);
			it = itBack;
			continue;
		} else {
			//Update rotation
			glm::vec3 t = player->ship->GetShipTranslation();
			(*it)->RotateToOrigin(t.x, t.y, t.z,player->upDirection);
		}
	}

}


//-----------------------------------------------------------------------
//STD WPN
//-----------------------------------------------------------------------

StandardWpn::StandardWpn(World * world, Player * player, ObjTemplate * laserTemplate, Timer * timer, float reload){
	this->world = world;
	this->timer = timer;
	this->laserTemplate = laserTemplate;
	this->reload = reload;
	this->actualReload = 0.0f;
	this->player = player;
}

StandardWpn::~StandardWpn(){
	for(auto it = lasersList.begin(); it!=lasersList.end(); it++){
		delete (*it);
	}
}

void StandardWpn::Render(){
	for(auto it = lasersList.begin(); it!=lasersList.end(); it++){
		(*it)->Render();
	}
	glDepthMask(false);
	for(auto it = lImpactList.begin(); it!=lImpactList.end(); it++){
		(*it)->Render();
	}
	glDepthMask(true);
}

void StandardWpn::RenderToBlur(){
	for(auto it = lasersList.begin(); it!=lasersList.end(); it++){
		(*it)->Render();
	}
}


void StandardWpn::Add( glm::vec3 pos, glm::vec3 addPos, glm::quat rotRa, glm::mat4 v){
	if(actualReload > reload){
		lasersList.push_back(new Laser(world, laserTemplate, pos, addPos, rotRa, v));
		lasersList.back()->Work(nullptr, nullptr);
		actualReload = 0.0f;
	}
}

void StandardWpn::Work(CollisionDetector * cd, Map * map){
	//Update Reload time
	actualReload += timer->GetDelta();

	//Update Rockets position
	for(auto it = lasersList.begin(); it != lasersList.end(); it++){
		(*it)->Work(cd, map);
		if((*it)->distance > 20.0f or (*it)->hit == true){

			//Add animations
			Sprite * explosionSprite;
			explosionSprite = new Sprite(world, {0,0,64,64}, rc->GetTexture("EXPLOSION1"), 0);
			explosionSprite->SetupAnimation(256,256,64,2.0f);
			explosionSprite->SetMode(true);
			explosionSprite->SetScale(0.4f, 0.4f, 1.0f);
			explosionSprite->SetPosition((*it)->laser1Mat[3][0], (*it)->laser1Mat[3][1], (*it)->laser1Mat[3][2]);
			lImpactList.push_back(explosionSprite);

			explosionSprite = new Sprite(world, {0,0,64,64}, rc->GetTexture("EXPLOSION1"), 0);
			explosionSprite->SetupAnimation(256,256,64,2.0f);
			explosionSprite->SetMode(true);
			explosionSprite->SetScale(0.4f, 0.4f, 1.0f);
			explosionSprite->SetPosition((*it)->laser2Mat[3][0], (*it)->laser2Mat[3][1], (*it)->laser2Mat[3][2]);
			lImpactList.push_back(explosionSprite);

			//Delete laser particles
			std::list<Laser*>::iterator itBack= it;
			std::advance(itBack, -1);
			delete (*it);
			lasersList.erase(it);
			it = itBack;
		}
	}

	//Update impact animation
	for(auto it = lImpactList.begin(); it != lImpactList.end(); it++){
		bool state = (*it)->Animate(timer);
		if(state == true){
			//Delete sprite
			std::list<Sprite*>::iterator itBack = it;
			std::advance(itBack, -1);
			delete (*it);
			lImpactList.erase(it);
			it = itBack;
			continue;
		} else {
			//Update rotation
			glm::vec3 t = player->ship->GetShipTranslation();
			(*it)->RotateToOrigin(t.x, t.y, t.z,player->upDirection);
		}
	}
}

