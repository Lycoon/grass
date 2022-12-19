#include "Pipeline.h"

Shader::Shader(ShaderType type, const char* path):
    type_(type), 
    path_(path)
{
    id_ = compile();
}

std::string getFileContent(std::string path)
{
    std::string code;
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(path);

        std::stringstream shaderStream;
        shaderStream << file.rdbuf();
        file.close();

        code = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    
    return code;
}

unsigned int Shader::compile()
{
    std::string utilsCode = getFileContent(SHADER_UTILS);
    std::string shaderCode_ = getFileContent(path_) + "\n\n" + utilsCode.c_str();
    const char* shaderCode = shaderCode_.c_str();
    
    unsigned int shaderId;
    switch (type_)
    {
	case VERTEX:
        shaderId = glCreateShader(GL_VERTEX_SHADER);
		break;
	case FRAGMENT:
        shaderId = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case GEOMETRY:
        shaderId = glCreateShader(GL_GEOMETRY_SHADER);
		break;
    default:
        shaderId = glCreateShader(GL_VERTEX_SHADER);
    }

    int success;
    char infoLog[512];
    glShaderSource(shaderId, 1, &shaderCode, NULL);
    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    
    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

	std::cout << "Shader compiled successfully: " << shaderId << std::endl;

    return shaderId;
}

Pipeline::Pipeline(std::vector<Shader> shaders)
{
    int success;
    char infoLog[512];
    
    // attaching shaders
    id_ = glCreateProgram();
    for (const auto& shader : shaders){
        glAttachShader(id_, shader.getId());
    }

    glLinkProgram(id_);
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(id_, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // deleting shaders
    for (const auto& shader : shaders){
        glDeleteShader(shader.getId());
    }
}

void Pipeline::use() {
    glUseProgram(id_);
}

void Pipeline::detach() {
    glDeleteProgram(id_);
}

void Pipeline::setBool(const std::string& name, bool value) const{ glUniform1i(glGetUniformLocation(id_, name.c_str()), (int)value); }
void Pipeline::setInt(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(id_, name.c_str()), value); }
void Pipeline::setFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(id_, name.c_str()), value); }
void Pipeline::setMat4(const std::string& name, const glm::f32* value) const { glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, value); }
void Pipeline::setVec3(const std::string& name, const glm::f32* value) const { glUniform3fv(glGetUniformLocation(id_, name.c_str()), 1, value); }

unsigned int Pipeline::getId() const { return id_; }
unsigned int Shader::getId() const { return id_; }
ShaderType Shader::getType() const { return type_; }
std::string& Shader::getPath() { return path_; }