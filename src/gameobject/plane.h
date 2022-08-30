#ifndef PLANE_H
#include "../GL/shader.h"
#define MAX_SPEED 15.0f
#define MIN_SPEED 1.0f

#define PI 3.1415926535f

class Plane
{
	float mX, mY; //Position of the plane
	float mSpeed; //Speed of the plane
	float mAcceleration;
	float mRotation; //Rotation of the planet
	float mRotationSpeed = 0.0f;	
	float mFuel = 100.0f;

	//Wind
	float mWindSpeed = 0.0f, mWindAngle = 0.0f;

	//Assumes angle is in radians
	static float clampAngle(float angle);
public:
	float getX();
	float getY();
	float getSpeed();
	float getFuel();

	float getWindSpeed();
	float getWindAngle();

	void updateWindSpeed();
	void updateWindAngle(int goalX, int goalY);
	void refuel();
	void setRotationSpeed(float rotationSpeed);
	void setAcceleration(float acceleration);

	void move(float timePassed); //Move the plane
	void draw(Shader shader); //Draw the plane
	Plane(float x, float y, float rotation);
};
#define PLANE_H
#endif
