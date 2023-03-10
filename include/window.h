#pragma once

#include <SDL3/SDL.h>
// #include <glad/glad.h>

#include <string>

#include "imgui.h"
#include <functional>
// #include "imgui/backend/imgui_impl_sdl3.h"
// #include "imgui/backend/imgui_impl_opengl3.h"

class Window {
    public:
        SDL_Window* _window = NULL;
        ImGuiIO io;
        SDL_GLContext glContext;

        unsigned int width, height;

        bool resizable = false;
        bool borderless = false;
        bool fullscreen = false;    //Settings
        bool vsync = true;
        bool ccw = true;
        unsigned int antialias = 16;
        float lineWidth = 1.0f;
        float pointSize = 1.0f;
        void imguiDraw();
        void render();

        bool init(std::string windowName, int _width, int _height);
        void quit();

        void updateGLSettings();

        bool process_event(const SDL_Event* event);
};