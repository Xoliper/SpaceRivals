//Standard Includes
#include <iostream>
#include <list>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <process.h>
#include <algorithm>



#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>

//Include Core Engine
#include "Engine/Common/Exceptions.hpp"
#include "Engine/Common/Utils/XmlParser.hpp"
#include "Engine/Common/Graphics/Color.hpp"
#include "Engine/Extensions/MonitorInfo.hpp"
#include "Engine/Common/Config.hpp"
#include "Engine/Common/Graphics/Font.hpp"
#include "Engine/Common/Graphics/Primitive.hpp"
#include "Engine/Common/Shaderloader.hpp"
#include "Engine/Common/Graphics/Sprite.hpp"
#include "Engine/Common/Graphics/Text.hpp"
#include "Engine/Common/Graphics/Texture.hpp"
#include "Engine/Common/Graphics/FrameBuffer.hpp"
#include "Engine/Common/World.hpp"
#include "Engine/Common/Timer.hpp"
#include "Engine/Wrappers/OpenglWrp.hpp"
#include "Engine/Common/Objects/ObjTemplate.hpp"
#include "Engine/Common/Objects/Object.hpp"

//Include Project files
#include "Game/Defs.hpp"
#include "Game/Console.hpp"
#include "Game/Gui.hpp"
#include "Game/Player.hpp"
#include "Game/Rocket.hpp"
#include "Game/Map.hpp"
#include "Game/ResContainer.hpp"

using namespace std;


//Console vars
ResContainer * rc;
std::list<ObjTemplate *> objsTemplates;
Light * globalLight = NULL;
Light * playerLight = NULL;
bool displayDepth = false;


int main(int argc, char **argv) {

	//Add console
	( HANDLE ) _beginthread( Console, 0, NULL );

//--------------------------------------------------------------------------------------------------------
//Initialize engine
//--------------------------------------------------------------------------------------------------------
	MonitorInfo mi;
	int monitor = 0;
	int monitorsAmount = mi.MonitorsAmount();
	if(monitorsAmount != 0) mi.SelectMonitor(monitor);
	RECT mir = mi.GetMonitorRect();

	OpenglManager ogl;
	ogl.CreateGLWindow("SpaceRivals", 200.0f, {0,0, abs(mir.right-mir.left), abs(mir.bottom-mir.top)}, monitor);
	ogl.SetupWindow();
	ogl.SetVSync(!FPS_DEBUG);

	World & world = *ogl.GetWorldHandle();
	world.Change2DPosition(-1.0f, -1.0f);				//Setup 2D scene (to display Primitives, Sprites and Text)
	world.Change3DLookPosition(0.0f, 0.0f, 0.0f);		//Setup 3D scene (to render VisualObjects)
	world.Change3DCameraPosition(0.0f, -0.7f, 0.15f);	//Setup 3D scene (to render VisualObjects)

	world.light.SetLightPos(0.0f, 300.0f, 0.0f);		//Setup Light in 3D scene;
	world.light.SetLightPower(8000.0f);
	world.light.color.SetColor(255,255,255,255);
	globalLight = &world.light;

	ShaderLoader sl;
	sl.Load(argv[0]);

	Timer timer;

	enum fbType {
		MAIN1,
		BLURV,
		BLURH
	};

	Texture sceneTex;
	FrameBufferManager fbm;
	std::cerr<<"Framebuffer: "<<abs(mir.right-mir.left)<<"x"<<abs(mir.bottom-mir.top)<<std::endl;
	fbm.AddBuffer(abs(mir.right-mir.left), abs(mir.bottom-mir.top));	//MAIN1			<-- player and all others
	fbm.AddBuffer(abs(mir.right-mir.left), abs(mir.bottom-mir.top));	//BLURV			<-- to be blurred
	fbm.AddBuffer(abs(mir.right-mir.left), abs(mir.bottom-mir.top));	//BLURH


//--------------------------------------------------------------------------------------------------------
//Game variables here
//--------------------------------------------------------------------------------------------------------
	rc = new ResContainer();

	Map map(&world);
	map.LoadSkyBox("Data\\Maps\\61ijxzx29cg0");
	map.GenerateAsteroids(rc->GetTemplate("ROCK0"), 350, 3.0f, 300, 0.07f, 10);
	map.GenerateAsteroids(rc->GetTemplate("ROCK1"), 50, 3.0f, 300, 0.15f, 10);
	map.GenerateAsteroids(rc->GetTemplate("ROCK2"), 50, 3.0f, 300, 0.20f, 10);
	map.GenerateAsteroids(rc->GetTemplate("ROCK3"), 1500, 2.0f, 2000, 0.03f, 10);
	map.LoadSun(rc->GetTemplate("SUN"), 100.0f, 0.0f, 1100.0f, 0.0f);

	Gui gui(&world);
	Player player(&world, &timer, rc->GetTemplate("PLAYER_SHIP"), rc->GetTemplate("THRUSTER"));
	player.FixMouse(mir.left, mir.top);
	player.EquipAdwWpn(rc->GetTemplate("ARROW_SHIP"), rc->GetTemplate("ARROW_SINGLE"), rc->GetTemplate("ARROW_THRUSTER"), 1.0f);
	player.EquipStdWpn(rc->GetTemplate("LASER"), 0.1f);



//--------------------------------------------------------------------------------------------------------
//Main main loop
//--------------------------------------------------------------------------------------------------------
	do {

	//--------------------------------------------------------------------------------------------------------
	//Physics & CPU section
	//--------------------------------------------------------------------------------------------------------

		timer.Update();
		fbm.ClearBuffers();


		player.Update(true, &map);
		gui.UpdateAdvWpnReload(player.advWpn->reload, player.advWpn->actualReload);
		gui.UpdateStdWpnReload(player.stdWpn->reload, player.stdWpn->actualReload);
		gui.UpdateFps(timer.GetFPS(), timer.GetInterval());

		if(GetAsyncKeyState(VK_CONTROL)){
			if(displayDepth){
				displayDepth = false;
			} else {
				displayDepth = true;
			}
			Sleep(150);
		}


	//--------------------------------------------------------------------------------------------------------
	//Render section
	//--------------------------------------------------------------------------------------------------------

	//NORMAL RENDERING------------------------------------
		fbm.SetActiveColorBuffer(MAIN1);
		map.Render();
		player.Render();

	//END NORMAL RENDERING--------------------------------

	//BLUR RENDERING--------------------------------Render object to be blurred
		fbm.SetActiveColorBuffer(BLURH);
		map.RenderToBlur();
		player.RenderToBlur();
	//END BLUR RENDERING

		fbm.SetActiveColorBuffer(MAIN1);
		gui.Render();
		//glDepthMask(false);
		//explosionSprite->Render();
		//glDepthMask(true);

	//POST PROCESSING--------------------------------------
		glDepthMask(false);
		Sprite renderSprite(&world, {abs(mir.right-mir.left)/2, abs(mir.bottom-mir.top)/2, abs(mir.right-mir.left), abs(mir.bottom-mir.top)}, &sceneTex,1);
		GLuint depthBufTexId = fbm.GetDepthStencilTexture(0);
		renderSprite.SetBlurDepthTex(depthBufTexId);
		fbm.SetActiveColorBuffer((int)BLURH);
		sceneTex.texture_id = fbm.GetFrameBufferColor(BLURH);		//Work on raw image
		renderSprite.SetShader("spriteBlur");
		renderSprite.SetShaderParam(1.0f/800.0f, 0.0f);
		renderSprite.Render();										//Horizontal blur in BLURH
		fbm.SetActiveColorBuffer((int)BLURV);
		sceneTex.texture_id = fbm.GetFrameBufferColor(BLURH);
		renderSprite.SetShaderParam(0.0f, 1.0f/(800.0f * ((float)abs(mir.bottom-mir.top)/abs(mir.right-mir.left))));
		renderSprite.Render();										//Vertical & horizontal blur in BLURV
		glDepthMask(true);
	//END POST PROCESSING------------------------------------

	//2D RENDERING-------------------------------------------
		fbm.SetDefaultFB();
		ogl.Clear();

		renderSprite.SetShader("sprite");
		sceneTex.texture_id = fbm.GetFrameBufferColor(MAIN1);
		renderSprite.Render();

		sceneTex.texture_id = fbm.GetFrameBufferColor(BLURV);
		renderSprite.Render();

		if(displayDepth){
			sceneTex.texture_id = fbm.GetDepthStencilTexture(0);
			renderSprite.Render();
		}
	//END 2D RENDERING----------------------------------------

		ogl.Swap();
	} while(ogl.IfOpen() and !GetAsyncKeyState(VK_ESCAPE));

	exitFlag = true;
	//TODO: wait for console thread to end

	return 0;
}






















/*
		if(GetAsyncKeyState(VK_RBUTTON)){
			rockets.push_back(new Rocket(&world, &arrows, &arrowt, player.pos, player.addPos, player.rot, player.addRot, player.addRotRa, world.dynamicContext.view));
			//Sleep(100);
		}
		for(auto i = rockets.begin(); i!=rockets.end(); i++){
			(*i)->Render();
		}
*/
