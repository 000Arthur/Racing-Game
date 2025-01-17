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
class Obstacle;

enum Enum{
	NO_INTERACTION,PLAYER_1, PLAYER_2, 
	SPEED_BOOST,NITRO_BOOST, HIT, TIRE, FINISH_LINE,CHECK,
	PUDDLE = 15, CRACK,ON_ROAD,OUT_ROAD,
};
enum CarTextures {
	CAR_1_NORMAL, CAR_1_BOOST,
	CAR_2_NORMAL, CAR_2_BOOST,
	CAR_3_NORMAL, CAR_3_BOOST
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
	void pathing(Car* NPC, Vector2 path);
	void Leader();

	float distanceToCheckpoint(float x1, float y1, float x2, float y2);

public:
	bool finish = false;
	bool player1_is_first = false;

	std::vector<PhysicEntity*> entities;

	Obstacle* obstacles;

	Texture2D startLight[6];

	Texture2D carsTexture[6];

	Texture2D tires[3];
	Texture2D speedBoost;
	Texture2D nitroBoost;

	Texture2D circuit;

	int currentFrame = 0;
	float frameTime = 1.0f;   // Duraci�n de cada frame en segundos
	float timer = 0.0f;

	float vel = 0.0f;
	float vel2 = 0.0f;
	float vel3 = 0.0f;

	const float FRICTION_COEFFICIENT = 0.2f;
	const float MAX_FRICTION_COEFFICIENT = 1.0f;

	const float MAX_ANGULAR_VELOCITY = 1.0f;
	const int BOOST_DURANTION = 50;
	const float BOOST_RECOVER = 8.0f;
	const float MAX_BOOST_QUANTITY = 20.0f;

	const float CAR_VELOCITY = 2.0f;
	const float CAR_TURN = 0.2F;

	int rectX = 200; // Posici�n X
	int rectY = 100; // Posici�n Y
	int rectHeight = 20; // Altura del rect�ngulo

	bool check = false;
	bool check_2 = false;
	bool salida = false;

	Car* player;
	Car* player2;
	Car* npc;

	Vector2 P1pos = {210.0f, 730.0f};
	Vector2 P2pos = {174.0f, 750.0f};

	vec2<int> ray;
	bool ray_on;
	int entitieQ = 34;
	bool out = false;

	int currentCheckpointIndex = 0;
	int currentCheckpointIndex2 = 0;
	std::vector<bool> checkpointStates;
	std::vector<bool> checkpointStates2;
	int num_checkpoint;
	int num_checkpoint2;

	std::vector<Vector2> path = {
		{120, 140},
		{305, 315},
		{480, 490},
		{745, 762},
		{1120, 1220},
		{1850,1890},
		{1940,2030},
		{2150,2245},
		{2390,2390}
	};

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
