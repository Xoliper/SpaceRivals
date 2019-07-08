#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Texture.hpp"


Texture::Texture(){
	texture_id = 0;
}

Texture::Texture(std::string path){
    texture_path = path;
	texture_id = 0;
    Load(path);
}

Texture::~Texture(){
	glDeleteTextures(1, &texture_id);
}

bool Texture::IsLoaded(){
	if(texture_id != 0){
		return true;
	}
	return false;
}

bool Texture::Load(std::string path){
	texture_path = path;
	try
    {
      std::unique_ptr<glimg::ImageSet> pImgSet(glimg::loaders::stb::LoadFromFile(path.c_str())); //Auto_ptr
      texture_id = glimg::CreateTexture(pImgSet.get(), 0);
      dimm = pImgSet.get()->GetDimensions();
      return true;
    }
    catch(glimg::loaders::stb::StbLoaderException &e)
    {
      //Image file loading failed.
    	std::cerr<<"#Exception# [Texture] -> Load -> Image file loading failed ("<<path<<")"<<std::endl;
    	texture_id = 0;
    }
    catch(glimg::TextureGenerationException &e)
    {
      //Texture creation failed.
    	std::cerr<<"#Exception# [Texture] -> Load -> Texture creation failed."<<std::endl;
    	texture_id = 0;
    }
    return false;
}

unsigned char * Texture::LoadPNGFileToMemory(std::string path, glimg::Dimensions & dimmOut){
	texture_path = path;
	try
    {
      std::unique_ptr<glimg::ImageSet> pImgSet(glimg::loaders::stb::LoadFromFile(path.c_str())); //Auto_ptr

      dimmOut = pImgSet->GetImage(0).GetDimensions();
      //Allocate buffers
      unsigned char * buff = new unsigned char[dimmOut.width*dimmOut.height*4];
      memcpy (buff, pImgSet->GetImage(0).GetImageData(), dimmOut.width*dimmOut.height*4);

      return buff;
    }
    catch(glimg::loaders::stb::StbLoaderException &e)
    {
    	std::cerr<<"#Exception# [Texture] -> Load -> Image file loading failed."<<std::endl;
    }
    catch(glimg::TextureGenerationException &e)
    {
    	std::cerr<<"#Exception# [Texture] -> Load -> Texture data loading failed."<<std::endl;
    }
	return nullptr;
}

bool Texture::LoadFromMemory(unsigned char * data, size_t size){
	try
    {
      std::unique_ptr<glimg::ImageSet> pImgSet(glimg::loaders::stb::LoadFromMemory(data, size)); //Auto_ptr
      texture_id = glimg::CreateTexture(pImgSet.get(), 0);
      dimm = pImgSet.get()->GetDimensions();
      return true;
    }
    catch(glimg::loaders::stb::StbLoaderException &e)
    {
      //Image file loading failed.
    	std::cerr<<"#Exception# [Texture] -> LoadFromMemory -> Image file loading failed."<<std::endl;
    	texture_id = 0;
    }
    catch(glimg::TextureGenerationException &e)
    {
      //Texture creation failed.
    	std::cerr<<"#Exception# [Texture] -> Load -> Texture creation failed."<<std::endl;
    	texture_id = 0;
    }
    return false;
}

void Texture::LoadRaw(unsigned char * bytes, int width, int height){

	glGenTextures(1, &texture_id);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bytes);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	dimm.depth = 4;
	dimm.width = width;
	std::cout<<"h "<<height<<std::endl;
	dimm.height = height;
	dimm.numDimensions = 2;

}



CubeMapTexture::CubeMapTexture(std::vector<std::string> faces){

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture_id);

	for (unsigned int i = 0; i < faces.size(); i++){

		try
	    {
	      std::unique_ptr<glimg::ImageSet> pImgSet(glimg::loaders::stb::LoadFromFile(faces[i].c_str())); //Auto_ptr
	      glimg::Dimensions dimm = pImgSet->GetImage(0).GetDimensions();
	      glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, dimm.width, dimm.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImgSet->GetImage(0).GetImageData());

	    }
	    catch(glimg::loaders::stb::StbLoaderException &e)
	    {
	    	std::cerr<<"#Exception# [Texture] -> Load -> Image file loading failed: "<<faces[i]<<std::endl;
	    }
	    catch(glimg::TextureGenerationException &e)
	    {
	    	std::cerr<<"#Exception# [Texture] -> Load -> Texture data loading failed."<<std::endl;
	    }
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

CubeMapTexture::~CubeMapTexture(){
	glDeleteTextures(1, &texture_id);
}

bool CubeMapTexture::IsLoaded(){
	if(texture_id != 0){
		return true;
	}
	return false;
}




