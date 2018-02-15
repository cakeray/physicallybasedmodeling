//  Contains all the shape generation functions

#include "Functions.h"
#include "Globals.h"
//  GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

//
//  Renders a sphere
//
void render_sphere()
{
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        GLuint vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<GLuint> indices;

        const GLuint X_SEGMENTS = 64;
        const GLuint Y_SEGMENTS = 64;
        const float PI = 3.14159265359;
        for (GLuint y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (GLuint x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = indices.size();

        std::vector<float> data;
        for (int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
        float stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

//
//  renders the box
//
void render_box() {
    float box_vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
                                          // Front face
                                          -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
                                          0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
                                          0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
                                          0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
                                          -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
                                          -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left                                 
                                                                            // Left face                                                                    
                                                                            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right                                                                    
                                                                            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
                                                                            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
                                                                            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
                                                                            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
                                                                            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
                                                                                                              // Right face
                                                                                                              0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
                                                                                                              0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
                                                                                                              0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
                                                                                                              0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
                                                                                                              0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
                                                                                                              0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
                                                                                                                                               // Bottom face
                                                                                                                                               -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
                                                                                                                                               0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
                                                                                                                                               0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
                                                                                                                                               0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
                                                                                                                                               -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
                                                                                                                                               -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
                                                                                                                                                                                 // Top face
                                                                                                                                                                                 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
                                                                                                                                                                                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
                                                                                                                                                                                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
                                                                                                                                                                                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
                                                                                                                                                                                 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
                                                                                                                                                                                 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left                                                                   
    };
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    float stride = (3 + 2) * sizeof(float);
    //  position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    //  texture coordinate attributes
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //  render the box
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
