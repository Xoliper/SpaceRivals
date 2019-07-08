/*
 * collisiondetector.hpp
 *
 *  Created on: 8 paü 2017
 *      Author: Arkadiusz Jarlaczyk
 */

#ifndef ENGINE_COMMON_COLLISIONDETECTOR_HPP_
#define ENGINE_COMMON_COLLISIONDETECTOR_HPP_

#include "objloader.hpp"
#include "../Objects/Object.hpp"
#include <list>

class CollisionResult {
public:
	CollisionResult();
	virtual ~CollisionResult();

	bool state;
	Drawable * colliderObj;
	Drawable * collidedObj;
	int colliderMeshId;
	int collidedMeshId;

	//angle results here??

};

enum CollisionMethod {
	AABB_BOX,
	AABB_POINT,
	SPHERE_POINT
};

class CollisionDetector {
public:
	CollisionDetector();
	virtual ~CollisionDetector();

	void Setup(CollisionMethod method = AABB_POINT, bool singleBoxMode = true, bool inverse = false, ObjType exclude = DK);
	CollisionResult CheckCollision(VisualObject * ref, std::list<Drawable *> * objs, float xOff = 0.0f, float yOff = 0.0f, float zOff = 0.0f);
	CollisionResult CheckCollision(PhantomPointObject * ref, std::list<Drawable *> * objs, float xOff = 0.0f, float yOff = 0.0f, float zOff = 0.0f);

private:
	CollisionMethod method;		//OK
	bool singleBoxMode;			//Not supported YET!
	bool inverse;
	ObjType exclude;			//OK

};


#endif /* ENGINE_COMMON_COLLISIONDETECTOR_HPP_ */
