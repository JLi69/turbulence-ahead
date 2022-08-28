#include "../GL/shader.h"

class Plane
{
	float mX, mY; //Position of the plane
	float mSpeed; //Speed of the plane
	float mAcceleration;
	float mRotation; //Rotation of the planet
	float mRotationSpeed = 0.0f;	
public:
	float getX();
	float getY();
	void setRotationSpeed(float rotationSpeed);
	void setAcceleration(float acceleration);

	void move(float timePassed); //Move the plane
	void draw(Shader shader); //Draw the plane
	Plane(float x, float y, float rotation);
};
