/*
 * ResContainer.cpp
 *
 *  Created on: 31 sie 2018
 *      Author: Xoliper
 */

#include "ResContainer.hpp"

ResContainer::ResContainer(){
	std::string objPath = "Data\\Objects\\";
	objTemplates.insert(std::pair<std::string, ObjTemplate*>("THRUSTER", new ObjTemplate(objPath+"thruster.fof")));
	objTemplates.insert(std::pair<std::string, ObjTemplate*>("PLAYER_SHIP", new ObjTemplate(objPath+"ship01_a.fof")));
	objTemplates.insert(std::pair<std::string, ObjTemplate*>("SUN", new ObjTemplate(objPath+"sun.fof")));
	objTemplates.insert(std::pair<std::string, ObjTemplate*>("ARROW_SHIP", new ObjTemplate(objPath+"arrowShip.fof")));
	objTemplates.insert(std::pair<std::string, ObjTemplate*>("ARROW_SINGLE", new ObjTemplate(objPath+"arrowSingle.fof")));
	objTemplates.insert(std::pair<std::string, ObjTemplate*>("ARROW_THRUSTER", new ObjTemplate(objPath+"arrowThruster.fof")));
	objTemplates.insert(std::pair<std::string, ObjTemplate*>("ROCK0", new ObjTemplate(objPath+"Rock0.fof")));
	objTemplates.insert(std::pair<std::string, ObjTemplate*>("ROCK1", new ObjTemplate(objPath+"Rock1.fof")));
	objTemplates.insert(std::pair<std::string, ObjTemplate*>("ROCK2", new ObjTemplate(objPath+"Rock2.fof")));
	objTemplates.insert(std::pair<std::string, ObjTemplate*>("ROCK3", new ObjTemplate(objPath+"Rock3.fof")));
	objTemplates.insert(std::pair<std::string, ObjTemplate*>("LASER", new ObjTemplate(objPath+"laser.fof")));

	std::string texPath = "Data\\Textures\\";
	texTemplates.insert(std::pair<std::string, Texture*>("EXPLOSION1", new Texture(texPath+"1.png")));
	texTemplates.insert(std::pair<std::string, Texture*>("EXPLOSION2", new Texture(texPath+"2.png")));
	texTemplates.insert(std::pair<std::string, Texture*>("EXPLOSION3", new Texture(texPath+"3.png")));
	texTemplates.insert(std::pair<std::string, Texture*>("EXPLOSION4", new Texture(texPath+"4.png")));

}

ResContainer::~ResContainer(){
}

ObjTemplate * ResContainer::GetTemplate(std::string objName){
	try {
		return objTemplates.at(objName);
	} catch(std::string & exceptionStr){
		throw Exception("ResContainer", "GetTemplate", "Cannot find object: "+objName);
	}
	return nullptr;
}

Texture * ResContainer::GetTexture(std::string texName){
	try {
		return texTemplates.at(texName);
	} catch(std::string & exceptionStr){
		throw Exception("ResContainer", "TexTemplate", "Cannot find texture: "+texName);
	}
	return nullptr;
}
