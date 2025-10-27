#pragma once

#include "gl.hh"
#include "texture.hh"

class Billboard
{
public:
    /// render the billboard
    void draw();

    // Texture for billboard.
    Texture tex_;

    ~Billboard();

private:
    void initialize();

    /// indices of the triangle vertices
    unsigned int n_indices_ = 0;

    // vertex array object
    GLuint vao_ = 0;
    /// vertex buffer object
    GLuint vbo_ = 0;
    GLuint nbo_ = 0;
    /// texture coordinates buffer object
    GLuint tbo_ = 0;
    /// index buffer object
    GLuint ibo_ = 0;
};

