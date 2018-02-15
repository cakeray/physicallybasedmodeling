#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <SOIL/stb_image.h>

#include <iostream>
#include <string>


class Texture
{
public:
    GLuint m_texID;
    GLenum m_texType, m_texInternalFormat,m_texFormat;
    std::string m_name;

    Texture()
    {

    }

    ~Texture()
    {
        glDeleteTextures(1, &this->m_texID);
    }
    
    GLuint LoadTexture(GLchar* path, std::string name);
    GLuint LoadHDR(GLchar* path, std::string name);
    GLuint GetTextureID();
};


#endif // !_TEXTURE_H_

