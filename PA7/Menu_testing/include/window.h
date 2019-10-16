#ifndef WINDOW_H
#define WINDOW_H
#define GLM_ENABLE_EXPERIMENTAL

#include <SDL2/SDL.h>
#include <string>
using namespace std;

class Window
{
  public:
    Window();
    ~Window();
    bool Initialize(const string &name, int* width, int* height);
    void Swap();
		SDL_Window* get_SDL_Window();
		SDL_GLContext get_SDL_Context();

  private:
    SDL_Window* gWindow;
    SDL_GLContext gContext;
};

#endif /* WINDOW_H */
