
#include "engine.h"

Engine::Engine(string name, int width, int height)
{
  m_WINDOW_NAME = name;
  m_WINDOW_WIDTH = width;
  m_WINDOW_HEIGHT = height;
  m_FULLSCREEN = false;
}

Engine::Engine(string name)
{
  m_WINDOW_NAME = name;
  m_WINDOW_HEIGHT = 0;
  m_WINDOW_WIDTH = 0;
  m_FULLSCREEN = true;
}

Engine::~Engine()
{
  delete m_window;
  delete m_graphics;
  m_window = NULL;
  m_graphics = NULL;
}

bool Engine::Initialize(char* vertexFilename, char* fragmentFilename, char* propertiesFilename, std::vector<std::string> allFiles)
{
  // Start a window
  m_window = new Window();
  if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT))
  {
    printf("The window failed to initialize.\n");
    return false;
  }

  // Start physics engine
  m_physics = new Physics();
  // Should probably have a throw if somehting doesn't go right, but whatever

  // Start the graphics
  m_graphics = new Graphics(m_physics);
  if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, vertexFilename, fragmentFilename, propertiesFilename, allFiles))
  {
    printf("The graphics failed to initialize.\n");
    return false;
  }

  // Set the time
  m_currentTimeMillis = GetCurrentTimeMillis();

  // No errors
  return true;
}

void Engine::Run()
{
  m_running = true;

  while(m_running)
  {
    // Update the DT
    m_DT = getDT();

    // free camera stuff
    m_graphics->processInput(m_DT);
    // Check the keyboard input
    while(SDL_PollEvent(&m_event) != 0)
    {
      Keyboard();
    }

    // Update and render the graphics and physics
    m_graphics->Update(m_DT);
    m_graphics->updateCamera();
    m_graphics->Render();

    // Swap to the Window
    m_window->Swap();
  }
}

void Engine::Keyboard()
{
  if(m_event.type == SDL_QUIT)
  {
    m_running = false;
  }
  else if (m_event.type == SDL_KEYDOWN)
  {
    // handle key down events here
    if (m_event.key.keysym.sym == SDLK_ESCAPE)
    {
      m_running = false;
    }
		else if(m_event.key.keysym.sym >= SDLK_0 && m_event.key.keysym.sym <= SDLK_3 || m_event.key.keysym.sym == SDLK_r)
		{
      m_graphics->updateCamera(m_event.key.keysym.sym);
    }
		else if(m_event.key.keysym.sym == SDLK_f)
		{
      m_graphics->toggleFreeFly();
  	}
		else if (m_event.key.keysym.sym == SDLK_i || m_event.key.keysym.sym == SDLK_j || m_event.key.keysym.sym == SDLK_k || m_event.key.keysym.sym == SDLK_l || m_event.key.keysym.sym == SDLK_u || m_event.key.keysym.sym == SDLK_o)				// i,j,k,l == w,a,s,d   (wasd used for Camera)
		{
			m_graphics->getModel(1)->buttonHandler(m_event.key.keysym.sym);
		} else if (m_event.key.keysym.sym == SDLK_LEFT)
    {
      m_graphics->MoveCube("left");
    } else if (m_event.key.keysym.sym == SDLK_RIGHT)
    {
      m_graphics->MoveCube("right");
    } else if (m_event.key.keysym.sym == SDLK_UP)
    {
      m_graphics->MoveCube("up");
    } else if (m_event.key.keysym.sym == SDLK_DOWN)
    {
      m_graphics->MoveCube("down");
    }  else if (m_event.key.keysym.sym == SDLK_z)
    {
      m_graphics->SwapShaders();
    }  else if (m_event.key.keysym.sym == SDLK_x)
    {
      m_graphics->SwapShaders();
    }
	}
	else if(m_event.type == SDL_MOUSEMOTION)
	{
  	m_graphics->mouseMovement(m_event.motion.xrel, m_event.motion.yrel, m_DT);
	}
}

unsigned int Engine::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
