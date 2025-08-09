#include "MyMap.h"

MyMap::MyMap() 
{
	std::cout << "MyMap constructed!" << std::endl;
	position = { 0, 0 };
}

MyMap::~MyMap() 
{
	std::cout << "MyMap destructed!" << std::endl;
}

void MyMap::Initialize() 
{
	position = { 400, 300 };
}

void MyMap::Update(float dt)
{
	// Example logic
	position.x += 100 * dt;	
}

void MyMap::Draw()
{
	DrawCircle((int)position.x, (int)position.y, 50, YELLOW);
}

extern "C" __declspec(dllexport) GameMap* CreateGameMap() 
{
	return new MyMap();
}