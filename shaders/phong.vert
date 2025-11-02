//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 140
#extension GL_ARB_explicit_attrib_location : enable

layout (location = 0) in vec4 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texcoord;

out vec2 v2f_texcoord;
out vec3 v2f_normal;
out vec3 v2f_light;
out vec3 v2f_view;

uniform mat4 modelview_projection_matrix;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform vec4 light_position; //in eye space coordinates already



void main()
{
    /** \todo Setup all outgoing variables so that you can compute in the fragment shader
      the phong lighting. You will need to setup all the uniforms listed above, before you
      can start coding this shader.

      Hint: Compute the vertex position, normal and light_position in eye space.
      Hint: Write the final vertex position to gl_Position
    */

    // vertex position in view space
    vec4 vpos_vertex = modelview_matrix * v_position;

    // vertex normals in view space
    v2f_normal = normal_matrix * v_normal;

    // light direction (l) in view space
    v2f_light = vec3(light_position) - vec3(vpos_vertex);

    // view direction (v) in view space
    v2f_view = vec3(-vpos_vertex); //camera is at origin in view space

    // texture coords are 2D and remain unchanged
    v2f_texcoord = v_texcoord;

    //final position
    gl_Position = modelview_projection_matrix * v_position;
}
