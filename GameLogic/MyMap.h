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
    int sWidth;
    int sHeight;
    short direction;

    Bubble b;

public:
    MyMap();
    ~MyMap() override;
	void Initialize() override;
    void Update(float dt) override;
	void Draw() override;
};

// Factory function for DLL export
extern "C" __declspec(dllexport) GameMap* CreateGameMap();