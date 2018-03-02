#pragma once
#ifndef SHADER_H
#define SHADER_H

#include<string.h>
#include<fstream>
#include<sstream>
#include<iostream>
using namespace std;

// For opengl headers
#include<glad/glad.h>

// GLM Mathemtics Header
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader
{
public:
    //the program ID
    GLuint Program;
    //constructor reads and builds the shader
    Shader()
    {

    }
    ~Shader()
    {

    }
    //  loading vertex and fragment shaders
    void LoadShader(const GLchar* vertexPath, const GLchar* fragmentPath);
    
    //  use the program
    void Use();

    //  Set uniforms
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetVec2(const std::string &name, const glm::vec2 &value) const;
    void SetVec2(const std::string &name, float x, float y) const;
    void SetVec3(const std::string &name, const glm::vec3 &value) const;
    void SetVec3(const std::string &name, float x, float y, float z) const;
    void SetMat2(const std::string &name, const glm::mat2 &mat) const;
    void SetMat3(const std::string &name, const glm::mat3 &mat) const;
    void SetMat4(const std::string &name, const glm::mat4 &mat) const;

private:

};

#endif // !SHADER_H

