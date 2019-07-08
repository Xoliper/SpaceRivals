/*
 * FoFFormat.cpp
 *
 *  Created on: 25 cze 2018
 *      Author: Xoliper
 */

#include "FoFFormat.hpp"
#include "ObjTemplate.hpp"

unsigned short GetHeaderType(std::fstream & file){
	unsigned short descriptor = 0x0000;
	file.read((char*)&descriptor, sizeof(unsigned short));
	return descriptor;
}

FoFManager::FoFManager(){
}

FoFManager::~FoFManager(){
}

void FoFManager::CreateFilesList(std::string path){

	std::cout<<"-------------------------------------------------"<<std::endl;
	passedPath = path;
	flatPath = path.substr(0, path.rfind("\\"));
	fileName = path.substr(path.rfind('\\')+1, path.rfind('_')-path.rfind('\\')-1);
	mtlFile = passedPath.substr(0, passedPath.rfind('.'))+".mtl";
	std::cout<<"Path: "<<passedPath<<std::endl<<"Flat: "<<flatPath<<std::endl<<"Mtl:  "<<mtlFile<<std::endl<<"Name: "<<fileName<<std::endl;
	std::cout<<"-------------------------------------------------"<<std::endl;


	//Finding all corresponding .obj and .png files
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (flatPath.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			std::string file = std::string(ent->d_name);

			if(!file.compare(0, fileName.size(), fileName) and !file.compare(file.size()-4, 4, ".obj")){
				objPaths.push_back(flatPath+"\\"+file);

				//Calculating frame time
				int keyFrame = stoi(file.substr(file.rfind('_')+1, file.rfind('.')-file.rfind('_')-1), nullptr, 10);
				float frameTime = ((float(keyFrame)-1)/60.0);
				framesTime.push_back(frameTime);

				std::cout<<"Added .obj: "<<file<<" to processing.    ->   Time: "<<frameTime<<std::endl;

				continue;
			}

			if(!file.compare(0, fileName.size(), fileName) and !file.compare(file.size()-4, 4, ".png")){
				//texPaths.push_back(file);
				//std::cout<<"Added .png: "<<file<<" to processing."<<std::endl;
				continue;
			}

		}
		closedir (dir);
	} else {
		std::cerr<<"Cannot open dir"<<std::endl;
	}
}

void FoFManager::CreateCombinedObjFile(std::string path){
	//Create combined .obj file (with frames)
	std::fstream file;
	file.open(path.c_str(), std::ios::out);
	if(file.good()){
		for(unsigned int i = 0; i<objPaths.size(); i++){
			//Write frame header
			file << "r ";
			file << i;
			file << " ";
			file << std::fixed << std::setprecision(6) << framesTime[i];
			file << "\n";

			//Write part-file
			std::fstream partFile;
			partFile.open(objPaths[i].c_str(), std::ios::in);
			if(partFile.good()){

				partFile.seekg(0, std::ios_base::end);
				size_t toLoad = partFile.tellg();
				partFile.seekg(0, std::ios_base::beg);

				//Load .obj file to buff
				char * buff = new char[toLoad+1];
				partFile.read(buff, toLoad);
				buff[toLoad] = '\0'; // 0?

				file << buff;

				delete [] buff;
				partFile.close();
			} else {
				std::cout<<"Error while opening part-file for combining!"<<std::endl;
			}
		}
		file.close();
	} else {
		std::cout<<"Cannot create combined file!"<<std::endl;
	}
	std::cout<<"Combined file created!"<<std::endl;
}


void FoFManager::DeleteCombinedObjFile(std::string path){
	if( remove( path.c_str() ) != 0 ){
		std::cout<<"Error deleting file"<<std::endl;
	} else {
		std::cout<<"Combined file successfully deleted!"<<std::endl;
	}
}

void FoFManager::ReadMtlFile(std::string path){
	std::cout<<"Reading .mtl file"<<std::endl;

	std::fstream mtl;
	mtl.open(path, std::ios::in);
	if(mtl.good()){
		std::string line;
		while(!mtl.eof()){
			getline(mtl, line);

			//Process line
			if(line.compare(0, 6, "newmtl") == 0){
				//Found new material
				std::vector<std::string> materialVecData;
				materialVecData.push_back(line.substr(7, line.size()-7)); //Push material name
				materialData.push_back(materialVecData);
				std::cout<<"Found material: "<<(line.substr(7, line.size()-7))<<std::endl;
			} else if(line.rfind(".png") != std::string::npos){
				//Found new texture
				materialData[materialData.size()-1].push_back(line); //Push texture data
				std::cout<<"Found texture: "<<line<<std::endl;
			} else if((line.compare(0, 2, "Ka") == 0) or (line.compare(0, 2, "Kd") == 0) or (line.compare(0, 2, "Ks") == 0)){
				materialData[materialData.size()-1].push_back(line); //Push texture data
				std::cout<<"Found color data: "<<line<<std::endl;
			}

		}
		mtl.close();
	} else {
		std::cout<<"Cannot load .mtl file!"<<std::endl;
		//If cannot find that file, check file name stored in "ot"
	}

}

size_t FoFManager::GetFileSize(std::string path){
	size_t out = 0;
	std::fstream file;
	file.open(path.c_str(), std::ios::in | std::ios::binary);
	if(file.good()){
		file.seekg(0, std::ios_base::end);
		out = file.tellg();
		file.seekg(0, std::ios_base::beg);
		file.close();
	} else {
		std::cout<<"GetFileSize failed."<<std::endl;
	}
	return out;
}

std::string FoFManager::GetAutoOutputName(){
	return flatPath + "\\" + fileName + ".fof";
}

void FoFManager::CreateFoFFile(std::string path){

	std::cout<<"-------------------------------------------------"<<std::endl;

	//Create combined .obj file
	CreateCombinedObjFile(fileName+"_combined.obj");

	//Load created .obj file
	ObjTemplate * ot = new ObjTemplate(fileName+"_combined.obj");
	if(ot != nullptr) std::cout<<"Loaded combined file!"<<std::endl;

	//Delete combined .obj file
	DeleteCombinedObjFile(fileName+"_combined.obj");


	//Check for .mtl file
	//Read .mtl + load materials name to std::vector + it textures path to another vector
	//Later save data: material header (with name) + raw data from textures path
	std::cout<<"-------------------------------------------------"<<std::endl;
	ReadMtlFile(mtlFile);

	std::cout<<"-------------------------------------------------"<<std::endl;

	std::cout<<"Creating .fof file..."<<std::endl;

	std::fstream file;
	file.open(path.c_str(), std::ios::out | std::ios::binary);
	if(file.good()){

		std::cout<<"Writing header..."<<std::endl;
		//Write header
		FoFHeader fh;
		memset(&fh, 0, sizeof(FoFHeader));
		fh.framesAmount = ot->frames.size();
		fh.meshesAmount = ot->frames[0]->meshesTemplate.size();
		fh.materialsAmount = ot->materials.size();
		fh.majorVersion = FOF_MAJOR_VER;
		fh.minorVersion = FOF_MINOR_VER;
		WriteFoFHeader<FoFHeader>(FOF_HEADER, fh, file);

		//Write frames & meshes
		for(unsigned int i = 0; i<ot->frames.size(); i++){
			std::cout<<"Writing frame ["<<i<<"]..."<<std::endl;
			//Write frame header
			FrameHeader frh;
			memset(&frh, 0, sizeof(FrameHeader));
			frh.frameId = ot->frames[i]->frameId;
			frh.appearTime = ot->frames[i]->sTime;
			WriteFoFHeader<FrameHeader>(FOF_FRAME_HEADER, frh, file);

			//Write meshes in frame
			for(unsigned int j = 0; j<ot->frames[i]->meshesTemplate.size(); j++){
				std::cout<<"Writing mesh ["<<j<<"]"<<std::endl;

				//Write mesh header
				MeshHeader mh;
				memset(&mh, 0, sizeof(MeshHeader));
				//memset(mh.name, '\0', 255); memset(mh.usedMaterialName, '\0', 64);
				strcpy(mh.name, ot->frames[i]->meshesTemplate[j]->name.c_str());
				//std::cout<<mh.name<<std::endl;
				strcpy(mh.usedMaterialName, ot->frames[i]->meshesTemplate[j]->materialName.c_str());
				//mh.usedMaterialId = ot->frames[i]->meshesTemplate[j]->materialId;
				mh.verticesSize = ot->frames[i]->meshesTemplate[j]->otVertices.size();
				mh.normalsSize = ot->frames[i]->meshesTemplate[j]->otNormals.size();
				mh.uvsSize = ot->frames[i]->meshesTemplate[j]->otUvs.size();
				mh.indicesSize = ot->frames[i]->meshesTemplate[j]->otIndices.size();
				mh.tangentSize = ot->frames[i]->meshesTemplate[j]->otTangents.size();
				mh.bitangentSize = ot->frames[i]->meshesTemplate[j]->otBitangents.size();
				WriteFoFHeader<MeshHeader>(FOF_MESH_HEADER, mh, file);

				//Write mesh data
				for(unsigned int x = 0; x<ot->frames[i]->meshesTemplate[j]->otVertices.size(); x++){
					file.write((char*)&ot->frames[i]->meshesTemplate[j]->otVertices[x], sizeof(glm::vec3));
					//file.write((char*)&ot->frames[i]->meshesTemplate[j]->otVertices[x].x, sizeof(float));		//Vertices
					//file.write((char*)&ot->frames[i]->meshesTemplate[j]->otVertices[x].y, sizeof(float));		//Vertices
					//file.write((char*)&ot->frames[i]->meshesTemplate[j]->otVertices[x].z, sizeof(float));		//Vertices
				}
				for(unsigned int x = 0; x<ot->frames[i]->meshesTemplate[j]->otNormals.size(); x++){
					file.write((char*)&ot->frames[i]->meshesTemplate[j]->otNormals[x], sizeof(glm::vec3));
					//file.write((char*)&ot->frames[i]->meshesTemplate[j]->otNormals[x].x, sizeof(float));		//Normals
					//file.write((char*)&ot->frames[i]->meshesTemplate[j]->otNormals[x].y, sizeof(float));		//Normals
					//file.write((char*)&ot->frames[i]->meshesTemplate[j]->otNormals[x].z, sizeof(float));		//Normals
				}
				for(unsigned int x = 0; x<ot->frames[i]->meshesTemplate[j]->otUvs.size(); x++){
					file.write((char*)&ot->frames[i]->meshesTemplate[j]->otUvs[x], sizeof(glm::vec2));
					//file.write((char*)&ot->frames[i]->meshesTemplate[j]->otUvs[x].x, sizeof(float));			//Uvs
					//file.write((char*)&ot->frames[i]->meshesTemplate[j]->otUvs[x].y, sizeof(float));			//Uvs
				}
				for(unsigned int x = 0; x<ot->frames[i]->meshesTemplate[j]->otIndices.size(); x++){
					file.write((char*)&ot->frames[i]->meshesTemplate[j]->otIndices[x], sizeof(unsigned short));	//Indices
				}
				for(unsigned int x = 0; x<ot->frames[i]->meshesTemplate[j]->otTangents.size(); x++){
					file.write((char*)&ot->frames[i]->meshesTemplate[j]->otTangents[x], sizeof(glm::vec3));		//Tangents
				}
				for(unsigned int x = 0; x<ot->frames[i]->meshesTemplate[j]->otBitangents.size(); x++){
					file.write((char*)&ot->frames[i]->meshesTemplate[j]->otBitangents[x], sizeof(glm::vec3));	//Bitangents
				}

				//Write BoxDimm data
				file.write((char*)&ot->frames[i]->meshesTemplate[j]->dimm, sizeof(BoxDimm));					//BoxDimm

			}
		}

		std::cout<<"-------------------------------------------------"<<std::endl;

		//Write materials
		for(unsigned int i = 0; i<materialData.size(); i++){

			MaterialHeader mh;
			memset(&mh, 0, sizeof(MaterialHeader));
			strcpy(mh.materialName, materialData[i][0].c_str());

			std::vector<std::string> texPaths;
			std::vector<unsigned short> texOrder;

			for(unsigned int j = 1; j<materialData[i].size(); j++){

				std::string alias = materialData[i][j];
				std::string texPath = alias.substr(alias.find(' ')+1, alias.size() - alias.find(' ')-1);

				//Get Texture type & size
				if(alias.compare(0, 6, "map_Kd") == 0){
					mh.diffuseSize = GetFileSize(flatPath+"\\"+texPath);
					texPaths.push_back(flatPath+"\\"+texPath);
					texOrder.push_back(0);
				} else if(alias.compare(0, 8, "map_Bump") == 0){
					mh.normalSize = GetFileSize(flatPath+"\\"+texPath);
					texPaths.push_back(flatPath+"\\"+texPath);
					texOrder.push_back(1);
				} else if(alias.compare(0, 6, "map_Ns") == 0 or alias.compare(0, 6, "map_Ks") == 0){
					mh.specularSize = GetFileSize(flatPath+"\\"+texPath);
					texPaths.push_back(flatPath+"\\"+texPath);
					texOrder.push_back(2);
				} else if(alias.compare(0, 2, "Ka") == 0){
					sscanf(&alias[3], "%f %f %f\n", &mh.Ka.r, &mh.Ka.g, &mh.Ka.b );
				} else if(alias.compare(0, 2, "Kd") == 0){
					sscanf(&alias[3], "%f %f %f\n", &mh.Kd.r, &mh.Kd.g, &mh.Kd.b );
				} else if(alias.compare(0, 2, "Ks") == 0){
					sscanf(&alias[3], "%f %f %f\n", &mh.Ks.r, &mh.Ks.g, &mh.Ks.b );
				}
			}

			//Write header
			std::cout<<"Writing material header..."<<std::endl;
			WriteFoFHeader<MaterialHeader>(FOF_MATERIAL_HEADER, mh, file);

			//Write textures
			unsigned int texAmount = texOrder.size();
			unsigned int curTexType = 0;
			unsigned int index = (unsigned int)-1;

			for(unsigned int n = 0; n<texAmount; n++){

				//Find index
				for(unsigned int x = 0; x<texOrder.size(); x++){
					if(texOrder[x] == curTexType){
						index = x;
						break;
					}
				}

				if(index != (unsigned int)-1){
					std::cout<<"Writing textures data..."<<std::endl;
					std::fstream texFile;
					texFile.open(texPaths[index].c_str(), std::ios::in | std::ios::binary);
					if(texFile.good()){
						texFile.seekg(0, std::ios_base::end);
						size_t size = texFile.tellg();
						texFile.seekg(0, std::ios_base::beg);
						char * buff = new char[size];
						texFile.read(buff, size);
						file.write(buff, sizeof(char) * size);
						delete [] buff;
						texFile.close();
					} else {
						std::cout<<"Cannot open texture file to write material!"<<std::endl;
					}
				}
				++curTexType;
			}
		}


		//Write end header
		std::cout<<"Writing end descriptor..."<<std::endl;
		unsigned short endDescriptor = 0x0000;
		file.write((char*)&endDescriptor, sizeof(unsigned short));
		file.flush();
		file.close();
		std::cout<<"-------------------------------------------------"<<std::endl;
		std::cout<<"Done."<<std::endl;
	} else {
		std::cout<<"Cannot create .fof file!"<<std::endl;
	}

	delete ot;
}
