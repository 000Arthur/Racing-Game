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
	
	Board* board;

	Texture2D car;
	Texture2D bike;
	Texture2D circuit;
	float vel = 0.0f;
	float vel2 = 0.0f;

	const float MAX_VELOCITY = 2.0f;
	const float FRICTION_COEFFICIENT = 0.2f;
	const float MAX_ANGULAR_VELOCITY = 1.0f;

	bool accelerate = false;

	Camera2D camera1 = { 0 };

	Car* player;
	Car* player2;

	Bike* cono;

	vec2<int> ray;
	bool ray_on;
};
