#include "GameEditor.h"
#include "background.h"
#include "water.h"

int main() 
{
	// Initialization
	GameEditorInit(900, 900, "GameDemo");
	SetTargetFPS(60);

	Background background("Assets/oggy.jpeg"); // Load a background image
	Water particle;
	particle.position = { 450, 450 };

	char selected_file_path[1024] = "";

	// Main game loop
	while (!WindowShouldClose()) 
	{
		float delta_time = GetFrameTime();
		particle.Update(delta_time);

		// Start Drawing
		BeginDrawing();
		ClearBackground(RAYWHITE);

		// Draw the background
		background.Draw();
		particle.Draw();

		// Start ImGui frame
		rlImGuiBegin();

		// Your ImGui Code
		ImGui::Begin("Demo Window");
		ImGui::Text("Hello, world!");

		static int counter = 0;
		if (ImGui::Button("Click Me")) {
			counter++;
		}
		ImGui::Text("Button pressed %d times", counter);

		if (ImGui::Button("Open File Dialog"))
		{
			const char* filterPatterns[] = { "*.png", "*.jpg", "*.jpeg" };
			const char* file = tinyfd_openFileDialog
			(
				"Select an Image",
				"",
				3,
				filterPatterns,
				"Image files",
				0
			);

			if (file)
			{
				strncpy_s(selected_file_path, sizeof(selected_file_path), file, _TRUNCATE);
			}
		}

		if (selected_file_path[0] != '\0')
		{
			ImGui::Text("Selected File: %s", selected_file_path);
		}

		ImGui::End();

		// End ImGui Frame
		rlImGuiEnd();
		EndDrawing();
	}

	GameEditorClose();
	return 0;
}
