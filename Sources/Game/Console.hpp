/*
 * Console.hpp
 *
 *  Created on: 10 lip 2018
 *      Author: Xoliper
 */

#ifndef CONSOLE_HPP_
#define CONSOLE_HPP_

#include <list>
#include "../Engine/Common/Light.hpp"
#include "../Engine/Common/Objects/ObjTemplate.hpp"

extern std::list<ObjTemplate *> objsTemplates;
extern Light * globalLight;
extern Light * playerLight;


bool exitFlag = false;
void __cdecl Console( void * Args )
{

	Material * matToChange = NULL;

	while(!exitFlag){

		std::string cmd;
		std::getline( std::cin, cmd );


		//MISC COMMANDS
		if(cmd == "commands"){
			std::cout<<"Commands:"<<std::endl;
			std::cout<<"	list mat"<<std::endl;
			std::cout<<"	disp mat <name>"<<std::endl;
			std::cout<<"	change mat <name>"<<std::endl;
			std::cout<<"	Ka <r g b>"<<std::endl;
			std::cout<<"	Kd <r g b>"<<std::endl;
			std::cout<<"	Ks <r g b>"<<std::endl;
			std::cout<<"	stop change mat"<<std::endl;
			std::cout<<"	light pow <power>"<<std::endl;
			std::cout<<"	light pos <x y z>"<<std::endl;
			std::cout<<"	light color <r g b>"<<std::endl;
		}


		//--------------------------------------
		//MATERIALS OPTIONS
		//--------------------------------------

		//List command
		if(cmd == "list mat"){
			std::cout<<"Material list:"<<std::endl;
			for(auto i : objsTemplates){
				for(auto j : i->materials){
					std::cout<<"  "<<j->materialName<<std::endl;
				}
			}
		}

		//Disp material command
		if(cmd.compare(0, 8, "disp mat") == 0){
			std::string materialName = cmd.substr(9, cmd.size()-8-1);
			//Find material
			Material * tempMat = NULL;
			for(auto i : objsTemplates){
				for(auto j : i->materials){
					if(j->materialName == materialName){
						tempMat = j;
						break;
					}
					if(tempMat != NULL) break;
				}
			}
			if(tempMat == NULL){
				std::cout<<"Cannot find '"<<materialName<<"' material!"<<std::endl;
			} else {
				std::cout<<"Material '"<<materialName<<"':"<<std::endl;
				std::cout<<"  Ka: "<<tempMat->mcd.Ka.r<<"  "<<tempMat->mcd.Ka.g<<"  "<<tempMat->mcd.Ka.b<<std::endl;
				std::cout<<"  Kd: "<<tempMat->mcd.Kd.r<<"  "<<tempMat->mcd.Kd.g<<"  "<<tempMat->mcd.Kd.b<<std::endl;
				std::cout<<"  Ks: "<<tempMat->mcd.Ks.r<<"  "<<tempMat->mcd.Ks.g<<"  "<<tempMat->mcd.Ks.b<<std::endl;
			}
		}


		//Set change material command
		if(cmd.compare(0, 10, "change mat") == 0){
			std::string materialName = cmd.substr(11, cmd.size()-10-1);
			//Find material
			Material * tempMat = NULL;
			for(auto i : objsTemplates){
				for(auto j : i->materials){
					if(j->materialName == materialName){
						tempMat = j;
						break;
					}
					if(tempMat != NULL) break;
				}
			}
			if(tempMat == NULL){
				std::cout<<"Cannot find '"<<materialName<<"' material!"<<std::endl;
			} else {
				matToChange = tempMat;
				std::cout<<"  ->  "<<std::endl;
			}
		}

		//Change material command
		if(cmd.compare(0, 2, "Ka") == 0){
			std::replace( cmd.begin(), cmd.end(), '.', ',');
			if(matToChange != NULL){
				float r = 0.0f, g = 0.0f, b = 0.0f;
				sscanf(&cmd[2], "%f %f %f\n", &r, &g, &b );
				matToChange->mcd.Ka.r = r;
				matToChange->mcd.Ka.g = g;
				matToChange->mcd.Ka.b = b;
			} else std::cout<<"First select material to change!"<<std::endl;
		}

		if(cmd.compare(0, 2, "Kd") == 0){
			std::replace( cmd.begin(), cmd.end(), '.', ',');
			if(matToChange != NULL){
				float r = 0.0f, g = 0.0f, b = 0.0f;
				sscanf(&cmd[2], "%f %f %f\n", &r, &g, &b );
				matToChange->mcd.Kd.r = r;
				matToChange->mcd.Kd.g = g;
				matToChange->mcd.Kd.b = b;
			} else std::cout<<"First select material to change!"<<std::endl;
		}

		if(cmd.compare(0, 2, "Ks") == 0){
			std::replace( cmd.begin(), cmd.end(), '.', ',');
			if(matToChange != NULL){
				float r = 0.0f, g = 0.0f, b = 0.0f;
				sscanf(&cmd[2], "%f %f %f\n", &r, &g, &b );
				matToChange->mcd.Ks.r = r;
				matToChange->mcd.Ks.g = g;
				matToChange->mcd.Ks.b = b;
			} else std::cout<<"First select material to change!"<<std::endl;
		}

		//Done with material change
		if(cmd.compare(0, 15, "stop change mat") == 0){
			matToChange = NULL;
			std::cout<<"  <-  "<<std::endl;
		}


		//--------------------------------------
		//LIGHT OPTIONS
		//--------------------------------------
		if(cmd.compare(0, 9, "light pow") == 0){
			std::cout<<cmd<<std::endl;
			std::replace( cmd.begin(), cmd.end(), '.', ',');
			float power = 0.0f;
			sscanf(&cmd[10], "%f\n", &power);
			globalLight->SetLightPower(power);
			playerLight->SetLightPower(power);
		}

		if(cmd.compare(0, 9, "light pos") == 0){
			std::cout<<cmd<<std::endl;
			std::replace( cmd.begin(), cmd.end(), '.', ',');
			float x = 0.0f, y = 0.0f, z = 0.0f;
			sscanf(&cmd[10], "%f %f %f\n", &x, &y, &z );
			std::cout<<"Set: "<<x<<"   "<<y<<"   "<<z<<std::endl;

			globalLight->SetLightPos(x, y, z);
			playerLight->SetLightPos(x, y, z);
		}

		if(cmd.compare(0, 11, "light color") == 0){
			std::cout<<cmd<<std::endl;
			std::replace( cmd.begin(), cmd.end(), '.', ',');
			float r = 0.0f, g = 0.0f, b = 0.0f;
			sscanf(&cmd[12], "%f %f %f\n", &r, &g, &b );
			globalLight->color.SetColor(r, g, b, 1.0f);
			playerLight->color.SetColor(r, g, b, 1.0f);
		}

	}

    _endthread();
}


#endif /* CONSOLE_HPP_ */
