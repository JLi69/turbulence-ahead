#include "app.h"
#include <glad/glad.h>
#include <cmath>
#include <iostream>
#include "GL/gl-func.h"

int App::digits(int num)
{
	if(num == 0) return 1;
	return (num > 0) ? ((int)log10(num) + 1) : ((int)log10(-num) + 1);
}

void App::display(float timePassed)
{		
	mDefaultShader.use();
	glClear(GL_COLOR_BUFFER_BIT);		

	int winWidth, winHeight;
	glfwGetWindowSize(mWindow, &winWidth, &winHeight);

	//Draw the title
	if(mState == MENU)
	{				
		//Title	
		activateTexture(mTitle, GL_TEXTURE0);
		glUniform1f(mDefaultShader.getUniformLocation("uDarkness"), 1.0f);	
		glUniform1f(mDefaultShader.getUniformLocation("uTexFrac"), 1.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 0.0f, 0.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uTexSize"), 128.0f, 72.0f);
		glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uScale"), 960.0f, 540.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uOffset"), 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);				
	
		//Selected
		activateTexture(mTexture, GL_TEXTURE0);
		glUniform1f(mDefaultShader.getUniformLocation("uTexFrac"), 1.0f / 16.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 0.0f / 16.0f, 10.0f / 16.0f);	
		glUniform2f(mDefaultShader.getUniformLocation("uTexSize"), 256.0f, 256.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uScale"), 64.0f, 64.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uOffset"), -40.0f, -90.0f - mSelected * 90.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		getGLErrors();

		return;
	}
	//Draw the credits creen
	else if(mState == CREDITS)
	{
		activateTexture(mCredits, GL_TEXTURE0);
		glUniform1f(mDefaultShader.getUniformLocation("uDarkness"), 1.0f);	
		glUniform1f(mDefaultShader.getUniformLocation("uTexFrac"), 1.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 0.0f, 0.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uTexSize"), 256.0f, 144.0f);
		glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uScale"), 960.0f, 540.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uOffset"), 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);	
		
		getGLErrors();

		return;
	}

	glUniform1f(mDefaultShader.getUniformLocation("uDarkness"), 1.0f - 0.2f * mWeather);
	glUniform2f(mDefaultShader.getUniformLocation("uTexSize"), 256.0f, 256.0f);
	activateTexture(mTexture, GL_TEXTURE0);
	
	//Draw the world centered on the player		
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);
	mLevel->displayWorld(mPlayer.getX(),
						mPlayer.getY(),
						mDefaultShader,
						timePassed);

	//Draw the tornadoes	
	glUniform2f(mDefaultShader.getUniformLocation("uScale"), 96.0f, 96.0f);
	for(auto tornado : mTorandoes)
		tornado.draw(mDefaultShader, mPlayer.getX(), mPlayer.getY());

	//Draw the player
	mPlayer.draw(mDefaultShader);

	//Draw the clouds	
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);	
	glUniform2f(mDefaultShader.getUniformLocation("uScale"), 96.0f, 96.0f);
	for(auto cloud : mClouds)
		cloud.draw(mDefaultShader, mPlayer.getX(), mPlayer.getY());	

	//Raindrop effect
	if(mWeather >= RAIN)
	{
		static std::pair<float, float> offsets[] = 
		{
			{ 2050.0f, 1000.0f },
			{ 2090.0f, 1100.0f },	
		};

		mRainShader.use();
		glUniform2f(mRainShader.getUniformLocation("uScreenDimensions"), winWidth, winHeight);
		glUniform1f(mRainShader.getUniformLocation("uTime"), mTotalTime - floorf(mTotalTime));
		for(int i = 0; i < 2; i++)
		{			
			glUniform2f(mRainShader.getUniformLocation("uOffset"), offsets[i].first, offsets[i].second);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 2400);
		}	
		mDefaultShader.use();
	}

	glUniform1f(mDefaultShader.getUniformLocation("uDarkness"), 1.0f);	
	//Speedometer
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uScale"), 120.0f, 120.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 1.0f / 16.0f, 14.0f / 16.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uOffset"), -winWidth / 2.0f + 100.0f, -winHeight / 2.0f + 30.0f);
	glDrawArrays(GL_TRIANGLES, 0, 6);	
	glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 2.0f / 16.0f, 14.0f / 16.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uOffset"), -winWidth / 2.0f + 100.0f, -winHeight / 2.0f + 30.0f);
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 
			3.1415926535f * (mPlayer.getSpeed() - MIN_SPEED) / (MAX_SPEED - MIN_SPEED));	
	glDrawArrays(GL_TRIANGLES, 0, 6);	
	
	//Fuel meter
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 6.0f / 16.0f, 14.0f / 16.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uOffset"), -winWidth / 2.0f + 100.0f, -winHeight / 2.0f + 160.0f);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 2.0f / 16.0f, 14.0f / 16.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uOffset"), -winWidth / 2.0f + 100.0f, -winHeight / 2.0f + 159.0f);
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 
			3.1415926535f * mPlayer.getFuel() / 100.0f);	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Wind direction and strength
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 5.0f / 16.0f, 14.0f / 16.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uOffset"), -winWidth / 2.0f + 240.0f, -winHeight / 2.0f + 90.0f);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glUniform2f(mDefaultShader.getUniformLocation("uScale"), mPlayer.getWindSpeed() * 60.0f, mPlayer.getWindSpeed() * 60.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 3.0f / 16.0f, 14.0f / 16.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uOffset"), -winWidth / 2.0f + 240.0f, -winHeight / 2.0f + 90.0f);
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), -mPlayer.getWindAngle());	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Weather report
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uScale"), 80.0f, 80.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), (float)mWeather / 16.0f, 13.0f / 16.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uOffset"), -winWidth / 2.0f + 70.0f, winHeight / 2.0f - 60.0f);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Arrow to point to the next goal
	std::pair<int, int> goal = mLevel->getCurrentGoal();
	glUniform2f(mDefaultShader.getUniformLocation("uScale"), 32.0f, 32.0f);
	float angle = atan(((float)goal.second - mPlayer.getY()) / ((float)goal.first - mPlayer.getX()));

	//Correct the angle
	if((float)goal.first - mPlayer.getX() < 0.0f && (float)goal.second - mPlayer.getY() < 0.0f ||
	   (float)goal.first - mPlayer.getX() < 0.0f && (float)goal.second - mPlayer.getY() > 0.0f)
		angle += 3.14159f;
	angle *= -1.0f;

	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), angle);
	glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 4.0f / 16.0f, 14.0f / 16.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uOffset"), 60.0f * cosf(angle), -60.0f * sinf(angle));
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Timer for speedrunning or something
	int seconds = (int)floorf(mTotalTime - floorf(mTotalTime / 60.0f) * 60.0f),
		minutes = (int)floorf(mTotalTime / 60.0f);
	char* minuteNum = (char*)malloc(digits(minutes));
	char* secondNum = (char*)malloc(digits(seconds));
	sprintf(minuteNum, "%d", minutes);
	sprintf(secondNum, "%d", seconds);

	float x = winWidth / 2.0f - 30.0f, y = winHeight / 2.0f - 30.0f;
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uScale"), 32.0f, 32.0f);
	for(int i = digits(seconds) - 1; i >= 0; i--)
	{
		glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 1.0f / 16.0f * (secondNum[i] - '0'),  9.0f / 16.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uOffset"), x, y);
		x -= 32.0f;
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	//Extra zero if seconds is single digit
	if(digits(seconds) == 1)
	{
		glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 0.0f,  9.0f / 16.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uOffset"), x, y);
		x -= 32.0f;
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	//Colon
	glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 10.0f / 16.0f,  9.0f / 16.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uOffset"), x, y);
	x -= 32.0f;
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Minutes
	for(int i = digits(minutes) - 1; i >= 0; i--)
	{
		glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 1.0f / 16.0f * (minuteNum[i] - '0'),  9.0f / 16.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uOffset"), x, y);
		x -= 32.0f;
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	//Deallocate the minute and second timers
	free(minuteNum);
	free(secondNum);

	//Message box
	if(mState == GAME_OVER)
	{
		activateTexture(mMessageTextures[LOSE], GL_TEXTURE0);
		glUniform2f(mDefaultShader.getUniformLocation("uTexSize"), 128.0f, 32.0f);
		glUniform1f(mDefaultShader.getUniformLocation("uTexFrac"), 1.0f);
		glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uScale"), 640.0f, 160.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 0.0f, 0.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uOffset"), 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	else if(mPaused)
	{		
		activateTexture(mMessageTextures[mMessageType], GL_TEXTURE0);
		glUniform2f(mDefaultShader.getUniformLocation("uTexSize"), 128.0f, 32.0f);
		glUniform1f(mDefaultShader.getUniformLocation("uTexFrac"), 1.0f);
		glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uScale"), 640.0f, 160.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 0.0f, 0.0f);
		glUniform2f(mDefaultShader.getUniformLocation("uOffset"), 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);	
	}

	//Print out any opengl errors
	getGLErrors();
}
