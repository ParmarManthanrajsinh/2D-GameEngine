# RayWaves Particle System

Simple particle effects using FireParticle class.

## Quick Start

```cpp
#include "FireParticle.h"

class MyLevel : public GameMap {
private:
    std::vector<FireParticle> m_Particles;
    
public:
    void Update(float delta_time) override {
        // Spawn particles
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            m_Particles.emplace_back(mousePos.x, mousePos.y);
        }
        
        // Update particles
        for (auto& particle : m_Particles) {
            particle.Update(delta_time);
        }
        
        // Remove dead particles
        m_Particles.erase(
            std::remove_if(m_Particles.begin(), m_Particles.end(),
                [](const FireParticle& p) { return !p.IsAlive(); }),
            m_Particles.end()
        );
    }
    
    void Draw() override {
        for (const auto& particle : m_Particles) {
            particle.Draw();
        }
    }
};
```

## FireParticle Configuration

```cpp
FireParticle particle(x, y);

// Basic properties
particle.SetLifetime(2.0f);         // Seconds to live
particle.SetVelocity({100, -200});  // Initial velocity
particle.SetGravity({0, 98.0f});    // Gravity effect

// Visual effects
particle.SetSize(5.0f);             // Particle size
particle.SetColor(RED);             // Start color
particle.SetEndColor(YELLOW);       // End color (fades)

// Advanced effects
particle.SetFadeOut(true);          // Fade alpha over time
particle.SetSizeDecrease(true);     // Shrink over time
particle.SetRandomness(0.5f);       // Random velocity variation
```

## Common Patterns

### Fire Effect
```cpp
FireParticle fire(x, y);
fire.SetVelocity({GetRandomValue(-50, 50), GetRandomValue(-100, -200)});
fire.SetGravity({0, -20.0f});  // Upward gravity for fire
fire.SetColor(ORANGE);
fire.SetEndColor(RED);
fire.SetLifetime(1.5f);
```

### Explosion Effect
```cpp
for (int i = 0; i < 20; i++) {
    FireParticle particle(explosionX, explosionY);
    float angle = (float)i / 20.0f * 2.0f * PI;
    float speed = GetRandomValue(100, 300);
    particle.SetVelocity({cos(angle) * speed, sin(angle) * speed});
    particle.SetColor(YELLOW);
    particle.SetLifetime(2.0f);
    particles.push_back(particle);
}
```

### Sparks Effect
```cpp
FireParticle spark(x, y);
spark.SetVelocity({GetRandomValue(-100, 100), GetRandomValue(-50, 0)});
spark.SetGravity({0, 300.0f});  // Strong downward gravity
spark.SetSize(2.0f);
spark.SetColor(WHITE);
spark.SetLifetime(0.8f);
```

## Performance Tips

- Limit particle count (remove old particles)
- Use object pooling for better performance
- Consider using simpler shapes for many particles
- Batch similar particle rendering when possible

## Custom Particle Systems

Extend FireParticle or create your own:

```cpp
class MyParticle {
private:
    Vector2 m_Position;
    Vector2 m_Velocity;
    float m_Lifetime;
    Color m_Color;
    
public:
    void Update(float delta_time) {
        m_Position.x += m_Velocity.x * delta_time;
        m_Position.y += m_Velocity.y * delta_time;
        m_Lifetime -= delta_time;
    }
    
    void Draw() {
        DrawCircleV(m_Position, 3.0f, m_Color);
    }
    
    bool IsAlive() const { return m_Lifetime > 0; }
};
```

---

For more advanced effects, see the FireParticle.h implementation in GameLogic folder.