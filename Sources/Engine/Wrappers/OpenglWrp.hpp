#ifndef WRAPPERS_OPENGL_HPP_
#define WRAPPERS_OPENGL_HPP_

//-----------------------------------------------------
// Defines & Includes
//-----------------------------------------------------

// Definitions
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include "../Common/World.hpp"
#include <dwmapi.h>

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

// Include GLEW & GLFW & GLM & GLIMG
#include <glload/gl_load.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Windows specific headers
#include <windows.h>

// Namespaces
using namespace glm;


//-----------------------------------------------------
// Classes
//-----------------------------------------------------

class OpenglManager {

public:
	OpenglManager();
	virtual ~OpenglManager();

	void CreateGLWindow(std::string name, float fov, WorldRect wr, int monitorId);
	void SetupWindow();
	void Swap();
	void SetVSync(bool mode);
	bool IfOpen();
	void Clear();
	void ClearWithoutDepth();
	void SetDebugMode(bool mode);

	GLFWwindow * GetHandle();
	HWND GetHWND();
	World * GetWorldHandle();
	WorldRect * GetWindowRect();

private:
	GLFWwindow* window;
	World * world;
	WorldRect wr;


};

#endif /* WRAPPERS_OPENGL_HPP_ */
