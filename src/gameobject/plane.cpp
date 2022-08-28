#include "plane.h"
#include <glad/glad.h>
#include "../GL/gl-func.h"
#include <cmath>

float Plane::getX()
{
	return mX;
}

float Plane::getY()
{
	return mY;
}

void Plane::move(float timePassed)
{
	mSpeed += mAcceleration * timePassed;
	
	if(mSpeed > 8.0f)
		mSpeed = 8.0f;
	else if(mSpeed < 2.0f)
		mSpeed = 2.0f;
	
	mRotation += mRotationSpeed * timePassed;
	mX += mSpeed * sinf(mRotation) * timePassed;
	mY += mSpeed * cosf(mRotation) * timePassed;
}

void Plane::draw(Shader shader)
{
	glUniform2f(shader.getUniformLocation("uScale"), 64.0f, 64.0f);
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
