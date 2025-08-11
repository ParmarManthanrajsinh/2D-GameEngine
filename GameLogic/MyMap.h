#pragma once
#include "Bubble.h"
#include "GameMap.h"
#include <iostream>

class MyMap : public GameMap 
{
    Vector2 postition;
    Vector2 velocity;
    float radius;
    float speed;
    short direction;

    Bubble bubble_particle;

public:
    MyMap();
    ~MyMap() override;
	void Initialize() override;
    void Update(float DeltaTime) override;
	void Draw() override;
};

// Factory function for DLL export
extern "C" __declspec(dllexport) GameMap* CreateGameMap();