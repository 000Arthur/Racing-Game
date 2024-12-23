#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class PhysicEntity;
class Car;
class Bike;
class Board;
class Bost;


class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);


public:

	std::vector<PhysicEntity*> entities;
	std::vector<Bost*> bosters;
	
	Board* board;

	Texture2D car;
	Texture2D greenCar;
	Texture2D tires[3];

	Texture2D bike;
	Texture2D circuit;

	int currentFrame = 0;
	float frameTime = 0.15f;   // Duraci�n de cada frame en segundos
	float timer = 0.0f;
	Texture2D frames[8];

	float vel = 0.0f;
	float vel2 = 0.0f;

	const float MAX_VELOCITY = 2.0f;
	const float FRICTION_COEFFICIENT = 0.2f;
	const float MAX_ANGULAR_VELOCITY = 1.0f;

	bool accelerateP1 = false;
	int cnt = 0;

	bool accelerateP2 = false;
	int cnt2 = 0;

	Camera2D camera1 = { 0 };

	Car* player;
	Car* player2;
	//Bost* impulser;
	Bike* cono;

	vec2<int> ray;
	bool ray_on;

	std::vector<Vector2> tiresPos = {
		{365, 604},
		{383, 592},
		{414, 578},
		{451, 570},
		{478, 570},
		{517, 571},
		{550, 583},
		{579, 596},
		{597, 606}
	};

	std::vector<Vector2> BostersPos = {
	{563, 300},
	{178, 960},
	{1283, 726}
	};
};
