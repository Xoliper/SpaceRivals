/*
 * Player.cpp
 *
 *  Created on: 10 lip 2018
 *      Author: Xoliper
 */

#include "Player.hpp"

Player::Player(World * world, Timer * timer, ObjTemplate * obj, ObjTemplate * obj2){
	this->globalWorld = world;
	this->timer = timer;

	ship = new Ship(globalWorld, obj);
	engine = new Engine(globalWorld, obj2);
	advWpn = nullptr;
	stdWpn = nullptr;


	hPeakTrans = 0.0f;
	vPeakTrans = 0.0f;
	zPeakTrans = 0.0f;
	slerpTime = 0.0f;

	bounceX = false;
	bounceZ = false;
	bounceAngle = 0.0f;




	bounceSkip = 0;
	bounceBonus = 1.0f;

	toRot = angleAxis(float(0), 0.0f, 0.0f, 1.0f);
	prevRot = toRot;
	glm::normalize(toRot);
	glm::normalize(prevRot);

	upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	camPosition = glm::vec3(0,0,0);

}

Player::~Player(){
}

void Player::EquipAdwWpn(ObjTemplate * rocketToShip, ObjTemplate * obj, ObjTemplate * obj2, float reload){
	if(advWpn != nullptr) delete advWpn;
	advWpn = new AdvancedWpn(globalWorld, this, rocketToShip, obj, obj2, timer, reload);

}

void Player::EquipStdWpn(ObjTemplate * obj, float reload){
	if(stdWpn != nullptr) delete stdWpn;
	stdWpn = new StandardWpn(globalWorld, this, obj, timer, reload);
}

void Player::FixMouse(int x, int y){
	fixX = x;
	fixY = y;
}

Force Player::GetForce(){
	Force f;
	POINT p;
	if(GetCursorPos(&p)){
		f.y = -(((float)(p.y-fixY)/(globalWorld->height/2))-1.0f);
		f.x = -(((float)(p.x-fixX)/(globalWorld->width/2))-1.0f);
	}
	return f;
}



void Player::UpdateLocRot(Force & force, bool collision){

	//Reset time
	slerpTime = 0.0f;

	//Save rotation data
	prevRot = toRot;
	oldPos = toPos; //

	//Calculate new data
	toPos = toPos + vec3(engine->GetHorizontalSpeed()*hPeakTrans, engine->GetCurSpeed()*bounceBonus, 0.0f) * rotAngle;
	glm::quat qx = angleAxis(-force.x*engine->GetTurnSpeed(), vec3(0.0f, 0.0f, 1.0f) * (rotAngle)) ;
	glm::quat qy = angleAxis(-force.y*engine->GetTurnSpeed(), vec3(1.0f, 0.0f, 0.0f) * (rotAngle));
	glm::normalize(qx);
	glm::normalize(qy);

	//Bounce calc
	glm::quat bounce;
	if(collision and bounceSkip == 0){
		bounceSkip = 60;
		bounceBonus = 7.0f;
		glm::mat4 model = glm::mat4_cast(prevRot);
		glm::vec3 dir = glm::normalize(glm::vec3(model[1]));

		glm::vec3 dirIndexs(0.0f, 0.0f, 0.0f);
		glm::vec3 absDir(abs(dir[0]), abs(dir[1]), abs(dir[2]));
		float max = std::max(std::max(absDir.x, absDir.y), absDir.z);
		if(absDir.x == max) dir.x > 0 ? dirIndexs.x = ceil(dir.x) : dirIndexs.x = floor(dir.x);
		if(absDir.y == max) dir.y > 0 ? dirIndexs.y = ceil(dir.y) : dirIndexs.y = floor(dir.y);
		if(absDir.z == max) dir.z > 0 ? dirIndexs.z = ceil(dir.z) : dirIndexs.z = floor(dir.z);

		//Horizontal
		glm::vec3 perpendic;
		if(dirIndexs.y != 0.0f) perpendic = vec3(1,0,0);
		if(dirIndexs.x != 0.0f) perpendic = vec3(0,1,0);
		if(dirIndexs.z != 0.0f) perpendic = vec3(0,1,0);
		std::cerr<<"Dir: ("<<dirIndexs.x<<", "<<dirIndexs.y<<", "<<dirIndexs.z<<") "<<std::endl;

		//Tests
		float angle = glm::dot(dir, perpendic);
		angle = (2*glm::degrees(acos(angle)));
		if(angle > 180.0f) angle = (angle-360.0f);
		angle = -angle;

		if(dirIndexs.y < 0 or dirIndexs.z < 0) angle = -angle;
		if(dirIndexs.x > 0) angle = -(360-angle);

		if(dirIndexs.y != 0.0f or dirIndexs.x != 0.0f){
			bounce = angleAxis(360.0f + angle, vec3(0.0f, 0.0f, 1.0f) );
			bounceZ = true;
		} else if(dirIndexs.z != 0.0f){
			bounce = angleAxis(angle, vec3(1.0f, 0.0f, 0.0f));
			bounceX = true;
		}
		bounceAngle = angle;
		bounce = angleAxis(bounceAngle, vec3(0.0f, bounceX, bounceZ) * (rotAngle)) ;
	} else {
		if(bounceSkip > 0) --bounceSkip;
		bounceBonus = 1.0f;
	}
	glm::normalize(bounce);


	//Finalize update
	rotAngle = rotAngle * qx * qy * bounce;
	toRot = rotAngle;
	glm::normalize(toRot);

}


void Player::UpdateCameraBehaviour(Force & force){
	float vPeakTransTemp = sqrt(force.y*force.y + force.x*force.x)/20;
	if(force.y > 0) vPeakTransTemp = -vPeakTransTemp;
	if(vPeakTransTemp > 0){
		vPeakTrans += 0.0008*22;
		if(vPeakTrans > 0.04*abs(force.y)) vPeakTrans = 0.04*abs(force.y);
	} else if(vPeakTransTemp < 0){
		vPeakTrans -= 0.0008*22;
		if(vPeakTrans < -0.04*abs(force.y)) vPeakTrans = -0.04*abs(force.y);
	}

	if(GetAsyncKeyState(0x44)){
		if(hPeakTrans < 0.07f){
			hPeakTrans += 0.0015f*6;
		}
		if(hPeakTrans > 0.07f) hPeakTrans = 0.07f;
	} else if(GetAsyncKeyState(0x41)){
		if(hPeakTrans > -0.07f){
			hPeakTrans -= 0.0015f*6;
		}
		if(hPeakTrans < -0.07f) hPeakTrans = -0.07f;
	} else {
		if(hPeakTrans > 0){
			hPeakTrans-=0.0015f*6;
			if(hPeakTrans < 0) hPeakTrans = 0;
		} else if(hPeakTrans < 0){
			hPeakTrans+=0.0015f*6;
			if(hPeakTrans > 0) hPeakTrans = 0;
		}
	}
}




void Player::Update(bool ifUpdate, Map * map){


	Force force = GetForce();
	UpdateCameraBehaviour(force);

	cd.Setup(SPHERE_POINT, false, false, DK);
	CollisionResult cr = cd.CheckCollision(ship->shipVo, map->GetCollisionObjects(), 0.0f, 0.0f, 0.0f);
	if(ifUpdate){
		UpdateLocRot(force, cr.state);
	}
	zPeakTrans = engine->Work();

	//Interpolate position + rotation
	slerpTime += timer->GetDelta();
	float ratio = 1.0f;//slerpTime/((float)1/60);

	glm::quat rot = glm::slerp(prevRot,toRot, ratio);
	glm::vec3 pos = glm::mix(oldPos, toPos, ratio);

	//Change camera
	glm::mat4 tview = glm::lookAt (
			ship->GetShipCameraOffset()*rot+ship->GetShipTranslation(),
			ship->GetShipTranslation()+vec3(0.0,10.0,0.0)*rot,
			vec3(0,1,0)*rot
	);
	globalWorld->dynamicContext.view = tview;

	//Get ship rotation
	glm::quat mr = angleAxis(90.0f, vec3(1.0f, 0.0f, 0.0f));
	glm::quat ya = angleAxis(force.y*12*2, vec3(1.0f, 0.0f, 0.0f) );
	glm::quat xa = angleAxis(force.x*12*2, vec3(0.0f, 0.0f, 1.0f) );
	float rForce = 0.0f;
	if(force.y != 0 and force.x != 0) {
		rForce = atan (force.x/force.y) * 180 / PI;
		ra = angleAxis(-rForce*sqrt(force.y*force.y + force.x*force.x), vec3(0.0f, 0.0f, 1.0f) );
		if(force.y > 0) ra = glm::inverse(ra);
	}

	addPos = glm::vec3(hPeakTrans, vPeakTrans, 0.0f);
	addRotRa = ra;

	if(ifUpdate){
		engine->UpdateVisuals(pos, addPos+glm::vec3(0.0f, 0.0f, zPeakTrans), glm::inverse(rot) * xa * ya * mr, ra);
		ship->UpdateVisuals(pos, addPos, glm::inverse(rot) * xa * ya * mr, ra);

		if(advWpn != nullptr){
			advWpn->Work(pos, addPos+glm::vec3(0.0f, 0.0f, zPeakTrans), glm::inverse(rot) * xa * ya * mr, ra, &cd, map);
			if(GetAsyncKeyState(VK_RBUTTON)){
				advWpn->Add(pos, addPos, ra, tview);
			}
		}

		if(stdWpn != nullptr){
			stdWpn->Work(&cd, map);
			if(GetAsyncKeyState(VK_LBUTTON)){
				stdWpn->Add(pos, addPos, ra, tview);
			}
		}
	}

}



void Player::Render(){
	ship->Render();
	if(advWpn != nullptr) advWpn->Render();
	if(stdWpn != nullptr) stdWpn->Render();
}

void Player::RenderToBlur(){
	engine->Render();
	if(advWpn != nullptr) advWpn->RenderToBlur();
	//if(stdWpn != nullptr) stdWpn->Render();
}
