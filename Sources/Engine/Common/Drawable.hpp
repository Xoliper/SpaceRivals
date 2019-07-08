#ifndef ENGINE_COMMON_DRAWABLE_HPP_
#define ENGINE_COMMON_DRAWABLE_HPP_

#include <GL/glew.h>
#include <vector>

#include <glm/glm.hpp>
#include "World.hpp"

class BoxDimm {
public:
	BoxDimm();
	BoxDimm(const BoxDimm & box);
	virtual ~BoxDimm();

	void CalculateDimmFromVertices(std::vector<glm::vec3> & otVertices);

	float width, height, depth;
	float x_min, x_max;
	float y_min, y_max;
	float z_min, z_max;
};

enum ObjType {
	STATIC,
	DYNAMIC,
	DK
};

//To coœ, bêdzie wyœwietlanie i updatowane zgodnie z ramk¹ :D (na podstawie ObjTemplate)
class SubObject {
public:
	SubObject();
	SubObject(const SubObject & obj);
	~SubObject();

	void RecreateBuffers();
	void CreateBuffers(ObjType & ot);
	void UpdateVertexBuffers(ObjType & ot);


	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;

	GLuint vertexArrayID;
	GLuint vertexBuffer;
	GLuint uvBuffer;
	GLuint normalBuffer;

	GLuint tangentBuffer;
	GLuint bitangentBuffer;
	GLuint elementBuffer;

	BoxDimm dimm;
	int materialId;

	bool init;
};

class Drawable {
public:
	Drawable(){
		params = nullptr; world = nullptr; 	objType = DK;
	};
	virtual ~Drawable(){};
	virtual void Render() = 0;

	std::vector<GLuint> * params;
	World * world;
	glm::mat4 model;
	glm::mat4 scale;
	glm::mat4 rotation;
	glm::mat4 translation;
	ObjType objType;
	std::vector<SubObject> meshes;


};

#endif /* ENGINE_COMMON_DRAWABLE_HPP_ */
