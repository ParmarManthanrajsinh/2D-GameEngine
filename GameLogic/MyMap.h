#pragma once
#include "GameMap.h"

class MyMap : public GameMap 
{
    Vector2 position;
public:
    MyMap();
    ~MyMap() override;
	void Initialize() override;
    void Update(float dt) override;
	void Draw() override;
};

// Factory function for DLL export
extern "C" __declspec(dllexport) GameMap* CreateGameMap();