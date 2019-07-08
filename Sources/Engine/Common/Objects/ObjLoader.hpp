/*
 * ObjLoader.hpp
 *
 *  Created on: 12 maj 2018
 *      Author: Xoliper
 */

#ifndef ENGINE_COMMON_OBJECTS_OBJLOADER_HPP_
#define ENGINE_COMMON_OBJECTS_OBJLOADER_HPP_

#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <math.h>
#include "Frame.hpp"
#include "VBOIndexers.hpp"
#include "TangentSpace.hpp"
#include "../Exceptions.hpp"
#include "../Utils/Conversions.hpp"

char * CreateLine(char * source, size_t startPos, size_t length, size_t buffSize);
std::vector<Frame*> LoadObjFile(std::string path); 	//Loading .obj file. Supported one more new command "r <start time in float>"
												   	//	-> that way you can load multiple frames in one file
													// 	-> "r" stands for new frame, and following number is frame start time.

#endif /* ENGINE_COMMON_OBJECTS_OBJLOADER_HPP_ */
