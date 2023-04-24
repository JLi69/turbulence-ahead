#include "app.h"
#include <iostream>
#include <cstdlib>
#include <glad/glad.h>
#include <time.h>
#include "world/world.h"
#include "GL/gl-func.h"
#include <chrono> 
#include "Audio/music.h"

App::App()
{ 
	 	
}

App::~App()
{ }

App* App::get()
{
	static App* application = new App;
	return application;
}

void App::init()
{
	//Initialize glfw	
	if(!glfwInit())
	{
		std::cout << "Failed to initialize glfw!\n";
		exit(-1);
	}

	//Attempt to create the window
	mWindow = glfwCreateWindow(960, 540, "Turbulence Ahead!", NULL, NULL);
	//Check if window creation failed
	if(!mWindow)
	{
		std::cout << "Failed to create window!\n";	
		glfwTerminate();	
		exit(-1);
	}
	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(1);	

	//Initialize glad
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load glad!\n";
		glfwTerminate();
		exit(-1);
	}

	mDefaultShader = Shader("res/shaders/vert.glsl", "res/shaders/frag.glsl");
	mRainShader = Shader("res/shaders/raindrop-vert.glsl", "res/shaders/raindrop-frag.glsl");	
	mSquare = createSquare();
	mTexture = loadTexture("res/textures/textures.png");
	mTitle = loadTexture("res/textures/title.png");
	mCredits = loadTexture("res/textures/credits.png");
	
	mMessageTextures[NONE] = loadTexture("res/textures/visited0.png");
	mMessageTextures[VISITED1] = loadTexture("res/textures/visited1.png");
	mMessageTextures[VISITED2] = loadTexture("res/textures/visited2.png");
	mMessageTextures[VISITED3] = loadTexture("res/textures/visited3.png");
	mMessageTextures[VISITED4] = loadTexture("res/textures/visited4.png");
	mMessageTextures[VISITED5] = loadTexture("res/textures/visited5.png");
	mMessageTextures[WIN] = loadTexture("res/textures/visited6.png");
	mMessageTextures[LOSE] = loadTexture("res/textures/gameover.png");

	mSoundEffects->addSoundEffect("res/audio/crash.wav");	
	mSoundEffects->addSoundEffect("res/audio/select.wav");
}

void App::run()
{
	bindGLObj(mSquare);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Listen for events
	event->setCallback(mWindow);

	MusicBuffer* background = new MusicBuffer("res/audio/wind.wav");	
	ALint backgroundALStatus;

	std::chrono::time_point<std::chrono::system_clock> start = 
		std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> end;
	float timePassed = 0.0f;
	while(!glfwWindowShouldClose(mWindow))
	{
		display(timePassed);	
		update(timePassed);

		glfwSwapBuffers(mWindow);
		glfwPollEvents();

		end = std::chrono::system_clock::now();
		std::chrono::duration<float> frameDuration =  end - start;
		timePassed = frameDuration.count();
		start = std::chrono::system_clock::now();

		//Handle background sound
		alGetSourcei(background->getSource(), AL_SOURCE_STATE, &backgroundALStatus);	
		if(backgroundALStatus != AL_PLAYING && mState == GAME)
		{
			if(background != nullptr)
			{
				delete background;
				if(mWeather <= CLOUD)
					background = new MusicBuffer("res/audio/wind.wav");
				else if(mWeather >= RAIN)
					background = new MusicBuffer("res/audio/rain_thunder_loop.wav");
			}
			background->Play();
		}
		else if(mState != GAME)
		{
			delete background;
			background = new MusicBuffer("res/audio/wind.wav");
		}
		else if(backgroundALStatus == AL_PLAYING)
			background->UpdateBufferStream();
	}
	glfwTerminate();

	free(mSquare);
}
