#pragma once
#include "GameMap.h"
#include <iostream>

class MyMap : public GameMap 
{
public:
    MyMap();
    ~MyMap() override;
    void Update(float dt) override;
	void Draw() override;
};

// Factory function for DLL export
extern "C" __declspec(dllexport) GameMap* CreateGameMap();