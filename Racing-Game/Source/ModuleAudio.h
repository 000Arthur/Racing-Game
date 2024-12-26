#pragma once

#include "Module.h"

#define MAX_SOUNDS	16
#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a sound in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded sound
	bool PlayFx(unsigned int fx, int repeat = 0);

	void StopFx(unsigned int fx_id);
	void SoundsFx();
	void UpdateMusic();

	unsigned int accelerate_fx;
	unsigned int accelerate_fx_2;

	unsigned int collision_cars_fx;
	unsigned int collision_object_fx;

	unsigned int start_engines_fx;
	unsigned int start_engines_fx_2;

	unsigned int engine_fx;
	unsigned int engine_fx_2;

	unsigned int in_Reverse_fx;
	unsigned int in_Reverse_fx_2;

	unsigned int finish_line_fx;

	unsigned int checkpoint_fx;

	unsigned int bost_fx;
	unsigned int bost_fx_2;

private:
	Music music;
	Sound fx[MAX_SOUNDS];
    unsigned int fx_count;
};
