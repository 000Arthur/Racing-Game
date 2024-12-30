#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include <string>

// TODO 1: Create an enum to represent physics categories for collision detection
enum PhysicCategory
{
	DEFAULT =	1 << 0,
	CAR =		1 << 2,
	BIKE =		1 << 4
};

// TODO 4: Create an enum to define different physics groups
enum PhysicGroup {
	LAND = 1,
};

class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body, Module* _listener)
		: body(_body)
		, listener(_listener)
	{
		body->listener = listener;
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

class Box : public PhysicEntity
{
public:
	Box(ModulePhysics* physics, int _x, int _y, int width, int height, Module* _listener, Texture2D _texture, uint16 category, uint16 maskBits, int16 groupIndex = 0)
		: PhysicEntity(physics->CreateRectangle(_x, _y, width, height, category, maskBits, groupIndex), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);

		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x , (float)y, (float)texture.width, (float)texture.height },
			Vector2{ ((float)texture.width / 2.0f), (float)texture.height / 2.0f}, body->GetRotation() * RAD2DEG, WHITE);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}

private:
	Texture2D texture;
};

class onRoad : public PhysicEntity
{
public:
	static constexpr int on_circuit[262] =
	{
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

	onRoad(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateChainSenor(0, 0, on_circuit, 262, 11), _listener)
		, texture(_texture)
	{
		body->id = ON_ROAD;
	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
	}

private:
	Texture2D texture;
};

class outRoad : public PhysicEntity
{
public:
	static constexpr int out_circuit[164] =
	{
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

	outRoad(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateChainSenor(0, 0, out_circuit, 164, 11), _listener)
		, texture(_texture)
	{
		body->id = OUT_ROAD;
	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
	}

private:
	Texture2D texture;
};

class Board : public PhysicEntity
{
public:
	static constexpr int board_circuit[140] =
	{
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

	Board(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateChain(0, 0, board_circuit, 140, b2_staticBody,0), _listener)
		, texture(_texture)
	{
		body->id = HIT;
	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x, (float)y}, body->GetRotation() * RAD2DEG, 2.0f, WHITE);
	}

private:
	Texture2D texture;
};

class FinishLine : public PhysicEntity {
public:
	FinishLine(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture, int id)
		: PhysicEntity(physics->CreateRectangleSensor(_x, _y, 126, 17, id), _listener) {
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

class Checkpoint : public PhysicEntity {
public:
	Checkpoint(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture, int id)
		: PhysicEntity(physics->CreateRectangleSensor(_x, _y, 126, 126, id), _listener) {
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

class Puddle : public PhysicEntity {
public:
	Puddle(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture, int id)
		: PhysicEntity(physics->CreateRectangleSensor(_x, _y, 45, 90, id), _listener) {
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
	float angle=30;
};


class Bost : public PhysicEntity
{
public:
	Bost(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture, int id)
		: PhysicEntity(physics->CreateRectangleSensor(_x, _y, 20, 20,id), _listener), texture(_texture){
	}

	void Update() override
	{
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

class Bike : public Box {
public:
	Bike(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture) : Box(physics, _x, _y, 24, 24, _listener, _texture, PhysicCategory::BIKE, PhysicCategory::DEFAULT, PhysicGroup::LAND) {
		body->id = TIRE;
	}
};

class Car : public PhysicEntity {
public:
	Car(ModulePhysics* physics, int _x, int _y, Module* _listener, const Texture2D& _texture, int id)
		: PhysicEntity(physics->CreateCar(_x, _y, 15, 28,b2_dynamicBody, id), _listener), texture(_texture) {
		
	}

	void Update() override
	{
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
	}
public:
	Texture2D texture;

	float BOOST = 3.0F;
	float BOOST_QUANTITY = 20.0F;
	float BOOST_CNT = 0.0F;
	float MAX_VELOCITY = 2.0f;
	bool accelerate = false;
	int cnt = 0;
	int counter = 0;

	int lap = 0;
};


ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;
	checkpointStates.resize(checkpointPos.size(), false); //Player 1
	checkpointStates2.resize(checkpointPos.size(), false); //Player 2
	App->audio->SoundsFx();
	App->audio->PlayMusic("Assets/Audio/Music/background.ogg", 1.0f);
	App->audio->PlayFx(App->audio->start_fx);
	App->renderer->camera.x = App->renderer->camera.y = 0;

	speedBoost = LoadTexture("Assets/SpeedBoost.png");
	nitroBoost = LoadTexture("Assets/NitroBoost.png");

	car2[0] = LoadTexture("Assets/Car20.png");
	car2[1] = LoadTexture("Assets/Car21.png");

	car1[0] = LoadTexture("Assets/Car10.png");
	car1[1] = LoadTexture("Assets/Car11.png");

	car3[0] = LoadTexture("Assets/Car30.png");
	car3[1] = LoadTexture("Assets/Car31.png");

	circuit = LoadTexture("Assets/HockenHaum.png");
	circuit.width = 1280/1.3;
	circuit.height = 720/1.2;

	for (int i = 0; i < 3; ++i) {
		std::string filename = "Assets/Tire" + std::to_string(i+1) + ".png";
		tires[i] = LoadTexture(filename.c_str());
	}

	for (int i = 0; i < 6; ++i) {
		std::string filename = "Assets/Start/0" + std::to_string(i + 1) + ".png";
		startLight[i] = LoadTexture(filename.c_str());
	}

	board = new Board(App->physics, 0, 0, this, circuit);
	limit = new outRoad(App->physics, 0, 0, this, circuit);
	piano = new onRoad(App->physics, 0, 0, this, circuit);

	player = new Car(App->physics, P1pos.x, P1pos.y, this, car1[0], PLAYER_1);
	player2 = new Car(App->physics, P2pos.x, P2pos.y, this, car2[0], PLAYER_2);
	npc = new Car(App->physics, P1pos.x, P1pos.y+40, this, car3[0], 20);

	entities.push_back(player);

	int actualTire = 0;
	for (int i = 0; i < 27; ++i) {
		entities.push_back(new Bike(App->physics, tiresPos[i].x, tiresPos[i].y, this, tires[actualTire]));
		if (actualTire >= 2)actualTire = 0;
		else actualTire++;
	}
	for (int i = 0; i < 6; ++i) {
		if (i<3)entities.push_back(new Bost(App->physics, BostersPos[i].x, BostersPos[i].y, this, speedBoost, SPEED_BOOST));// poner enum y no 3
		else entities.push_back(new Bost(App->physics, BostersPos[i].x, BostersPos[i].y, this, nitroBoost, NITRO_BOOST));// poner enum y no 3
	}
	entities.push_back(new FinishLine(App->physics, 193, 686, this, tires[1], FINISH_LINE));
	entities.push_back(new Puddle(App->physics, 350, 795, this, tires[1], PUDDLE));

	for (int i = 0; i < 7; ++i) 
		entities.push_back(new Checkpoint(App->physics, checkpointPos[i].x, checkpointPos[i].y, this, tires[1], CHECK +i));
	
	for (int i = 0; i < 3; i++)
		entities.push_back(new Crack(App->physics, crackpointPos[i].x, crackpointPos[i].y, this, tires[1], CRACK));

	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");
	return true;
}

void limitVelocity(b2Body* body, float maxSpeed) {
	// Obt�n la velocidad actual del cuerpo
	b2Vec2 velocity = body->GetLinearVelocity();

	// Calcula la magnitud de la velocidad
	float speed = velocity.Length();

	// Si la velocidad supera el m�ximo, ajusta el vector de velocidad
	if (speed > maxSpeed) {
		// Normaliza la velocidad y ajusta su magnitud al l�mite m�ximo
		velocity *= maxSpeed / speed; // Reduce proporcionalmente x e y
		body->SetLinearVelocity(velocity); // Asigna la velocidad ajustada
	}
}
void limitAngularVelocity(b2Body* body, float maxAngularVelocity) {
	// Obt�n la velocidad angular actual
	float angularVelocity = body->GetAngularVelocity();

	// Si la velocidad angular supera el l�mite, ajustarla
	if (fabs(angularVelocity) > maxAngularVelocity) {
		// Clampear la velocidad angular al rango permitido
		angularVelocity = (angularVelocity > 0 ? maxAngularVelocity : -maxAngularVelocity);
		body->SetAngularVelocity(angularVelocity);
	}
}
void applyFriction(b2Body* body, float frictionCoefficient) {
	// Obt�n la velocidad actual del cuerpo
	b2Vec2 velocity = body->GetLinearVelocity();

	// Si la velocidad es muy baja, det�n completamente el movimiento
	if (velocity.Length() < 0.01f) {
		body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		return;
	}

	// Calcula la fuerza de fricci�n proporcional a la velocidad
	b2Vec2 frictionForce = -velocity; // Direcci�n opuesta al movimiento
	frictionForce *= frictionCoefficient; // Ajusta seg�n el coeficiente de fricci�n

	// Aplica la fuerza de fricci�n al centro del cuerpo
	body->ApplyForceToCenter(frictionForce, true);
}

// Update: draw background
void pathing(Car* NPC, Vector2 path) {

	b2Vec2 f = NPC->body->body->GetWorldVector(b2Vec2(0.0f, -2.0f));
	NPC->body->body->ApplyForceToCenter(f, true);
	
	if (NPC->cnt > path.x && NPC->counter >= 8) {
		NPC->cnt = 0;
		NPC->counter = 0;
	}

	else if (NPC->cnt > path.x && NPC->cnt < path.y && (NPC->counter == 3 || NPC->counter == 6)){
		NPC->body->body->ApplyTorque(-0.2f, true);
	}
	else if (NPC->cnt > path.x && NPC->cnt < path.y) {
		NPC->body->body->ApplyTorque(0.2f, true);
	}
	else {
		double r = NPC->body->body->GetAngularVelocity();
		NPC->body->body->ApplyTorque(-r / 100, true);
	}
	if (NPC->cnt > path.y) NPC->counter++;

	NPC->cnt++;
	limitVelocity(NPC->body->body, NPC->MAX_VELOCITY);
	limitAngularVelocity(NPC->body->body, 1.0f);
}

update_status ModuleGame::Update()
{
	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);
	double r, r2;
	vec2f normal(0.0f, 0.0f);

	App->audio->UpdateMusic();

	if (IsKeyPressed(KEY_T)) printf("%d, %d, \n", mouse.x, mouse.y); // DELETE LATER


	switch (state)
	{
	case PRE_START:
			
		App->audio->PlayFx(App->audio->aplause_fx);
		App->audio->PlayFx(App->audio->traffic_light_fx);

		//Stop Sounds
		App->audio->StopFx(App->audio->bost_fx);
		App->audio->StopFx(App->audio->accelerate_fx);
		App->audio->StopFx(App->audio->bost_fx_2);
		App->audio->StopFx(App->audio->accelerate_fx_2);

		if (IsKeyPressed(KEY_ENTER)) {
			state = STATE::START;
		}
		break;

	case START:
		App->audio->StopFx(App->audio->start_fx);

		timer += GetFrameTime();
		if (timer >= frameTime) {
			timer = 0.0f;
			if(currentFrame<5)currentFrame++;
			else {
				currentFrame = 0;
				state = STATE::IN_GAME;
			}
		}
		break;
	case IN_GAME:		

		pathing(npc, path[npc->counter]);

		//Player 1 controls
		if(!player->accelerate && player->lap < 3){
			if (IsKeyPressed(KEY_SPACE) && player->BOOST_QUANTITY > 0)
				App->audio->PlayFx(App->audio->accelerate_fx);
		
			if (IsKeyDown(KEY_SPACE)&& player->BOOST_QUANTITY > 0){
				vel = -2.0f * player->BOOST;
				player->BOOST_QUANTITY -= 0.1f;
				limitVelocity(player->body->body, player->MAX_VELOCITY + player->BOOST);
			
				player->texture = car1[1];
			}
			else{
				App->audio->StopFx(App->audio->bost_fx);
				App->audio->StopFx(App->audio->accelerate_fx);
				player->texture = car1[0];

				if (IsKeyDown(KEY_W)) vel = -2.0f;
				else if (IsKeyDown(KEY_S)) vel = 0.2f;
				else {
					vel = 0.0f;
					applyFriction(player->body->body, FRICTION_COEFFICIENT);
					App->audio->StopFx(App->audio->engine_fx);
					App->audio->StopFx(App->audio->in_Reverse_fx);

				}
				if (IsKeyPressed(KEY_W)) { 
					App->audio->StopFx(App->audio->in_Reverse_fx); 
					if (App->audio->PlayFx(App->audio->accelerate_fx))	App->audio->StopFx(App->audio->engine_fx);
					else if (!App->audio->PlayFx(App->audio->accelerate_fx)) App->audio->PlayFx(App->audio->engine_fx);
				}
				else if (IsKeyPressed(KEY_S)) {
					App->audio->PlayFx(App->audio->in_Reverse_fx); 
					App->audio->StopFx(App->audio->engine_fx); 
				}
				limitVelocity(player->body->body, player->MAX_VELOCITY);
			}

			b2Vec2 f = player->body->body->GetWorldVector(b2Vec2(0.0f, vel));
			player->body->body->ApplyForceToCenter(f, true);
		}
		else{
			player->texture = car1[1];
			b2Vec2 f = player->body->body->GetWorldVector(b2Vec2(0.0f, 1.5f));
			player->body->body->ApplyForceToCenter(-f, true);
		
			if (player->cnt == 50) {
				player->accelerate = false;
				player->cnt = 0;
			}
			else player->cnt++;
		}

		if (IsKeyDown(KEY_A)) player->body->body->ApplyTorque(-0.2f, true);
		else if (IsKeyDown(KEY_D)) player->body->body->ApplyTorque(0.2f, true);
		else{
    		 r = player->body->body->GetAngularVelocity();
			 player->body->body->ApplyTorque(-r/100, true);
		}

		limitAngularVelocity(player->body->body, MAX_ANGULAR_VELOCITY);

		//Player 2 controls
		if (!player2->accelerate && player2->lap < 3){
			if (IsKeyPressed(KEY_RIGHT_SHIFT) && player->BOOST_QUANTITY > 0){
				App->audio->PlayFx(App->audio->accelerate_fx_2);
			}
			if (IsKeyDown(KEY_RIGHT_SHIFT)&& player2->BOOST_QUANTITY > 0){
				vel2 = -2.0f * player2->BOOST;
				player2->BOOST_QUANTITY -= 0.1f;
				limitVelocity(player2->body->body, player2->MAX_VELOCITY + player2->BOOST);
				player2->texture = car2[1];

			}
			else{
				player2->texture = car2[0];

				App->audio->StopFx(App->audio->bost_fx_2);
				App->audio->StopFx(App->audio->accelerate_fx_2);

				if (IsKeyDown(KEY_UP)) vel2 = -2.0f;
				else if (IsKeyDown(KEY_DOWN)) vel2 = 0.2f;
				else {
					vel2 = 0.0f;
					applyFriction(player2->body->body, FRICTION_COEFFICIENT);
					App->audio->StopFx(App->audio->engine_fx_2);
				}
				if (IsKeyPressed(KEY_UP)) {
					App->audio->StopFx(App->audio->in_Reverse_fx_2);
				
					if (App->audio->PlayFx(App->audio->accelerate_fx_2))	App->audio->StopFx(App->audio->engine_fx_2);
					else if(!App->audio->PlayFx(App->audio->accelerate_fx_2)) App->audio->PlayFx(App->audio->engine_fx_2);

				}
				else if (IsKeyPressed(KEY_DOWN)) {
					App->audio->PlayFx(App->audio->in_Reverse_fx_2);
					App->audio->StopFx(App->audio->engine_fx_2);
				}
				limitVelocity(player2->body->body, player2->MAX_VELOCITY);
			}
			b2Vec2 f2 = player2->body->body->GetWorldVector(b2Vec2(0.0f, vel2));
			player2->body->body->ApplyForceToCenter(f2, true);
		}
		else
		{
			player2->texture = car2[1];
			b2Vec2 f2 = player2->body->body->GetWorldVector(b2Vec2(0.0f, 1.5f));
			player2->body->body->ApplyForceToCenter(-f2, true);

			if (player2->cnt == 50) {
				player2->accelerate = false;
				player2->cnt = 0;
			}
			else player2->cnt++;
		}
	
		if (IsKeyDown(KEY_LEFT)) player2->body->body->ApplyTorque(-0.2f, true);
		else if (IsKeyDown(KEY_RIGHT)) player2->body->body->ApplyTorque(0.2f, true);
		else{
			r2 = player2->body->body->GetAngularVelocity();
			player2->body->body->ApplyTorque(-r2/100, true);
		}

		limitAngularVelocity(player2->body->body, MAX_ANGULAR_VELOCITY);

	break;
	case END:
		App->audio->StopFx(App->audio->engine_fx);
		App->audio->StopFx(App->audio->engine_fx);
		App->audio->StopFx(App->audio->engine_fx_2);
		App->audio->StopFx(App->audio->engine_fx_2);

		App->renderer->timer.Stop();
		App->renderer->timer2.Stop();

		if (IsKeyDown(KEY_ENTER)) {
			player->SetPos((P1pos.x * 2) / 100, (P1pos.y * 2) / 100);
			player2->SetPos((P2pos.x * 2) / 100, (P2pos.y * 2) / 100);
			state = STATE::PRE_START;
		}

		break;
	default:
		break;
	}

	//UPDATE---------------------
	board->Update();
	limit->Update();
	piano->Update();
	npc->Update();

	if(state==STATE::START)DrawTexture(startLight[currentFrame], 140, 600, WHITE);

	int rectWidth = player->BOOST_QUANTITY * 20; // Ancho del rect�ngulo
	DrawRectangle(rectX, rectY, rectWidth, rectHeight, RED);

	rectWidth = player2->BOOST_QUANTITY * 20; // Ancho del rect�ngulo
	DrawRectangle(rectX + 1200, rectY, rectWidth, rectHeight, BLUE);

	for (int i = 1; i < entitieQ; i++) {
		applyFriction(entities[i]->body->body, FRICTION_COEFFICIENT);
		limitAngularVelocity(entities[i]->body->body, 0.0f);
		entities[i]->Update();
	}

	if(player->lap < 3)player->Update();
	else{
		App->renderer->timer.Restart();
		App->audio->StopFx(App->audio->engine_fx);
		App->audio->StopFx(App->audio->bost_fx);
	}

	if(player2->lap < 3)player2->Update();
	else {
		App->renderer->timer2.Restart();
		App->audio->StopFx(App->audio->engine_fx_2);
		App->audio->StopFx(App->audio->bost_fx_2);
	}
	return UPDATE_CONTINUE;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA != nullptr && bodyB != nullptr)
	{ 
		if (bodyA->id == PLAYER_1 && bodyB->id == SPEED_BOOST) player->accelerate = true; App->audio->PlayFx(App->audio->bost_fx);
		if (bodyA->id == PLAYER_2 && bodyB->id == SPEED_BOOST) player2->accelerate = true; App->audio->PlayFx(App->audio->bost_fx_2);

		if (bodyA->id == PLAYER_1 && bodyB->id == NITRO_BOOST)
			if (player->BOOST_QUANTITY < 10.0f)player->BOOST_QUANTITY = 10.0f;
		
		if (bodyA->id == 2 && bodyB->id == NITRO_BOOST)
			if (player2->BOOST_QUANTITY < 10.0f)player2->BOOST_QUANTITY = 10.0f;

		// Detect collision between cars or a wall:
		if ((bodyA->id == PLAYER_1 || bodyA->id == PLAYER_2) && (bodyB->id == PLAYER_1 || bodyB->id == PLAYER_2 || bodyB->id == HIT))
			App->audio->PlayFx(App->audio->collision_cars_fx);
		
		//Check if a car collides with a wheels wall
		if ((bodyA->id == PLAYER_1 || bodyA->id == PLAYER_2) && (bodyB->id == TIRE))
			App->audio->PlayFx(App->audio->collision_object_fx);

		//puddle
		if ((bodyA->id == PLAYER_1 || bodyA->id == PLAYER_2) && (bodyB->id == PUDDLE))
			App->audio->PlayFx(App->audio->puddle_fx);

		//crack
		if ((bodyA->id == PLAYER_1 || bodyA->id == PLAYER_2) && (bodyB->id == CRACK))
			App->audio->PlayFx(App->audio->crack_fx);

		if (bodyA->id == PLAYER_1  && bodyB->id == ON_ROAD)
			player->MAX_VELOCITY = 2.0f;
		else if (bodyA->id == PLAYER_1  && bodyB->id == OUT_ROAD)
			player->MAX_VELOCITY = 1.0f;

		if (bodyA->id == PLAYER_2 && bodyB->id == ON_ROAD)
			player2->MAX_VELOCITY = 2.0f;
		else if (bodyA->id == PLAYER_2 && bodyB->id == OUT_ROAD)
			player2->MAX_VELOCITY = 1.0f;

		for (int i = 0; i < checkpointStates.size(); ++i)
		{
			if (bodyA->id == PLAYER_1 && bodyB->id == CHECK + i) // Checkpoint IDs start at 9
			{
				if (i == 0 || checkpointStates[i - 1]) // Ensure previous checkpoints are activated
				{
					if (!checkpointStates[i]) {
						checkpointStates[i] = true;
						App->audio->PlayFx(App->audio->checkpoint_fx);  // Play checkpoint sound
						printf("Checkpoint %d passed!\n", i + 1);
					}
					else printf("Checkpoint %d ignore!\n", i + 1);
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

		for (int i = 0; i < checkpointStates2.size(); ++i)
		{
			if (bodyA->id == PLAYER_2 && bodyB->id == CHECK + i) // Player 2
			{
				if (i == 0 || checkpointStates2[i - 1]) // Ensure previous checkpoints are activated
				{
					if (!checkpointStates2[i]) {
						checkpointStates2[i] = true;
						App->audio->PlayFx(App->audio->checkpoint_fx);  // Play checkpoint sound
						printf("Checkpoint %d passed!\n", i + 1);
					}
					else printf("Checkpoint %d ignore!\n", i + 1);
					break;
				}
			}
		}

		bool allCheckpointsPassed2 = true;
		for (bool state : checkpointStates2) {
			if (!state) {
				allCheckpointsPassed2 = false;
				break; 
			}
		}

		if ((bodyA->id == PLAYER_1) && (bodyB->id == FINISH_LINE) && allCheckpointsPassed){
			double lap_time = App->renderer->timer.ReadSec();
			App->renderer->timer.Restart();

			App->renderer->player1_time = lap_time; //Final time
			printf("Tiempo player 1 %f", App->renderer->player1_time);
			
			if (player->lap < 3)
			{
				App->renderer->timer_1[player->lap] = lap_time;
				App->renderer->timer.Start();
				player->lap++;
			}
			printf("Tiempo player 1 vuelta %d: %f segundos\n", player->lap, lap_time);

			App->renderer->Best_Time();

			App->audio->PlayFx(App->audio->finish_line_fx);

			for (int i = 0; i < checkpointStates.size(); ++i)
				checkpointStates[i] = false;				
			
			allCheckpointsPassed = false;
		}

		if ((bodyA->id == PLAYER_2) && (bodyB->id == FINISH_LINE) && allCheckpointsPassed2){
			double lap_time = App->renderer->timer2.ReadSec(); // Tiempo total desde el inicio de la vuelta
			App->renderer->timer2.Restart();

			App->renderer->player2_time = lap_time;

			if (player2->lap < 3)
			{
				App->renderer->timer_2[player2->lap] = lap_time;
				App->renderer->timer2.Start();
				player2->lap++;
			}
			printf("Tiempo player 2 vuelta %d: %f segundos\n", player2->lap, lap_time);
			App->renderer->Best_Time();

			App->audio->PlayFx(App->audio->finish_line_fx);

			for (int i = 0; i < checkpointStates2.size(); ++i)
				checkpointStates2[i] = false;
			
			allCheckpointsPassed2 = false;
		}

		if (player->lap >= 3 && player2->lap >= 3) {
			state = END;
		}

		if ((bodyA->id == PLAYER_1 || bodyA->id == PLAYER_2) && (bodyB->id == FINISH_LINE)) {
			App->audio->PlayFx(App->audio->aplause_fx);

		}
	}
}

