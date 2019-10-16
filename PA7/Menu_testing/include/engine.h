#ifndef ENGINE_H
#define ENGINE_H
#define GLM_ENABLE_EXPERIMENTAL

#include <sys/time.h>
#include <assert.h>

#include "window.h"
#include "graphics.h"

class Engine
{
  public:
    Engine(string name, int width, int height);
		Engine(string name, int width, int height, string vShader, string fShader);
    Engine(string name);
    ~Engine();
    bool Initialize();
    void Run();
    void Keyboard();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
  
  private:
    // Window related variables
    Window *m_window;
    string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;
    SDL_Event m_event;

		// Shader related variables
		string m_Vertex_Shader;
		string m_Fragment_Shader;

    Graphics *m_graphics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;

		// Variables for keyboard control
		/* 
			 1 = q	<-- flips orbit
			 2 = w	<-- flips rotation
			 3 = e	<-- stops orbit
			 4 = r	<-- stop rotation
			 5 = t	<-- pause?
		*/ 
		int m_keyValue = 0;

		// glsl version for ImGui
		const char* glsl_version = "#version 130";
};

#endif // ENGINE_H
