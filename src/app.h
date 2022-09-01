#ifndef APP_H
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "world/world.h"
#include "event/event.h"
#include "GL/gl-func.h"
#include "GL/shader.h"
#include "gameobject/plane.h"
#include "gameobject/weather.h"

#define NUM_OF_MESSAGES 8

#define TORNADO_ROTATION_SPEED 15.0f

class App
{
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

	enum Weather
	{
		SUN,
		CLOUD,
		RAIN,
		STORM
	};

	enum GameState
	{
		MENU,
		CREDITS,
		GAME
	};

	GLFWwindow* mWindow;
	World mLevel;	
	EventHandler* event = EventHandler::get();
	GameState mState = MENU;
	int mSelected = 0;

	Shader mDefaultShader;
	GLBufferObj* mSquare;
	unsigned int mTexture,
				 mTitle,
				 mCredits;

	bool mPaused = false;
	unsigned int mMessageTextures[NUM_OF_MESSAGES];
	Message mMessageType = NONE;

	Weather mWeather = SUN;

	float mTotalTime = 0.0f;

	//Player plane
	Plane mPlayer = Plane(500.0f, 500.0f, 0.0f);
	//Weather objects
	std::vector<Cloud> mClouds;
	std::vector<Tornado> mTorandoes;

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
