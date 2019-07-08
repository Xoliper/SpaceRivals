/*
 * Map.cpp
 *
 *  Created on: 31 sie 2018
 *      Author: Xoliper
 */

#include "Map.hpp"

Map::Map(World * world){
	this->world = world;
	cmt = nullptr;
	sk = nullptr;
	sun = nullptr;
}

std::list<Drawable * > * Map::GetCollisionObjects(){
	return &lPpo;
}

void Map::LoadSkyBox(std::string path){
	faces.clear();
	faces.push_back(path+"\\right.png");
	faces.push_back(path+"\\left.png");
	faces.push_back(path+"\\bottom.png");
	faces.push_back(path+"\\top.png");
	faces.push_back(path+"\\front.png");
	faces.push_back(path+"\\back.png");

	if(cmt != nullptr) delete cmt;
	if(sk != nullptr) delete sk;
	cmt = new CubeMapTexture(faces);
	sk = new SkyBox(world, cmt);
}

void Map::GenerateAsteroids(ObjTemplate * objTemplate, int amount, float radiusBase, int radiusDeviation, float sizeBase, int sizeDeviation){
	MeshInstances * meiTemp = new MeshInstances(world, objTemplate->GetMeshData(0), &objTemplate->materials[0]->diffuseTex, &objTemplate->materials[0]->normalTex, &objTemplate->materials[0]->specularTex);
	for(int i = 0; i<amount; i++){

		double theta = 2 * M_PI * rng.GetRandom();
		double phi = acos(1 - 2 * rng.GetRandom());
		double x = sin(phi) * cos(theta);
		double y = sin(phi) * sin(theta);
		double z = cos(phi);


		float radius = (float)(rand() % radiusDeviation)/100 + radiusBase;
		float size = (float)(rand() % sizeDeviation)/1000 + sizeBase;


		meiTemp->particles_position_buffer_data.push_back(x*radius);
		meiTemp->particles_position_buffer_data.push_back(y*radius);	//HUGE
		meiTemp->particles_position_buffer_data.push_back(z*radius);
		meiTemp->particles_position_buffer_data.push_back(size); //HUGE

		PhantomPointObject * ppo = new PhantomPointObject(x*radius, y*radius, z*radius, size*0.12f);
		lPpo.push_back((Drawable *)ppo);

	}
	meiTemp->UpdateBuffers();
	asteroids.push_back(meiTemp);
}

void Map::LoadSun(ObjTemplate * objTemplate, float scale, float x, float y, float z){
	if(sun != nullptr) delete sun;
	sun = new VisualObject(world, objTemplate);
	sun->SetShader("object");
	sun->SetPosition(x, y, z);
	sun->SetScale(scale, scale, scale);
}

Map::~Map(){
	if(sk != nullptr) delete sk;
	if(cmt != nullptr) delete cmt;
}

void Map::Render(){
	sk->Render();
	for(auto it = asteroids.begin(); it != asteroids.end(); it++){
		(*it)->Render();
	}
}

void Map::RenderToBlur(){
	sun->Render();
}
