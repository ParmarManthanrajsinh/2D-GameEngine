# Particle System Guide

**Version:** 1.0  
**Last Updated:** January 2025

This guide provides comprehensive documentation for the FireParticle system, including usage examples, configuration options, and advanced techniques.

---

## Table of Contents

1. [Overview](#overview)
2. [Basic Usage](#basic-usage)
3. [Configuration Options](#configuration-options)
4. [Emitter Types](#emitter-types)
5. [Particle Shapes](#particle-shapes)
6. [Texture-Based Particles](#texture-based-particles)
7. [Performance Optimization](#performance-optimization)
8. [Advanced Effects](#advanced-effects)

---

## Overview

The FireParticle system is a comprehensive particle engine that supports:
- Multiple emitter shapes (Point, Line, Circle, Rectangle)
- Various particle types (Circle, Square, Triangle, Custom)
- Texture-based particles
- Color transitions and alpha fading
- Physics simulation with gravity and acceleration
- Configurable emission rates and particle lifetimes

---

## Basic Usage

### Creating a Simple Fire Effect

```cpp
#include "FireParticle.h"

class MyMap : public GameMap
{
private:
    FireParticle m_fire;
    
public:
    MyMap() : m_fire(400.0f, 300.0f) {}  // Position at (400, 300)
    
    void Update(float delta_time) override 
    {
        m_fire.Update(delta_time);
    }
    
    void Draw() override 
    {
        ClearBackground(BLACK);
        m_fire.Draw();
    }
};
```

### Multiple Particle Systems

```cpp
class EffectsMap : public GameMap
{
private:
    std::vector<FireParticle> m_effects;
    
public:
    void Initialize() override 
    {
        // Campfire at (200, 400)
        FireParticle campfire(200.0f, 400.0f);
        campfire.start_color = {255, 100, 0, 255};  // Orange
        campfire.end_color = {255, 0, 0, 255};      // Red
        m_effects.push_back(campfire);
        
        // Torch at (600, 300)
        FireParticle torch(600.0f, 300.0f);
        torch.start_color = {255, 200, 100, 255};   // Bright orange
        torch.end_color = {100, 0, 0, 255};         // Dark red
        torch.circle_radius = 10.0f;                // Smaller radius
        m_effects.push_back(torch);
    }
    
    void Update(float delta_time) override 
    {
        for (auto& effect : m_effects) {
            effect.Update(delta_time);
        }
    }
    
    void Draw() override 
    {
        ClearBackground(DARKBLUE);
        for (auto& effect : m_effects) {
            effect.Draw();
        }
    }
};
```

---

## Configuration Options

### Core Properties

```cpp
FireParticle particles(x, y);

// Emission Settings
particles.emission_rate = 50.0f;           // Particles per second
particles.max_particles = 1000;            // Maximum particle count

// Particle Movement
particles.velocity = {0.0f, -50.0f};       // Base velocity (upward)
particles.velocity_variation = {30.0f, 20.0f}; // Random velocity range
particles.acceleration = {0.0f, -30.0f};   // Gravity/wind effect

// Particle Appearance
particles.start_color = {255, 255, 0, 255}; // Yellow
particles.end_color = {255, 0, 0, 255};     // Red
particles.min_size = 2.0f;                  // Smallest particle size
particles.max_size = 8.0f;                  // Largest particle size

// Particle Lifetime
particles.min_life = 1.0f;                  // Minimum seconds alive
particles.max_life = 3.0f;                  // Maximum seconds alive

// Rotation
particles.rotation_speed = 45.0f;           // Degrees per second
```

### Color Transitions

Particles smoothly transition from start_color to end_color over their lifetime:

```cpp
// Fire effect
particles.start_color = {255, 255, 100, 255}; // Bright yellow
particles.end_color = {255, 50, 0, 255};      // Dark red

// Smoke effect
particles.start_color = {100, 100, 100, 255}; // Dark gray
particles.end_color = {200, 200, 200, 100};   // Light gray (transparent)

// Magic sparkles
particles.start_color = {100, 100, 255, 255}; // Blue
particles.end_color = {255, 100, 255, 255};   // Purple

// No color transition
particles.start_color = {255, 0, 0, 255};     // Red
particles.end_color = {255, 0, 0, 255};       // Same red (no change)
```

---

## Emitter Types

### Point Emitter

Emits particles from a single point:

```cpp
particles.e_EmitterType = POINT;
particles.position = {400.0f, 300.0f};

// Use case: Explosion center, torch flame, magical orb
```

### Line Emitter

Emits particles along a line:

```cpp
particles.e_EmitterType = LINE;
particles.position = {400.0f, 300.0f};      // Center of line
particles.line_length = 100.0f;             // Length of emission line

// Use case: Waterfall, wall of fire, laser beam effect
```

### Circle Emitter

Emits particles within a circular area:

```cpp
particles.e_EmitterType = CIRCLE;
particles.position = {400.0f, 300.0f};      // Center of circle
particles.circle_radius = 25.0f;            // Radius of emission area

// Use case: Campfire, explosion, magic circle
```

### Rectangle Emitter

Emits particles within a rectangular area:

```cpp
particles.e_EmitterType = RECTANGLE;
particles.position = {400.0f, 300.0f};      // Center of rectangle
particles.rect_size = {50.0f, 30.0f};       // Width and height

// Use case: Chimney smoke, vehicle exhaust, area effects
```

### Advanced Emitter Example

```cpp
class WeatherMap : public GameMap
{
private:
    FireParticle m_rain;
    FireParticle m_snow;
    
public:
    void Initialize() override 
    {
        // Rain effect across top of screen
        m_rain.e_EmitterType = LINE;
        m_rain.position = {640.0f, 0.0f};        // Top center
        m_rain.line_length = 1280.0f;            // Full screen width
        m_rain.velocity = {0.0f, 500.0f};        // Fast downward
        m_rain.velocity_variation = {50.0f, 0.0f}; // Some horizontal variation
        m_rain.start_color = {100, 150, 255, 255}; // Light blue
        m_rain.end_color = {100, 150, 255, 255};   // Same (no fade)
        m_rain.min_size = 1.0f;
        m_rain.max_size = 2.0f;
        m_rain.emission_rate = 200.0f;           // Heavy rain
        
        // Snow effect
        m_snow.e_EmitterType = LINE;
        m_snow.position = {640.0f, -50.0f};      // Above screen
        m_snow.line_length = 1400.0f;            // Wider than screen
        m_snow.velocity = {0.0f, 100.0f};        // Slow downward
        m_snow.velocity_variation = {100.0f, 50.0f}; // Drifting motion
        m_snow.start_color = WHITE;
        m_snow.end_color = WHITE;
        m_snow.min_size = 3.0f;
        m_snow.max_size = 6.0f;
        m_snow.emission_rate = 50.0f;            // Gentle snowfall
        m_snow.b_Active = false;                 // Start inactive
    }
    
    void Update(float delta_time) override 
    {
        // Toggle weather with keys
        if (IsKeyPressed(KEY_R)) {
            m_rain.b_Active = !m_rain.b_Active;
        }
        if (IsKeyPressed(KEY_S)) {
            m_snow.b_Active = !m_snow.b_Active;
        }
        
        m_rain.Update(delta_time);
        m_snow.Update(delta_time);
    }
    
    void Draw() override 
    {
        ClearBackground(DARKGRAY);
        m_rain.Draw();
        m_snow.Draw();
        
        DrawText("Press R for Rain, S for Snow", 10, 10, 20, WHITE);
    }
};
```

---

## Particle Shapes

### Circle Particles

Default circular particles:

```cpp
particles.e_ParticleType = CIRCULER;  // Note: spelling preserved for compatibility
particles.min_size = 3.0f;
particles.max_size = 10.0f;

// Good for: Fire, sparks, bubbles, stars
```

### Square Particles

Rectangular particles that can rotate:

```cpp
particles.e_ParticleType = SQUARE;
particles.rotation_speed = 90.0f;  // Rotate 90 degrees per second

// Good for: Confetti, debris, pixel effects
```

### Triangle Particles

Three-sided polygonal particles:

```cpp
particles.e_ParticleType = TRIANGLE;
particles.rotation_speed = 180.0f;  // Fast spinning triangles

// Good for: Shards, crystals, magical effects
```

### Custom K-shaped Particles

Special custom shape resembling the letter "K":

```cpp
particles.e_ParticleType = K_CHAR;
particles.rotation_speed = 45.0f;

// Good for: Logo effects, special branding, unique visual style
```

### Shape Comparison Example

```cpp
class ShapeDemo : public GameMap
{
private:
    std::vector<FireParticle> m_demonstrations;
    
public:
    void Initialize() override 
    {
        // Circle demo
        FireParticle circles(200.0f, 300.0f);
        circles.e_ParticleType = CIRCULER;
        circles.start_color = RED;
        m_demonstrations.push_back(circles);
        
        // Square demo
        FireParticle squares(400.0f, 300.0f);
        squares.e_ParticleType = SQUARE;
        squares.start_color = GREEN;
        squares.rotation_speed = 90.0f;
        m_demonstrations.push_back(squares);
        
        // Triangle demo
        FireParticle triangles(600.0f, 300.0f);
        triangles.e_ParticleType = TRIANGLE;
        triangles.start_color = BLUE;
        triangles.rotation_speed = 180.0f;
        m_demonstrations.push_back(triangles);
        
        // K-shape demo
        FireParticle kshapes(800.0f, 300.0f);
        kshapes.e_ParticleType = K_CHAR;
        kshapes.start_color = YELLOW;
        kshapes.rotation_speed = 45.0f;
        m_demonstrations.push_back(kshapes);
    }
};
```

---

## Texture-Based Particles

### Loading and Using Textures

```cpp
FireParticle textured_particles(400.0f, 300.0f);

// Load texture file
if (textured_particles.b_LoadTexture("Assets/Textures/star.png")) {
    textured_particles.SetUseTexture(true);
    std::cout << "Texture loaded successfully!" << std::endl;
} else {
    std::cout << "Failed to load texture, using geometric shapes" << std::endl;
}
```

### Texture Requirements

- **Supported Formats**: PNG (recommended), JPG, BMP, TGA
- **Transparency**: PNG with alpha channel works best
- **Size**: Keep textures small (32x32 to 128x128 typical)
- **Design**: Centered design works best for rotation

### Texture Management

```cpp
class TexturedEffect : public GameMap
{
private:
    FireParticle m_stars;
    FireParticle m_smoke;
    
public:
    void Initialize() override 
    {
        // Star particles with texture
        m_stars.position = {400.0f, 300.0f};
        if (m_stars.b_LoadTexture("Assets/star.png")) {
            m_stars.SetUseTexture(true);
            m_stars.rotation_speed = 90.0f;  // Rotating star particles
        }
        
        // Smoke particles with texture
        m_smoke.position = {200.0f, 400.0f};
        if (m_smoke.b_LoadTexture("Assets/smoke.png")) {
            m_smoke.SetUseTexture(true);
            m_smoke.velocity = {0.0f, -30.0f};         // Upward movement
            m_smoke.start_color = {255, 255, 255, 255}; // Full opacity
            m_smoke.end_color = {255, 255, 255, 0};     // Fade to transparent
        }
    }
    
    ~TexturedEffect() 
    {
        // Textures are automatically unloaded in FireParticle destructor
        // No manual cleanup needed
    }
};
```

### Texture vs Geometric Performance

- **Geometric Shapes**: Faster rendering, good for many particles
- **Textured Particles**: More detailed appearance, slightly slower
- **Recommendation**: Use textures for important effects, geometry for ambient effects

---

## Performance Optimization

### Particle Count Management

```cpp
class OptimizedParticles : public GameMap
{
private:
    FireParticle m_effect;
    
public:
    void Initialize() override 
    {
        // Limit maximum particles for performance
        m_effect.max_particles = 500;  // Reduced from default 1000
        
        // Adjust emission rate accordingly
        m_effect.emission_rate = 100.0f;  // Fewer particles per second
        
        // Shorter lifetime = fewer active particles
        m_effect.min_life = 0.5f;
        m_effect.max_life = 1.5f;
    }
    
    void Update(float delta_time) override 
    {
        // Monitor performance
        if (GetFPS() < 30) {
            // Reduce particle count if FPS drops
            if (m_effect.max_particles > 100) {
                m_effect.max_particles -= 50;
                std::cout << "Reduced particles to: " << m_effect.max_particles << std::endl;
            }
        }
        
        m_effect.Update(delta_time);
    }
};
```

### Optimization Guidelines

1. **Limit Active Systems**: Don't run too many particle systems simultaneously
2. **Reduce Particle Count**: Start with fewer particles and increase if performance allows
3. **Optimize Texture Size**: Use smallest texture size that looks good
4. **Control Emission Rate**: Lower emission rates can still look great
5. **Shorter Lifetimes**: Particles that live shorter consume fewer resources

### Performance Monitoring

```cpp
class PerformanceAware : public GameMap
{
private:
    FireParticle m_effect;
    float m_performanceTimer = 0.0f;
    
public:
    void Update(float delta_time) override 
    {
        m_performanceTimer += delta_time;
        
        // Check performance every second
        if (m_performanceTimer >= 1.0f) {
            int fps = GetFPS();
            int particle_count = m_effect.GetParticleCount();
            
            std::cout << "FPS: " << fps 
                      << ", Particles: " << particle_count << std::endl;
            
            m_performanceTimer = 0.0f;
        }
        
        m_effect.Update(delta_time);
    }
};
```

---

## Advanced Effects

### Explosion Effect

```cpp
class ExplosionEffect 
{
private:
    FireParticle m_explosion;
    float m_timer = 0.0f;
    bool m_active = false;
    
public:
    void Trigger(Vector2 position) 
    {
        m_explosion.position = position;
        m_explosion.e_EmitterType = CIRCLE;
        m_explosion.circle_radius = 5.0f;
        m_explosion.emission_rate = 500.0f;        // Intense burst
        m_explosion.velocity = {0.0f, 0.0f};       // Explode in all directions
        m_explosion.velocity_variation = {300.0f, 300.0f}; // High variation
        m_explosion.start_color = {255, 255, 0, 255}; // Bright yellow
        m_explosion.end_color = {255, 0, 0, 255};     // Red
        m_explosion.min_life = 0.2f;               // Short lived
        m_explosion.max_life = 1.0f;
        m_explosion.b_Active = true;
        m_active = true;
        m_timer = 0.0f;
    }
    
    void Update(float delta_time) 
    {
        if (m_active) {
            m_timer += delta_time;
            
            // Stop emitting after short burst
            if (m_timer > 0.2f) {
                m_explosion.b_Active = false;
            }
            
            // Deactivate when all particles are gone
            if (m_timer > 2.0f) {
                m_active = false;
            }
            
            m_explosion.Update(delta_time);
        }
    }
    
    void Draw() 
    {
        if (m_active) {
            m_explosion.Draw();
        }
    }
};
```

### Fountain Effect

```cpp
void CreateFountain(FireParticle& fountain, Vector2 position) 
{
    fountain.position = position;
    fountain.e_EmitterType = CIRCLE;
    fountain.circle_radius = 15.0f;
    
    // Upward fountain motion
    fountain.velocity = {0.0f, -200.0f};           // Strong upward
    fountain.velocity_variation = {100.0f, 50.0f}; // Some spread
    fountain.acceleration = {0.0f, 300.0f};        // Gravity pulls down
    
    // Water-like colors
    fountain.start_color = {100, 150, 255, 255};   // Light blue
    fountain.end_color = {0, 100, 200, 100};       // Darker blue, transparent
    
    fountain.emission_rate = 100.0f;
    fountain.min_life = 1.0f;
    fountain.max_life = 2.5f;
    fountain.min_size = 2.0f;
    fountain.max_size = 5.0f;
}
```

### Trail Effect

```cpp
class TrailEffect 
{
private:
    std::vector<FireParticle> m_trail_segments;
    Vector2 m_last_position;
    float m_trail_timer = 0.0f;
    
public:
    void Update(Vector2 current_position, float delta_time) 
    {
        m_trail_timer += delta_time;
        
        // Add new trail segment periodically
        if (m_trail_timer > 0.1f && 
            Vector2Distance(current_position, m_last_position) > 10.0f) {
            
            FireParticle trail_segment(current_position.x, current_position.y);
            trail_segment.e_EmitterType = POINT;
            trail_segment.emission_rate = 50.0f;
            trail_segment.velocity = {0.0f, 0.0f};     // Stationary
            trail_segment.start_color = {255, 255, 0, 255}; // Yellow
            trail_segment.end_color = {255, 0, 0, 0};       // Fade to transparent
            trail_segment.min_life = 0.5f;
            trail_segment.max_life = 1.0f;
            trail_segment.b_Active = true;
            
            m_trail_segments.push_back(trail_segment);
            m_last_position = current_position;
            m_trail_timer = 0.0f;
            
            // Limit trail length
            if (m_trail_segments.size() > 20) {
                m_trail_segments.erase(m_trail_segments.begin());
            }
        }
        
        // Update all trail segments
        for (auto& segment : m_trail_segments) {
            segment.Update(delta_time);
        }
    }
    
    void Draw() 
    {
        for (auto& segment : m_trail_segments) {
            segment.Draw();
        }
    }
};
```

---

This particle system guide provides comprehensive coverage of the FireParticle system capabilities. Experiment with different combinations of settings to create unique visual effects for your games!