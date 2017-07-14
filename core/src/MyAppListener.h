#pragma once

#include <GL.h>
#include <math/Vector3.h>
#include <PerspectiveCamera.h>
#include <ShapeCreator.hpp>
#include <LibGDXApplication.h>
#include <graphics/glutils/ShaderProgram.h>
#include <graphics/Mesh.h>
#include <chrono>
#include <memory>

class MyAppListener: public ApplicationListener{
    bool isCreated = false;
    static bool isPaused;
    PerspectiveCamera camera;
    std::vector<ShaderProgram> shaderProgram;
    int counter = 0;
    
    //Texture/Image testing
    void buildImage();
    bool setup();
    SDL_Surface* image;
    GLuint tex;
    
    //Audio testing
    static Uint8 *audio_pos; // global pointer to the audio buffer to be played
    static Uint32 audio_len; // remaining length of the sample we have to play
    static void my_audio_callback(void *userdata, Uint8 *stream, int len);
    
    Vector3 center;
    float near = 0.1f,far = 100.0f,fov = 67.0f;
    int width = 640,height = 480;
    
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::chrono::time_point<std::chrono::system_clock> lastTime;
public:
    static bool audioTest();
 bool create();
 void resize(int width, int height);
 void render();
 void pause();
 void resume();
 void dispose();
};

