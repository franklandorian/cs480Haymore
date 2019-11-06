#include "graphics.h"

Graphics::Graphics()
{

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
    physicsWorld->createObject(m_properties[i]);
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

  return true;
}

void Graphics::Update(unsigned int dt)
{
	m_dt = dt;
  // Update the objects
  for (int i = 0; i < m_objs.size(); ++i)		
	{
    physicsWorld->Update(dt, m_objs[i], i);
    m_objs[i]->Update(dt, m_objs[i]->getObjType());
	}
  
	//m_camera->printCameraPos();
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
  glUniform4f(m_shader->GetUniformLocation("AmbientProduct"), 0.5, 0.5, 0.5,1);
  glUniform4f(m_shader->GetUniformLocation("DiffuseProduct"), 1,1,1,1);
  glUniform4f(m_shader->GetUniformLocation("SpecularProduct"), 1,1,1,1);

  glUniform4f(m_shader->GetUniformLocation("LightPosition"), 0,2,0,0);
  glUniform1f(m_shader->GetUniformLocation("Shininess"), 10);

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