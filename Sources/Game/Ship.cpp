/*
 * Ship.cpp
 *
 *  Created on: 1 wrz 2018
 *      Author: Xoliper
 */

#include "Ship.hpp"

Ship::Ship(World * world, ObjTemplate * shipObjTemplate){
	this->world = world;
	this->shipVo = new VisualObject(world, shipObjTemplate);
	this->shipVo->SetScale(0.009f, 0.009f, 0.009f);
	this->shipVo->ChangeRotation(0,0,1,90.0f);

	offset = vec3(0.0f, -0.7f, 0.07f);
}
glm::vec3 Ship::GetShipTranslation(){
	return glm::vec3(shipVo->translation[3][0],shipVo->translation[3][1],shipVo->translation[3][2]);
}

glm::vec3 Ship::GetShipCameraOffset(){
	return offset;
}

void Ship::UpdateVisuals(glm::vec3 pos, glm::vec3 addPos, glm::quat rot, glm::quat addRot){
	//vo->translation = glm::translate(pos);
	//vo->rotation = glm::mat4_cast(glm::inverse(rot) * xa * ya * mr) * glm::translate(hPeakTrans, vPeakTrans, zPeakTrans) * glm::mat4_cast(ra);
	shipVo->translation = glm::translate(pos);
	shipVo->rotation = glm::mat4_cast(rot) * glm::translate(addPos) * glm::mat4_cast(addRot);
}

void Ship::Render(){
	shipVo->Render();
}
