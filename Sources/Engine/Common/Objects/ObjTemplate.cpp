#include "ObjTemplate.hpp"

//-------------------------------------------------------------------------------
//OBJ TEMPLATE
//-------------------------------------------------------------------------------

ObjTemplate::ObjTemplate(std::string path, std::string texPath) {
	totalTime = 0.0f;

	size_t dot = path.find_last_of(".");
	if(dot != std::string::npos){

		std::string ext = path.substr(dot+1);
		if(ext == "obj"){


			std::vector<std::string> texInfo;
			std::vector<Frame*> fs = LoadObjFile(path.c_str());
			if(fs.size() <= 0){
				throw Exception("ObjTemplate", "Constructor", "Corrupted '."+ext+"'  file: "+path.c_str());
			}
			for(unsigned int i = 0; i< fs.size(); i++){
				frames.push_back(fs[i]);
			}

			//Load Texture
			if(!texPath.empty()){	// Tex file (provided by path)
				Material * nm = new Material();
				nm->diffuseTex.Load(texPath);
				materials.push_back(nm);

				for(unsigned int i = 0; i<frames.size(); i++){
					for(unsigned int j = 0; j<frames[i]->meshesTemplate.size(); j++){
						frames[i]->meshesTemplate[j]->materialId = 0;
					}
				}
			}

			//Update anim time
			totalTime = frames.back()->sTime;

		} else if(ext == "fof") {
			//Load materials, frames and object data
			FoFLoad(path);
			//Fix-up data
			for(unsigned int i = 0; i<frames.size(); i++){
				for(unsigned int j = 0; j<frames[i]->meshesTemplate.size(); j++){
					//Fill-up BoxDimms
					frames[i]->meshesTemplate[j]->dimm.CalculateDimmFromVertices(frames[i]->meshesTemplate[j]->otVertices);

					//Associate materials names with IDs
					for(unsigned int x = 0; x<materials.size(); x++){
						if(frames[i]->meshesTemplate[j]->materialName == materials[x]->materialName){
							frames[i]->meshesTemplate[j]->materialId = x;
							break;
						}
					}
				}
			}

			//Update anim time
			totalTime = frames.back()->sTime;

		} else {
			throw Exception("ObjTemplate", "Constructor", "Wrong file extension: '"+ext+"'. Supported filetypes: .obj .fof");
		}
	} else {
		throw Exception("ObjTemplate", "Constructor", "The file must have the extension!");
	}
}

ObjTemplate::~ObjTemplate() {
	for(unsigned int i = 0; i<frames.size(); i++){
		delete frames[i];
	}
	for(unsigned int i = 0; i<materials.size(); i++){
		delete materials[i];
	}
}

void ObjTemplate::FoFLoad(std::string path){
	std::fstream file;
	file.open(path.c_str(), std::ios::in | std::ios::binary);
	if(file.good()){

		//Get filesize
		file.seekg(0, std::ios_base::end);
		size_t fileSize = file.tellg();
		file.seekg(0, std::ios_base::beg);

		//Check if yo are going to deal with .FOF file & read header
		FoFHeader fh;
		unsigned short descriptor = GetHeaderType(file);

		if(descriptor == FOF_HEADER){
			ReadFoFHeader<FoFHeader>(fh, file);
		} else {
			throw Exception("ObjTemplate", "FoFLoad", "File header corrupted or you are trying to open wrong file!");
		}

		Frame * curFrame = nullptr;

		//Process file
		int endFlag = false;
		while(file.tellg() <= fileSize){
			descriptor = GetHeaderType(file);
			//Start switch
			switch(descriptor){

				case FOF_FRAME_HEADER: {
					FrameHeader fmh;
					ReadFoFHeader<FrameHeader>(fmh, file);
					Frame * tempFrame = new Frame();
					tempFrame->frameId = fmh.frameId;
					tempFrame->sTime = fmh.appearTime;
					curFrame = tempFrame;
					frames.push_back(tempFrame);
				} break;

				case FOF_MESH_HEADER: {

					MeshHeader mh;
					ReadFoFHeader<MeshHeader>(mh, file);

					//Load mesh data
					if(curFrame != nullptr){
					SubObjTemplate * tsot = new SubObjTemplate();
					tsot->name = std::string(mh.name);
					tsot->materialName = std::string(mh.usedMaterialName);

					for(int x = 0; x<mh.verticesSize; x++){
						glm::vec3 tempVec;
						file.read((char*)&tempVec, sizeof(glm::vec3));
						tsot->otVertices.push_back(tempVec);
					}

					for(int x = 0; x<mh.normalsSize; x++){
						glm::vec3 tempVec;
						file.read((char*)&tempVec, sizeof(glm::vec3));
						tsot->otNormals.push_back(tempVec);
					}

					for(int x = 0; x<mh.uvsSize; x++){
						glm::vec2 tempVec;
						file.read((char*)&tempVec, sizeof(glm::vec2));
						tsot->otUvs.push_back(tempVec);
					}

					for(int x = 0; x<mh.indicesSize; x++){
						unsigned short indiceTemp;
						file.read((char*)&indiceTemp, sizeof(unsigned short));										//Indices
						tsot->otIndices.push_back(indiceTemp);
					}

					for(int x = 0; x<mh.tangentSize; x++){
						glm::vec3 tempVec;
						file.read((char*)&tempVec, sizeof(glm::vec3));												//Tangents
						tsot->otTangents.push_back(tempVec);
					}

					for(int x = 0; x<mh.bitangentSize; x++){
						glm::vec3 tempVec;
						file.read((char*)&tempVec, sizeof(glm::vec3));												//Tangents
						tsot->otBitangents.push_back(tempVec);
					}

					//Read BoxDimm data
					file.read((char*)&tsot->dimm, sizeof(BoxDimm));													//BoxDimm

					//Put mesh into frame
					curFrame->meshesTemplate.push_back(tsot);
					continue;
					} else {
						throw Exception("ObjTemplate", "FoFLoad", "File corrupted - trying to load mesh without created frame!");
					}

				} break;

				case FOF_MATERIAL_HEADER: {
					//Texture order in material raw data: diffuse -> normal -> specular
					MaterialHeader mh;
					ReadFoFHeader<MaterialHeader>(mh, file);

					Material * m = new Material();
					m->materialName = std::string(mh.materialName);
					m->mcd.Ka = mh.Ka;
					m->mcd.Kd = mh.Kd;
					m->mcd.Ks = mh.Ks;

					if(mh.diffuseSize){
						unsigned char * buff = new unsigned char[mh.diffuseSize];
						file.read((char*)buff, sizeof(unsigned char) * mh.diffuseSize);
						m->diffuseTex.LoadFromMemory(buff, mh.diffuseSize);
						delete [] buff;
					}
					if(mh.normalSize){
						unsigned char * buff = new unsigned char[mh.normalSize];
						file.read((char*)buff, sizeof(unsigned char) * mh.normalSize);
						m->normalTex.LoadFromMemory(buff, mh.normalSize);
						delete [] buff;
					}
					if(mh.specularSize){
						unsigned char * buff = new unsigned char[mh.specularSize];
						file.read((char*)buff, sizeof(unsigned char) * mh.specularSize);
						m->specularTex.LoadFromMemory(buff, mh.specularSize);
						delete [] buff;
					}

					materials.push_back(m);
					continue;
				} break;

				case 0x0000: {
					endFlag = true;
				} break;

			}
			//End switch
			if(endFlag == true){
				break;
			}
		}

		file.close();
	} else {
		std::cout<<"Cannot open .fof file!"<<std::endl;
	}

}

MeshData ObjTemplate::GetMeshData(int index){
	if(index > -1 and index < frames[0]->meshesTemplate.size()){
		MeshData md;
		md.vertices = frames[0]->meshesTemplate[0]->otVertices;
		md.normals =  frames[0]->meshesTemplate[0]->otNormals;
		md.uvs =  frames[0]->meshesTemplate[0]->otUvs;
		md.indices =  frames[0]->meshesTemplate[0]->otIndices;
		md.tangents =  frames[0]->meshesTemplate[0]->otTangents;
		md.bitangents =  frames[0]->meshesTemplate[0]->otBitangents;
		return md;
	} else {
		throw Exception("ObjTemplate", "GetMeshData", "Index is out of range. Loaded ["+std::to_string(frames[0]->meshesTemplate.size())+"] meshes but trying to access ["+std::to_string(index)+"]!");
	}
}

size_t ObjTemplate::GetFramesAmount(){
	return frames.size();
}

float ObjTemplate::GetAnimationDuration(){
	return totalTime;
}

