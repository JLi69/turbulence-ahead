#include "weather.h"
#include <glad/glad.h>
#include "../world/world.h"
#include <cmath>

//Clouds
float Cloud::getX()
{
	return mX;
}

float Cloud::getY()
{
	return mY;
}

float Cloud::getSpeed()
{
	return mSpeed;
}

CloudType Cloud::getType()
{
	return mType;
}

void Cloud::setSpeed(float speed)
{
	mSpeed = speed;
}

void Cloud::setCloudType(CloudType type)
{
	mType = type;
}

Cloud::Cloud(float x, float y, float speed, CloudType type)
{
	mX = x;
	mY = y;
	mSpeed = speed;	
	mType = type;
}

void Cloud::move(float timePassed)
{
	mX += mSpeed * timePassed;

	//Update animation time
	mAnimationTime += timePassed;
	if(mAnimationTime > 1.0f)
		mAnimationTime = 0.0f;
}

void Cloud::setCloudPos(float x, float y)
{
	mX = x;
	mY = y;
}

void Cloud::draw(Shader shader, float cameraX, float cameraY)
{
	//Cloud is out of range, don't draw	
	if(mX - cameraX > 1000.0f / TILE_SIZE ||
	   mX - cameraX < -1000.0f / TILE_SIZE || 
	   mY - cameraY > 600.0f / TILE_SIZE ||
	   mY - cameraY < -600.0f / TILE_SIZE)
		return;

	if(mAnimationTime < 0.5f && mType > DARK || mType <= DARK)
		glUniform2f(shader.getUniformLocation("uTexOffset"), (float)mType * 1.0f / 16.0f, 12.0f / 16.0f);
	else if(mAnimationTime >= 0.5f && mType > DARK)
		glUniform2f(shader.getUniformLocation("uTexOffset"), (float)mType * 1.0f / 16.0f, 11.0f / 16.0f);

	glUniform2f(shader.getUniformLocation("uOffset"), (mX - cameraX) * TILE_SIZE,
													  (mY - cameraY) * TILE_SIZE);	
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

//Tornado
float Tornado::getX()
{
	return mX;
}

float Tornado::getY()
{
	return mY;
}

float Tornado::getSpeed()
{
	return mSpeed;
}

void Tornado::setSpeed(float speed)
{
	mSpeed = speed;
}

void Tornado::setTornadoPos(float x, float y)
{
	mX = x;
	mY = y;
}

Tornado::Tornado(float x, float y, float speed)
{
	mX = x;
	mY = y;
	mSpeed = speed;
}

void Tornado::setAngle(float angle)
{
	mAngle = angle;
}

void Tornado::move(float timePassed)
{
	mAnimationTime += timePassed;
	if(mAnimationTime > 1.0f)
		mAnimationTime = 0.0f;

	//Move the tornado
	mX += mSpeed * cosf(mAngle) * timePassed;
	mY += mSpeed * sinf(mAngle) * timePassed;
}

void Tornado::draw(Shader shader, float cameraX, float cameraY)
{
	//Tornado is out of range, don't draw	
	if(mX - cameraX > 1000.0f / TILE_SIZE ||
	   mX - cameraX < -1000.0f / TILE_SIZE || 
	   mY - cameraY > 600.0f / TILE_SIZE ||
	   mY - cameraY < -600.0f / TILE_SIZE)
		return;

	if(mAnimationTime < 0.5f)
		glUniform2f(shader.getUniformLocation("uTexOffset"), 4.0f / 16.0f + 1.0f / 16.0f * floorf(mAnimationTime * 8.0f), 12.0f / 16.0f);
	else if(mAnimationTime >= 0.5f)
		glUniform2f(shader.getUniformLocation("uTexOffset"), 4.0f / 16.0f + 1.0f / 16.0f * floorf(8.0f - mAnimationTime * 8.0f), 12.0f / 16.0f);

	glUniform2f(shader.getUniformLocation("uOffset"), (mX - cameraX) * TILE_SIZE,
													  (mY - cameraY) * TILE_SIZE);	
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
