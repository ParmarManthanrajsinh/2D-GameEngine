#include "MyMap.h"

MyMap::MyMap() 
{
	postition = { 0, 0 };
    scene_dimension = { 0, 0 };
	velocity = { 0, 0 };
    direction = 0;
	speed = 0.0f;
    radius = 0;

	std::cout << "MyMap constructed!" << std::endl;
}

MyMap::~MyMap() 
{
	std::cout << "MyMap destructed!" << std::endl;
}

void MyMap::Initialize() 
{
	postition = {100, 100};
	radius = 30;
    speed = 3.0f;
	direction = -1;
    velocity = { 200.0f, 150.0f };
    scene_dimension = GetSceneBounds();


    bubble_particle.position = { 400, 400 };
    bubble_particle.b_LoadTexture("Assets\\Bubble.png");
}

void MyMap::Update(float DeltaTime)
{
    postition.x += velocity.x * speed * DeltaTime;
    postition.y += velocity.y * speed * DeltaTime;


    // Bounce horizontally
    if (postition.x - radius <= 0.0f || postition.x + radius >= static_cast<float>(scene_dimension.x))
    {
        velocity.x *= -1.0f;
    }

    // Bounce vertically
    if (postition.y - radius <= 0.0f || postition.y + radius >= static_cast<float>(scene_dimension.y))
    {
        velocity.y *= -1.0f;
    }

    bubble_particle.Update(DeltaTime);
}

void MyMap::Draw()
{	
	DrawCircle(static_cast<int>(postition.x), static_cast<int>(postition.y), radius, MAGENTA);

    bubble_particle.Draw();
}

extern "C" __declspec(dllexport) GameMap* CreateGameMap() 
{
	return new MyMap();
}