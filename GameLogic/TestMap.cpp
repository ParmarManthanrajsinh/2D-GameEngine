#include "TestMap.h"

TestMap::TestMap()
{
    m_Position = { 0, 0 };
    m_SceneDimension = { 0, 0 };
    m_Velocity = { 0, 0 };
    m_Direction = 0;
    m_Speed = 0.0f;
    m_Radius = 0;

}

TestMap::~TestMap()
{
}

void TestMap::Initialize()
{
    m_Position = { 100, 100 };
    m_Radius = 30;
    m_Speed = 3.0f;
    m_Direction = -1;
    m_Velocity = { 200.0f, 150.0f };
    m_SceneDimension = GetSceneBounds();
}

void TestMap::Update(float dt)
{
    m_Position.x += m_Velocity.x * m_Speed * dt;
    m_Position.y += m_Velocity.y * m_Speed * dt;


    // Bounce horizontally
    if (m_Position.x - m_Radius <= 0.0f
        || m_Position.x + m_Radius >= static_cast<float>(m_SceneDimension.x))
    {
        m_Velocity.x *= -1.0f;
    }

    // Bounce vertically
    if (m_Position.y - m_Radius <= 0.0f
        || m_Position.y + m_Radius >= static_cast<float>(m_SceneDimension.y))
    {
        m_Velocity.y *= -1.0f;
    }
}

void TestMap::Draw()
{
    DrawCircle
    (
        static_cast<int>(m_Position.x),
        static_cast<int>(m_Position.y),
        m_Radius,
        RED
    );
}