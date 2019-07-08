#ifndef ENGINE_COMMON_OBJECTS_OBJTEMPLATE_HPP_
#define ENGINE_COMMON_OBJECTS_OBJTEMPLATE_HPP_

#include <iostream>
#include <string>
#include <vector>
#include "Frame.hpp"
#include "FoFFormat.hpp"
#include "ObjLoader.hpp"
#include "Material.hpp"
#include "MeshData.hpp"
#include "../Graphics/Texture.hpp"

class ObjTemplate {
private:
	void FoFLoad(std::string path);

public:
	ObjTemplate(std::string path, std::string texPath = "");
	virtual ~ObjTemplate();

	MeshData GetMeshData(int index);
	size_t GetFramesAmount();
	float GetAnimationDuration();

	std::vector<Material*> materials;
	std::vector<Frame*> frames;
	float totalTime;

};

#endif /* ENGINE_COMMON_OBJECTS_OBJTEMPLATE_HPP_ */
