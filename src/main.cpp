#include <iostream>
#include <window.h>
#include <chrono>
#include <thread>
#include <shader.h>

#include <imgui.h>
#include <imgui/backend/imgui_impl_opengl3.h>
#include <imgui/backend/imgui_impl_sdl3.h>

// Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};


int counter = 0;
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
    ImGui::Text("Main window width: %d", width);
    ImGui::Text("Main window height: %d", height);

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
        std::cout << "counter button clicked" << std::endl;
        counter++;
        if (counter == 9) { ImGui::OpenPopup("Easter egg"); }
    }
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    if (ImGui::BeginPopupModal("Easter egg", NULL))
    {
        ImGui::Text("Ho-ho, you found me!");
        if (ImGui::Button("Buy Ultimate Orb")) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }

    ImGui::Dummy(ImVec2(0.0f, 15.0f));

    ImGui::End();
}

int main(int argc, char * argv[]) {
	
    Window w;
    // w.fullscreen = true;
    // w.borderless = true;
    w.resizable = true;
    w.init("Game", 680, 430);
	
	SDL_Event event;	 // used to store any events from the OS
	bool running = true; // used to determine if we're running the game
	
    
    SDL_version linked;
    SDL_GetVersion(&linked);
    std::cout << "Linked with "
              << std::to_string(linked.major)
              << "." << std::to_string(linked.minor)
              << "." << std::to_string(linked.patch) << std::endl;
    //std::cout << linkedVal.str() << std::endl;

    // setup SDL window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(
        SDL_GL_CONTEXT_PROFILE_MASK,
        SDL_GL_CONTEXT_PROFILE_CORE);

    std::cout << "[INFO] OpenGL renderer: "
              << glGetString(GL_RENDERER)
              << std::endl;
    
    auto now = std::chrono::steady_clock::now();
    auto then = std::chrono::steady_clock::now();
    double dt;
    int interval_ms = 10;
    // gladLoadGLLoader((GLADloadproc) );
    Shader s("vertex shader 1", Shader::vertex);
    s.loadFromFile("./shader/test.vs");


	
	while(running) {

        
        now = std::chrono::steady_clock::now();
        dt = std::chrono::duration_cast<std::chrono::nanoseconds> (now - then).count() * 0.000000001;
        then = now;

        auto next = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval_ms);
        std::this_thread::sleep_until(next);
        
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // without it you won't have keyboard input and other things
            w.process_event(&event);
            // you might also want to check io.WantCaptureMouse and io.WantCaptureKeyboard
            // before processing events

            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                running = false;
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                glViewport(0, 0, event.window.data1, event.window.data2);
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                }
                break;
            }
        }
        
        w.render();
	}
	

	return 0;
}