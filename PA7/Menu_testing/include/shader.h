#ifndef SHADER_H
#define SHADER_H
#define GLM_ENABLE_EXPERIMENTAL

#include <vector>

#include "graphics_headers.h"

class Shader
{
  public:
    Shader();
		Shader(std::string vShader, std::string fShader);
    ~Shader();
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);

  private:
    GLuint m_shaderProg;    
    std::vector<GLuint> m_shaderObjList;

		// Shader string variables and methods
		std::string m_Vertex;
		std::string m_Fragment;
		
};

#endif  /* SHADER_H */
