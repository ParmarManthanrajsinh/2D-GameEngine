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
	bp.position = { 300, 300 };
}

void MyMap::Update(float dt)
{
	bp.Update(dt);
}

void MyMap::Draw()
{	
	bp.Draw();
}

extern "C" __declspec(dllexport) GameMap* CreateGameMap() 
{
	return new MyMap();
}