/*
 * Engine.cpp
 *
 *  Created on: 1 wrz 2018
 *      Author: Xoliper
 */

#include "Engine.hpp"

Engine::Engine(World * world, ObjTemplate * engineObjTemplate){
	thruster = new VisualObject(world, engineObjTemplate);
	thruster->SetScale(0.009f, 0.009f, 0.009f);
	thruster->ChangeRotation(0,0,1,90.0f);


	booster = false;
	boosterSkip = 0;

	horizontalSpeed = 0.14f;
	turnSpeed = 0.8f;
	curSpeed = 0.0f;
	maxSpeed = 0.02f;
	minSpeed = 0.002f;
}

Engine::~Engine(){
}

float Engine::Work(){
	//Calculate change
	float zPeakTrans = 0.0f;
	if(boosterSkip > 0){
		--boosterSkip;
	} else if(boosterSkip == 0){
		booster = false;
		zPeakTrans=0.0;
	}
	if(GetAsyncKeyState(VK_SPACE)){
		if(booster == false){
			booster = true;
			boosterSkip = 1;
			zPeakTrans=-0.075;
			//vo->SetupAnimation(true, false, true, false);
		}
		if(curSpeed < maxSpeed*3){
			curSpeed += 0.0095;
		}
		if(curSpeed > maxSpeed*2) curSpeed = maxSpeed*2;
	} else if(GetAsyncKeyState(0x57)){//Speed upp
		if(curSpeed < maxSpeed){
			curSpeed += 0.0015;
		}
		if(curSpeed > maxSpeed) curSpeed = maxSpeed;
	} else if(GetAsyncKeyState(0x53)){
		if(curSpeed > minSpeed){
			curSpeed -= 0.0015;
		}
		if(curSpeed < minSpeed) curSpeed = minSpeed;
	} else {
		if(curSpeed > ((maxSpeed+minSpeed)/2)) curSpeed -= 0.0015;
		if(curSpeed < ((maxSpeed+minSpeed)/2)) curSpeed += 0.0015;
	}

	//Thruster animation
	float r3 = 0.95 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.05-0.95)));
	if(booster == true){
		thruster->SetScale(1.0f*0.009f, 1.0f*0.009f, r3*0.011f);
	} else {
		thruster->SetScale(1.0f*0.009f, 1.0f*0.009f, r3*0.0094f);
	}

	return zPeakTrans;
}

void Engine::UpdateVisuals(glm::vec3 pos, glm::vec3 addPos, glm::quat rot, glm::quat addRot){
	//Translation + rotation
	thruster->translation = glm::translate(pos);
	thruster->rotation = glm::mat4_cast(rot) * glm::translate(addPos)  * glm::mat4_cast(addRot);
}

void Engine::Render(){
	thruster->Render();
}

float Engine::GetCurSpeed(){
	return curSpeed;
}

float Engine::GetTurnSpeed(){
	return turnSpeed;
}

float Engine::GetHorizontalSpeed(){
	return horizontalSpeed;
}
