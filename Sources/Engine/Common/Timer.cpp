#include "Timer.hpp"

Timer::Timer(int physicsTick, float dataRefresh){
	this->dataRefresh = dataRefresh;
	this->physicsTick = (float)1/physicsTick;

	//Time measuring vars
	nbFrames = nbFramesBackup = 0;
	lastTime = glfwGetTime();
	deltaTime = nowTime = 0;
	backupTime = lastTime;

	//Physics measuring vars
	accumulator = 0.0f;
	totalTime = 0.0;
	pUpdateState = true;
}

Timer::~Timer(){
}

void Timer::Update(){
	//Performance GPU FPS calculations
    nowTime = glfwGetTime();
    deltaTime = (nowTime - lastTime);
    lastTime = nowTime;

    //Check if we need to update Physics
   if(accumulator > physicsTick){
    	pUpdateState = true;
    	accumulator -= physicsTick;
    } else {
    	pUpdateState = false;
    }
   accumulator += deltaTime;
   totalTime += deltaTime;

    //Prepare FPS/Interval stats
    nbFrames++;
	if ( nowTime - backupTime >= dataRefresh ){
		nbFramesBackup = nbFrames;
		nbFrames = 0;
		backupTime = glfwGetTime();
	}
}

bool Timer::GetPhysicsUpdateState(){
	return pUpdateState;
}

double Timer::GetDelta(){
	return deltaTime;
}

double Timer::GetInterval(){
	return 1000.0/double(nbFramesBackup);
}

double Timer::GetFPS(){
	return (double)nbFrames/(nowTime - backupTime);
}




