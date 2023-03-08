#include <window.h>

#include <iostream>
#include <imgui.h>
#include "imgui/backend/imgui_impl_sdl3.h"
#include "imgui/backend/imgui_impl_opengl3.h"

bool Window::init(std::string window_name, int _width, int _height) {
    // Initialize data
    width = _width;
    height = _height;

	// Initialize SDL with video
	SDL_Init(SDL_INIT_VIDEO);

    // Create Window
    unsigned int flags = SDL_WINDOW_OPENGL | (SDL_WINDOW_BORDERLESS * borderless) | (SDL_WINDOW_FULLSCREEN * fullscreen) | (SDL_WINDOW_RESIZABLE * resizable);
    _window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, flags);
    
    gl_context = SDL_GL_CreateContext(_window);

    // Check OpenGL properties
    printf("OpenGL loaded\n");
    gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    std::cout << glGetString(GL_VERSION) <<std::endl;

    // setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    // Imgui styling stuff
    ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // setup platform/renderer bindings
    ImGui_ImplSDL3_InitForOpenGL(_window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");
    SDL_GL_MakeCurrent(_window, gl_context);

    // GL Settings
    glEnable(GL_DEPTH_TEST);        //Setup Global OpenGL State!
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    if(ccw) glFrontFace(GL_CCW);
    else glFrontFace(GL_CW);
    glLineWidth(lineWidth);
    glPointSize(pointSize);
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    SDL_GL_SetSwapInterval(vsync);

    return true;
}

void Window::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    imgui_draw();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Swap OpenGL buffers
    SDL_GL_SwapWindow(_window);
}

void Window::quit() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Window::process_event(const SDL_Event* event) {
    ImGui_ImplSDL3_ProcessEvent(event);
    switch (event->type)
    {
    case SDL_EVENT_WINDOW_RESIZED:
        width = event -> window.data1;
        height = event -> window.data2;
        glViewport(0, 0, width, height);
        break;
    }
}