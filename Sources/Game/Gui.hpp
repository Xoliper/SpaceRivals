/*
 * GUI.hpp
 *
 *  Created on: 31 sie 2018
 *      Author: Xoliper
 */

#ifndef GUI_HPP_
#define GUI_HPP_

#include <list>
#include <map>
#include <string>

#include "../Engine/Common/World.hpp"
#include "../Engine/Common/Graphics/Sprite.hpp"
#include "../Engine/Common/Graphics/Texture.hpp"
#include "../Engine/Common/Graphics/Text.hpp"
#include "../Engine/Common/Graphics/Font.hpp"
#include "../Engine/Common/Graphics/Primitive.hpp"

class Gui {
public:
	Gui(World * world);
	~Gui();

	void UpdateAdvWpnReload(float reload, float actReload);
	void UpdateStdWpnReload(float reload, float actReload);

	void Render();
	void UpdateFps(int FPS, float interval);

private:
	//Main vars
	World * world;
	Font * boldFont;
	std::map<std::string, Texture*> textures;


	//To display
	std::list<Sprite*> staticSprites;
	Text * fpsDisplay;
	Text * intervalDisplay;

	RectanglePrim * advWpnRect;
	RectanglePrim * stdWpnRect;

};

#endif /* GUI_HPP_ */
