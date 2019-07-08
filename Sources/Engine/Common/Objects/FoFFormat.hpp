/*
 * FoFFormat.hpp
 *
 *  Created on: 25 cze 2018
 *      Author: Xoliper
 */

#ifndef ENGINE_COMMON_OBJECTS_FOFFORMAT_HPP_
#define ENGINE_COMMON_OBJECTS_FOFFORMAT_HPP_

#include <dirent.h>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <glm/glm.hpp>

#include "Material.hpp"

#define FOF_MAJOR_VER	1
#define FOF_MINOR_VER	0

#define FOF_HEADER 	0xFFFF
#define FOF_FRAME_HEADER 	0xFFF0
#define FOF_MESH_HEADER		0xFF00
#define FOF_MATERIAL_HEADER	0xF000

typedef struct {
	int majorVersion;
	int minorVersion;
	int framesAmount;
	int meshesAmount;
	int materialsAmount;
} FoFHeader;

typedef struct {
	int appearTime; //TODO
	int frameId;
} FrameHeader;

typedef struct {
	char name[255];
	char usedMaterialName[64];
	int verticesSize;
	int normalsSize;
	int uvsSize;
	int indicesSize;
	int tangentSize;
	int bitangentSize;
} MeshHeader;

typedef struct {
	char materialName[64];
	unsigned int diffuseSize;
	unsigned int normalSize;
	unsigned int specularSize;
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
} MaterialHeader;

template <class T>
void WriteFoFHeader(unsigned short descriptor, T & header, std::fstream & file){
	file.write((char*)&descriptor, sizeof(unsigned short));
	file.write((char*)&header, sizeof(T));
}

template <class T>
void ReadFoFHeader(T & header, std::fstream & file){
	file.read((char*)&header, sizeof(T));
}

unsigned short GetHeaderType(std::fstream & file);

class FoFManager {

private:

	std::vector<std::string> objPaths;
	std::vector<float> framesTime;
	std::vector<std::vector<std::string>> materialData;

	std::string passedPath;
	std::string flatPath;
	std::string fileName;
	std::string mtlFile;

public:

	FoFManager();
	~FoFManager();

	void CreateFilesList(std::string path);
	void CreateCombinedObjFile(std::string path);
	void DeleteCombinedObjFile(std::string path);
	void ReadMtlFile(std::string path);
	size_t GetFileSize(std::string path);
	std::string GetAutoOutputName();
	void CreateFoFFile(std::string path);

};

#endif /* ENGINE_COMMON_OBJECTS_FOFFORMAT_HPP_ */
