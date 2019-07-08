#include "Drawable.hpp"



//-------------------------------------------------------------------------------
//BOX DIMM
//-------------------------------------------------------------------------------

BoxDimm::BoxDimm(){
	x_min = x_max = y_min = y_max = z_min = z_max = 0.0f;
	width = height = depth = 0.0f;
};

BoxDimm::BoxDimm(const BoxDimm & box){
	x_max = box.x_max;
	x_min = box.x_min;
	y_max = box.y_max;
	y_min = box.y_min;
	z_max = box.z_max;
	z_min = box.z_min;
	width = box.width;
	height = box.height;
	depth = box.depth;
}

BoxDimm::~BoxDimm(){
}

void BoxDimm::CalculateDimmFromVertices(std::vector<glm::vec3> & otVertices){
	if(otVertices.size()>=1){
		x_max = x_min = otVertices[0].x;
		y_max = y_min = otVertices[0].y;
		z_max = z_min = otVertices[0].z;
	}
	for(unsigned int i = 0; i<otVertices.size(); i++){
		//for(int x = 0; x<frames[i]->meshesTemplate[j]->otVertices.size(); x++){

		if(x_max < otVertices[i].x) x_max = otVertices[i].x;
		if(x_min > otVertices[i].x) x_min = otVertices[i].x;
		if(y_max < otVertices[i].y) y_max = otVertices[i].y;
		if(y_min > otVertices[i].y) y_min = otVertices[i].y;
		if(z_max < otVertices[i].z) z_max = otVertices[i].z;
		if(z_min > otVertices[i].z) z_min = otVertices[i].z;
	}
	//std::cout<<y_max<<std::endl;

	width = abs(x_max-x_min);
	height = abs(y_max-y_min);
	depth = abs(z_max-z_min);
}


SubObject::SubObject(){
	init = true;
	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &uvBuffer);
	glGenBuffers(1, &normalBuffer);
	glGenBuffers(1, &elementBuffer);
	glGenBuffers(1, &tangentBuffer);
	glGenBuffers(1, &bitangentBuffer);
	materialId = -1;
}

SubObject::SubObject(const SubObject & obj){
	vertices = obj.vertices;
	uvs = obj.uvs;
	normals = obj.normals;
	indices = obj.indices;
	tangents = obj.tangents;
	bitangents = obj.bitangents;
	init = true;
	dimm = obj.dimm;
	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &uvBuffer);
	glGenBuffers(1, &normalBuffer);
	glGenBuffers(1, &elementBuffer);
	glGenBuffers(1, &tangentBuffer);
	glGenBuffers(1, &bitangentBuffer);
	materialId = obj.materialId;
}

SubObject::~SubObject(){
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &elementBuffer);
	glDeleteBuffers(1, &tangentBuffer);
	glDeleteBuffers(1, &bitangentBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void SubObject::RecreateBuffers(){
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &uvBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &elementBuffer);
	glDeleteBuffers(1, &tangentBuffer);
	glDeleteBuffers(1, &bitangentBuffer);
	glDeleteVertexArrays(1, &vertexArrayID);

	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &uvBuffer);
	glGenBuffers(1, &normalBuffer);
	glGenBuffers(1, &elementBuffer);
	glGenBuffers(1, &tangentBuffer);
	glGenBuffers(1, &bitangentBuffer);
}

void SubObject::CreateBuffers(ObjType & ot){

	int bufType = 0;
	if(ot == STATIC){
		bufType = GL_STATIC_DRAW;
	} else if(ot == DYNAMIC){
		bufType = GL_DYNAMIC_DRAW;
	}

	RecreateBuffers();

	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], bufType);

	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], bufType);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], bufType);

	glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], bufType);

	glBindBuffer(GL_ARRAY_BUFFER, bitangentBuffer);
	glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0], bufType);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], bufType);
}

void SubObject::UpdateVertexBuffers(ObjType & ot){

	int bufType = 0;
	if(ot == STATIC){
		bufType = GL_STATIC_DRAW;
	} else if(ot == DYNAMIC){
		bufType = GL_STREAM_DRAW;
	}

	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], bufType);
}
