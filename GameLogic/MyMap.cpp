#include "MyMap.h"
#include <iostream>

MyMap::MyMap() {
    std::cout << "MyMap constructed!" << std::endl;
}

MyMap::~MyMap() {
    std::cout << "MyMap destructed!" << std::endl;
}

void MyMap::Update(float dt) {
    // Example logic
    std::cout << "MyMap::Update called with dt = " << dt << std::endl;
}

void MyMap::Draw()
{
    DrawCircle(400, 300, 100, RED);
}

extern "C" __declspec(dllexport) GameMap* CreateGameMap() {
    return new MyMap();
}