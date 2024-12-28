#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "Timer.h"

#include <math.h>
#include <string>


ModuleRender::ModuleRender(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    background = RAYWHITE;
    timer.Start();
    timer2.Start();

}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
    myFont = LoadFont("Assets/Font/SpeedyRegular-7BLoE.ttf");
    if (myFont.baseSize == 0) {
        LOG("Failed to load font.");
        return false;
    }
	bool ret = true;

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRender::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Update: debug camera
update_status ModuleRender::Update()
{
    ClearBackground(background);

    // NOTE: This function setups render batching system for
    // maximum performance, all consecutive Draw() calls are
    // not processed until EndDrawing() is called
    BeginDrawing();

	return UPDATE_CONTINUE;
}

void ModuleRender::Timer_Player1()
{

    // Display the elapsed time
    time_elapsed1 = timer.ReadSec();

    // Calculate minutes, seconds, and milliseconds
    int minutes = static_cast<int>(time_elapsed1) / 60;
    int seconds = static_cast<int>(time_elapsed1) % 60;
    int milliseconds = static_cast<int>((time_elapsed1 - static_cast<int>(time_elapsed1)) * 1000);

    // Format the time string to "mm:ss:ms"
    std::string time_text =
        (minutes < 10 ? "TIME PLAYER 1: 0" : "") + std::to_string(minutes) + ":" +
        (seconds < 10 ? "0" : "") + std::to_string(seconds) + ":" +
        (milliseconds < 100 ? "00" : (milliseconds < 10 ? "0" : "")) + std::to_string(milliseconds);

    // Display the formatted time on the screen
    DrawText(time_text.c_str(), 20, 50, myFont, 1, BLACK);

}

void ModuleRender::Timer_Player2()
{

    // Display the elapsed time
    time_elapsed2 = timer2.ReadSec();


    // Calculate minutes, seconds, and milliseconds
    int minutes = static_cast<int>(time_elapsed2) / 60;
    int seconds = static_cast<int>(time_elapsed2) % 60;
    int milliseconds = static_cast<int>((time_elapsed2 - static_cast<int>(time_elapsed2)) * 1000);

    // Format the time string to "mm:ss:ms"
    std::string time_text =
        (minutes < 10 ? "TIME PLAYER 2: 0" : "") + std::to_string(minutes) + ":" +
        (seconds < 10 ? "0" : "") + std::to_string(seconds) + ":" +
        (milliseconds < 100 ? "00" : (milliseconds < 10 ? "0" : "")) + std::to_string(milliseconds);

    // Display the formatted time on the screen
    DrawText(time_text.c_str(), 1425, 50, myFont, 1, BLACK);
}

void ModuleRender::Best_Time()
{
    int minutes, seconds, milliseconds;
    if (player1_time < player2_time)
    {
         minutes = static_cast<int>(player1_time) / 60;
         seconds = static_cast<int>(player1_time) % 60;
         milliseconds = static_cast<int>((player1_time - static_cast<int>(player1_time)) * 1000);

      
    }
    else {
         minutes = static_cast<int>(player2_time) / 60;
         seconds = static_cast<int>(player2_time) % 60;
         milliseconds = static_cast<int>((player2_time - static_cast<int>(player2_time)) * 1000);
    }

    std::string time_text =
        (minutes < 10 ? "BEST TIME: 0" : "") + std::to_string(minutes) + ":" +
        (seconds < 10 ? "0" : "") + std::to_string(seconds) + ":" +
        (milliseconds < 100 ? "00" : (milliseconds < 10 ? "0" : "")) + std::to_string(milliseconds);

    // Display the formatted time on the screen
    DrawText(time_text.c_str(), 770, 50, myFont, 1, BLACK);
}

// PostUpdate present buffer to screen
update_status ModuleRender::PostUpdate()
{
    BeginDrawing();

    ClearBackground(background);

    Timer_Player1(); 
 
    Timer_Player2();

    Best_Time();
    
    // Draw everything in our batch!
    DrawFPS(10, 10);

    EndDrawing();

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
    timer.Stop();


    UnloadFont(myFont);
	return true;
}

void ModuleRender::SetBackgroundColor(Color color)
{
	background = color;
}

// Draw to screen
bool ModuleRender::Draw(Texture2D texture, int x, int y, const Rectangle* section, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;

	float scale = 1.0f;
    Vector2 position = { (float)x, (float)y };
    Rectangle rect = { 0.f, 0.f, (float)texture.width, (float)texture.height };

    if (section != NULL) rect = *section;

    position.x = (float)(x-pivot_x) * scale + camera.x;
    position.y = (float)(y-pivot_y) * scale + camera.y;

	rect.width *= scale;
	rect.height *= scale;

    DrawTextureRec(texture, rect, position, WHITE);

	return ret;
}

bool ModuleRender::DrawText(const char * text, int x, int y, Font font, int spacing, Color tint) const
{
    bool ret = true;

    Vector2 position = { (float)x, (float)y };

    DrawTextEx(font, text, position, (float)font.baseSize, (float)spacing, tint);

    return ret;
}
