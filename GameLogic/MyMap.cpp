#include "MyMap.h"

MyMap::MyMap() 
{
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

    sWidth = GetScreenWidth();
    sHeight = GetScreenHeight();
}

void MyMap::Update(float dt)
{
    postition.x += velocity.x * speed * dt;
    postition.y += velocity.y * speed * dt;

    // Bounce horizontally
    if (postition.x - radius <= 0.0f || postition.x + radius >= static_cast<float>(sWidth))
    {
        velocity.x *= -1.0f;
    }

    // Bounce vertically
    if (postition.y - radius <= 0.0f || postition.y + radius >= static_cast<float>(sHeight))
    {
        velocity.y *= -1.0f;
    }
}

void MyMap::Draw()
{	
	DrawCircle(static_cast<int>(postition.x), static_cast<int>(postition.y), radius, MAGENTA);
}

extern "C" __declspec(dllexport) GameMap* CreateGameMap() 
{
	return new MyMap();
}