#pragma once

#include "Globals.h"
#include "Timer.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	double ReadSec() const;
	void Stop();
	void Restart();
private:
	bool running = false;

	// Start time in seconds
	double started_at;
};