#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include <string>

class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body, Module* _listener)
		: body(_body)
		, listener(_listener)
	{
		if (body != nullptr) {
			body->listener = listener;
		}
	}

public:
	virtual ~PhysicEntity() = default;
	virtual void Update() = 0;
	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	}

public:
	PhysBody* body;
	Module* listener;
};

class Obstacle : public PhysicEntity
{
public:

	static constexpr int board[140] = {
		352, 618,
		334, 635,
		319, 660,
		309, 688,
		303, 712,
		303, 737,
		302, 752,
		296, 774,
		282, 782,
		271, 775,
		265, 762,
		264, 746,
		266, 568,
		277, 544,
		294, 530,
		431, 372,
		469, 369,
		743, 369,
		750, 372,
		759, 383,
		760, 434,
		764, 450,
		1030, 754,
		1051, 777,
		1062, 788,
		1081, 799,
		1107, 807,
		1127, 810,
		1711, 813,
		1109, 817,
		1092, 832,
		1057, 871,
		1030, 878,
		679, 876,
		668, 868,
		661, 845,
		661, 723,
		659, 704,
		657, 690,
		646, 666,
		629, 638,
		613, 618,
		615, 612,
		640, 644,
		662, 683,
		668, 846,
		692, 871,
		1032, 871,
		1093, 820,
		1092, 809,
		1071, 799,
		1047, 782,
		766, 464,
		754, 446,
		753, 386,
		743, 374,
		453, 372,
		430, 380,
		281, 546,
		274, 559,
		270, 592,
		269, 756,
		274, 772,
		283, 771,
		295, 757,
		296, 733,
		299, 703,
		307, 671,
		320, 646,
		334, 624
	};

	static constexpr int outRoad[164] = {
		124, 961,
		124, 527,
		124, 504,
		129, 486,
		134, 466,
		141, 449,
		149, 435,
		161, 419,
		316, 241,
		333, 226,
		343, 217,
		356, 211,
		375, 203,
		392, 200,
		838, 197,
		855, 200,
		870, 204,
		890, 217,
		904, 230,
		918, 246,
		927, 264,
		932, 281,
		936, 306,
		935, 414,
		941, 422,
		1120, 627,
		1132, 637,
		1148, 644,
		1163, 647,
		1174, 648,
		1825, 648,
		1843, 651,
		1859, 657,
		1874, 666,
		1886, 675,
		1901, 690,
		1913, 708,
		1918, 950,
		1913, 971,
		1903, 991,
		1891, 1004,
		1879, 1014,
		1861, 1026,
		1830, 1033,
		1358, 1035,
		1322, 1031,
		1298, 1020,
		1274, 1004,
		1254, 984,
		1239, 969,
		1210, 969,
		1194, 983,
		1165, 1008,
		1128, 1024,
		1086, 1033,
		632, 1033,
		601, 1026,
		579, 1016,
		565, 1000,
		543, 970,
		533, 934,
		532, 922,
		531, 782,
		526, 764,
		510, 749,
		507, 745,
		484, 742,
		467, 753,
		454, 765,
		451, 787,
		452, 965,
		445, 1002,
		432, 1028,
		425, 1039,
		396, 1063,
		371, 1072,
		356, 1075,
		215, 1074,
		186, 1067,
		165, 1051,
		151, 1037,
		126, 985
	};

	static constexpr int onRoad[262] = {
		133, 526,
		133, 510,
		134, 500,
		138, 486,
		143, 473,
		152, 453,
		164, 432,
		174, 419,
		188, 404,
		204, 389,
		230, 360,
		249, 340,
		270, 319,
		292, 290,
		309, 273,
		323, 257,
		335, 242,
		354, 232,
		363, 226,
		381, 219,
		415, 213,
		466, 210,
		495, 212,
		545, 212,
		602, 213,
		674, 210,
		724, 210,
		784, 212,
		831, 212,
		854, 216,
		867, 219,
		885, 230,
		903, 248,
		914, 271,
		920, 296,
		923, 321,
		925, 351,
		925, 379,
		925, 406,
		925, 417,
		933, 431,
		961, 466,
		980, 490,
		1010, 522,
		1036, 552,
		1057, 579,
		1097, 624,
		1113, 640,
		1130, 652,
		1150, 660,
		1169, 663,
		1198, 662,
		1232, 662,
		1320, 664,
		1405, 666,
		1484, 667,
		1573, 666,
		1633, 671,
		1673, 669,
		1741, 667,
		1776, 667,
		1823, 666,
		1842, 669,
		1867, 682,
		1878, 693,
		1889, 713,
		1897, 732,
		1903, 764,
		1902, 787,
		1905, 849,
		1906, 920,
		1906, 940,
		1902, 957,
		1894, 970,
		1884, 986,
		1871, 999,
		1856, 1007,
		1834, 1013,
		1818, 1016,
		1780, 1019,
		1679, 1020,
		1635, 1017,
		1590, 1015,
		1484, 1014,
		1371, 1014,
		1329, 1011,
		1293, 996,
		1265, 973,
		1240, 952,
		1209, 950,
		1184, 965,
		1159, 988,
		1136, 1001,
		1101, 1015,
		1059, 1015,
		958, 1015,
		873, 1014,
		785, 1017,
		689, 1016,
		624, 1016,
		589, 1003,
		569, 980,
		552, 945,
		545, 917,
		546, 871,
		547, 813,
		545, 774,
		537, 752,
		506, 727,
		466, 734,
		447, 755,
		437, 772,
		436, 816,
		433, 872,
		432, 930,
		434, 967,
		432, 988,
		420, 1014,
		406, 1033,
		378, 1047,
		353, 1056,
		307, 1055,
		247, 1052,
		225, 1054,
		199, 1051,
		174, 1039,
		152, 1006,
		137, 962,
		136, 908,
		143, 818,
		136, 736
	};
	static constexpr int Limit[56] = {
		207, 1110,
		382, 1105,
		521, 996,
		662, 1100,
		1017, 1099,
		1022, 1062,
		1569, 1051,
		1584, 1105,
		1916, 1103,
		1914, 579,
		1694, 556,
		1668, 482,
		1394, 474,
		1386, 560,
		1277, 571,
		1232, 403,
		1128, 399,
		1108, 473,
		1014, 425,
		932, 143,
		862, 136,
		816, 178,
		767, 133,
		636, 155,
		320, 171,
		96, 415,
		94, 727,
		103, 1059
	};

	Obstacle(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(nullptr, _listener), texture(_texture)
	{
		CreateChain(physics, board, sizeof(board) / sizeof(board[0]), _x, _y, HIT);
		CreateChain(physics, Limit, sizeof(Limit) / sizeof(Limit[0]), _x, _y, HIT);
		CreateChainSensor(physics, outRoad, sizeof(outRoad) / sizeof(outRoad[0]), _x, _y, OUT_ROAD);
		CreateChainSensor(physics, onRoad, sizeof(onRoad) / sizeof(onRoad[0]), _x, _y, ON_ROAD);
	}

	void Update() override
	{
		for (const auto& body : bodies) {
			int x, y;
			body->GetPhysicPosition(x, y);
			DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
		}
	}

private:
	Texture2D texture;
	std::vector<PhysBody*> bodies; 


	void CreateChain(ModulePhysics* physics, const int* circuit, int size, int _x, int _y, int i)
	{
		PhysBody* body = physics->CreateChain(_x, _y, circuit, size, b2_staticBody, i);
		if (body != nullptr) {
			bodies.push_back(body);
		}
	}

	void CreateChainSensor(ModulePhysics* physics, const int* circuit, int size, int _x, int _y, int i)
	{
		PhysBody* body = physics->CreateChainSenor(_x, _y, circuit, size, i);
		if (body != nullptr) {
			bodies.push_back(body);
		}
	}
};

class InteractEntity : public PhysicEntity {
public:
	InteractEntity(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture,int width,int height, int id)
		: PhysicEntity(physics->CreateRectangleSensor(_x, _y, width, height, id), _listener) {
	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x , (float)y, (float)texture.width, (float)texture.height },
			Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);
	}
public:
	Texture2D texture;
};

class Crack : public PhysicEntity {
public:
	Crack(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture, int id)
		: PhysicEntity(physics->CreateRectangleSensor(_x, _y, 45, 60, id), _listener), angle(30.0f) {
	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		float rotationDegrees = body->GetRotation() * RAD2DEG;

		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x , (float)y, (float)texture.width, (float)texture.height },
			Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f }, rotationDegrees + angle, WHITE);
	}
public:
	Texture2D texture;
	float angle = 30;
};


class Bost : public PhysicEntity
{
public:
	Bost(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture, int id)
		: PhysicEntity(physics->CreateRectangleSensor(_x, _y, 20, 20, id), _listener), texture(_texture) {
	}

	void Update() override {
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x , (float)y, (float)texture.width, (float)texture.height },
			Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}
public:
	Texture2D texture;
};

class Tire : public PhysicEntity {
public:
	Tire(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture, int id)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 24, 24, b2_dynamicBody, id), _listener), texture(_texture) {
	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);

		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x , (float)y, (float)texture.width, (float)texture.height },
			Vector2{ ((float)texture.width / 2.0f), (float)texture.height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}

private:
	Texture2D texture;
};

class Car : public PhysicEntity {
public:
	Car(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture, int id)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 15, 28, b2_dynamicBody, id), _listener), texture(_texture) {
	}

	int lapsCompleted = 0;
	int checkpointIndex = -1;
	float distanceToNextCheckpoint = 0.0f;

	void Update() override {
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x , (float)y, (float)texture.width, (float)texture.height },
			Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}
	void SetPos(float x, float y) {
		body->body->SetTransform({ x,y }, 0.0f);
		accelerate = false;
		cnt = 0;
		counter = 0;
	}
	void AvoidSpin(){
		float r = body->body->GetAngularVelocity();
		body->body->ApplyTorque(-r / 100, true);
	}

public:
	Texture2D texture;

	float BOOST = 1.5F;
	float BOOST_QUANTITY = 20.0F;
	float BOOST_CNT = 0.0F;
	float MAX_VELOCITY = 2.0f;
	float BACK_MAX_VELOCITY = 0.8f;

	bool accelerate = false;

	int cnt = 0;
	int counter = 0;

	int lap = 0;
};


ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	player1_is_first = true;
	ray_on = false;
}

ModuleGame::~ModuleGame()
{
}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	checkpointStates.resize(checkpointPos.size(), false); //Player 1
	checkpointStates2.resize(checkpointPos.size(), false); //Player 2
	App->audio->SoundsFx();

	App->audio->PlayMusic(App->audio->music_paths[0], 1.0f);
	App->audio->current_music_index = 0;

	App->audio->PlayFx(App->audio->start_fx);
	App->renderer->camera.x = App->renderer->camera.y = 0;

	//ASSETS LOAD
	speedBoost = LoadTexture("Assets/SpeedBoost.png");
	nitroBoost = LoadTexture("Assets/NitroBoost.png");

	circuit = LoadTexture("Assets/HockenHaum.png");
	circuit.width = 1280 / 1.3;
	circuit.height = 720 / 1.2;

	for (int i = 0; i < 6; ++i) {
		std::string filename = "Assets/Car" + std::to_string(i) + ".png";
		carsTexture[i] = LoadTexture(filename.c_str());

		filename = "Assets/Start/0" + std::to_string(i + 1) + ".png";
		startLight[i] = LoadTexture(filename.c_str());

		if(i < 3){
			filename = "Assets/Tire" + std::to_string(i + 1) + ".png";
			tires[i] = LoadTexture(filename.c_str());
		}
	}

	//ENTITIES CREATION
	obstacles = new Obstacle(App->physics, 0, 0, this, circuit);
	player = new Car(App->physics, P1pos.x, P1pos.y, this, carsTexture[CAR_1_NORMAL], PLAYER_1);
	player2 = new Car(App->physics, P2pos.x, P2pos.y, this, carsTexture[CAR_2_NORMAL], PLAYER_2);
	npc = new Car(App->physics, P1pos.x, P1pos.y + 40, this, carsTexture[CAR_3_NORMAL], 20);

	int actualTire = 0;
	for (int i = 0; i < 27; ++i) {
		entities.push_back(new Tire(App->physics, tiresPos[i].x, tiresPos[i].y, this, tires[actualTire],TIRE));
		if (actualTire >= 2)actualTire = 0;
		else actualTire++;
	}
	for (int i = 0; i < 6; ++i) {
		if (i < 3)entities.push_back(new Bost(App->physics, BostersPos[i].x, BostersPos[i].y, this, speedBoost, SPEED_BOOST));
		else entities.push_back(new Bost(App->physics, BostersPos[i].x, BostersPos[i].y, this, nitroBoost, NITRO_BOOST));
	}

	entities.push_back(new InteractEntity(App->physics, 193, 686, this, tires[1], 126, 17, FINISH_LINE));
	entities.push_back(new InteractEntity(App->physics, 350, 795, this, tires[1], 45, 90, PUDDLE));

	for (int i = 0; i < 7; ++i)
		entities.push_back(new InteractEntity(App->physics, checkpointPos[i].x, checkpointPos[i].y, this, tires[1], 126, 126, CHECK + i));

	for (int i = 0; i < 3; i++)
		entities.push_back(new Crack(App->physics, crackpointPos[i].x, crackpointPos[i].y, this, tires[1], CRACK));

	return ret;
}

void limitVelocity(b2Body* body, float maxSpeed) {
	b2Vec2 velocity = body->GetLinearVelocity();
	float speed = velocity.Length();

	if (speed > maxSpeed) {
		velocity *= maxSpeed / speed; // Proportionally reduce x and y
		body->SetLinearVelocity(velocity); // Assigns the set speed
	}
}
void limitAngularVelocity(b2Body* body, float maxAngularVelocity) {
	float angularVelocity = body->GetAngularVelocity();

	if (fabs(angularVelocity) > maxAngularVelocity) {		
		angularVelocity = (angularVelocity > 0 ? maxAngularVelocity : -maxAngularVelocity);		//Clamp angular velocity to allowable range
		body->SetAngularVelocity(angularVelocity);
	}
}
void applyFriction(b2Body* body, float frictionCoefficient) {
	b2Vec2 velocity = body->GetLinearVelocity();

	if (velocity.Length() < 0.01f) {
		body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		return;
	}
	
	b2Vec2 frictionForce = -velocity;		 //Opposite direction of movement
	frictionForce *= frictionCoefficient;		 // Adjust according to the coefficient of friction
	body->ApplyForceToCenter(frictionForce, true);
}

void ModuleGame::pathing(Car* NPC, Vector2 path) {
	
	b2Vec2 f = NPC->body->body->GetWorldVector(b2Vec2(0.0f, -CAR_VELOCITY));
	NPC->body->body->ApplyForceToCenter(f, true);

	//End of first lap
	if (NPC->cnt > path.x && NPC->counter >= 8) {
		NPC->cnt = 0;
		NPC->counter = 0;
	}
	else if (NPC->cnt > path.x && NPC->cnt < path.y && (NPC->counter == 3 || NPC->counter == 6)) //Counter is the varible that controls every move
		NPC->body->body->ApplyTorque(-CAR_TURN, true);
	else if (NPC->cnt > path.x && NPC->cnt < path.y)		//The space between path.x and path.y is the duration of the move
		NPC->body->body->ApplyTorque(CAR_TURN, true);										
	else NPC->AvoidSpin();

	if (NPC->cnt > path.y) NPC->counter++;		//End of the actual move
	NPC->cnt++;																					
	limitVelocity(NPC->body->body, NPC->MAX_VELOCITY);
	limitAngularVelocity(NPC->body->body, MAX_ANGULAR_VELOCITY);
}

update_status ModuleGame::Update()
{
	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);
	App->audio->UpdateMusic();

	switch (state)
	{
	case PRE_START:

		if (IsKeyPressed(KEY_M)) App->audio->ChangeMusic();			//RADIO

		//Stop Sounds
		App->audio->StopFx(App->audio->accelerate_fx);
		App->audio->StopFx(App->audio->accelerate_fx_2);

		if (IsKeyPressed(KEY_ENTER)) state = STATE::START;

		break;

	case START:
		App->audio->PlayFx(App->audio->aplause_fx);
		App->audio->PlayFx(App->audio->traffic_light_fx);

		if (IsKeyPressed(KEY_M)) App->audio->ChangeMusic();			//RADIO

		App->audio->StopFx(App->audio->start_fx);

		//Timer that controls start light
		timer += GetFrameTime();
		if (timer >= frameTime) {
			timer = 0.0f;
			if (currentFrame < 5)currentFrame++;
			else {
				currentFrame = 0;
				App->audio->ChangeMusic();
				state = STATE::IN_GAME;
			}
		}

		if (IsKeyDown(KEY_W)){
			App->audio->PlayFx(App->audio->burnOut_fx);
			App->audio->StopFx(App->audio->idle_fx);
		}
		else App->audio->PlayFx(App->audio->idle_fx);
		
		if (IsKeyReleased(KEY_W)) 
			App->audio->StopFx(App->audio->burnOut_fx);
		
		if (IsKeyPressed(KEY_UP)){
			App->audio->PlayFx(App->audio->burnOut_fx_2);
			App->audio->StopFx(App->audio->idle_fx_2);
		}
		else App->audio->PlayFx(App->audio->idle_fx);
		if (IsKeyReleased(KEY_UP)) 
			App->audio->StopFx(App->audio->burnOut_fx_2);

		break;

	case IN_GAME:
		if (IsKeyPressed(KEY_M)) App->audio->ChangeMusic();
		//Stop all sounds from pre start
		App->audio->StopFx(App->audio->burnOut_fx);
		App->audio->StopFx(App->audio->burnOut_fx_2);
		App->audio->StopFx(App->audio->aplause_fx);
		App->audio->StopFx(App->audio->traffic_light_fx);
		App->audio->StopFx(App->audio->idle_fx);
		App->audio->StopFx(App->audio->idle_fx_2);

		//enemy move
		pathing(npc, path[npc->counter]);

		//Player 1 controls
		if (player->lap < 3){
			if (!player->accelerate) {
				if (IsKeyDown(KEY_SPACE) && player->BOOST_QUANTITY > 0) {	//Accelerate car like using nitro
					App->audio->PlayFx(App->audio->accelerate_fx);

					vel = -CAR_VELOCITY * player->BOOST;					//Multiply car velocity and boost force
					player->BOOST_QUANTITY -= 0.1f;	

					limitVelocity(player->body->body, player->MAX_VELOCITY + player->BOOST); //Increase velocity limit to make the car run faster

					player->texture = carsTexture[CAR_1_BOOST];				
				}
				else {
					App->audio->StopFx(App->audio->accelerate_fx);
					player->texture = carsTexture[CAR_1_NORMAL];
	
					if (IsKeyDown(KEY_W)) {		//Change vel to move forwards
						vel = -CAR_VELOCITY;
						App->audio->PlayFx(App->audio->engine_fx);

						limitVelocity(player->body->body, player->MAX_VELOCITY);		//Limit car max velocity forwards
					}
					else if (IsKeyDown(KEY_S)){	//Change vel to move backwards
						vel = CAR_VELOCITY;		
						App->audio->PlayFx(App->audio->in_Reverse_fx);

						limitVelocity(player->body->body, player->BACK_MAX_VELOCITY);		//Limit car max velocity backwards
					}
					else {
						vel = 0.0f;											
						applyFriction(player->body->body, FRICTION_COEFFICIENT);		//Simulates friction to stop the car while it is not accelerating
						App->audio->StopFx(App->audio->engine_fx);
						App->audio->StopFx(App->audio->in_Reverse_fx);
					}

					if (IsKeyReleased(KEY_W))App->audio->StopFx(App->audio->engine_fx);
					if (IsKeyReleased(KEY_S))App->audio->StopFx(App->audio->in_Reverse_fx);

				}
				//Move the car in the direction it is facing
				b2Vec2 f = player->body->body->GetWorldVector(b2Vec2(0.0f, vel)); 
				player->body->body->ApplyForceToCenter(f, true);
			}
			else {															//If car takes boost item it will move faster for a set time (BOOST_DURANTION).
				player->texture = carsTexture[CAR_1_BOOST];
				//Move the car in the direction it is facing
				b2Vec2 f = player->body->body->GetWorldVector(b2Vec2(0.0f, 1.5f));
				player->body->body->ApplyForceToCenter(-f, true);

				if (player->cnt == BOOST_DURANTION) {
					player->accelerate = false;
					player->cnt = 0;
				}
				else player->cnt++;
			}

			if (IsKeyDown(KEY_A)) player->body->body->ApplyTorque(-CAR_TURN, true);		//Turns the car to the left
			else if (IsKeyDown(KEY_D)) player->body->body->ApplyTorque(CAR_TURN, true); //Turns the car to the right
			else {
				player->AvoidSpin();
			}

			limitAngularVelocity(player->body->body, MAX_ANGULAR_VELOCITY);				//limit angular velocity to avoid the car turns to fast
		}

		//Player 2 controls
		if(player2->lap < 3){ 
			if (!player2->accelerate) {
				if (IsKeyDown(KEY_RIGHT_SHIFT) && player2->BOOST_QUANTITY > 0) {	//Accelerate car like using nitro
					App->audio->PlayFx(App->audio->accelerate_fx_2);

					vel2 = -2.0f * player2->BOOST;									//Multiply car velocity and boost force
					player2->BOOST_QUANTITY -= 0.1f;

					limitVelocity(player2->body->body, player2->MAX_VELOCITY + player2->BOOST);	//Increase velocity limit to make the car run faster
					player2->texture = carsTexture[CAR_2_BOOST];
				}
				else {
					App->audio->StopFx(App->audio->accelerate_fx_2);
					player2->texture = carsTexture[CAR_2_NORMAL];

					if (IsKeyDown(KEY_UP)){				//Change vel to move forwards
						vel2 = -CAR_VELOCITY;
						App->audio->PlayFx(App->audio->engine_fx_2);
						limitVelocity(player2->body->body, player2->MAX_VELOCITY);		//Limit car max velocity forwards

					}
					else if (IsKeyDown(KEY_DOWN)){		//Change vel to move backwards
						vel2 = CAR_VELOCITY;				
						App->audio->PlayFx(App->audio->in_Reverse_fx_2);
						limitVelocity(player2->body->body, player2->BACK_MAX_VELOCITY);						//Limit car max velocity backwards
					}
					else {
						vel2 = 0.0f;
						applyFriction(player2->body->body, FRICTION_COEFFICIENT);		//Simulates friction to stop the car while it is not accelerating
					}
					if (IsKeyReleased(KEY_UP)) App->audio->StopFx(App->audio->engine_fx_2);
					if (IsKeyReleased(KEY_DOWN)) App->audio->StopFx(App->audio->in_Reverse_fx_2);

				}
				//Move the car in the direction it is facing
				b2Vec2 f2 = player2->body->body->GetWorldVector(b2Vec2(0.0f, vel2));
				player2->body->body->ApplyForceToCenter(f2, true);
			}
			else{																	//If car takes boost item it will move faster for a set time (BOOST_DURANTION).
				player2->texture = carsTexture[CAR_2_BOOST];
				//Move the car in the direction it is facing
				b2Vec2 f2 = player2->body->body->GetWorldVector(b2Vec2(0.0f, 1.5f));
				player2->body->body->ApplyForceToCenter(-f2, true);

				if (player2->cnt == BOOST_DURANTION) {
					player2->accelerate = false;
					player2->cnt = 0;
				}
				else player2->cnt++;
			}
		
			if (IsKeyDown(KEY_LEFT)) player2->body->body->ApplyTorque(-CAR_TURN, true);		//Turns the car to the left
			else if (IsKeyDown(KEY_RIGHT)) player2->body->body->ApplyTorque(CAR_TURN, true);//Turns the car to the right
			else {
				player2->AvoidSpin();
			}
			limitAngularVelocity(player2->body->body, MAX_ANGULAR_VELOCITY);		//limit angular velocity to avoid the car turns to fast
		}
		
		//Controls on debug mode
		if (App->physics->debug) {
			if (IsKeyPressed(KEY_F2)) player->lap++;						//Add one lap to player 1
			if (IsKeyPressed(KEY_F3)) player2->lap++;						//Add one lap to player 2
			if (IsKeyPressed(KEY_F4)) player->BOOST_QUANTITY = 20.0F;		//Recover boost quantity to Player 1
			if (IsKeyPressed(KEY_F5)) player2->BOOST_QUANTITY = 20.0F;		//Recover boost quantity to Player 2
		}
		
		Leader();		//Controls who is leading the race
		
		break;

	case END:
		//Stop all audios
		App->audio->StopFx(App->audio->engine_fx);
		App->audio->StopFx(App->audio->engine_fx);
		App->audio->StopFx(App->audio->engine_fx_2);
		App->audio->StopFx(App->audio->engine_fx_2);

		App->renderer->timer.Stop();
		App->renderer->timer2.Stop();

		//Stop players and npc bodies to avoid restart errors
		applyFriction(player->body->body, MAX_FRICTION_COEFFICIENT); //Aply max friction to to stop the car
		player->AvoidSpin();			 //Applies reverse angular velocity to prevent the car from spinning.

		applyFriction(player2->body->body, MAX_FRICTION_COEFFICIENT);
		player2->AvoidSpin();

		applyFriction(npc->body->body, MAX_FRICTION_COEFFICIENT);
		npc->AvoidSpin();

		if (IsKeyDown(KEY_ENTER)) {
			player->SetPos((P1pos.x * 2) / 100, (P1pos.y * 2) / 100);
			player->lap = 0;

			player2->SetPos((P2pos.x * 2) / 100, (P2pos.y * 2) / 100);
			player2->lap = 0;

			npc->SetPos((P1pos.x * 2) / 100, ((P1pos.y + 40) * 2) / 100);

			state = STATE::PRE_START;
		}
		break;

	default:
		break;
	}

	//UPDATE---------------------
	obstacles->Update();

	if (state == STATE::START)DrawTexture(startLight[currentFrame], 140, 600, WHITE); //Draw start light

	//Draw rectangle to show player 1 boost quantity
	int rectWidth = player->BOOST_QUANTITY * 20; // Ancho del rectángulo
	DrawRectangle(rectX, rectY, rectWidth, rectHeight, BEIGE);

	//Draw rectangle to show player 2 boost quantity
	rectWidth = player2->BOOST_QUANTITY * 20; // Ancho del rectángulo
	DrawRectangle(rectX + 1200, rectY, rectWidth, rectHeight, LIGHTBROWN);

	//Aply friction to all entities (not cars) like tires to simulate real physics
	for (int i = 1; i < entitieQ; i++) {
		applyFriction(entities[i]->body->body, FRICTION_COEFFICIENT);
		limitAngularVelocity(entities[i]->body->body, 0.0f);
		entities[i]->Update();
	}

	if (player->lap < 3)player->Update(); //Makes player 1 disapear when the race is complete
	else {
		player->SetPos(0.0f, 0.0f);
		App->renderer->timer.Restart();
		App->audio->StopFx(App->audio->engine_fx);
	}

	if (player2->lap < 3)player2->Update();//Makes player 2 disapear when the race is complete
	else {
		player2->SetPos(0.0f, 0.0f);
		App->renderer->timer2.Restart();
		App->audio->StopFx(App->audio->engine_fx_2);
	}

	if (npc->lap < 3)npc->Update();

	return UPDATE_CONTINUE;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA != nullptr && bodyB != nullptr)
	{
		if (bodyA->id == PLAYER_1 && bodyB->id == SPEED_BOOST) player->accelerate = true; 
		if (bodyA->id == PLAYER_2 && bodyB->id == SPEED_BOOST) player2->accelerate = true;

		if (bodyA->id == PLAYER_1 && bodyB->id == NITRO_BOOST)
			if (player->BOOST_QUANTITY < MAX_BOOST_QUANTITY){
				player->BOOST_QUANTITY += BOOST_RECOVER;
				if (player->BOOST_QUANTITY > MAX_BOOST_QUANTITY) player->BOOST_QUANTITY = MAX_BOOST_QUANTITY;
				App->audio->PlayFx(App->audio->takeNitro_fx, false);
			}

		if (bodyA->id == 2 && bodyB->id == NITRO_BOOST)
			if (player2->BOOST_QUANTITY < MAX_BOOST_QUANTITY) {
				player2->BOOST_QUANTITY += BOOST_RECOVER;
				if (player2->BOOST_QUANTITY > MAX_BOOST_QUANTITY) player2->BOOST_QUANTITY = MAX_BOOST_QUANTITY;
				App->audio->PlayFx(App->audio->takeNitro_fx_2, false);
			}

		// Detect collision between cars or a wall:
		if ((bodyA->id == PLAYER_1 || bodyA->id == PLAYER_2) && (bodyB->id == PLAYER_1 || bodyB->id == PLAYER_2 || bodyB->id == HIT))
			App->audio->PlayFx(App->audio->collision_cars_fx, true);

		//Check if a car collides with a wheels wall
		if ((bodyA->id == PLAYER_1 || bodyA->id == PLAYER_2) && (bodyB->id == TIRE)) App->audio->PlayFx(App->audio->collision_object_fx,true);

		//puddle
		if ((bodyA->id == PLAYER_1 || bodyA->id == PLAYER_2) && (bodyB->id == PUDDLE)) App->audio->PlayFx(App->audio->puddle_fx);

		//crack
		if ((bodyA->id == PLAYER_1 || bodyA->id == PLAYER_2) && (bodyB->id == CRACK)) App->audio->PlayFx(App->audio->crack_fx);

		if (bodyA->id == PLAYER_1 && bodyB->id == ON_ROAD) player->MAX_VELOCITY = 2.0f;
		else if (bodyA->id == PLAYER_1 && bodyB->id == OUT_ROAD) player->MAX_VELOCITY = 1.0f;

		if (bodyA->id == PLAYER_2 && bodyB->id == ON_ROAD) player2->MAX_VELOCITY = 2.0f;
		else if (bodyA->id == PLAYER_2 && bodyB->id == OUT_ROAD) player2->MAX_VELOCITY = 1.0f;


		for (int i = 0; i < checkpointStates.size(); ++i)
		{
			if (bodyA->id == PLAYER_1 && bodyB->id == CHECK + i) // Checkpoint IDs start at 9
			{
				if (i == 0 || checkpointStates[i - 1]) // Ensure previous checkpoints are activated
				{
					if (!checkpointStates[i]) {
						checkpointStates[i] = true;
						num_checkpoint = i;
						App->audio->PlayFx(App->audio->checkpoint_fx);  // Play checkpoint sound
					}
					break;
				}
			}
			if (bodyA->id == PLAYER_2 && bodyB->id == CHECK + i) // Player 2
			{
				if (i == 0 || checkpointStates2[i - 1]) // Ensure previous checkpoints are activated
				{
					if (!checkpointStates2[i]) {
						checkpointStates2[i] = true;
						num_checkpoint2 = i;
						App->audio->PlayFx(App->audio->checkpoint_fx);  // Play checkpoint sound
					}
					break;
				}
			}
		}

		bool allCheckpointsPassed = true;
		for (bool state : checkpointStates) {
			if (!state) {
				allCheckpointsPassed = false;
				break;
			}
		}

		bool allCheckpointsPassed2 = true;
		for (bool state : checkpointStates2) {
			if (!state) {
				allCheckpointsPassed2 = false;
				break;
			}
		}

		if ((bodyA->id == PLAYER_1) && (bodyB->id == FINISH_LINE) && allCheckpointsPassed) {
			double lap_time = App->renderer->timer.ReadSec();
			App->renderer->timer.Restart();
			App->renderer->player1_time = lap_time; //Final time

			if (player->lap < 3){
				App->renderer->timer_1[player->lap] = lap_time;
				App->renderer->timer.Start();
				player->lap++;
			}
			App->renderer->Best_Time();
			App->audio->PlayFx(App->audio->finish_line_fx);

			for (int i = 0; i < checkpointStates.size(); ++i)
				checkpointStates[i] = false;

			allCheckpointsPassed = false;
			num_checkpoint = 0;
		}

		if ((bodyA->id == PLAYER_2) && (bodyB->id == FINISH_LINE) && allCheckpointsPassed2) {
			double lap_time = App->renderer->timer2.ReadSec(); // Total time since lap start
			App->renderer->timer2.Restart();
			App->renderer->player2_time = lap_time;

			if (player2->lap < 3){
				App->renderer->timer_2[player2->lap] = lap_time;
				App->renderer->timer2.Start();
				player2->lap++;
			}
			App->renderer->Best_Time();
			App->audio->PlayFx(App->audio->finish_line_fx);

			for (int i = 0; i < checkpointStates2.size(); ++i)
				checkpointStates2[i] = false;

			allCheckpointsPassed2 = false;
			num_checkpoint2 = 0;
		}
		if (player->lap >= 3 && player2->lap >= 3) state = END;

		if ((bodyA->id == PLAYER_1 || bodyA->id == PLAYER_2) && (bodyB->id == FINISH_LINE))
			App->audio->PlayFx(App->audio->aplause_fx); 
	}
}

void ModuleGame::Leader()
{
	App->renderer->lapP1 = player->lap;
	App->renderer->lapP2 = player2->lap;

	if (player->lap > player2->lap)		//Check which player has the most laps to decide who will go first
		App->renderer->first = 1;
	
	else if (player->lap < player2->lap)
		App->renderer->first = 2;
	
	else if (player->lap == player2->lap && num_checkpoint > num_checkpoint2)		//Check which player has the most checkpoints to decide who will go first
		App->renderer->first = 1;
	
	else if (player->lap == player2->lap && num_checkpoint < num_checkpoint2) 
		App->renderer->first = 2;
	
	if (player->lap == player2->lap && num_checkpoint == num_checkpoint2){			//If they are at the same checkpoint, check the distance traveled to know who goes first
	
		Vector2 nextCheckpoint = checkpointPos[num_checkpoint];
		Vector2 nextCheckpoint2 = checkpointPos[num_checkpoint2];

		b2Vec2 playerPos = player->body->body->GetPosition();
		b2Vec2 playerPos2 = player2->body->body->GetPosition();

		float distance_player1 = distanceToCheckpoint(playerPos.x, playerPos.y, nextCheckpoint.x, nextCheckpoint.y);		//Distance traveled from the last checkpoint of player 1
		float distance_player2 = distanceToCheckpoint(playerPos2.x, playerPos2.y, nextCheckpoint2.x, nextCheckpoint2.y);	//Distance traveled from the last checkpoint of player 2

		if (num_checkpoint2 != 4 && num_checkpoint2 != 3) {
			if (distance_player1 < distance_player2) App->renderer->first = 1;
			else if (distance_player1 > distance_player2) App->renderer->first = 2;	
		}
		else if (num_checkpoint2 == 4 || num_checkpoint2 == 3) {
			if (distance_player1 > distance_player2) App->renderer->first = 1;
			else if (distance_player1 < distance_player2) App->renderer->first = 2;
		}
	}
}
float  ModuleGame::distanceToCheckpoint(float x1, float y1, float x2, float y2) { //Calculate distance between two points
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");
	delete player;
	delete player2;
	delete npc;
	delete obstacles;

	for (int i = 1; i < entitieQ; i++) 
		delete entities[i];

	return true;
}
