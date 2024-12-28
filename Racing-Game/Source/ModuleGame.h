#pragma once

#include "Globals.h"
#include "Module.h"
#include "ModuleRender.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class PhysicEntity;
class Car;
class Bike;
class Board;
class Bost;
class outRoad;
class onRoad;

enum Enum{
	NO_INTERACTION,PLAYER_1, PLAYER_2, 
	SPEED_BOOST,NITRO_BOOST,HIT, TIRE, FINISH_LINE,CHECK,
	PUDDLE = 15, CRACK,ON_ROAD,OUT_ROAD,
};


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
	bool finish = false;

	std::vector<PhysicEntity*> entities;
	std::vector<Bost*> bosters;
	
	Board* board;
	onRoad* piano;
	outRoad* limit;
	Texture2D startLight[6];
	Texture2D car1[2];
	Texture2D car2[2];
	Texture2D tires[3];
	Texture2D speedBoost;
	Texture2D nitroBoost;

	Texture2D bike;
	Texture2D circuit;

	int currentFrame = 0;
	float frameTime = 1.0f;   // Duración de cada frame en segundos
	float timer = 0.0f;
	Texture2D frames[6];

	float vel = 0.0f;
	float vel2 = 0.0f;

	const float FRICTION_COEFFICIENT = 0.2f;
	const float MAX_ANGULAR_VELOCITY = 1.0f;

	unsigned int accelerate_fx_id = 0;

	int rectX = 200; // Posición X
	int rectY = 100; // Posición Y
	int rectHeight = 20; // Altura del rectángulo

	bool check = false;
	bool check_2 = false;
	Camera2D camera1 = { 0 };

	Car* player;
	Car* player2;
	//Bost* impulser;
	Bike* cono;

	vec2<int> ray;
	bool ray_on;
	int entitieQ = 34;
	bool out = false;

	int currentCheckpointIndex = 0;
	int currentCheckpointIndex2 = 0;
	std::vector<bool> checkpointStates;
	std::vector<bool> checkpointStates2;

	std::vector<Vector2> tiresPos = {

		{365, 604},
		{383, 592},
		{414, 578},
		{451, 570},
		{478, 570},
		{517, 571},
		{550, 583},
		{579, 596},
		{597, 606},
		{285, 797},
		{285, 822},
		{285, 851},
		{285, 872},
		{285, 902},
		{1388, 865},
		{1412, 865},
		{1436, 865},
		{1460, 865},
		{1484, 865},
		{1508, 865},
		{1532, 865},
		{1556, 865},
		{1580, 865},
		{1604, 865},
		{1628, 865},
		{1652, 865},
		{1676, 865}


	};

	std::vector<Vector2> BostersPos = {
		{563, 300},
		{178, 960},
		{1283, 726},
		{964, 564},
		{479, 608},
		{1631, 991}
	};


	std::vector<Vector2> checkpointPos = {
		{850, 325},
		{1150, 720},
		{1816, 730},
		{1728, 958},
		{652, 955},
		{484, 670},
		{358, 996}
	};

	std::vector<Vector2> crackpointPos = {
		{850, 240},
		{900, 280},
		{1775, 880}		
	};

};
