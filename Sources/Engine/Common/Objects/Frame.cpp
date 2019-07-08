#include "Frame.hpp"




//-------------------------------------------------------------------------------
//SUB OBJ
//-------------------------------------------------------------------------------
SubObjTemplate::SubObjTemplate(){
	materialId = -1;
}

SubObjTemplate::~SubObjTemplate(){
}


//-------------------------------------------------------------------------------
//FRAME
//-------------------------------------------------------------------------------
Frame::Frame(){
	frameId = sTime = 0;
}

Frame::~Frame(){
	for(int i = 0; i<meshesTemplate.size(); i++){
		delete meshesTemplate[i];
	}
}

