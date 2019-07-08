#include "collisiondetector.hpp"


CollisionResult::CollisionResult(){
	state = false;
	colliderObj = NULL;
	collidedObj = NULL;
	colliderMeshId = -1;
	collidedMeshId = -1;
}

CollisionResult::~CollisionResult(){
}



CollisionDetector::CollisionDetector(){
	Setup();
}

CollisionDetector::~CollisionDetector(){
}

void CollisionDetector::Setup(enum CollisionMethod method, bool singleBoxMode, bool inverse, enum ObjType exclude){
	this->method = method;
	this->singleBoxMode = singleBoxMode;
	this->inverse = inverse;
	this->exclude = exclude;
}

CollisionResult CollisionDetector::CheckCollision(PhantomPointObject *ref, std::list<Drawable *> * objs, float xOff, float yOff, float zOff){
	CollisionResult cr;
	glm::vec3 refPtBase(ref->x, ref->y, ref->z);
	refPtBase.x += xOff;
	refPtBase.y += yOff;
	refPtBase.z += zOff;

	for(std::list<Drawable *>::iterator it = objs->begin(); it!= objs->end(); it++){ //Przelec po wszystkich obiektach
		PhantomPointObject * ppo = (PhantomPointObject*)(*it);

		glm::mat4 model = ref->translation * ref->rotation * ref->scale;
		float distance = abs(glm::dot(refPtBase - vec3(ppo->x, ppo->y, ppo->z), refPtBase - vec3(ppo->x, ppo->y, ppo->z)));

		if(distance <= ppo->diameter){
			cr.state = true;
			cr.collidedObj = (*it);
			cr.colliderObj = ref;
			cr.collidedMeshId = 0;
			cr.colliderMeshId = 0;
			return cr;
		}
		continue;
	}

	return cr;
}

CollisionResult CollisionDetector::CheckCollision(VisualObject *ref, std::list<Drawable *> * objs, float xOff, float yOff, float zOff){

	//Create Variables
	CollisionResult cr;
	Translation refPtBase = GetTranslation(ref->translation);
	refPtBase.x += xOff;
	refPtBase.y += yOff;
	refPtBase.z += zOff;


	for(std::list<Drawable *>::iterator it = objs->begin(); it!= objs->end(); it++){ //Przelec po wszystkich obiektach

		if(ref == (*it)) continue;
		if((*it)->objType == exclude) continue;


		//Sphere Point
		if(method == SPHERE_POINT){
			PhantomPointObject * ppo = (PhantomPointObject*)(*it);

			glm::mat4 model = ref->translation * ref->rotation * ref->scale;
			float distance = abs(glm::dot(vec3(model[3][0],model[3][1],model[3][2]) - vec3(ppo->x, ppo->y, ppo->z), vec3(model[3][0],model[3][1],model[3][2]) - vec3(ppo->x, ppo->y, ppo->z)));

			if(distance <= ppo->diameter){
				cr.state = true;
				cr.collidedObj = (*it);
				cr.colliderObj = ref;
				cr.collidedMeshId = 0;
				cr.colliderMeshId = 0;
				return cr;
			}
			continue;
		}


		Translation voPtBase = GetTranslation((*it)->translation);
		for(unsigned int j = 0; j < (*it)->meshes.size(); j++){			//Po ich wszystkich sub-obiektach

			//Pozycja sub-obiektu porównywanego
			BoxDimm  voBox = (*it)->meshes[j].dimm;
			//glm::vec3 voPt((voBox.x_max-voBox.x_min)/2, (voBox.y_max-voBox.y_min)/2, (voBox.z_max-voBox.z_min)/2);
			//voPt += glm::vec3(voPtBase.x, voPtBase.y, voPtBase.z);

			//Add translation to box
			voBox.x_max = voBox.x_max*(*it)->scale[0][0] + voPtBase.x;
			voBox.x_min = voBox.x_min*(*it)->scale[0][0] + voPtBase.x;
			voBox.y_max = voBox.y_max*(*it)->scale[1][1] + voPtBase.y;
			voBox.y_min = voBox.y_min*(*it)->scale[1][1] + voPtBase.y;
			voBox.z_max = voBox.z_max*(*it)->scale[2][2] + voPtBase.z;
			voBox.z_min = voBox.z_min*(*it)->scale[2][2] + voPtBase.z;


			for(unsigned int x = 0; x < ref->meshes.size(); x++){
				//Pozycja sub-obiektu referenta
				glm::vec3 refPt((ref->meshes[x].dimm.x_max-ref->meshes[x].dimm.x_min)*ref->scale[0][0],
								(ref->meshes[x].dimm.y_max-ref->meshes[x].dimm.y_min)*ref->scale[1][1],
								(ref->meshes[x].dimm.z_max-ref->meshes[x].dimm.z_min)*ref->scale[2][2]);
				refPt = glm::vec3(refPtBase.x, refPtBase.y, refPtBase.z);


			//	std::cout<<"Ship: "<<refPt.x<<"  "<<refPt.y<<"  "<<refPt.z<<std::endl;
			//	std::cout<<"Ast : "<<voBox.x_min<<"  "<<voBox.x_max<<"  "<<voBox.y_min<<"  "<<voBox.y_max<<"  "<<voBox.z_min<<"  "<<voBox.z_max<<std::endl;

				//------------------------------------
				//METHODS
				//------------------------------------

				//Point AABB
				if(method == AABB_POINT){
					if((refPt.x >= voBox.x_min && refPt.x <= voBox.x_max) &&
					   (refPt.y >= voBox.y_min && refPt.y <= voBox.y_max) &&
					   (refPt.z >= voBox.z_min && refPt.z <= voBox.z_max)){
							cr.state = true;
							cr.collidedObj = (*it);
							cr.colliderObj = ref;
							cr.collidedMeshId = j;
							cr.colliderMeshId = x;
							return cr;
					}

				//Box AABB
				} else if(method == AABB_BOX){

					BoxDimm refBox = ref->meshes[x].dimm;

					//Add translation to box
					refBox.x_max = refBox.x_max*ref->scale[0][0] + refPtBase.x;
					refBox.x_min = refBox.x_min*ref->scale[0][0] + refPtBase.x;
					refBox.y_max = refBox.y_max*ref->scale[1][1] + refPtBase.y;
					refBox.y_min = refBox.y_min*ref->scale[1][1] + refPtBase.y;
					refBox.z_max = refBox.z_max*ref->scale[2][2] + refPtBase.z;
					refBox.z_min = refBox.z_min*ref->scale[2][2] + refPtBase.z;

					if((voBox.x_min <= refBox.x_min && voBox.x_max >= refBox.x_max) &&
					   (voBox.y_min <= refBox.y_min && voBox.y_max >= refBox.y_max) &&
					   (voBox.z_min <= refBox.z_min && voBox.z_max >= refBox.z_max)) {
							cr.state = true;
							cr.collidedObj = (*it);
							cr.colliderObj = ref;
							cr.collidedMeshId = j;
							cr.colliderMeshId = x;
							return cr;
					}
				}
				//------------------------------------
			}

		}


	}
	return cr;
}


/*
 CollisionResult CollisionDetector::CheckCollision(VisualObject *ref, std::list<VisualObject *> * objs, float xOff, float yOff, float zOff){

	//Create Variables
	CollisionResult cr;
	Translation pt = GetTranslation(ref->translation);
	BoxDimm refBox;

	BoxDimm  refBox = ref->boxdimm[0];
	/*refBox.x_max += pt.x + xOff;
	refBox.x_min += pt.x + xOff;
	refBox.y_max += pt.y + yOff;
	refBox.y_min += pt.y + yOff;
	refBox.z_max += pt.z + zOff;
	refBox.z_min += pt.z + zOff;
	 */

/*	if(insideCheck == true){
		pt.y += yOff;
	} else {
	//	pt.y += yOff*deltaTime;
	}
	//pt.x += xOff*deltaTime;
	//pt.z += zOff*deltaTime;

	//for(unsigned int i = 0; i<objs->size(); i++){
	for(std::list<Object *>::iterator it = objs->begin(); it!= objs->end(); it++){

		if(ref == (*it)->obj) continue;
		if((*it)->objType == exclude) continue;

		//std::cout<<(*it)->obj<<std::endl;

		for(unsigned int j = 0; j < (*it)->obj->boxdimm.size(); j++){//
			BoxDimm  box = (*it)->obj->boxdimm[j];
			PureTranslation ptt = GetTranslation((*it)->obj->translation);

			if(insideCheck == false){
				box.x_max += ptt.x + refBox.width/2;
				box.x_min += ptt.x - refBox.width/2;
				box.y_max += ptt.y + refBox.height/2;
				box.y_min += ptt.y - refBox.height/2;
				box.z_max += ptt.z + refBox.depth/2;
				box.z_min += ptt.z - refBox.depth/2;
			} else {
				box.x_max += ptt.x;
				box.x_min += ptt.x;
				box.y_max += ptt.y;
				box.y_min += ptt.y;
				box.z_max += ptt.z;
				box.z_min += ptt.z;
			}
			//AABB collision
			//if((box.x_min <= refBox.x_min && box.x_max >= refBox.x_max) &&
			//   (box.y_min <= refBox.y_min && box.y_max >= refBox.y_max) &&
			//   (box.z_min <= refBox.z_min && box.z_max >= refBox.z_max)) {


			//Point AABB collision
			if((pt.x >= box.x_min && pt.x <= box.x_max) &&
			   (pt.y >= box.y_min && pt.y <= box.y_max) &&
			   (pt.z >= box.z_min && pt.z <= box.z_max)){


				cr.state = true;
				cr.collidedObj = (*it);
				cr.boxNumber = j;
				return cr;
			}
		}


	}
	return cr;
}
 */








