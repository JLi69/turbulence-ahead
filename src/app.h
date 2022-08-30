#ifndef APP_H
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "world/world.h"
#include "event/event.h"
#include "GL/gl-func.h"
#include "GL/shader.h"
#include "gameobject/plane.h"

#define NUM_OF_MESSAGES 8

enum Message
{
	VISITED1,
	VISITED2,
	VISITED3,
	VISITED4,
	VISITED5,
	WIN,
	LOSE,
	NONE
};

class App
{
	GLFWwindow* mWindow;
	World mLevel;	
	EventHandler* event = EventHandler::get();

	Shader mDefaultShader;
	GLBufferObj* mSquare;
	unsigned int mTexture;

	bool mPaused = false;
	unsigned int mMessageTextures[NUM_OF_MESSAGES];
	Message mMessageType = NONE;

	//Player plane
	Plane mPlayer = Plane(500.0f, 500.0f, 0.0f);

	void display(float timePassed);
	void update(float timePassed);
	
	//Private constructor and destructor
	App();
	~App();
public:
	static App* get();
		
	void init();	
	void run();
};

#define APP_H
#endif
