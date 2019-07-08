#ifndef ENGINE_COMMON_OBJECTS_FRAME_HPP_
#define ENGINE_COMMON_OBJECTS_FRAME_HPP_

#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include "../Drawable.hpp"

class SubObjTemplate {
public:
	SubObjTemplate();
	virtual ~SubObjTemplate();

	std::vector<glm::vec3> otVertices;
	std::vector<glm::vec2> otUvs;
	std::vector<glm::vec3> otNormals;
	std::vector<unsigned short> otIndices;
	std::vector<glm::vec3> otTangents;
	std::vector<glm::vec3> otBitangents;
	std::string name;
	BoxDimm dimm;

	std::string materialName;
	int materialId;
};

class Frame {
public:
	Frame();
	/*Frame(const Frame & obj){
		frameId = obj.frameId;
		sTime = obj.sTime;
		eTime = obj.eTime;
		meshesTemplate = obj.meshesTemplate;
	};*/
	virtual ~Frame();

	int frameId;
	float sTime;	//eTime-sTime = duration

	std::vector<SubObjTemplate*> meshesTemplate;
};

#endif /* ENGINE_COMMON_OBJECTS_FRAME_HPP_ */
