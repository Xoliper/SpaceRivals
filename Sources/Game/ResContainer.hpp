#ifndef GAME_RESCONTAINER_HPP_
#define GAME_RESCONTAINER_HPP_

#include <map>
#include "../Engine/Common/Objects/ObjTemplate.hpp"
#include "../Engine/Common/Exceptions.hpp"
#include "../Engine/Common/Graphics/Texture.hpp"

class ResContainer {
public:

	ResContainer();
	~ResContainer();

	ObjTemplate * GetTemplate(std::string name);
	Texture * GetTexture(std::string texName);

private:
	std::map<std::string, ObjTemplate *> objTemplates;
	std::map<std::string, Texture *> texTemplates;

};

#endif /* GAME_RESCONTAINER_HPP_ */
