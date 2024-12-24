#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#include "raylib.h"

#define MAX_FX_SOUNDS   64

ModuleAudio::ModuleAudio(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	fx_count = 0;
	music = Music{ 0 };
}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init()
{
	LOG("Loading Audio Mixer");
	bool ret = true;

    LOG("Loading raylib audio system");

    InitAudioDevice();

	return ret;
}

void ModuleAudio::UpdateMusic()
{
	if (IsMusicReady(music)) {
		UpdateMusicStream(music);
	}
}


// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

    // Unload sounds
	for (unsigned int i = 0; i < fx_count; i++)
	{
		UnloadSound(fx[i]);
	}

    // Unload music
	if (IsMusicReady(music))
	{
		StopMusicStream(music);
		UnloadMusicStream(music);
	}

    CloseAudioDevice();

	return true;
}

// Play a music file
bool ModuleAudio::PlayMusic(const char* path, float fade_time)
{
	if(IsEnabled() == false)
		return false;

	if (IsMusicReady(music)) {
		StopMusicStream(music);
		UnloadMusicStream(music);
	}

	music = LoadMusicStream(path);
	if (IsMusicReady(music))
	{
		PlayMusicStream(music);
		LOG("Successfully playing %s", path);
		SetMusicVolume(music,0.3f); 
	}
	return true;

}

// Load WAV
unsigned int ModuleAudio::LoadFx(const char* path)
{
	if(IsEnabled() == false)
		return 0;

	unsigned int ret = 0;

	Sound sound = LoadSound(path);

	if(sound.stream.buffer == NULL)
	{
		LOG("Cannot load sound: %s", path);
	}
	else
	{
        fx[fx_count] = sound;
		ret = fx_count++;
	}

	return ret;
}

// Play WAV
bool ModuleAudio::PlayFx(unsigned int id, int repeat)
{
	if (IsEnabled() == false)
	{
		return false;
	}

	bool ret = false;

	if(id < fx_count) PlaySound(fx[id]);

	return ret;
}
void ModuleAudio::StopFx(unsigned int fx_id)
{
	if (fx_id < fx_count) {
		StopSound(fx[fx_id]); 
	}
}

void ModuleAudio::SoundsFx()
{
	accelerate_fx = LoadFx("Assets/Audio/Fx/Accelerating.mp3");
	accelerate_fx_2 = LoadFx("Assets/Audio/Fx/Accelerating.mp3");


	collision_cars_fx = LoadFx("Assets/Audio/Fx/car-crash.mp3");

	collision_object_fx = LoadFx("Assets/Audio/Fx/crash.mp3");

	start_engines_fx = LoadFx("Assets/Audio/Fx/engine-start.mp3");
	start_engines_fx_2 = LoadFx("Assets/Audio/Fx/engine-start.mp3");

	in_Reverse_fx = LoadFx("Assets/Audio/Fx/Reverse.mp3");
	in_Reverse_fx_2 = LoadFx("Assets/Audio/Fx/Reverse.mp3");

	finish_line_fx= LoadFx("Assets/Audio/Fx/Finishline.mp3");

}