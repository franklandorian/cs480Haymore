#include "graphics.h"

Graphics::Graphics()
{
  shaderFlag = false;
}

Graphics::Graphics(Physics *physicsWorld) : physicsWorld(physicsWorld)
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height, char* vertexFilename, char* fragmentFilename, char* propertiesFilename, std::vector<std::string> allFiles)
{
  // Used for the linux OS
  #if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
  #endif

  // For OpenGL 3
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

	// Set the object properties before creating them
	initProperties(propertiesFilename);
  // Create the objects
	for (int i = 0; i < allFiles.size(); ++i)
	{
		m_objs.push_back(new model(allFiles[i], m_properties[i]));
    physicsWorld->createObject(m_properties[i], m_objs[i]->getTriangles());
	}

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER, vertexFilename))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, fragmentFilename))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  m_AmbientProduct = m_shader->GetUniformLocation("AmbientProduct");
  m_DiffuseProduct = m_shader->GetUniformLocation("DiffuseProduct");
  m_SpecularProduct = m_shader->GetUniformLocation("SpecularProduct");
  m_LightPosition = m_shader->GetUniformLocation("LightPosition");
  m_Shininess = m_shader->GetUniformLocation("Shininess");

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Intialize lighting
  ambient = glm::vec4(0.5,0.5,0.5,1);
  specular = glm::vec4(0.5,0.5,0.5,1);
  rainbowColors.emplace_back(glm::vec4(255,0,0,1)); // Red
  rainbowColors.emplace_back(glm::vec4(255,127,0,1)); // Orange
  rainbowColors.emplace_back(glm::vec4(255,255,0,1)); // Yellow
  rainbowColors.emplace_back(glm::vec4(0,255,0,1)); // Green
  rainbowColors.emplace_back(glm::vec4(0,0,255,1)); // Blue
  rainbowColors.emplace_back(glm::vec4(75,0,130,1)); // Indigo
  rainbowColors.emplace_back(glm::vec4(148,0,211,1)); // Violet

  // Because this filter looks the best
  currentColor = 5;

  // We start with three lifes
  lifes = 3;
  gameOver = false;
  totalScore = 0;
  score = 0;

  return true;
}

void Graphics::Update(unsigned int dt)
{
    if(!gameOver){
        m_dt = dt;
      // Update the objects
      for (int i = 0; i < m_objs.size(); ++i)
        {
        physicsWorld->Update(dt, m_objs[i], i);
        m_objs[i]->Update(dt, m_objs[i]->getObjType());
        }

        // This is the ball
        float x = m_objs[1]->getX();
        float z = m_objs[1]->getZ();

        // Only increase Score if in the play space
        if(x >= -1.3 && x <= 4.6){
            score += 1;
        }

        // Detect if it is "lost"
        if((x >= -1.3 && x <= 4.6) && (z <= -6.0)){
            // lifes--;
            std::cout << "Lost Ball! You now have: " << --lifes << " lifes" << std::endl;
            std::cout << "You gained: " << score << " points" << std::endl;

            totalScore += score;
            score = 0;

            // Restart the position
            physicsWorld->RestartBall(m_objs[1]);

            if(!lifes){
                std::cout << "Game Over. Your final score is : " << totalScore << std::endl;
                gameOver = true;
            }
        }
    }
}

model* Graphics::getModel(int objIndex)
{
	return m_objs[objIndex];
}

void Graphics::processInput(unsigned int DT){
	m_camera->update(DT);
}

void Graphics::updateCamera(){
  m_camera->update();
}

void Graphics::mouseMovement(float deltaX, float deltaY, unsigned int DT){
	m_camera->mouseMovement(deltaX, deltaY, DT);
}

void Graphics::toggleFreeFly(){
	m_camera->toggleFreeFly();
}

void Graphics::updateCamera(SDL_Keycode keycode){
  glm::vec3 followDistance = glm::vec3(0.0,-8.0,-16.0);
  if(keycode == SDLK_0){
    m_camera->setFocus(m_objs[0], followDistance);
  }else if(keycode >= 48 && keycode <= 58){
    switch (keycode-48){
      case 1:
        followDistance = glm::vec3(0.0,-0.1,0.1);
      break;
      case 2:
        followDistance = glm::vec3(0.0,-0.3,0.5);
      break;
      case 3:
        followDistance = glm::vec3(0.0,-0.3,0.5);
      	break;
			default:
				break;
    }
    m_camera->setFocus(m_objs[keycode-48], followDistance);
  }else if(keycode == SDLK_r){
    m_camera->setFocus(glm::vec3(0.0,0.0,0.0), followDistance);
  }
}

void Graphics::updateEye(int eye)
{
  m_camera->changeEyeView(eye);
}

void Graphics::Render()
{
  //clear the screen
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Start the correct program
  m_shader->Enable();

  // Send in the projection and view to the shader
  glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
  glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

  // Some lighting things
  glm::vec4 l(0.5, 0.5, 0.5, 1.0);
  // glUniform4f(m_AmbientProduct, 0.5, 0.5, 0.5,1);
  // glUniform4f(m_SpecularProduct, 1,1,1,1);
  // glUniform4f(m_DiffuseProduct,0, 0, 255.0,1);

  glUniform4fv(m_AmbientProduct, 1, glm::value_ptr(ambient));
  glUniform4fv(m_SpecularProduct, 1, glm::value_ptr(specular));
  glUniform4fv(m_DiffuseProduct, 1, glm::value_ptr(rainbowColors[currentColor]));
  glUniform4fv(m_LightPosition, 1, glm::value_ptr(l));
  glUniform1f(m_Shininess, 1.0f);

  // Render the object
	// NEED TO RENDER EACH OBJECT
	for (int i = 0; i < m_objs.size(); ++i)
	{
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_objs[i]->GetModel()));
		m_objs[i]->Render();
	}

  // Get any errors from OpenGL
  auto error = glGetError();
  if ( error != GL_NO_ERROR )
  {
    string val = ErrorString( error );
    std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
  }
}

std::string Graphics::ErrorString(GLenum error)
{
  if(error == GL_INVALID_ENUM)
  {
    return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
  }

  else if(error == GL_INVALID_VALUE)
  {
    return "GL_INVALID_VALUE: A numeric argument is out of range.";
  }

  else if(error == GL_INVALID_OPERATION)
  {
    return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
  }

  else if(error == GL_INVALID_FRAMEBUFFER_OPERATION)
  {
    return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
  }

  else if(error == GL_OUT_OF_MEMORY)
  {
    return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
  }
  else
  {
    return "None";
  }
}

// initializes our setting for the objects
void Graphics::initProperties(char* file)
{
	vector<string> lines;
	string line;
	ifstream propFile;
	propFile.open(file);

	if (propFile.fail())
		cout << "Failed to open properties file\n";
	else
	{
		while (getline(propFile, line))
			lines.push_back(line);
	} // end file io
	propFile.close();

	// create our regex so we can pull obj properties from properties file
	smatch setMatch;
	regex objNamePull("([A-Za-z]+)\\: ([0-9]+)");
	regex objPropPull("([a-zA-Z]+)\\: ([-+]?[0-9]+\\.[0-9]+)");
	regex objEndPull("(\\!)");\
	string name;
	float dec;
	int num;
	objProp passIn;

	for (int i = 0; i < lines.size(); ++i)
	{
		if (regex_search(lines[i], setMatch, objPropPull))
		{
			name = setMatch[1];
			dec = stof(setMatch[2]);
      // std::cout << name << std::endl;
			if (name.compare("x") == 0)
				passIn.startPos[0] = dec;
			else if (name.compare("y") == 0)
				passIn.startPos[1] = dec;
      else if (name.compare("z") == 0)
				passIn.startPos[2] = dec;
      else if(name.compare("size") == 0)
			{
        // std::cout << dec << std::endl;
				passIn.size = dec;
			}
		}
		else if (regex_search(lines[i], setMatch, objNamePull))
		{
			name = setMatch[1];
			num = stoi(setMatch[2]);
			if(name.compare("shape") == 0)
			{
				passIn.shape = num;
			}
			else
			{
				passIn.name = name;
				passIn.type = num;
			}
		}
		else if (regex_search(lines[i], setMatch, objEndPull))
		{
			m_properties.push_back(passIn);
			passIn = {};		// clears the struct
		}
	}
}

void Graphics::MoveCube(std::string command)
{
  // The cube is the first index, I know it's hard coded but idc
  physicsWorld->Move(command);
}

bool Graphics::SwapShaders()
{
  delete m_shader;
  m_shader = nullptr;

  std::string vertexFilename;
  std::string fragmentFilename;

  if(!shaderFlag){
    vertexFilename = "../shaders/perF-vertex.glsl";
    fragmentFilename = "../shaders/perF-fragment.glsl";
  } else {
    vertexFilename = "../shaders/perV-vertex.glsl";
    fragmentFilename = "../shaders/perV-fragment.glsl";
  }

  // Set up the shaders
  m_shader = new Shader();
  if(!m_shader->Initialize())
  {
    printf("Shader Failed to Initialize\n");
    return false;
  }

  // Add the vertex shader
  if(!m_shader->AddShader(GL_VERTEX_SHADER, const_cast<char *>  (vertexFilename.c_str())))
  {
    printf("Vertex Shader failed to Initialize\n");
    return false;
  }

  // Add the fragment shader
  if(!m_shader->AddShader(GL_FRAGMENT_SHADER, const_cast<char *> (fragmentFilename.c_str())))
  {
    printf("Fragment Shader failed to Initialize\n");
    return false;
  }

  // Connect the program
  if(!m_shader->Finalize())
  {
    printf("Program to Finalize\n");
    return false;
  }

  m_AmbientProduct = m_shader->GetUniformLocation("AmbientProduct");
  m_DiffuseProduct = m_shader->GetUniformLocation("DiffuseProduct");
  m_SpecularProduct = m_shader->GetUniformLocation("SpecularProduct");
  m_LightPosition = m_shader->GetUniformLocation("LightPosition");
  m_Shininess = m_shader->GetUniformLocation("Shininess");

  // Locate the projection matrix in the shader
  m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
  if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("m_projectionMatrix not found\n");
    return false;
  }

  // Locate the view matrix in the shader
  m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
  if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("m_viewMatrix not found\n");
    return false;
  }

  // Locate the model matrix in the shader
  m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
  if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
  {
    printf("m_modelMatrix not found\n");
    return false;
  }

  //enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  shaderFlag = !shaderFlag;

  return true;
}

void Graphics::IncreaseAmbience()
{
  if(ambient.x < 5){
      ambient += glm::vec4(0.5,0.5,0.5,1);
  }
}
void Graphics::DecreaseAmbience()
{
  if(ambient.x > 0){
      ambient -= glm::vec4(0.5,0.5,0.5,0);
  }
}
void Graphics::IncreaseSpecular()
{
  if(specular.x < 5){
      specular += glm::vec4(0.5,0.5,0.5,1);
  }
  // Testign out git
}
void Graphics::DecreaseSpecular()
{
  if(specular.x > 0){
      specular -= glm::vec4(0.5,0.5,0.5,0);
  }
}

void Graphics::ChangeColorFilter()
{
  currentColor = (currentColor + 1) % 7; // Seven Colors in the rainbow
}

void Graphics::LaunchPlunger(float force){
  physicsWorld->LaunchPlunger(force);
}

void Graphics::RestartGame(){
  if(gameOver){
      std::cout << "Game has been restarted. You have 3 Lifes" << std::endl;
      lifes = 3;
      gameOver = false;
      score = 0;
      totalScore = 0;
  }
}
