#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

enum ShaderType {
    VERTEX,
    FRAGMENT,
    GEOMETRY
};

class Shader {
public:
    Shader(ShaderType type, const char* path);
    
    unsigned int compile();
    unsigned int getId() const;
    ShaderType getType() const;
    std::string& getPath();
    
private:
    const std::string SHADER_UTILS = "./shaders/utils.glsl";

    ShaderType type_;
    std::string path_;
    unsigned int id_;
};

class Pipeline
{
public:
    Pipeline(std::vector<Shader>);
    
    void use();
    void detach();
    
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::f32* value) const;
    void setVec3(const std::string& name, const glm::f32* value) const;

	unsigned int getId() const;
    
private:
    unsigned int id_;
};