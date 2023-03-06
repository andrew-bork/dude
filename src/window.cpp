#include <window.h>

#include <iostream>
#include <imgui.h>
#include "imgui/backend/imgui_impl_sdl3.h"
#include "imgui/backend/imgui_impl_opengl3.h"

bool Window::init(std::string window_name, int _width, int _height) {
	// Initialize SDL with video
	SDL_Init(SDL_INIT_VIDEO);
    width = _width;
    height = _height;
    _window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    
    gl_context = SDL_GL_CreateContext(_window);



    // setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // setup platform/renderer bindings
    ImGui_ImplSDL3_InitForOpenGL(_window, gl_context);
    SDL_GL_MakeCurrent(_window, gl_context);
    
    SDL_GL_SetSwapInterval(vsync);

    ImGui_ImplOpenGL3_Init("#version 130");

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

    std::cout<<glGetString(GL_VERSION)<<std::endl;

    return true;
}

void Window::imgui_draw() {

        // start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    

        ImGui::Begin("Controls", NULL, ImGuiWindowFlags_NoResize);

        ImGui::Dummy(ImVec2(0.0f, 1.0f));
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Time");
        // ImGui::Text("%s", currentTime(std::chrono::system_clock::now()).c_str());

        ImGui::Dummy(ImVec2(0.0f, 3.0f));
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
        ImGui::Text("%s", SDL_GetPlatform());
        ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
        ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);

        ImGui::Dummy(ImVec2(0.0f, 3.0f));
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Application");
        // ImGui::Text("Main window width: %d", sdl_width);
        // ImGui::Text("Main window height: %d", sdl_height);

        ImGui::Dummy(ImVec2(0.0f, 3.0f));
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "SDL");
        // ImGui::Text("%s", compiledVal.str().c_str());
        // ImGui::Text("%s", linkedVal.str().c_str());

        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        
        // buttons and most other widgets return true when clicked/edited/activated
        if (ImGui::Button("Counter button"))
        {
            std::cout << "counter button clicked\n";
            // counter++;
            // if (counter == 9) { ImGui::OpenPopup("Easter egg"); }
        }
        ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);

        if (ImGui::BeginPopupModal("Easter egg", NULL))
        {
            ImGui::Text("Ho-ho, you found me!");
            if (ImGui::Button("Buy Ultimate Orb")) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        // if (!show_demo_window)
        // {
        //     if (ImGui::Button("Open standard demo"))
        //     {
        //         show_demo_window = true;
        //     }
        // }

        // ImGui::Checkbox("show a custom window", &show_another_window);
        // if (show_another_window)
        // {
        //     ImGui::SetNextWindowSize(
        //         ImVec2(400.0f, 350.0f),
        //         ImGuiCond_FirstUseEver // after first launch it will use values from imgui.ini
        //         );
        //     // the window will have a closing button that will clear the bool variable
        //     ImGui::Begin("A custom window", &show_another_window);
            
        //     ImGui::Dummy(ImVec2(0.0f, 1.0f));
        //     ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Files in the current folder");
                            
        //     //ImGui::TextColored(ImVec4(128 / 255.0f, 128 / 255.0f, 128 / 255.0f, 1.0f), "%s", currentPath.string().data());
        //     ImGui::Dummy(ImVec2(0.0f, 0.5f));

        //     // static int currentFile = 0;
        //     // ImVec2 windowSize = ImGui::GetWindowSize();
        //     // ImGui::PushItemWidth(windowSize.x - 15);
        //     // ImGui::ListBox(
        //     //     "",
        //     //     &currentFile,
        //     //     vector_getter,
        //     //     &files,
        //     //     static_cast<int>(files.size())
        //     //     );

        //     ImGui::Dummy(ImVec2(0.0f, 1.0f));
        //     if (ImGui::Button("Close"))
        //     {
        //         std::cout << "close button clicked\n";
        //         show_another_window = false;
        //     }

        //     ImGui::End();
        // }

        ImGui::End();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    imgui_draw();
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
}