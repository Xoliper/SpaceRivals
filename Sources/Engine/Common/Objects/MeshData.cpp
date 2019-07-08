/*
 * MeshData.cpp
 *
 *  Created on: 30 cze 2018
 *      Author: Xoliper
 */

#include "MeshData.hpp"

MeshData::MeshData(){
}

MeshData::~MeshData(){
}

MeshData::MeshData(const MeshData & obj){
	vertices = obj.vertices;
	uvs = obj.uvs;
	normals = obj.normals;
	indices = obj.indices;
	tangents = obj.tangents;
	bitangents = obj.bitangents;
}
