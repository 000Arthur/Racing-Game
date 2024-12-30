#pragma once

#include "Module.h"
#include <vector>

#define MAX_SOUNDS	25
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
	void ChangeMusic();

	unsigned int start_fx;
	unsigned int aplause_fx;
	unsigned int traffic_light_fx;

	unsigned int accelerate_fx;
	unsigned int accelerate_fx_2;

	unsigned int collision_cars_fx;
	unsigned int collision_object_fx;

	unsigned int engine_fx;
	unsigned int engine_fx_2;

	unsigned int in_Reverse_fx;
	unsigned int in_Reverse_fx_2;

	unsigned int finish_line_fx;

	unsigned int checkpoint_fx;
	unsigned int puddle_fx;
	unsigned int crack_fx;

	unsigned int bost_fx;
	unsigned int bost_fx_2;

	//List music
	int current_music_index = -1;
	std::vector<const char*> music_paths = {
		"Assets/Audio/Music/background.ogg",
		"Assets/Audio/Music/Music1.mp3",
		"Assets/Audio/Music/Music3.mp3",
		"Assets/Audio/Music/Music6.mp3",
		"Assets/Audio/Music/Music8.mp3"
	};
private:
	Music music;
	Sound fx[MAX_SOUNDS];
	unsigned int fx_count;
};