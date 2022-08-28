#include "app.h"

void App::update(float timePassed)
{
	//Rotate the player	
	if(event->keyPressed(GLFW_KEY_LEFT))
		mPlayer.setRotationSpeed(-1.0f);
	else if(event->keyPressed(GLFW_KEY_RIGHT))
		mPlayer.setRotationSpeed(1.0f);
	else
		mPlayer.setRotationSpeed(0.0f);

	//Accelerate the player
	if(event->keyPressed(GLFW_KEY_UP))
		mPlayer.setAcceleration(1.0f);
	else if(event->keyPressed(GLFW_KEY_DOWN))
		mPlayer.setAcceleration(-1.0f);
	else
		mPlayer.setAcceleration(0.0f);

	mPlayer.move(timePassed);
}
