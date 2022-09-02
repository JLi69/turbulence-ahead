#include "sound-dev.h"
#include <stdio.h>

SoundDev::SoundDev()
{	
	mDevice = alcOpenDevice(nullptr); //Default sound device
	if(!mDevice)
		fprintf(stderr, "Failed to open default sound device!\n");
	
	mContext = alcCreateContext(mDevice, nullptr); //Create a contex
	if(!mContext)
		fprintf(stderr, "Failed to create context!\n");
	//Attempt to make the contex current
	if(!alcMakeContextCurrent(mContext))
		fprintf(stderr, "Failed to make context current!\n");
}

SoundDev::~SoundDev()
{
	if(!alcMakeContextCurrent(nullptr))
		fprintf(stderr, "failed to set context to null!\n");
	alcDestroyContext(mContext);
	if(mContext)
		fprintf(stderr, "Failed to destroy context!\n");
	if(!alcCloseDevice(mDevice))
		fprintf(stderr, "Failed to close device!\n");
}

SoundDev* SoundDev::get()
{
	static SoundDev* dev = new SoundDev;
	return dev;
}
