/*******************************************************************************************
*
*   raylib-extras [ImGui] example - Simple Integration
*
*	This is a simple ImGui Integration
*	It is done using C++ but with C style code
*	It can be done in C as well if you use the C ImGui wrapper
*	https://github.com/cimgui/cimgui
*
*   Copyright (c) 2021 Jeffery Myers
*
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"

#include "imgui.h"
#include "rlImGui.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

void UpdatePaintFrame(void)
{
  BeginDrawing();
  ClearBackground(DARKGRAY);

  // start ImGui Conent
  rlImGuiBegin();

  // show ImGui Content
  bool open = true;
  ImGui::ShowDemoWindow(&open);

  // end ImGui Content
  rlImGuiEnd();

  EndDrawing();
}

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1280;
	int screenHeight = 800;

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(screenWidth, screenHeight, "raylib-Extras [ImGui] example - simple ImGui Demo");
	SetTargetFPS(144);
	rlImGuiSetup(true);

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdatePaintFrame, 0, 1);
#else

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
    UpdatePaintFrame();
	}
	rlImGuiShutdown();

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------
#endif
	return 0;
}
