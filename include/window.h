#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <string>

#include "imgui.h"
// #include "imgui/backend/imgui_impl_sdl3.h"
// #include "imgui/backend/imgui_impl_opengl3.h"

class Window {
    public:
        SDL_Window* _window = NULL;
        ImGuiIO io;
        SDL_GLContext gl_context;

        unsigned int width, height;


        bool windowed = false;
        bool fullscreen = false;    //Settings
        bool vsync = true;
        bool ccw = true;
        unsigned int antialias = 16;
        float lineWidth = 1.0f;
        float pointSize = 1.0f;
        void imgui_draw();
        void render();

        bool init(std::string window_name, int _width, int _height);
        void quit();

        void process_event(const SDL_Event* event);
};