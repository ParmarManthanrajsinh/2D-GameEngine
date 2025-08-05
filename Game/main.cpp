#include "GameEditor.h"
#include "GameEngine.h"

class MyMap : public GameMap
{
	Vector2 position;
public:
	MyMap() : GameMap("MyMap") 
	{
		position = { 0, 0 };
	}
	~MyMap() = default;
	
	void Initialize() override
	{
		position = { 400, 300 };
	}

	void Draw() override
	{
		DrawCircle(int(position.x), int(position.y), 50, RED);
	}

	void Update(float deltaTime) override
	{
		position.x += 100 * deltaTime;
	}
};

int main()
{
	printf("Game Engine Starting...\n");

	std::unique_ptr<GameMap> my_map = std::make_unique<MyMap>();

	GameEditor editor;

	editor.Init(1200,700,"My Game");
	editor.LoadMap(my_map);
	editor.Run();
    
    return 0;
}