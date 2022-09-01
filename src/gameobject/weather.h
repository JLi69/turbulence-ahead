#ifndef CLOUD_H
#include "../GL/shader.h"

enum CloudType
{
	NORMAL,
	DARK,
	RAIN_CLOUD,
	STORM_CLOUD
};

class Cloud
{
	float mX, mY; //Position of the cloud
	float mSpeed; //Speed of the cloud
	float mAnimationTime = 0.0f;
	CloudType mType;
public:
	float getX();
	float getY();
	float getSpeed();
	void setSpeed(float speed);
	void setCloudType(CloudType type);
	void setCloudPos(float x, float y);
	Cloud(float x, float y, float speed, CloudType type);

	void move(float timePassed);
	void draw(Shader shader, float cameraX, float cameraY);
};

class Tornado
{
	float mX, mY; //Position of the Tornado
	float mSpeed; //Speed of the tornado
	float mAnimationTime = 0.0f;
	float mAngle = 0.0f;
public:
	float getX();
	float getY();
	void setAngle(float angle);
	float getSpeed();
	void setSpeed(float speed);
	void setTornadoPos(float x, float y);
	Tornado(float x, float y, float speed);

	void move(float timePassed);
	void draw(Shader shader, float cameraX, float cameraY);
};
#define CLOUD_H
#endif
