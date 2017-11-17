#pragma once
#include <string>

using namespace std;

static const int msecArraySize = 600;

static int msecArrayLength = 0;
static int msecArrayLocation = 0;
static float msecArray[msecArraySize];

static void storeMsec(float msec) {

	msecArrayLocation %= msecArraySize;
	msecArray[msecArrayLocation] = msec;
	msecArrayLocation++;
	if (msecArrayLength < msecArraySize) msecArrayLength++;
}
static std::string getFPS() {

	float totalTime = 0.0f;
	float fps;
	for (int i = 0; i < msecArrayLength; i++) {
		totalTime += msecArray[i];
	}

	fps = 1000.f / (totalTime / msecArrayLength);
	return to_string(fps);
}


