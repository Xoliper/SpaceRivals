#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <GLFW/glfw3.h>
#include "Config.hpp"

class Timer {
public:
	Timer(int physicsTick = 60, float dataRefresh = 1.0f);
	~Timer();

	void Update();

	bool GetPhysicsUpdateState();
	double GetDelta();
	double GetInterval();
	double GetFPS();

private:
	//Delta
    double deltaTime, nowTime, lastTime, backupTime;
	int nbFrames, nbFramesBackup;
	float dataRefresh;

	//Physics
	float physicsTick, accumulator;
	double totalTime;
	bool pUpdateState;
};




#endif /* TIMER_HPP_ */
