#include "ObjLoader.hpp"

char * CreateLine(char * source, size_t startPos, size_t length, size_t buffSize){
	//std::cout<<"Bf size: "<<buffSize<<"    StartPos: "<<startPos<<"    Lenght:"<<length<<std::endl;
	if(startPos+length-1 > buffSize) return nullptr;
	if(length <= 0) return nullptr;
	char * line = new char[length];
	memcpy(&line[0], &source[startPos], length);
	line[length-1] = 0;
	//std::cout<<"Line: "<<line<<std::endl;
	return line;
}

std::vector<Frame*> LoadObjFile(std::string path){

	std::vector<Frame*> output;
	try {

		std::fstream file;
		file.open(path.c_str(), std::ios::in);
		if(file.good()){

			file.seekg(0, std::ios_base::end);
			size_t toLoad = file.tellg();
			file.seekg(0, std::ios_base::beg);

			//Load .obj file to buff
			char * buff = new char[toLoad+1];
			file.read(buff, toLoad);
			buff[toLoad] = 0; //End buffer


			//Prepare loader
			size_t buffSize = file.gcount();
			size_t cur = 0;
			char * pch = strchr(buff,'\n');

			int tempFrameId = 0;
			float tempsTime = 0.0f;
			float tempeTime = 0.0f;

			while(pch != NULL){

				//Create Frames
				Frame * tempFrame = new Frame;
				tempFrame->frameId = tempFrameId;
				tempFrame->sTime = tempsTime;

				std::vector<glm::vec3> temp_vertices;
				std::vector<glm::vec2> temp_uvs;
				std::vector<glm::vec3> temp_normals;

				char objName[255];
				memset(objName, '\0', 255);

				while(pch != NULL){

					//Create SubObject
					bool newFrameExit = false;
					std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
					char objNameTemp[255], materialName[64];
					memset(objNameTemp, '\0', 255);
					memset(materialName, '\0', 64);

					while (pch != NULL){

						//Fill SubObject

						char * line = CreateLine(buff, cur, (pch-buff+1)-cur, buffSize);
						if(line != NULL){

							//Process obj file
							if(line[0] == 'r' and line[1] == ' '){
								//std::cout<<"New Frame detected!"<<std::endl;
								int tempFrameIdFix = 0;
								sscanf(&line[2], "%i %f\n", &tempFrameIdFix, &tempsTime);
								if(tempFrameIdFix != 0){
									tempFrameId = tempFrameIdFix;
									newFrameExit = true;
									cur = pch-buff+1;
									pch = strchr(pch+1,'\n');

									delete [] line;
									break;
								}
							} else if(line[0] == 'o' and line[1] == ' ' and vertexIndices.size() != 0){
								//std::cout<<"New object detected!"<<std::endl;
								sscanf(&line[2], "%s\n", &objNameTemp);

								cur = pch-buff+1;
								pch = strchr(pch+1,'\n');

								delete [] line;
								break;
							} else if(line[0] == 'o' and line[1] == ' '){
								//std::cout<<"First object detected!"<<std::endl;
								sscanf(&line[2], "%s\n", &objName);
							} else if(line[0] == 'v' and line[1] == ' '){
								glm::vec3 vertex;
								sscanf(&line[2], "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
								temp_vertices.push_back(vertex);
							} else if ( line[0] == 'v' and line[1] == 't' ){
								glm::vec2 uv;
								sscanf(&line[3], "%f %f\n", &uv.x, &uv.y );
								//uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
								temp_uvs.push_back(uv);
							} else if ( line[0] == 'v' and line[1] == 'n' ){
								glm::vec3 normal;
								sscanf(&line[3], "%f %f %f\n", &normal.x, &normal.y, &normal.z );
								temp_normals.push_back(normal);
							} else if (line[0] == 'f' and line[1] == ' '){
								std::string vertex1, vertex2, vertex3;
								unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
								int matches = sscanf(&line[2], "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
								if (matches != 9){
									delete [] buff;
									throw Exception("ObjLoader", "Constructor", "Cannot parse '"+path+"' file!");
								}
								vertexIndices.push_back(vertexIndex[0]);
								vertexIndices.push_back(vertexIndex[1]);
								vertexIndices.push_back(vertexIndex[2]);
								uvIndices    .push_back(uvIndex[0]);
								uvIndices    .push_back(uvIndex[1]);
								uvIndices    .push_back(uvIndex[2]);
								normalIndices.push_back(normalIndex[0]);
								normalIndices.push_back(normalIndex[1]);
								normalIndices.push_back(normalIndex[2]);
							} else if(line[0] =='u' and line[1] =='s' and line[2] =='e' and line[3] == 'm' and line[4] == 't' and line[5] == 'l'){
								sscanf(&line[7], "%s\n", &materialName);
							}

							cur = pch-buff+1;
							pch = strchr(pch+1,'\n');
							delete [] line;
						} else {
							pch = NULL;
						}


					}

					//Post-fix for each vertex of each triangle
					SubObjTemplate * sot = new SubObjTemplate;
					if(temp_uvs.size() == 0) throw Exception("ObjLoader", "Constructor", "Wavefront file '"+path+"' dont have UVs, please add Texture to it!");

					//Final VBO calculating
					std::vector<glm::vec3> otVerticesToIndex;
					std::vector<glm::vec2> otUvsToIndex;
					std::vector<glm::vec3> otNormalsToIndex;
					//And tangents & bitangents
					std::vector<glm::vec3> otTangentsToIndex;
					std::vector<glm::vec3> otBitangentsToIndex;


					for( unsigned int i=0; i<vertexIndices.size(); i++ ){
						// Get the indices of its attributes
						unsigned int vertexIndex = vertexIndices[i];
						unsigned int uvIndex = uvIndices[i];
						unsigned int normalIndex = normalIndices[i];

						// Get the attributes thanks to the index
						glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
						glm::vec2 uv = temp_uvs[ uvIndex-1 ];
						glm::vec3 normal = temp_normals[ normalIndex-1 ];
						//std::cout<<"Normal: "<<normal.x<<std::endl;

						// Put the attributes in buffers
						otVerticesToIndex.push_back(vertex);
						otUvsToIndex 	 .push_back(uv);
						otNormalsToIndex .push_back(normal);
					}

					ComputeTangentBasis(otVerticesToIndex, otUvsToIndex, otNormalsToIndex, otTangentsToIndex, otBitangentsToIndex);
					IndexVboTbn (
						otVerticesToIndex, otUvsToIndex, otNormalsToIndex, otTangentsToIndex, otBitangentsToIndex,
						 sot->otIndices, sot->otVertices, sot->otUvs, sot->otNormals, sot->otTangents, sot->otBitangents
					);

					/*for(int i = 0; i<sot->otTangents.size(); i++){
						if(isnan(sot->otTangents[i].x)) sot->otTangents[i].x = 1.0f;
						if(isnan(sot->otTangents[i].y)) sot->otTangents[i].y = 1.0f;
						if(isnan(sot->otTangents[i].z)) sot->otTangents[i].z = 1.0f;
					}


					for(int i = 0; i<sot->otBitangents.size(); i++){
						if(isnan(sot->otBitangents[i].x)) sot->otBitangents[i].x = 1.0f;
						if(isnan(sot->otBitangents[i].y)) sot->otBitangents[i].y = 1.0f;
						if(isnan(sot->otBitangents[i].z)) sot->otBitangents[i].z = 1.0f;
					}*/

					//IndexVbo(otVerticesToIndex, otUvsToIndex, otNormalsToIndex, sot->otIndices, sot->otVertices, sot->otUvs, sot->otNormals);


					BoxDimm box;
					if(sot->otVertices.size()>=1){
						box.x_max = box.x_min = sot->otVertices[0].x;
						box.y_max = box.y_min = sot->otVertices[0].y;
						box.z_max = box.z_min = sot->otVertices[0].z;
					}
					for(unsigned int i = 0; i<sot->otVertices.size(); i++){
						if(box.x_max < sot->otVertices[i].x) box.x_max = sot->otVertices[i].x;
						if(box.x_min > sot->otVertices[i].x) box.x_min = sot->otVertices[i].x;
						if(box.y_max < sot->otVertices[i].y) box.y_max = sot->otVertices[i].y;
						if(box.y_min > sot->otVertices[i].y) box.y_min = sot->otVertices[i].y;
						if(box.z_max < sot->otVertices[i].z) box.z_max = sot->otVertices[i].z;
						if(box.z_min > sot->otVertices[i].z) box.z_min = sot->otVertices[i].z;
					}
					box.width = abs(box.x_max-box.x_min);
					box.height = abs(box.y_max-box.y_min);
					box.depth = abs(box.z_max-box.z_min);
					sot->dimm = box;

					sot->materialId = -1;
					sot->materialName = std::string(materialName);
					sot->name = std::string(objName);
					tempFrame->meshesTemplate.push_back(sot);

					strncpy(objName, objNameTemp, 255);

					if(newFrameExit == true){
						break;
					}
				}

				output.push_back(tempFrame);
			}

			delete [] buff;
			file.close();

			return output;
		} else {
			throw Exception("ObjLoader", "Constructor", "Cannot open '"+path+"' file!");
		}
	} catch (Exception & exce){
		std::cerr<<exce.what()<<std::endl;
		return output;
	}
}
