#include "OpenglWrp.hpp"

OpenglManager::OpenglManager() {

	HRESULT hr = DwmEnableComposition(DWM_EC_DISABLECOMPOSITION);
	if (!SUCCEEDED(hr)) {
	  std::cerr<<"Failed to disable composition"<<std::endl;
	}

	window = NULL;
	world = NULL;

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
	}

	glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	//glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR,GLFW_RELEASE_BEHAVIOR_FLUSH);
	glfwWindowHint(GLFW_SAMPLES, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);


	/*int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
*/
}

OpenglManager::~OpenglManager() {
	delete world;
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

void OpenglManager::CreateGLWindow(std::string name, float fov, WorldRect wr, int monitorId){
	// Open a window and create its OpenGL context

	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);

	//window = glfwCreateWindow( wr.w, wr.h, name.c_str(), NULL, NULL);

	window = glfwCreateWindow( wr.w, wr.h, name.c_str(), monitors[monitorId], NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have some Intel GPU, they are not 3.3 compatible.\n" );
		glfwTerminate();
	}
	glfwSetWindowPos(window, wr.x, wr.y);




	// Initialize GLEW
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	world = new World();
	world->SetupFov(fov, wr.w, wr.h);
	this->wr = wr;

}


void OpenglManager::SetupWindow(){
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f,0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	glShadeModel( GL_SMOOTH);
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


	if(ogl_LoadFunctions() == ogl_LOAD_FAILED){
		std::cerr<<"Fail to load GLIMG"<<std::endl;
	}
}

void OpenglManager::Swap(){
	// Swap buffers
	//glFlush();
	glfwSwapBuffers(window);
	glfwPollEvents();
	//glFinish();

}

void OpenglManager::Clear(){
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenglManager::ClearWithoutDepth(){
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenglManager::SetDebugMode(bool mode){
	if(mode == true){
		glEnable( GL_POLYGON_SMOOTH );
		glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	} else {
		glDisable( GL_POLYGON_SMOOTH );
		glHint( GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE );
	}
}

void OpenglManager::SetVSync(bool mode){
    glfwSwapInterval(mode); //0 - off, 1 - on
}

GLFWwindow* OpenglManager::GetHandle(){
	return window;
}

HWND OpenglManager::GetHWND(){
	return glfwGetWin32Window(window);
}

World * OpenglManager::GetWorldHandle(){
	return world;
}

WorldRect * OpenglManager::GetWindowRect(){
	return &wr;
}


bool OpenglManager::IfOpen(){
	return !glfwWindowShouldClose(GetHandle());
}
