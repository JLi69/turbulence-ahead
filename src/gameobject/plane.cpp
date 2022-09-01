#include "plane.h"
#include <glad/glad.h>
#include "../GL/gl-func.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

float Plane::clampAngle(float angle)
{
	if(angle >= 2.0f * PI || angle < 0.0f)
		return angle - floorf(angle / (2.0f * PI)) * 2.0f * PI;
	return angle;
}

float Plane::getX()
{
	return mX;
}

float Plane::getY()
{
	return mY;
}

float Plane::getSpeed()
{
	return mSpeed;
}

float Plane::getFuel()
{
	return mFuel;
}

float Plane::getWindSpeed()
{
	return mWindSpeed;
}

float Plane::getWindAngle()
{
	return mWindAngle;
}

void Plane::updateWindSpeed()
{
	mWindSpeed += 0.5f;
	if(mWindSpeed > 2.0f)
		mWindSpeed = 2.0f;
}

void Plane::updateWindAngle(int goalX, int goalY)
{
	if(rand() % 2 == 0)
	{
		if(goalX >= mX)
			mWindAngle = (float)rand() / (float)RAND_MAX * PI - PI * 0.5f;
		else if(goalX < mX)
			mWindAngle = (float)rand() / (float)RAND_MAX * PI + PI * 0.5f;
		mWindAngle = clampAngle(mWindAngle);	
	}
	else
	{
		if(goalY >= mY)
			mWindAngle = (float)rand() / (float)RAND_MAX * PI;
		else if(goalY < mY)
			mWindAngle = (float)rand() / (float)RAND_MAX * PI + PI;	
		mWindAngle = clampAngle(mWindAngle);	
	}
}

void Plane::refuel()
{
	mFuel = 100.0f;
}

void Plane::move(float timePassed)
{
	mSpeed += mAcceleration * timePassed;
	
	if(mSpeed > MAX_SPEED)
		mSpeed = MAX_SPEED;
	else if(mSpeed < MIN_SPEED)
		mSpeed = MIN_SPEED;
	
	mRotation += mRotationSpeed * timePassed;
	//Clamp the rotation
	if(mRotation >= 2.0f * PI || mRotation < 0.0f)
		mRotation -= floorf(mRotation / (2.0f * PI)) * 2.0f * PI;

	mX += mSpeed * sinf(mRotation) * timePassed;
	mY += mSpeed * cosf(mRotation) * timePassed;

	//Lose fuel over time
	mFuel -= mSpeed * mSpeed * timePassed * 1.0f / 50.0f;

	//the wind
	float angle = clampAngle(-clampAngle(mWindAngle - PI / 2.0f));	
	mX += mWindSpeed * sinf(angle) * timePassed;
	mY += mWindSpeed * cosf(angle) * timePassed;	
	//Rotate plane based on the direction of the wind
	float minAngle = (angle > clampAngle(angle + PI)) ? clampAngle(angle + PI) : angle,
		  maxAngle = (angle > clampAngle(angle + PI)) ? angle : clampAngle(angle + PI);
	if((angle > 0.0f && angle < PI) && (mRotation < angle || mRotation > clampAngle(angle + PI)) ||
	   (angle >= PI && angle <= 2 * PI) && (mRotation > minAngle && mRotation < maxAngle))
	{
		mRotation += mWindSpeed * 0.2f * timePassed;
	}
	else
	{	
		mRotation -= mWindSpeed * 0.2f * timePassed;
	}
}

void Plane::draw(Shader shader)
{
	glUniform2f(shader.getUniformLocation("uScale"), 56.0f, 56.0f);
	glUniform1f(shader.getUniformLocation("uTexFrac"), 1.0f / 16.0f);
	glUniform2f(shader.getUniformLocation("uTexOffset"), 0.0f, 14.0f / 16.0f);
	glUniform2f(shader.getUniformLocation("uOffset"), 0.0f, 0.0f);	
	glUniform1f(shader.getUniformLocation("uRotation"), mRotation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Plane::setRotationSpeed(float rotationSpeed)
{
	mRotationSpeed = rotationSpeed;
}

void Plane::setAcceleration(float acceleration)
{
	mAcceleration = acceleration;
}

Plane::Plane(float x, float y, float rotation)
{
	mX = x;
	mY = y;
	mSpeed = 3.0f;
	mRotation = 0.0f;
	mAcceleration = 0.0f;
}
