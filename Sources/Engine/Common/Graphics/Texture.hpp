#ifndef TEXTURE_HPP
#define TEXTURE_HPP

//-----------------------------------------------------
// Defines & Includes
//-----------------------------------------------------


// Include GLIMG
#include <memory>
//#include <glload/gl_all.h>
//#include <glload/gl_load.h>
#include <GL/glew.h>
#include <glimg/glimg.h>

// Standard libs
#include <iostream>
#include <string>
#include <vector>


//-----------------------------------------------------
// Classes
//-----------------------------------------------------

class Texture {
public:
	Texture();
	Texture(std::string path);
	~Texture();

	bool IsLoaded();
	bool Load(std::string path);
	bool LoadFromMemory(unsigned char * data, size_t size);
	void LoadRaw(unsigned char * bytes, int width, int height); //planes only 3 (RGB)
	unsigned char * LoadPNGFileToMemory(std::string path, glimg::Dimensions & dimm);

	std::string texture_path;
	GLuint texture_id;
	glimg::Dimensions dimm;

};


class CubeMapTexture {
public:
	CubeMapTexture(std::vector<std::string> faces);
	~CubeMapTexture();

	bool IsLoaded();

	GLuint texture_id;

};

#endif
