#include "app.h"
#include <cmath>

void App::update(float timePassed)
{
	if(event->keyPressed(GLFW_KEY_SPACE))
		mPaused = false;

	if(mPaused)
		return;

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

	//Check if the player is at the goal
	if((floorf(mPlayer.getX()) == mLevel.getCurrentGoal().first || ceilf(mPlayer.getX()) == mLevel.getCurrentGoal().first) &&
	   (floorf(mPlayer.getY()) == mLevel.getCurrentGoal().second || ceilf(mPlayer.getY()) == mLevel.getCurrentGoal().second))
	{	
		if(mMessageType == NONE)
			mMessageType = VISITED1;
		else
			mMessageType = (Message)((int)mMessageType + 1);
		mLevel.updateGoal();
		mPaused = true;

		//Refuel the plane
		mPlayer.refuel();
		mPlayer.updateWindSpeed();
		mPlayer.updateWindAngle(mLevel.getCurrentGoal().first, mLevel.getCurrentGoal().second);
	}
}
