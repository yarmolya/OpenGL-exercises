//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#include "texture.hh"
#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include "lodepng.h"

//=============================================================================


Texture::Texture() :
    id_(0)
{
}


//-----------------------------------------------------------------------------


Texture::~Texture()
{
    if (id_) glDeleteTextures(1, &id_);
}


//-----------------------------------------------------------------------------

bool Texture::loadPNG(const char* filename)
{
    std::cout << "Load texture " << filename << "\n" << std::flush;

    std::vector<uint8_t> img;
    unsigned width, height;

    unsigned error = lodepng::decode(img, width, height, filename);
    if (error) {
        std::cout << "read error: " << lodepng_error_text(error) << std::endl;
        return false;
    }

    return uploadImage(img, width, height);
}

//-----------------------------------------------------------------------------


bool Texture::uploadImage(std::vector<uint8_t> &img, unsigned width, unsigned height)
{
    if (!id_) {
        std::cerr << "Texture: initialize before loading!\n";
        return false;
    }

    // flip vertically in order to adhere to how OpenGL interpretes image data
    if (height % 2) throw std::runtime_error("Image must be of even height");
    assert(height % 2 == 0);
    for (unsigned int y = 0; y < height/2; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            for (unsigned int c = 0; c < 4; ++c) {
                std::swap(img[(              y  * width + x) * 4 + c],
                          img[((height - y - 1) * width + x) * 4 + c]);
            }
        }
    }

    // upload texture data
    glActiveTexture(unit_);
    glBindTexture(type_, id_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glTexImage2D(type_, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img[0]);


    if(minfilter_==GL_LINEAR_MIPMAP_LINEAR)
    {
        // comment out to disable mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    return true;
}


//-----------------------------------------------------------------------------

bool Texture::createSunBillboardTexture()
{
    std::cout << "creating sun billboard " << "\n" << std::flush;

    std::vector<uint8_t> img;
    int width = 900;
    int height = 900;
    img.resize(width*height * 4);

    /** \todo Set up the texture for the sun billboard.
  *   - Draw an opaque circle with a 150 pixel radius in its middle
  *   - Outside that circle the texture should become more and more transparent to mimic a nice glow effect
  *   - Make sure that your texture is fully transparent at its borders to avoid seeing visible edges
  *   - Experiment with the color and with how fast you change the transparency until the effect satisfies you
  **/


    //assign texture centerpoint
    float center_x = width / 2.0f;
    float center_y = height / 2.0f;

    //assign radius values
    float inner_radius = 150; //full effect strength radius
    float outer_radius = 450; //gradual fading between inner radius and this

    //base sun glow color to fill
    uint8_t R = 255;
    uint8_t G = 110;
    uint8_t B = 50;


    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            float dist_x = fabs(x - center_x);
            float dist_y = fabs(y - center_y);
            float dist_center = sqrtf((dist_x * dist_x) + (dist_y * dist_y));
            float alpha;

            if (dist_center < inner_radius) {
                //color opaque
                alpha = 1;
            }
            else if (dist_center < outer_radius) {
                //modify alpha based on dist_center
                float rel_dist = (outer_radius - dist_center) / (outer_radius - inner_radius);
                float attenuate = 3.5; //soften glow strength towards edges
                alpha = powf(rel_dist,attenuate);
                
                G = std::max((200 * alpha), 120.0f); //add bright highlight close to surface
            }
            else 
            {
                alpha = 0;
            }

            uint8_t A = 255 * alpha;

            img[(y * width + x) * 4 + 0] = R;
            img[(y * width + x) * 4 + 1] = G;
            img[(y * width + x) * 4 + 2] = B;
            img[(y * width + x) * 4 + 3] = A;

        }
    }
    return uploadImage(img, width, height);
}


//-----------------------------------------------------------------------------


void Texture::init(GLenum unit, GLenum type, GLint minfilter, GLint magfilter, GLint wrap)
{
    // remember this
    unit_ = unit;
    type_ = type;
    minfilter_ = minfilter;

    // activate texture unit
    glActiveTexture(unit_);

    // create texture object
    glGenTextures(1, &id_);
    glBindTexture(type_, id_);

    // set texture parameters
    glTexParameteri(type_, GL_TEXTURE_MAG_FILTER, magfilter);
    glTexParameteri(type_, GL_TEXTURE_MIN_FILTER, minfilter);
    glTexParameteri(type_, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(type_, GL_TEXTURE_WRAP_T, wrap);
}


//-----------------------------------------------------------------------------


void Texture::bind()
{
    assert(id_);
    glActiveTexture(unit_);
    glBindTexture(type_, id_);
}



//=============================================================================
