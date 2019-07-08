/*
 * GUI.cpp
 *
 *  Created on: 31 sie 2018
 *      Author: Xoliper
 */

#include "Gui.hpp"

Gui::Gui(World * world){
	this->world = world;

	//Load Textures
	textures.insert(std::pair<std::string, Texture*>("viewFinder", new Texture("Data\\Textures\\viewFinder.png")));

	//Load Gui
	boldFont = new Font("Data\\Fonts\\Impact.ttf", 36);
	fpsDisplay = new Text(world, boldFont, "", world->width-20, world->height-30, 2);
	fpsDisplay->Colorize(255,255,255,255);
	intervalDisplay = new Text(world, boldFont, "", world->width-100, world->height-30, 2);
	intervalDisplay->Colorize(255,255,255,255);
	staticSprites.push_back(new Sprite(world, {world->width/2-textures.at("viewFinder")->dimm.width/2,world->height/2-textures.at("viewFinder")->dimm.height/2,textures.at("viewFinder")->dimm.width,textures.at("viewFinder")->dimm.height}, textures.at("viewFinder"), 0));
	staticSprites.back()->SetPosition(1.0f, 0.9f, 0.0f);

	advWpnRect = new RectanglePrim(world, WorldRect(10, world->height-50, 180, 10), Color(255,255,255,180), 1);
	stdWpnRect = new RectanglePrim(world, WorldRect(10, world->height-20, 180, 10), Color(255,255,255,180), 1);

}

Gui::~Gui(){
	for(auto it = staticSprites.begin(); it!= staticSprites.end(); it++){
		delete (*it);
	}
	for(auto it = textures.begin(); it!= textures.end(); it++){
		delete (*it).second;
	}
	delete fpsDisplay;
	delete boldFont;
}

void Gui::UpdateAdvWpnReload(float reload, float actReload){
	float ratio = (float)actReload/reload;
	if(ratio > 1.0f) ratio = 1.0f;
	advWpnRect->Resize(ratio * 180, 10);
}
void Gui::UpdateStdWpnReload(float reload, float actReload){
	float ratio = (float)actReload/reload;
	if(ratio > 1.0f) ratio = 1.0f;
	stdWpnRect->Resize(ratio * 180, 10);
}

void Gui::Render(){
	glDepthMask(false);
	for(auto it = staticSprites.begin(); it!= staticSprites.end(); it++){
		(*it)->Render();
	}
	fpsDisplay->Render();
	intervalDisplay->Render();
	advWpnRect->Render();
	stdWpnRect->Render();
	glDepthMask(true);
}

void Gui::UpdateFps(int FPS, float interval){
	fpsDisplay->SetText("FPS: "+std::to_string(FPS));
	intervalDisplay->SetText("("+std::to_string(interval)+"ms)");
}
