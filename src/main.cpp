#include <iostream>
#include <window.h>
#include <chrono>
#include <thread>

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
        SDL_GL_CONTEXT_PROFILE_CORE
        );

    std::cout << "[INFO] OpenGL renderer: "
              << glGetString(GL_RENDERER)
              << std::endl;
    
    auto now = std::chrono::steady_clock::now();
    auto then = std::chrono::steady_clock::now();
    double dt;
    int interval_ms = 10;

	glClearColor(1, 0, 0, 1);
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