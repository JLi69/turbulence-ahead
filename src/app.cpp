#include "app.h"
#include <iostream>
#include <cstdlib>
#include <glad/glad.h>
#include <time.h>
#include "world/world.h"
#include "GL/gl-func.h"
#include <chrono>

App::App()
{ 
	mLevel = World(time(NULL));
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
	mWindow = glfwCreateWindow(960, 540, "One Lone Coder 2022 CodeJam", NULL, NULL);
	//Check if window creation failed
	if(!mWindow)
	{
		std::cout << "Failed to create window!\n";	
		glfwTerminate();	
		exit(-1);
	}
	glfwMakeContextCurrent(mWindow);

	//Initialize glad
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load glad!\n";
		glfwTerminate();
		exit(-1);
	}

	mDefaultShader = Shader("res/shaders/vert.glsl", "res/shaders/frag.glsl");
	mSquare = createSquare();
	mTexture = loadTexture("res/textures/textures.png");

	mMessageTextures[VISITED1] = loadTexture("res/textures/visited1.png");
	mMessageTextures[VISITED2] = loadTexture("res/textures/visited2.png");
	mMessageTextures[VISITED3] = loadTexture("res/textures/visited3.png");
	mMessageTextures[VISITED4] = loadTexture("res/textures/visited4.png");
	mMessageTextures[VISITED5] = loadTexture("res/textures/visited5.png");
	mMessageTextures[WIN] = loadTexture("res/textures/visited6.png");
}

void App::run()
{
	bindGLObj(mSquare);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Listen for events
	event->setCallback(mWindow);

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
	}
	glfwTerminate();

	free(mSquare);
}
