//  Contains member functions of TEXTURE.H

#include "Texture.h"

GLuint Texture::LoadTexture(GLchar* path, std::string name)
{
    this->m_name = name;
    // Generate texture ID and load texture data 
    glGenTextures(1, &this->m_texID);
    int width, height, numChannels;
    unsigned char* image = stbi_load(path, &width, &height, &numChannels, 0);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, this->m_texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);

    return this->m_texID;
}

GLuint Texture::LoadHDR(GLchar* path, std::string name)
{
    this->m_name = name;
    this->m_texType = GL_TEXTURE_2D;

    std::string tempPath = std::string(path);

    glGenTextures(1, &this->m_texID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->m_texID);

    if (stbi_is_hdr(tempPath.c_str()))
    {
        int width, height, numComponents;
        float* texData = stbi_loadf(tempPath.c_str(), &width, &height, &numComponents, 0);

        if (texData)
        {
            // Need a higher precision format for HDR to not lose informations, thus 32bits floating point
            if (numComponents == 3)
            {
                this->m_texInternalFormat = GL_RGB32F;
                this->m_texFormat = GL_RGB;
            }
            else if (numComponents == 4)
            {
                this->m_texInternalFormat = GL_RGBA32F;
                this->m_texFormat = GL_RGBA;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, this->m_texInternalFormat, width, height, 0, this->m_texFormat, GL_FLOAT, texData);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glGenerateMipmap(GL_TEXTURE_2D);
        }

        else
        {
            std::cerr << "HDR TEXTURE - FAILED LOADING : " << path << std::endl;
        }

        stbi_image_free(texData);
    }

    else
    {
        std::cerr << "HDR TEXTURE - FILE IS NOT HDR : " << path << std::endl;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    return this->m_texID;
}

GLuint Texture::GetTextureID()
{
    return this->m_texID;
}