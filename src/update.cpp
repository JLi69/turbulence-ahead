#include "app.h"
#include <cmath>
#include <algorithm>

void App::update(float timePassed)
{
	static bool sPlayerHitTornado = false;

	//On menu
	if(mState == MENU)
	{
		if(event->keyTapped(GLFW_KEY_UP)) mSelected--;
		else if(event->keyTapped(GLFW_KEY_DOWN)) mSelected++;
		mSelected %= 2;
		if(mSelected < 0)
			mSelected = 1;

		//Select option
		if(event->keyTapped(GLFW_KEY_ENTER) || event->keyTapped(GLFW_KEY_SPACE))
		{
			if(mSelected == 0)
				mState = GAME;
			else if(mSelected == 1)
				mState = CREDITS;
		}
		return;
	}
	//On credits screen
	else if(mState == CREDITS)
	{
		if(event->keyTapped(GLFW_KEY_ESCAPE)) mState = MENU;
		return;	
	}

	if(event->keyPressed(GLFW_KEY_SPACE))
	{
		if(mMessageType == WIN)
		{
			//TODO: put player back on main menu
			exit(0);
		}
		mPaused = false;
	}

	if(mPaused)
		return;

	//Rotate the player	
	if(event->keyPressed(GLFW_KEY_LEFT))
		mPlayer.setRotationSpeed(sPlayerHitTornado ? TORNADO_ROTATION_SPEED : -1.0f);
	else if(event->keyPressed(GLFW_KEY_RIGHT))
		mPlayer.setRotationSpeed(sPlayerHitTornado ? TORNADO_ROTATION_SPEED : 1.0f);
	else
		mPlayer.setRotationSpeed(sPlayerHitTornado ? TORNADO_ROTATION_SPEED : 0.0f);

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

		if(mWeather < STORM)
			mWeather = (Weather)((int)mWeather + 1);

		//Update the clouds based on the weather
		switch(mWeather)
		{
		case CLOUD:		
			for(auto &cloud : mClouds)
			{	
				cloud.setCloudType(DARK);
				cloud.setSpeed(cloud.getSpeed() + 0.5f);	
			}	
			break;
		case RAIN:
			//Add a bunch of torandoes
			for(int i = 0; i < 12; i++)
			{
				Tornado temp = Tornado((float)rand() / (float)RAND_MAX * 100.0f + mPlayer.getX() - 50.0f, (float)rand() / (float)RAND_MAX * 100.0f + mPlayer.getY() - 50.0f, 2.0f);	
				temp.setAngle((float)rand() / (float)RAND_MAX * 2.0f * 3.1415926535f);	
				mTorandoes.push_back(temp);	
			}	
			for(auto &cloud : mClouds)
			{
				if(rand() % 3 != 0) // 2/3 of the time will the cloud turn into a rain cloud
					cloud.setCloudType(RAIN_CLOUD);		
				cloud.setSpeed(cloud.getSpeed() + 0.5f);	
			}	
			break;
		case STORM:
			//Add more tornadoes
			for(int i = 0; i < 4; i++)
			{
				Tornado temp = Tornado((float)rand() / (float)RAND_MAX * 100.0f + mPlayer.getX() - 50.0f, (float)rand() / (float)RAND_MAX * 100.0f + mPlayer.getY() - 50.0f, 2.0f);	
				temp.setAngle((float)rand() / (float)RAND_MAX * 2.0f * 3.1415926535f);	
				mTorandoes.push_back(temp);	
			}	
			for(auto &cloud : mClouds)
				if(rand() % 3 != 0) // 2/3 of the time will the cloud turn into a storm cloud
					cloud.setCloudType(STORM_CLOUD);		
			break;
		default:
			break;
		}
	}

	//Update the clouds
	for(auto &cloud : mClouds)
	{
		//Cloud is out of range	
		if((cloud.getX() - mPlayer.getX()) * (cloud.getX() - mPlayer.getX()) + (cloud.getY() - mPlayer.getY()) * (cloud.getY() - mPlayer.getY())  >= 2000.0f / TILE_SIZE * 2000.0f / TILE_SIZE)
		{
			float dist = 1200.0f / TILE_SIZE + (float)rand() / (float)RAND_MAX * 800.0f / TILE_SIZE,
				  angle = (float)rand() / (float)RAND_MAX * 3.1415926535f * 2.0f; 	
			cloud.setCloudPos(dist * cosf(angle) + mPlayer.getX(),
							  dist * sinf(angle) + mPlayer.getY()); 
		}	
		cloud.move(timePassed);	
	}

	//Update the tornadoes
	sPlayerHitTornado = false;
	//Sort the torandoes
	std::sort(mTorandoes.begin(), mTorandoes.end(),
			  [](Tornado t1, Tornado t2)
			  {
					return t1.getY() > t2.getY();
			  });
	for(auto &tornado : mTorandoes)
	{
		//Tornado is out of range	
		if((tornado.getX() - mPlayer.getX()) * (tornado.getX() - mPlayer.getX()) + (tornado.getY() - mPlayer.getY()) * (tornado.getY() - mPlayer.getY())  >= 2000.0f / TILE_SIZE * 2000.0f / TILE_SIZE)
		{
			float dist = 1200.0f / TILE_SIZE + (float)rand() / (float)RAND_MAX * 800.0f / TILE_SIZE,
				  angle = (float)rand() / (float)RAND_MAX * 3.1415926535f * 2.0f; 	
			tornado.setTornadoPos(dist * cosf(angle) + mPlayer.getX(),
								  dist * sinf(angle) + mPlayer.getY()); 
			tornado.setAngle(3.1415926f * 2.0f * (float)rand() / (float)RAND_MAX);	
		}	
		tornado.move(timePassed);
		//Spin the player
		if((tornado.getX() - mPlayer.getX()) * (tornado.getX() - mPlayer.getX()) + (tornado.getY() - mPlayer.getY()) * (tornado.getY() - mPlayer.getY()) <= 1.0f * 1.0f)
			sPlayerHitTornado = true;	
	}

	mTotalTime += timePassed;
}
