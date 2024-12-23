#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

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

class Box2 : public PhysicEntity
{
public:
	Box2(ModulePhysics* physics, int _x, int _y, int width, int height, Module* _listener, Texture2D _texture, uint16 category, uint16 maskBits, int16 groupIndex = 0)
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
			Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f }, body->GetRotation() * RAD2DEG, WHITE);
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
	static constexpr int board_circuit[84] =
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
		613, 618
	};

	Board(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateChain(0, 0, board_circuit, 84, b2_staticBody,1), _listener)
		, texture(_texture)
	{
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

class Bike : public Box {
public:
	Bike(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture) : Box(physics, _x, _y, 18, 35, _listener, _texture, PhysicCategory::BIKE, PhysicCategory::DEFAULT, PhysicGroup::LAND) {
	}
};

class Car : public Box2 {
public:
	Car(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture) : Box2(physics, _x, _y, 26, 43, _listener, _texture, PhysicCategory::CAR, PhysicCategory::DEFAULT, PhysicGroup::LAND) {
		
	}
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
	App->renderer->camera.x = App->renderer->camera.y = 0;

	car = LoadTexture("Assets/Car.png");

	bike = LoadTexture("Assets/Bike.png");
	circuit = LoadTexture("Assets/HockenHaum.png");
	circuit.width = 1280/1.3;
	circuit.height = 720/1.2;

	board = new Board(App->physics, 0, 0, this, circuit);

	player = new Car(App->physics, 100 + SCREEN_WIDTH * 0.25f, 100, this, car);
	player2 = new Car(App->physics, 150 + SCREEN_WIDTH * 0.25f, 100, this, car);

	entities.push_back(player);
	cono = new Bike(App->physics, 10,10, this, bike);
	for (int i = 0; i < 6; ++i) {
		entities.push_back(new Bike(App->physics, i * 1000 + SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f, this, bike));
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
	double r;

	double r2;
	vec2f normal(0.0f, 0.0f);
	b2Vec2 novel(0.0f, 0.0f);
	b2Vec2 playervel = player->body->body->GetLinearVelocity();

	if (IsKeyPressed(KEY_A)) printf("%d, %d, \n", mouse.x, mouse.y);
	//Player 1 controls
	if (IsKeyDown(KEY_W)) vel = -2.0f;
	else if (IsKeyDown(KEY_S)) vel = 2.0f;
	else {
		vel = 0.0f;
		applyFriction(player->body->body, FRICTION_COEFFICIENT);
	}

	if (IsKeyDown(KEY_A)) player->body->body->ApplyTorque(-0.2f, true);
	else if (IsKeyDown(KEY_D)) player->body->body->ApplyTorque(0.2f, true);
	else{
    	 r = player->body->body->GetAngularVelocity();
		 player->body->body->ApplyTorque(-r, true);
	}

	b2Vec2 f = player->body->body->GetWorldVector(b2Vec2(0.0f, vel));
	limitVelocity(player->body->body, MAX_VELOCITY);
	player->body->body->ApplyForceToCenter(f, true);
	limitAngularVelocity(player->body->body, MAX_ANGULAR_VELOCITY);

	//Player 2 controls
	if (IsKeyDown(KEY_UP)) vel2 = -2.0f;
	else if (IsKeyDown(KEY_DOWN)) vel2 = 2.0f;
	else {
		vel2 = 0.0f;
		applyFriction(player2->body->body, FRICTION_COEFFICIENT);
	}

	if (IsKeyDown(KEY_LEFT)) player2->body->body->ApplyTorque(-0.2f, true);
	else if (IsKeyDown(KEY_RIGHT)) player2->body->body->ApplyTorque(0.2f, true);
	else{
		r2 = player2->body->body->GetAngularVelocity();
		player2->body->body->ApplyTorque(-r2, true);
	}

	b2Vec2 f2 = player2->body->body->GetWorldVector(b2Vec2(0.0f, vel2));
	limitVelocity(player2->body->body, MAX_VELOCITY);
	player2->body->body->ApplyForceToCenter(f2, true);
	limitAngularVelocity(player2->body->body, MAX_ANGULAR_VELOCITY);

	for (PhysicEntity* entity : entities){
		entity->Update();
		if (ray_on){
			int hit = entity->RayHit(ray, mouse, normal);
			if (hit >= 0)ray_hit = hit;
		}
	}

	// ray -----------------
	if(ray_on == true){
		vec2f destination((float)(mouse.x-ray.x), (float)(mouse.y-ray.y));
		destination.Normalize();
		destination *= (float)ray_hit;
	}

	board->Update();
	player->Update();
	player2->Update();
	b2Vec2 enmyPos;

	for (int i = 0; i < 7; i++){
		entities[i]->Update();
		enmyPos = entities[i]->body->body->GetPosition();
	}
	return UPDATE_CONTINUE;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	
}
