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

#include "range_slider.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include <cstdio>
#include <vector>
#include <ctype.h>          // toupper
#include <string>

struct ExampleAppConsole
{
  char                  InputBuf[256];
  ImVector<char*>       Items;
  ImVector<const char*> Commands;
  int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
  ImGuiTextFilter       Filter;
  bool                  AutoScroll;
  bool                  ScrollToBottom;

  ExampleAppConsole()
    {
    }
  ~ExampleAppConsole()
    {
    }

  void    Draw(const char* title, bool* p_open)
    {
      ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
      if (!ImGui::Begin(title, p_open))
      {
        ImGui::End();
        return;
      }

      ImGui::End();
    }
};

std::vector<ExampleAppConsole> WORKSPACE;


void UpdatePaintFrame(void)
{
  BeginDrawing();
  ClearBackground(DARKGRAY);

  // start ImGui Conent
  rlImGuiBegin();

#if 1
  static bool create_new_workspace = false;

  if(create_new_workspace)
  {
    WORKSPACE.push_back(ExampleAppConsole());
    create_new_workspace = false;
    printf("%lu\n", WORKSPACE.size());
  }

  ImGuiWindowFlags window_flags = 0;
  window_flags |= ImGuiWindowFlags_MenuBar;
  window_flags |= ImGuiWindowFlags_NoTitleBar;
  window_flags |= ImGuiWindowFlags_NoScrollbar;
  window_flags |= ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoResize;
  window_flags |= ImGuiWindowFlags_NoCollapse;
  window_flags |= ImGuiWindowFlags_NoNav;
  window_flags |= ImGuiWindowFlags_NoDocking;

  const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
  const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  bool use_work_area = true;
  ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
  ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
  dockspace_flags |= ImGuiDockNodeFlags_AutoHideTabBar;

  //bool open = true;
  if (!ImGui::Begin("Dear ImGui Demo", nullptr, window_flags))
  {
    // Early out if the window is collapsed, as an optimization.
    ImGui::End();
    return;
  }
  ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
  if (ImGui::BeginMenuBar())
  {
    if (ImGui::BeginMenu("Menu"))
    {
      ImGui::MenuItem("New workspace", NULL, &create_new_workspace);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  //tabs -------------------------------------------------------------
  static ImVector<int> active_tabs;
  static int next_tab_id = 0;
  if (next_tab_id == 0) // Initialize with some default tabs
    for (int i = 0; i < 1; i++)
      active_tabs.push_back(next_tab_id++);

  // Expose some other flags which are useful to showcase how they interact with Leading/Trailing tabs
  static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_TabListPopupButton;

  if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
  {
    if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip))
    {
      active_tabs.push_back(next_tab_id++); // Add new tab
    }

    // Submit our regular tabs
    for (int n = 0; n < active_tabs.Size; )
    {
      bool open = true;
      char name[16];
      snprintf(name, IM_ARRAYSIZE(name), "%04d", active_tabs[n]);

      if (ImGui::BeginTabItem(name, &open, ImGuiTabItemFlags_None))
      {
        // -----------------------------------------------------------
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
          std::string d_id = "MyDockSpace" + std::to_string(n);
          ImGuiID dockspace_id = ImGui::GetID(d_id.c_str());
          ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        ImGui::EndTabItem();
      }
      else
      {
        std::string d_id = "MyDockSpace" + std::to_string(n);
        ImGuiID dockspace_id = ImGui::GetID(d_id.c_str());
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_KeepAliveOnly);
      }

      if (!open)
        active_tabs.erase(active_tabs.Data + n);
      else
        n++;
    }

    ImGui::EndTabBar();
  }
  // -----------------------------------------------------------------

  ImGui::PopItemWidth();
  ImGui::End();

  for(size_t ii = 0; ii < WORKSPACE.size(); ++ii)
  {
    bool open = true;
    std::string name = "test" + std::to_string(ii);
    WORKSPACE[ii].Draw(name.c_str(),&open);
  }
#else
  // show ImGui Content
  bool open = true;
  ImGui::ShowDemoWindow(&open);
#endif


  // end ImGui Content
  rlImGuiEnd();

  EndDrawing();
}

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1280;
	int screenHeight = 720;

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(screenWidth, screenHeight, "raylib-Extras [ImGui] example - simple ImGui Demo");
	SetTargetFPS(144);
	rlImGuiSetup(true);

  ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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
