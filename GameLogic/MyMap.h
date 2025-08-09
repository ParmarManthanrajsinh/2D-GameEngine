#pragma once
#include "Blood_fountain.h"
#include "GameMap.h"
#include <iostream>

class MyMap : public GameMap 
{
    BloodParticle bp;
public:
    MyMap();
    ~MyMap() override;
	void Initialize() override;
    void Update(float dt) override;
	void Draw() override;
};

// Factory function for DLL export
extern "C" __declspec(dllexport) GameMap* CreateGameMap();