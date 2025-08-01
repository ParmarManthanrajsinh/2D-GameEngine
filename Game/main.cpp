#include "GameEditor.h"

// Simple game object structure for demo
struct GameObject
{
	char name[64];
	float position[3] = {0.0f, 0.0f, 0.0f};
	float rotation[3] = {0.0f, 0.0f, 0.0f};
	float scale[3] = {1.0f, 1.0f, 1.0f};
	bool enabled = true;
};

int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	GameEditorInit(1280, 720, "Simple Game Engine");
	SetTargetFPS(60);

	// Demo data
	GameObject gameObjects[10];
	int objectCount = 3;
	int selectedObject = 0;

	// Initialize some demo objects
	strcpy_s(gameObjects[0].name, "Player");
	strcpy_s(gameObjects[1].name, "Camera");
	strcpy_s(gameObjects[2].name, "Light");

	gameObjects[1].position[1] = 5.0f;
	gameObjects[2].position[0] = 2.0f;
	gameObjects[2].position[1] = 3.0f;

	bool showDemo = false;
	char logBuffer[1024] = "Engine initialized successfully\nReady to create!\n";

	ImGuiViewport *viewport = ImGui::GetMainViewport();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(DARKGRAY);

		rlImGuiBegin();

		// Create dockspace
		ImGui::DockSpaceOverViewport(0, viewport);

		// Menu Bar
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					strcat_s(logBuffer, "New scene created\n");
				}

				if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
				{
					strcat_s(logBuffer, "Scene loading...\n");
				}

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				{
					strcat_s(logBuffer, "Scene saved\n");
				}

				ImGui::Separator();
				if (ImGui::MenuItem("Exit"))
				{
					// Close window
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "Ctrl+Z"))
				{
				}
				if (ImGui::MenuItem("Redo", "Ctrl+Y"))
				{
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				ImGui::MenuItem("Show Demo", NULL, &showDemo);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Scene Hierarchy Panel
		ImGui::Begin("Scene Hierarchy");
		ImGui::Text("Objects: %d", objectCount);
		ImGui::Separator();

		for (int i = 0; i < objectCount; i++)
		{
			bool isSelected = (selectedObject == i);
			if (ImGui::Selectable(gameObjects[i].name, isSelected))
			{
				selectedObject = i;
			}
		}

		ImGui::Separator();
		if (ImGui::Button("Add Object"))
		{
			if (objectCount < 10)
			{
				sprintf_s(gameObjects[objectCount].name, "GameObject_%d", objectCount);
				objectCount++;
				strcat_s(logBuffer, "New object added\n");
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete") && objectCount > 0)
		{
			objectCount--;
			strcat_s(logBuffer, "Object deleted\n");
		}
		ImGui::End();

		// Inspector Panel
		ImGui::Begin("Inspector");
		if (selectedObject < objectCount)
		{
			GameObject &obj = gameObjects[selectedObject];

			ImGui::Text("Selected: %s", obj.name);
			ImGui::Separator();

			ImGui::InputText("Name", obj.name, sizeof(obj.name));
			ImGui::Checkbox("Enabled", &obj.enabled);

			ImGui::Spacing();
			ImGui::Text("Transform");
			ImGui::DragFloat3("Position", obj.position, 0.1f);
			ImGui::DragFloat3("Rotation", obj.rotation, 1.0f);
			ImGui::DragFloat3("Scale", obj.scale, 0.1f);

			ImGui::Spacing();
			if (ImGui::Button("Reset Transform"))
			{
				obj.position[0] = obj.position[1] = obj.position[2] = 0.0f;
				obj.rotation[0] = obj.rotation[1] = obj.rotation[2] = 0.0f;
				obj.scale[0] = obj.scale[1] = obj.scale[2] = 1.0f;
			}
		}
		else
		{
			ImGui::Text("No object selected");
		}
		ImGui::End();

		// Scene View (placeholder)
		ImGui::Begin("Scene View");
		ImVec2 viewSize = ImGui::GetContentRegionAvail();
		ImGui::Text("Scene Viewport (%dx%d)", (int)viewSize.x, (int)viewSize.y);
		ImGui::Text("Right-click to interact");

		// Simple scene representation
		ImDrawList *drawList = ImGui::GetWindowDrawList();
		ImVec2 canvasPos = ImGui::GetCursorScreenPos();
		ImVec2 canvasSize = ImGui::GetContentRegionAvail();

		if (canvasSize.x > 50 && canvasSize.y > 50)
		{
			drawList->AddRectFilled
			(
				canvasPos,
				ImVec2
				(
					canvasPos.x + canvasSize.x, 
					canvasPos.y + canvasSize.y
				),
				IM_COL32(50, 50, 50, 255)
			);

			// Draw objects as simple shapes
			for (int i = 0; i < objectCount; i++)
			{
				float x = canvasPos.x + canvasSize.x * 0.5f + gameObjects[i].position[0] * 20;
				float y = canvasPos.y + canvasSize.y * 0.5f - gameObjects[i].position[1] * 20;

				ImU32 color = (i == selectedObject) ? IM_COL32(255, 255, 0, 255) : IM_COL32(100, 150, 255, 255);
				drawList->AddCircleFilled(ImVec2(x, y), 8, color);
				drawList->AddText(ImVec2(x + 12, y - 8), IM_COL32(255, 255, 255, 255), gameObjects[i].name);
			}
		}
		ImGui::End();

		// Console/Log Panel
		ImGui::Begin("Console");
		ImGui::TextWrapped("%s", logBuffer);

		if (ImGui::Button("Clear"))
		{
			logBuffer[0] = '\0';
		}
		ImGui::SameLine();
		if (ImGui::Button("Test Log"))
		{
			strcat_s(logBuffer, "Test message added\n");
		}
		ImGui::End();

		// Assets Panel (placeholder)
		ImGui::Begin("Assets");
		ImGui::Text("Project Assets");
		ImGui::Separator();

		const char *assetTypes[] = 
		{
			"Textures", "Models", "Scripts", "Audio", "Materials"
		};
		for (int i = 0; i < 5; i++)
		{
			if (ImGui::TreeNode(assetTypes[i]))
			{
				ImGui::Text("  - asset_example_%d", i + 1);
				ImGui::Text("  - asset_example_%d", i + 2);
				ImGui::TreePop();
			}
		}
		ImGui::End();

		// Show ImGui demo if requested
		if (showDemo)
		{
			ImGui::ShowDemoWindow(&showDemo);
		}

		rlImGuiEnd();
		EndDrawing();
	}

	GameEditorClose();
	return 0;
}