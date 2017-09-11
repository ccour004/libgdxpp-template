#include "MyAppListener.h"
#include <iostream>
#include <memory>
#include <chrono>

bool MyAppListener::isPaused = false;

class MyRawInputProcessor: public RawInputProcessor{
public:
    bool controllerAxisEvent(const SDL_ControllerAxisEvent& event){
        SDL_Log("[SDL_ControllerAxisEvent] axis: %i,value: %i",event.axis,event.value);
    }
    
    bool controllerButtonEvent(const SDL_ControllerButtonEvent& event){
        switch(event.type){
            case SDL_CONTROLLERBUTTONDOWN: SDL_Log("[SDL_CONTROLLERBUTTONDOWN]");break;
            case SDL_CONTROLLERBUTTONUP: SDL_Log("[SDL_CONTROLLERBUTTONUP]");break;
        }
    }
    
    bool controllerDeviceEvent(const SDL_ControllerDeviceEvent& event){
        switch(event.type){
            case SDL_CONTROLLERDEVICEADDED: SDL_Log("[SDL_CONTROLLERDEVICEADDED]");break;
            case SDL_CONTROLLERDEVICEREMOVED: SDL_Log("[SDL_CONTROLLERDEVICEREMOVED]");break;
            case SDL_CONTROLLERDEVICEREMAPPED: SDL_Log("[SDL_CONTROLLERDEVICEREMAPPED]");break;
        }
    }

    bool touchFingerEvent(const SDL_TouchFingerEvent& event){
        switch(event.type){
            case SDL_FINGERDOWN: SDL_Log("[SDL_FINGERDOWN]");break;
            case SDL_FINGERUP: SDL_Log("[SDL_FINGERUP]");break;
            case SDL_FINGERMOTION: SDL_Log("[SDL_FINGERMOTION]");break;
        }
    }
    bool keyboardEvent(const SDL_KeyboardEvent& event){
        SDL_Log("[SDL_KEYBOARDEVENT]");
    }

    bool mouseMotionEvent(const SDL_MouseMotionEvent& event){
        SDL_Log("[SDL_MOUSEMOTIONEVENT]");
    }
    
    bool mouseButtonEvent(const SDL_MouseButtonEvent& event){
        switch(event.type){
            case SDL_MOUSEBUTTONDOWN: SDL_Log("[SDL_MOUSEBUTTONDOWN]");break; 
            case SDL_MOUSEBUTTONUP: SDL_Log("[SDL_MOUSEBUTTONUP]");break;
        }
    }
    
    bool mouseWheelEvent(const SDL_MouseWheelEvent& event){
        SDL_Log("[SDL_MOUSEWHEELEVENT]");
    }
    
    bool multiGestureEvent(const SDL_MultiGestureEvent& event){
        SDL_Log("[SDL_MULTIGESTUREEVENT]");
    }  
};

static int TestThread(void *ptr){
    return MyAppListener::audioTest();
}

bool MyAppListener::setup(){
        //Setup gl settings.
        glEnable(GL_DEPTH_TEST);
        glClearDepthf(1.0f);
        glDepthFunc(GL_LEQUAL);
        //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glClearColor( 0.66f, 0.66f, 0.66f, 1.f );
        
        //Build texture and setup blending.
        texture = Texture("assets/test.png",GL_NEAREST,GL_NEAREST,GL_REPEAT,GL_REPEAT);
        glEnable (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
        //Set input processor.
        setRawInputProcessor(std::make_shared<MyRawInputProcessor>());
    
        //Setup shader program.
        ShaderProgram::prependVertexCode = "#version 300 es\n";
        ShaderProgram::prependFragmentCode = "#version 300 es\n";
        shaderProgram.push_back(ShaderProgram(
            fileToString("assets/default.vert"),fileToString("assets/default.frag"),"default"));
        shaderProgram.push_back(ShaderProgram(
            fileToString("assets/alt.vert"),fileToString("assets/alt.frag"),"default"));
        shaderProgram.push_back(ShaderProgram(
            fileToString("assets/texTest.vert"),fileToString("assets/texTest.frag"),"default"));
        
        for(ShaderProgram program:shaderProgram)
            if(!program.isCompiled()) return false;
            
        shaderProgram[1].setAttributef("a_color",1,1,0,1);
        
        center = Vector3(0,0,0);
        camera = PerspectiveCamera(67,640,480);
        camera.near = 0.01f;
        camera.far = 200;
        camera.position.set(0,0,3);
        camera.lookAt(center);
        camera.update();
        
        std::vector<VertexAttribute> attr{VertexAttribute::position()},
            attr2{VertexAttribute::position(),VertexAttribute::texCoords(0)};
        std::vector<GLfloat> vertices{
                        -1,1,-2,   0,0,
                        1,1,-2,    1,0,
                        1,1,0,     1,0,
                        -1,1,0,    0,0,
                        -1,-1,-2,  0,1,
                        1,-1,-2,   1,1,
                        1,-1,0,    1,1,
                        -1,-1,0    ,0,1
        };

        std::vector<GLuint> indices{
            3,1,0,2,1,3, 
            0,5,4,1,5,0,
            3,4,7,0,4,3,
            1,6,5,2,6,1,
            2,7,6,3,7,2,
            6,4,5,7,4,6
        };
        
        meshes.push_back(std::make_shared<Mesh>(VERTEX_BUFFER_OBJECT,vertices,attr2,
                    INDEX_BUFFER_OBJECT,indices,true,true));
        
        //Setup instance.
        vertices = std::vector<GLfloat>();
        indices = std::vector<GLuint>();
        MeshBuilder::build(vertices,indices,2,2,2,20,20);
        meshes.push_back(std::make_shared<Mesh>(VERTEX_BUFFER_OBJECT,vertices,attr,
                    INDEX_BUFFER_OBJECT,indices,true,true));
}

bool MyAppListener::create(){
    //Audio creation test
    //SDL_DetachThread(SDL_CreateThread(TestThread, "TestThread", (void *)NULL));
    
    setup();
    return true;
 }
 
  void MyAppListener::pause(){
     //SDL_Log("ME PAUSE!");
     isPaused = true;
 }
 
 void MyAppListener::resume(){
     //SDL_Log("ME RESUME!");
     isPaused = false;
 }
 
  void MyAppListener::render(){
    glViewport(0,0,width,height);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /*camera.translate(0.0f,0.0f,0.1f);
      camera.update();*/
      std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
    std::chrono::duration<double> delta = now - lastTime;lastTime = now;
      camera.rotateAround(Vector3(0,0,0),Vector3(0,1,0),
        std::chrono::duration_cast<std::chrono::milliseconds>(delta).count() * 0.1f);
    camera.update();

    //
    shaderProgram[1].begin();
    shaderProgram[1].setUniformMatrix("u_projView",camera.combined);
    meshes[/*1*/0]->render(shaderProgram[1],GL_TRIANGLES);
    shaderProgram[1].end();
    
    //
    shaderProgram[2].begin();
    shaderProgram[2].setUniformMatrix("u_projView",camera.combined);
    meshes[0]->render(shaderProgram[2],GL_TRIANGLES);
    shaderProgram[2].end();
 }
 
#define MUS_PATH "assets/test.wav"
Uint8* MyAppListener::audio_pos;
Uint32 MyAppListener::audio_len;

 void my_audio_callback(void *userdata, Uint8 *stream, int len);
 
 bool MyAppListener::audioTest(){
     	// local variables
	static Uint32 wav_length; // length of our sample
	static Uint8 *wav_buffer; // buffer containing our audio file
	static SDL_AudioSpec wav_spec; // the specs of our piece of music
	
	
	/* Load the WAV */
	// the specs, length and buffer of our wav are filled
	if( SDL_LoadWAV(MUS_PATH, &wav_spec, &wav_buffer, &wav_length) == NULL ){
	  return 1;
	}
	// set the callback function
	wav_spec.callback = my_audio_callback;
    //wav_spec.freq = 22050;
    //wav_spec.format = AUDIO_S16;
    wav_spec.channels = 2;    //1 = mono, 2 = stereo 
    //wav_spec.samples = 1024;  // Good low-latency value for callback 
	wav_spec.userdata = NULL;
    
        /*
          wanted.freq = 22050;
    wanted.format = AUDIO_S16;
    wanted.channels = 2;    //1 = mono, 2 = stereo 
    wanted.samples = 1024;  // Good low-latency value for callback 
    wanted.callback = fill_audio;
    wanted.userdata = NULL;
     */
    
	// set our global static variables
	audio_pos = wav_buffer; // copy sound buffer
	audio_len = wav_length; // copy file length
	
	/* Open the audio device */
	if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
	  fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	  exit(-1);
	}else SDL_Log("Opening audio: %s",MUS_PATH);
	
	/* Start playing */
	SDL_PauseAudio(0);

	// wait until we're don't playing
	while ( audio_len > 0 ) {
        if(isPaused) SDL_PauseAudio(1);
        else SDL_PauseAudio(0);
		SDL_Delay(100); 
	}
	
	// shut everything down
	SDL_CloseAudio();
    SDL_FreeWAV(wav_buffer);
 }
 
 // audio callback function
// here you have to copy the data of your audio buffer into the
// requesting audio buffer (stream)
// you should only copy as much as the requested length (len)
void MyAppListener::my_audio_callback(void *userdata, Uint8 *stream, int len) {
	if (audio_len ==0)
		return;
	
	len = ( len > audio_len ? audio_len : len );
	SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
	//SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
	
	audio_pos += len;
	audio_len -= len;
}
 
 void MyAppListener::resize(int width, int height){
    this->width = width;this->height = height;
    camera.viewportWidth = width;
    camera.viewportHeight = height;
    camera.lookAt(center);
    camera.update();
 }
 
 void MyAppListener::dispose(){
     SDL_Log("DISPOSE CALLED!");
     delete texture;
 }