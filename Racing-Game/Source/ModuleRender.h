#pragma once
#include "Module.h"
#include "Globals.h"
#include "Timer.h"

#include <limits.h>

class ModuleRender : public Module
{
public:
	ModuleRender(Application* app, bool start_enabled = true);
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

    void SetBackgroundColor(Color color);
	bool Draw(Texture2D texture, int x, int y, const Rectangle* section = NULL, double angle = 0, int pivot_x = 0, int pivot_y = 0) const;
    bool DrawText(const char* text, int x, int y, Font font, int spacing, Color tint) const;

	void Timer_Player1();
	void Timer_Player2();
	void Best_Time();

public:

	Timer timer;
	Timer timer2;
	double game_timer = 0.0;
	double time_elapsed1;
	double time_elapsed2;
	double player1_time = 0.0;
	double player2_time = 0.0;

	Font myFont;

	Color background;
    Rectangle camera;
};