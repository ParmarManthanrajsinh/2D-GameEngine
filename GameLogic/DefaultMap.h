#pragma once
#include "../Engine/GameMap.h"
#include <iostream>

/**
 * @brief Default fallback map for when no other maps are available
 * 
 * This map provides a basic placeholder when no maps are registered,
 * showing helpful information to developers about how to register maps.
 */
class DefaultMap : public GameMap
{
private:
    float m_TimeAlive;
    int center_x = 0;
    int content_y = 140;
    int margin_left = 80;
    int step_spacing = 80;

public:
    DefaultMap();
    ~DefaultMap() override = default;

    void Initialize() override;
    void Update(float delta_time) override;
    void Draw() override;
};