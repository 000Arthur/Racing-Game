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
		body->id = 5;
		//2722 x 1466
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
		body->id = 6;
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
public:
	Texture2D texture;

	float BOOST = 3.0F;
	float BOOST_QUANTITY = 20.0F;
	float BOOST_COLDDOWN = 100.0F;
	float BOOST_CNT = 0.0F;

	bool accelerate = false;
	int cnt = 0;
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
	App->audio->SoundsFx();

	App->renderer->camera.x = App->renderer->camera.y = 0;

	car = LoadTexture("Assets/Car2.png");
	greenCar = LoadTexture("Assets/Car1.png");

	circuit = LoadTexture("Assets/HockenHaum.png");
	circuit.width = 1280/1.3;
	circuit.height = 720/1.2;

	for (int i = 0; i < 3; ++i) {
		std::string filename = "Assets/Tire" + std::to_string(i+1) + ".png";
		tires[i] = LoadTexture(filename.c_str());
	}

	board = new Board(App->physics, 0, 0, this, circuit);
	player = new Car(App->physics, 100 + SCREEN_WIDTH * 0.25f, 100, this, greenCar,1);
	player2 = new Car(App->physics, 150 + SCREEN_WIDTH * 0.25f, 100, this, car,2);

	entities.push_back(player);
	int actualTire = 0;
	for (int i = 0; i < 9; ++i) {
		entities.push_back(new Bike(App->physics, tiresPos[i].x, tiresPos[i].y, this, tires[actualTire]));
		if (actualTire >= 2)actualTire = 0;
		else actualTire++;
	}
	for (int i = 0; i < 6; ++i) {
		if (i<3)entities.push_back(new Bost(App->physics, BostersPos[i].x, BostersPos[i].y, this, tires[1], 3));// poner enum y no 3
		else entities.push_back(new Bost(App->physics, BostersPos[i].x, BostersPos[i].y, this, tires[2], 4));// poner enum y no 3
	}

	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

void limitVelocity(b2Body* body, float maxSpeed) {
	// Obtén la velocidad actual del cuerpo
	b2Vec2 velocity = body->GetLinearVelocity();

	// Calcula la magnitud de la velocidad
	float speed = velocity.Length();

	// Si la velocidad supera el máximo, ajusta el vector de velocidad
	if (speed > maxSpeed) {
		// Normaliza la velocidad y ajusta su magnitud al límite máximo
		velocity *= maxSpeed / speed; // Reduce proporcionalmente x e y
		body->SetLinearVelocity(velocity); // Asigna la velocidad ajustada
	}
}
void limitAngularVelocity(b2Body* body, float maxAngularVelocity) {
	// Obtén la velocidad angular actual
	float angularVelocity = body->GetAngularVelocity();

	// Si la velocidad angular supera el límite, ajustarla
	if (fabs(angularVelocity) > maxAngularVelocity) {
		// Clampear la velocidad angular al rango permitido
		angularVelocity = (angularVelocity > 0 ? maxAngularVelocity : -maxAngularVelocity);
		body->SetAngularVelocity(angularVelocity);
	}
}
void applyFriction(b2Body* body, float frictionCoefficient) {
	// Obtén la velocidad actual del cuerpo
	b2Vec2 velocity = body->GetLinearVelocity();

	// Si la velocidad es muy baja, detén completamente el movimiento
	if (velocity.Length() < 0.01f) {
		body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		return;
	}

	// Calcula la fuerza de fricción proporcional a la velocidad
	b2Vec2 frictionForce = -velocity; // Dirección opuesta al movimiento
	frictionForce *= frictionCoefficient; // Ajusta según el coeficiente de fricción

	// Aplica la fuerza de fricción al centro del cuerpo
	body->ApplyForceToCenter(frictionForce, true);
}

// Update: draw background
update_status ModuleGame::Update()
{
	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);
	double r, r2;
	vec2f normal(0.0f, 0.0f);
	b2Vec2 playervel = player->body->body->GetLinearVelocity();

	if (IsKeyPressed(KEY_T)) printf("%d, %d, \n", mouse.x, mouse.y);

	//Player 1 controls
	if(!player->accelerate){
		if (IsKeyPressed(KEY_SPACE) && player->BOOST_QUANTITY > 0)
		{
			App->audio->PlayFx(App->audio->accelerate_fx);
		}
		if (IsKeyDown(KEY_SPACE)&& player->BOOST_QUANTITY > 0)
		{
			vel = -2.0f * player->BOOST;
			player->BOOST_QUANTITY -= 0.1f;
			limitVelocity(player->body->body, MAX_VELOCITY + player->BOOST);
		}
		else
		{
			App->audio->StopFx(App->audio->accelerate_fx);

			if (IsKeyDown(KEY_W)) vel = -2.0f;
			else if (IsKeyDown(KEY_S)) vel = 0.2f;
			else {
				vel = 0.0f;
				applyFriction(player->body->body, FRICTION_COEFFICIENT);
			}
			limitVelocity(player->body->body, MAX_VELOCITY);
		}

		b2Vec2 f = player->body->body->GetWorldVector(b2Vec2(0.0f, vel));
		player->body->body->ApplyForceToCenter(f, true);
	}
	else
	{
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
	if (!player2->accelerate)
	{
		if (IsKeyPressed(KEY_RIGHT_SHIFT) && player->BOOST_QUANTITY > 0)
		{
			App->audio->PlayFx(App->audio->accelerate_fx_2);
		}
		if (IsKeyDown(KEY_RIGHT_SHIFT)&& player2->BOOST_QUANTITY > 0)
		{
			vel2 = -2.0f * player2->BOOST;
			player2->BOOST_QUANTITY -= 0.1f;
			limitVelocity(player2->body->body, MAX_VELOCITY + player2->BOOST);
		}
		else
		{
			App->audio->StopFx(App->audio->accelerate_fx_2);

			if (IsKeyDown(KEY_UP)) vel2 = -2.0f;
			else if (IsKeyDown(KEY_DOWN)) vel2 = 0.2f;
			else {
				vel2 = 0.0f;
				applyFriction(player2->body->body, FRICTION_COEFFICIENT);
			}
			limitVelocity(player2->body->body, MAX_VELOCITY);
		}
		b2Vec2 f2 = player2->body->body->GetWorldVector(b2Vec2(0.0f, vel2));
		player2->body->body->ApplyForceToCenter(f2, true);
	}
	else
	{
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

	// ray -----------------
	for (PhysicEntity* entity : entities){
		entity->Update();
		if (ray_on){
			int hit = entity->RayHit(ray, mouse, normal);
			if (hit >= 0)ray_hit = hit;
		}
	}

	if(ray_on == true){
		vec2f destination((float)(mouse.x-ray.x), (float)(mouse.y-ray.y));
		destination.Normalize();
		destination *= (float)ray_hit;
	}

	//UPDATE---------------------
	board->Update();
	player->Update();
	player2->Update();

	for (int i = 1; i < 16; i++){
		applyFriction(entities[i]->body->body, FRICTION_COEFFICIENT);
		limitAngularVelocity(entities[i]->body->body, 0.0f);
		entities[i]->Update();
	}
	return UPDATE_CONTINUE;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyA != nullptr && bodyB != nullptr)
	{ 
		if (bodyA->id == 1 && bodyB->id == 3) player->accelerate = true; 
		if (bodyA->id == 2 && bodyB->id == 3) player2->accelerate = true;

		if (bodyA->id == 1 && bodyB->id == 4)
			if(player->BOOST_QUANTITY <10.0f)player->BOOST_QUANTITY = 10.0f;
		

		if (bodyA->id == 2 && bodyB->id == 4)
			if (player2->BOOST_QUANTITY < 10.0f)player2->BOOST_QUANTITY = 10.0f;

		// Detect collision between cars or a wall:
		if ((bodyA->id == 1 || bodyA->id == 2) && (bodyB->id == 1 || bodyB->id == 2 || bodyB->id == 5))
		{
			App->audio->PlayFx(App->audio->collision_cars_fx);
		}
		
		//Check if a car collides with a wheels wall
		if ((bodyA->id == 1 || bodyA->id == 2) && (bodyB->id == 6))
		{
			App->audio->PlayFx(App->audio->collision_object_fx);
		}

	}
}
