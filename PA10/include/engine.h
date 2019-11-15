#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>
#include <string>
#include <vector>
#include <btBulletDynamicsCommon.h>

#include "physics.h"
#include "window.h"
#include "graphics.h"

class Engine
{
  public:
    Engine(string name, int width, int height);
    Engine(string name);
    ~Engine();
    bool Initialize(char* vertexFilename, char* fragmentFilename, char* propertiesFilename, std::vector<std::string> allFiles);
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

    Graphics *m_graphics;
    Physics *m_physics;
    unsigned int m_DT;
    long long m_currentTimeMillis;
    bool m_running;

    // These are specific to pinball hehe
    float m_force;
    unsigned int score;
};

#endif // ENGINE_H
