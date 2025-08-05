#include "GameEditor.h"
#include "GameEngine.h"

class MyMap : public GameMap
{
	Vector2 position;

	public:
	MyMap() : GameMap("MyMap") 
	{
		position = { 400, 300 };
	}
	~MyMap() {}
	
	void Draw() override
	{
		DrawCircle(int(position.x), int(position.y), 50, RED);
	}

	void Update(float deltaTime) override
	{
		position.x += 10 * deltaTime;
	}

	void Initialize() override
	{
	}
};

int main()
{
	printf("Game Engine Starting...\n");

	std::unique_ptr<GameMap> my_map = std::make_unique<MyMap>();

	GameEditor editor;
	editor.Init(1280,720,"My Game");
	editor.LoadMap(my_map);
	editor.Run();
    
    return 0;
}