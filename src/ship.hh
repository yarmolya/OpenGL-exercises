#pragma once

#include "glmath.hh"
#include "gl.hh"
#include <vector>
#include "texture.hh"

class Ship
{
    public:
        Ship();
        ~Ship();

        /// loads vertices and faces from .off file
        bool load_model(const char* filename);

        /// updates ships position and angle
        void update_ship();

        /// changes ship's forward speed
        void accelerate(float speedup);

        /// changes ship's angular speed
        void accelerate_angular(float angular_speedup);

        /// draws the ship
        void draw();

        /// main diffuse texture for the planet
        Texture tex_;

        void set_position(vec4 const&pos) { pos_ = pos; }
        void set_direction(vec4 const&dir) { direction_ = dir; }
        float get_scale() const {return 0.002f;}
    private:
        void compute_normals();
        void initialize_buffers();


        /// vertex array
        std::vector<vec3> vertices_;
        /// triangle index array
        std::vector<int> indices_;
        /// vertex normals
        std::vector<vec3> vertex_normals_;
        /// face normals
        std::vector<vec3> face_normals_;

        /// indices of the triangle vertices
        unsigned int n_indices_ = 0;

        // vertex array object
        GLuint vao_ = 0;
        /// vertex buffer object
        GLuint vbo_ = 0;
        /// texture coordinates buffer object
        GLuint tbo_ = 0;
        /// normals buffer object
        GLuint nbo_ = 0;
        /// index buffer object
        GLuint ibo_ = 0;


public:
        /// current position
        vec4 pos_ = vec4{0, 0, 0, 1};

        /// current y-axis angle
        float angle_ = 0.f;

        /// current direction in which the ship faces
        vec4 direction_ = vec4{0, 0, 1, 0};

        /// current forward speed (pos_ += speed*direction)
        float speed_ = 0.f;

        /// current angular speed (angle_ += angular_speed)
        float angular_speed_ = 0.f;
};

