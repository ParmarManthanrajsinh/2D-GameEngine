#pragma once
#include <raylib.h>
#include <vector>
#include <random>
#include <raymath.h>

struct t_Particle
{
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Color color;
	float life;
	float max_life;
	float size;
	float rotation;
	float rotation_speed;
	bool b_Active;
};

enum EmitterType
{
	POINT,
	LINE,
	CIRCLE,
	RECTANGLE
};

enum ParticleType
{
	CIRCULER,
	SQUARE,
	TRIANGLE,
	K_CHAR
};

class FireParticle
{
protected:
	friend class ParticleSaver;
	std::vector<t_Particle> particles;
	std::mt19937 rng;
	std::uniform_real_distribution<float> dist;

	// Emitter properties
	EmitterType e_EmitterType;
	ParticleType e_ParticleType;
	Texture2D particle_texture;
	bool b_UseTexture;
	float emission_rate;
	float emission_timer;

	// Particle properties
	Vector2 velocity;
	Vector2 velocity_variation;
	Vector2 acceleration;
	Color start_color;
	Color end_color;
	float min_life;
	float max_life;
	float min_size;
	float max_size;
	float rotation_speed;

	// Emitter shape properties
	float line_length;
	float circle_radius;
	Vector2 rect_size;
	bool b_Active;
	int max_particles;

	// For texture
	Rectangle texture_source_rect;
	Vector2 texture_half_size;
	Vector2 texture_center;
	int tex_width;
	int tex_height;
	int new_width;
	int new_height;
	int original_tex_width;
	int original_tex_height;
	float tex_size_percent;
	bool b_TextureDataCached;

	Vector2 GetEmissionPoint();
	void EmitParticle();
	void DrawEmitterShape() const;
	void Clear();
	int GetParticleCount() const;

public:
	Vector2 position;

	FireParticle();
	FireParticle(float x, float y); 
	~FireParticle(); 
	void Update(float dt);
	void Draw();

	// Texture management
	bool b_LoadTexture(const char* filename);
	void UnloadTexture();
	void SetUseTexture(bool use);
	bool b_IsUsingTexture() const;
};

// Constructor with current system parameters
inline FireParticle::FireParticle()
	: max_particles(1000),
	  rng(std::random_device{}()),
	  dist(0.0f, 1.0f),
	  position({ 672.00f, 504.50f }),
	  e_EmitterType(CIRCLE),
	  e_ParticleType(CIRCULER),
	  particle_texture({}),
	  b_UseTexture(false),
	  emission_rate(50.00f),
	  emission_timer(0.0f),
	  velocity({ 0.00f, -30.00f }),
	  velocity_variation({ 31.91f, 32.14f }),
	  acceleration({ 0.00f, -50.00f }),
	  start_color({ 230, 41, 55, 255 }),
	  end_color({ 255, 161, 0, 255 }),
	  min_life(1.00f),
	  max_life(3.00f),
	  min_size(2.00f),
	  max_size(8.00f),
	  rotation_speed(0.00f),
	  line_length(100.00f),
	  circle_radius(13.81f),
	  rect_size({ 100.00f, 100.00f }),
	  b_Active(true),
	  tex_width(0),
	  tex_height(0),
	  original_tex_width(0),
	  original_tex_height(0),
	  tex_size_percent(100.00f),
	  b_TextureDataCached(false)
{
	particles.reserve(max_particles);
}

inline FireParticle::FireParticle(float x, float y) : FireParticle()
{
	position = { x, y };
}

inline FireParticle::~FireParticle()
{
	UnloadTexture();
}

inline bool FireParticle::b_LoadTexture(const char* filename)
{
	// Unload Previous texture
	UnloadTexture();

	Image img = LoadImage(filename);
	original_tex_width = img.width;
	original_tex_height = img.height;

	new_width = static_cast<int>
	(
		(original_tex_width * tex_size_percent) / 100
	);

	new_height = static_cast<int>
	(
		(original_tex_height * tex_size_percent) / 100
	);

	if (new_width > 0 && new_height > 0)
	{
		ImageResize(&img, new_width, new_height);
	}

	particle_texture = LoadTextureFromImage(img);
	UnloadImage(img);
	if (particle_texture.id > 0)
	{
		b_UseTexture = true;
		texture_source_rect =
		{
			0, 0,
			static_cast<float>(particle_texture.width),
			static_cast<float>(particle_texture.height)
		};

		texture_half_size =
		{
			static_cast<float>(particle_texture.width) * 0.5f,
			static_cast<float>(particle_texture.height) * 0.5f
		};

		texture_center =
		{
			texture_source_rect.x / 2,
			texture_source_rect.y / 2
		};

		b_TextureDataCached = true;
		GenTextureMipmaps(&particle_texture);
		SetTextureFilter(particle_texture, TEXTURE_FILTER_TRILINEAR);
		return true;
	}

	b_UseTexture = false;
	b_TextureDataCached = false;
	return false;
}

inline void FireParticle::UnloadTexture()
{
	if (particle_texture.id > 0)
	{
		::UnloadTexture(particle_texture);
		particle_texture = {};
	}
	b_UseTexture = false;
	b_TextureDataCached = false;
}

inline void FireParticle::SetUseTexture(bool use)
{
	b_UseTexture = use && (particle_texture.id > 0);
}

inline bool FireParticle::b_IsUsingTexture() const
{
	return b_UseTexture && (particle_texture.id > 0);
}

inline Vector2 FireParticle::GetEmissionPoint()
{
	switch (e_EmitterType)
	{
	case POINT:
		return position;

	case LINE:
	{
		float t = dist(rng);
		return
		{
			position.x + (t - 0.5f) * line_length,
			position.y
		};
	}

	case CIRCLE:
	{
		float angle = (dist(rng) * 2.0f - 1.0f) * PI;
		float radius = dist(rng) * circle_radius;
		return
		{
			position.x + cosf(angle) * radius,
			position.y + sinf(angle) * radius
		};
	}

	case RECTANGLE:
	{
		return
		{
			position.x + (dist(rng) - 0.5f) * rect_size.x,
			position.y + (dist(rng) - 0.5f) * rect_size.y
		};
	}

	default:
		return position;
	}
}

inline void FireParticle::EmitParticle()
{
	if (particles.size() >= max_particles)
		return;

	t_Particle p{};
	p.position = GetEmissionPoint();
	p.velocity =
	{
		velocity.x + (dist(rng) - 0.5f) * velocity_variation.x,
		velocity.y + (dist(rng) - 0.5f) * velocity_variation.y
	};
	p.acceleration = acceleration;
	p.color = start_color;
	p.life = p.max_life = min_life + dist(rng) * (max_life - min_life);
	p.size = min_size + dist(rng) * (max_size - min_size);
	p.rotation = 0;
	p.rotation_speed = rotation_speed + (dist(rng) - 0.5f) * 2.0f;
	p.b_Active = true;
	particles.push_back(p);
}

inline void FireParticle::Update(float dt)
{
	if (!b_Active)
		return;

	// Emit new particles
	emission_timer += dt;
	const float EMISSION_INTERVAL = 1.0f / emission_rate;
	while (emission_timer >= EMISSION_INTERVAL)
	{
		EmitParticle();
		emission_timer -= EMISSION_INTERVAL;
	}

	bool b_ColorTransition =
		start_color.r != end_color.r ||
		start_color.g != end_color.g ||
		start_color.b != end_color.b;

	// Update existing particles
	for (size_t i = 0; i < particles.size();)
	{
		t_Particle& p = particles[i];
		if (!p.b_Active || p.life <= 0)
		{
			// Added optimization
			std::swap(particles[i], particles.back());
			particles.pop_back();
			continue;
		}

		// Physics update
		p.velocity.x += p.acceleration.x * dt;
		p.velocity.y += p.acceleration.y * dt;
		p.position.x += p.velocity.x * dt;
		p.position.y += p.velocity.y * dt;
		p.rotation += p.rotation_speed * dt;
		p.life -= dt;

		// Color fade
		float life_ratio = p.life / p.max_life;
		float t = 1.0f - life_ratio;

		if (b_ColorTransition)
		{
			p.color.r = static_cast<unsigned char>(Clamp(start_color.r * (1.0f - t) + end_color.r * t, 0, 255));
			p.color.g = static_cast<unsigned char>(Clamp(start_color.g * (1.0f - t) + end_color.g * t, 0, 255));
			p.color.b = static_cast<unsigned char>(Clamp(start_color.b * (1.0f - t) + end_color.b * t, 0, 255));
		}

		p.color.a = static_cast<unsigned char>(Clamp(255.0f * life_ratio, 0, 255));
		++i;
	}
}

inline void FireParticle::DrawEmitterShape() const
{
	Color shape_color = { 128, 128, 128, 100 };

	switch (e_EmitterType)
	{
	case POINT:
		DrawCircleV(position, 3, shape_color);
		break;

	case LINE:
		DrawLineEx
		(
			{ position.x - line_length / 2, position.y },
			{ position.x + line_length / 2, position.y },
			2, shape_color
		);
		break;

	case CIRCLE:
		DrawCircleLinesV(position, circle_radius, shape_color);
		break;

	case RECTANGLE:
		DrawRectangleLinesEx
		(
			{
				position.x - rect_size.x / 2,
				position.y - rect_size.y / 2,
				rect_size.x, rect_size.y
			},
			2, shape_color
		);
		break;
	}
}

inline void FireParticle::Draw()
{
	bool b_UseTex = b_IsUsingTexture() && b_TextureDataCached;

	for (const auto& p : particles)
	{
		if (!p.b_Active)
			continue;

		// If using texture, draw texture instead of geometric shapes
		if (b_UseTex)
		{
			Rectangle dest =
			{
				p.position.x - texture_center.x,
				p.position.y - texture_center.y,
				texture_source_rect.width,
				texture_source_rect.height
			};

			DrawTexturePro
			(
				particle_texture,
				texture_source_rect,
				dest,
				texture_half_size,
				p.rotation * RAD2DEG,
				WHITE
			);
		}
		else
		{
			// Draw geometric shapes 
			switch (e_ParticleType)
			{
			case CIRCULER:
			{
				DrawCircleV(p.position, p.size, p.color);
				break;
			}

			case SQUARE:
			{
				Rectangle rect =
				{
					p.position.x - p.size / 2,
					p.position.y - p.size / 2,
					p.size,
					p.size
				};

				DrawRectanglePro
				(
					rect,
					{ p.size / 2, p.size / 2 },
					p.rotation * RAD2DEG,
					p.color
				);
				break;
			}

			case TRIANGLE:
			{
				DrawPoly
				(
					p.position,
					3,
					p.size,
					p.rotation * RAD2DEG,
					p.color
				);
				break;
			}

			case K_CHAR:
			{
				Vector2 vertices[6] =
				{
					{-p.size / 2, p.size / 2},	// Top-left (vertical line top)
					{-p.size / 2, -p.size / 2}, // Bottom-left (vertical line bottom)
					{-p.size / 2, 0},			// Middle-left (center junction)
					{p.size / 2, p.size / 2},	// Top-right (upper diagonal end)
					{p.size / 2, -p.size / 2},	// Bottom-right (lower diagonal end)
					{-p.size / 2, 0}			// Middle-left (center junction - repeated for connection)
				};

				// Define the lines that make up the K shape
				int lines[][2] =
				{
					{0, 1}, // Vertical line (left side)
					{2, 3}, // Upper diagonal
					{2, 4}	// Lower diagonal
				};

				// Draw each line of the K
				for (int i = 0; i < 3; ++i)
				{
					Vector2& v1 = vertices[lines[i][0]];
					Vector2& v2 = vertices[lines[i][1]];

					// Apply rotation transformation
					Vector2 a =
					{
						p.position.x + v1.x * cosf(p.rotation) - v1.y * sinf(p.rotation),
						p.position.y + v1.x * sinf(p.rotation) + v1.y * cosf(p.rotation)
					};
					Vector2 b =
					{
						p.position.x + v2.x * cosf(p.rotation) - v2.y * sinf(p.rotation),
						p.position.y + v2.x * sinf(p.rotation) + v2.y * cosf(p.rotation)
					};

					DrawLineEx(a, b, 2, p.color);
				}
				break;
			}

			}
		}
	}

	EndScissorMode();
}

inline void FireParticle::Clear()
{
	particles.clear();
}

inline int FireParticle::GetParticleCount() const
{
	return static_cast<int>(particles.size());
}
