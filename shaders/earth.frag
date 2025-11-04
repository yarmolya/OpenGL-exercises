//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================

#version 140

in vec3 v2f_normal;
in vec2 v2f_texcoord;
in vec3 v2f_light;
in vec3 v2f_view;

out vec4 f_color;

uniform sampler2D day_texture;
uniform sampler2D night_texture;
uniform sampler2D cloud_texture;
uniform sampler2D gloss_texture;
uniform bool greyscale;

const float shininess = 20.0;
const vec3  sunlight = vec3(1.0, 0.941, 0.898);

void main()
{
    // we normalize vectors first
    vec3 N = normalize(v2f_normal);
    vec3 L = normalize(v2f_light);
    vec3 V = normalize(v2f_view);
    vec3 R = reflect(-L, N);

    // colors are RGBs and gloss is a 1-channel grayscale r=g=b
    vec3 day_color = texture(day_texture, v2f_texcoord).rgb;
    vec3 night_color = texture(night_texture, v2f_texcoord).rgb;
    vec3 cloud_color = texture(cloud_texture, v2f_texcoord).rgb;
    float gloss = texture(gloss_texture, v2f_texcoord).r;

    // Step 1: Combine gloss and cloudiness textures to get a grayscale value
    // specifying the amount of specularity ((0, 1).
    // clouds are white (high values) where present, gloss is high over water
    // we want specularity where there's water (high gloss) but no clouds
    float cloudiness = cloud_color.r;
    float specularity = gloss * (1.0 - cloudiness);

    // Step 2: Get a color of the day component by applying Phong lighting model
    // (disregarding the clouds) and day texture.
    float cos_theta = max(dot(N, L), 0.0);
    vec3 day_ambient = sunlight * 0.2 * day_color;
    vec3 day_diffuse = sunlight * day_color * cos_theta;
    vec3 day_specular = vec3(0.0);
    if (cos_theta > 0.0) {
        float cos_alpha = max(dot(R, V), 0.0);
        day_specular = sunlight * specularity * pow(cos_alpha, shininess);
    }

    vec3 day_component = day_ambient + day_diffuse + day_specular;

    // Step 3: Get the final color of the day component by interpolating between
    // the current day and clouds rendered with Lambertian lighting model;
    // interpolation weight is given by cloudiness.
    vec3 cloud_ambient = sunlight * 0.2 * cloud_color;
    vec3 cloud_diffuse = sunlight * cloud_color * cos_theta;
    vec3 clouds_lambertian = cloud_ambient + cloud_diffuse;

    // mix day surface with clouds based on cloudiness
    vec3 day_final = mix(day_component, clouds_lambertian, cloudiness);

    // Step 4: Get a color of the night component by sampling night texture and
    // scale it down by cloudiness.
    vec3 night_component = night_color * (1.0 - cloudiness);

    // Step 5: Mix day and night components by linear interpolation where you can use diffuse component
    // of the Phong lighting model as the interpolation weight.
    // use the diffuse lighting factor (cos_theta) as interpolation weight
    // when sun is up (cos_theta > 0), show day; when sun is down, show night
    // clamp cos_theta to [0,1] range for linear interpolation
    float day_night_mix = clamp(cos_theta, 0.0, 1.0);
    vec3 color = mix(night_component, day_final, day_night_mix);

    // convert to grayscale if requested
    if (greyscale) color = vec3(0.299*color.r + 0.587*color.g + 0.114*color.b);


    // final color
    f_color = vec4(color, 1.0);
}
