#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height, char* vertexFilename, char* fragmentFilename, char* settingFilename, std::vector<std::string> allFiles)
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

  // Init Camera
  m_camera = new Camera();
  if(!m_camera->Initialize(width, height))
  {
    printf("Camera Failed to Initialize\n");
    return false;
  }

	// Set the setting?
	initSetting(settingFilename);
	
  // Create the objects
	for (int i = 0; i < allFiles.size(); ++i)
		m_objs.push_back(new model(allFiles[i]));
	
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
  // Update the objects
  for (int i = 0; i < m_objs.size(); ++i)
		m_objs[i]->Update(dt, (i+1)*2);		// rn offset is just some increasing so the planets aren't overlaying each other

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
void Graphics::initSetting(char* settingFilename)
{
	vector<string> lines;
	string line;
	ifstream setFile;
	setFile.open(settingFilename);

	if (setFile.fail())
		cout << "Failed to open setting file\n";
	else
	{
		while (getline(setFile, line))
			lines.push_back(line);
	} // end file io
	setFile.close();

	smatch setMatch;
	regex objNamePull("([A-Za-z]+)\\: ([0-9]+)");
	regex objPropPull("([a-zA-Z]+)\\: ([-+]?[0-9]+\\.[0-9]+)");
	regex objEndPull("(\\!)");
	float val;
	string name;
	int index;
	setting passIn;
	for (int i = 0; i < lines.size(); ++i)
	{
		if (regex_search(lines[i], setMatch, objPropPull))
		{
			name = setMatch[1];
			val = stof(setMatch[2]);
			if (name.compare("radius") == 0)
					passIn.radius = val;
		}
		else if (regex_search(lines[i], setMatch, objNamePull))
		{
			name = setMatch[1];
			index = stoi(setMatch[2]);
			passIn.name = name;
			passIn.index = index;
		}
		else if (regex_search(lines[i], setMatch, objEndPull))
		{
			// push here?
			m_settings.push_back(passIn);
			passIn = {};		// clears the struct
		}
	}
}
