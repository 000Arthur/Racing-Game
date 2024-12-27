// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "Timer.h"

#include "raylib.h"

Timer::Timer()
{
	Start();
}

void Timer::Start()
{
	started_at = GetTime();
	running = true;
}

void Timer::Stop()
{
	running = false;

}

double Timer::ReadSec() const
{
	if (running) {
		return GetTime() - started_at;
	}
	return 0.0;
}

void Timer::Restart() {
	running = false;
	ReadSec();
}