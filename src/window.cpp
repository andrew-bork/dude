#include <window.h>

#include <iostream>
#include <imgui.h>
#include "imgui/backend/imgui_impl_sdl3.h"
#include "imgui/backend/imgui_impl_opengl3.h"
#include <glad/glad.h>

static void initializeGL(Window * window) {

    window -> glContext = SDL_GL_CreateContext(window->_window);

    if(!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        throw std::runtime_error("GLAD failed to load");
    }
    // Check OpenGL properties
    printf("OpenGL loaded\n");
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    SDL_GL_MakeCurrent(window->_window, window->glContext);
}

bool Window::init(std::string windowName, int _width, int _height) {
    // Initialize data
    width = _width;
    height = _height;

	// Initialize SDL with video
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		// printf("Video initialization failed: %s\n", SDL_GetError( ) );
        throw std::runtime_error("Video initialization failed: " + std::string(SDL_GetError()));
	}

    // Create Window
    unsigned int flags = SDL_WINDOW_OPENGL | (SDL_WINDOW_BORDERLESS * borderless) | (SDL_WINDOW_FULLSCREEN * fullscreen) | (SDL_WINDOW_RESIZABLE * resizable);
    _window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, flags);
    
    initializeGL(this);
    
    {
        // setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = ImGui::GetIO(); (void)io;
        
        // Imgui styling stuff
        ImGui::StyleColorsDark();
        ImGui::StyleColorsClassic();

        // setup platform/renderer bindings
        ImGui_ImplSDL3_InitForOpenGL(_window, glContext);
        ImGui_ImplOpenGL3_Init("#version 130");
    }

    updateGLSettings();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    SDL_GL_SetSwapInterval(vsync);

    return true;
}

void Window::render() {
    
    imguiDraw();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Swap OpenGL buffers
    SDL_GL_SwapWindow(_window);
}

void Window::quit() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

bool Window::process_event(const SDL_Event* event) {
    if(ImGui_ImplSDL3_ProcessEvent(event)) return true;
    switch (event->type)
    {
    case SDL_EVENT_WINDOW_RESIZED:
        width = event -> window.data1;
        height = event -> window.data2;
        glViewport(0, 0, width, height);
        break;
    }
    return false;
}

void Window::imguiDraw() {
    
}

void Window::updateGLSettings() {
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
    glViewport(0,0,width,height);
}