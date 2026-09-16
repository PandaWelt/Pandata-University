#define SDL_MAIN_HANDLED
#include "imgui.h"
#include <iostream>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#ifdef _WIN32
#include <windows.h>  // SetProcessDPIAware()
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif
#include "AppUISetup.h"


const char* CheckVersionGLSL()
{
            // Decide GL+GLSL versions
      #if defined(IMGUI_IMPL_OPENGL_ES2)
          // GL ES 2.0 + GLSL 100 (WebGL 1.0)
            const char* glsl_version = "#version 100";
      #elif defined(IMGUI_IMPL_OPENGL_ES3)
          // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
            const char* glsl_version = "#version 300 es";
      #elif defined(__APPLE__)
          // GL 3.2 Core + GLSL 150
            const char* glsl_version = "#version 150";
      #else
          // GL 3.0 + GLSL 130
            const char* glsl_version = "#version 130";
      #endif
            return glsl_version;
}

void SetGLAttributes()
{
            // Decide GL+Attributes
      #if defined(IMGUI_IMPL_OPENGL_ES2)
          // GL ES 2.0 + GLSL 100 (WebGL 1.0)
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
      #elif defined(IMGUI_IMPL_OPENGL_ES3)
          // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
      #elif defined(__APPLE__)
          // GL 3.2 Core + GLSL 150
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
      #else
          // GL 3.0 + GLSL 130
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
      #endif
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
}

float SetupImGUIContext()
{
      float main_scale = ImGui_ImplSDL2_GetContentScaleForDisplay(0);
      // Setup Dear ImGui context
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO& io = ImGui::GetIO(); (void)io;
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

      // Setup Dear ImGui style
      ImGui::StyleColorsDark();
      //ImGui::StyleColorsLight();

      // Setup scaling
      ImGuiStyle& style = ImGui::GetStyle();
      style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
      style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

      return main_scale;
}





int main(int, char**)
{
            // Setup SDL
      #ifdef _WIN32
            ::SetProcessDPIAware();
      #endif
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
            {
                  printf("Error: %s\n", SDL_GetError());
                  return 1;
            }
            // From 2.0.18: Enable native IME.
      #ifdef SDL_HINT_IME_SHOW_UI
            SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
      #endif

      const char* glsl_version = CheckVersionGLSL();
      SetGLAttributes();

      float main_scale = SetupImGUIContext();

      SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
      SDL_Window* window = SDL_CreateWindow
      (

            "Pandata.Bank",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            (int)(1280 * main_scale),
            (int)(800 * main_scale),
            window_flags

      );

      if (window == nullptr)
      {
            printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
            return 1;
      }

      SDL_GLContext gl_context = SDL_GL_CreateContext(window);
      if (gl_context == nullptr)
      {
            printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
            return 1;
      }

      SDL_GL_MakeCurrent(window, gl_context);
      SDL_GL_SetSwapInterval(1); // Enable vsync

      // Setup Platform/Renderer backends
      ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
      ImGui_ImplOpenGL3_Init(glsl_version);

      bool done = false;
#ifdef __EMSCRIPTEN__
      // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
      // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
      io.IniFilename = nullptr;
      EMSCRIPTEN_MAINLOOP_BEGIN
#else
      ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

      while (!done)
#endif
      {

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                  ImGui_ImplSDL2_ProcessEvent(&event);
                  if (event.type == SDL_QUIT)
                        done = true;
                  if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                        done = true;
            }
            if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
            {
                  SDL_Delay(10);
                  continue;
            }


            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            AppUISetup(clear_color);
            
            

            // Rendering
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            ImGui::Render();
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(window);
      }
#ifdef __EMSCRIPTEN__
      EMSCRIPTEN_MAINLOOP_END;
#endif

      // Cleanup
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplSDL2_Shutdown();
      ImGui::DestroyContext();

      SDL_GL_DeleteContext(gl_context);
      SDL_DestroyWindow(window);
      SDL_Quit();

      return 0;
}
