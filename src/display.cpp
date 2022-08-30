#include "app.h"
#include <glad/glad.h>
#include <cmath>
#include <iostream>
#include "GL/gl-func.h"

void App::display(float timePassed)
{		
	mDefaultShader.use();
	glClear(GL_COLOR_BUFFER_BIT);		


	glUniform2f(mDefaultShader.getUniformLocation("uTexSize"), 256.0f, 256.0f);
	activateTexture(mTexture, GL_TEXTURE0);
	
	//Draw the world centered on the player		
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);
	mLevel.displayWorld(mPlayer.getX(),
						mPlayer.getY(),
						mDefaultShader,
						timePassed);


	//Draw the player
	mPlayer.draw(mDefaultShader);

	int winWidth, winHeight;
	glfwGetWindowSize(mWindow, &winWidth, &winHeight);	
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

	//Wind direction
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), 0.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 5.0f / 16.0f, 14.0f / 16.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uOffset"), -winWidth / 2.0f + 240.0f, -winHeight / 2.0f + 90.0f);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glUniform2f(mDefaultShader.getUniformLocation("uScale"), mPlayer.getWindSpeed() * 40.0f, mPlayer.getWindSpeed() * 40.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uTexOffset"), 3.0f / 16.0f, 14.0f / 16.0f);
	glUniform2f(mDefaultShader.getUniformLocation("uOffset"), -winWidth / 2.0f + 240.0f, -winHeight / 2.0f + 90.0f);
	glUniform1f(mDefaultShader.getUniformLocation("uRotation"), -mPlayer.getWindAngle());	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Arrow to point to the next goal
	std::pair<int, int> goal = mLevel.getCurrentGoal();
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

	//Message box
	if(mPaused)
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
