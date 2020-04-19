// climbing wall gui
// Implements basic gui for the Ascent FunRock!

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#include <GL/gl3w.h>            // Initialize with gl3wInit()
#include "Shm_vars.h"

#include <string>
using std::string;

// Main code
int main(int, char**)
{
    SHM::connect_existing_shm();
    // Setup SDL
    if (SDL_Init(  SDL_INIT_VIDEO 
                 | SDL_INIT_TIMER 
                 | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions   // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags win_flags = (SDL_WindowFlags)(  SDL_WINDOW_OPENGL 
                                                  | SDL_WINDOW_RESIZABLE 
                                                  | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Ascent FunRock!", 
                                           SDL_WINDOWPOS_CENTERED, 
                                           SDL_WINDOWPOS_CENTERED, 
                                           1920, 
                                           1024, 
                                           win_flags);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    bool err = gl3wInit() != 0;
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Scale up ImGui for a high dpi monitor.
    constexpr double scale_factor = 3; 
    ImGui::GetStyle().ScaleAllSizes(scale_factor);
    ImGui::GetIO().FontGlobalScale = scale_factor;


    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (   event.type == SDL_WINDOWEVENT 
                && event.window.event == SDL_WINDOWEVENT_CLOSE 
                && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        {
            static int speed   =  SHM::req_speed->get();
            static int incline =  SHM::req_incline->get();

            static constexpr const char* 
              incline_label_overhang  = "(degrees overhang)"; 
            static constexpr const char* 
              incline_label_vertical  = "(dead vertical)"; 
            static constexpr const char* 
              incline_label_slab  = "(degrees slab)"; 

            static const ImVec2 button_size(200, 75); 

            ImGui::Begin("Simple Controls");      

            ImGui::SetNextItemWidth(400);
            ImGui::InputInt("speed (feet per minute)", &speed, 1, 3);    
            ImGui::SetNextItemWidth(400);
            ImGui::InputInt("incline", &incline, 1, 3);  
              ImGui::SameLine();   
              if (incline < 0)
                  {ImGui::Text(incline_label_overhang);}
              else if (incline == 0)
                  {ImGui::Text(incline_label_vertical);}
              else if (incline > 0)
                  {ImGui::Text(incline_label_slab);}
            
            // bounds checking on user input:
            if (speed   >   50) speed   =  50;
            if (speed   <   0 ) speed   =  0;
            if (incline < -90 ) incline = -90;
            if (incline >   15) incline =  15;

            if (ImGui::Button("Run", button_size))                             
            {
               SHM::req_speed->set(speed);
               SHM::req_incline->set(incline);
            }

            ImGui::SameLine();
            if (ImGui::Button("Pause", button_size)) SHM::req_speed->set(0);

            //ImGui::Button("Emergency Stop!!");
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
