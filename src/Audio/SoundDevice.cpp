#include "SoundDevice.h"
#include <AL/al.h>
#include <stdio.h>

SoundDevice* SoundDevice::get()
{
	static SoundDevice* sndDevice = new SoundDevice();
	return sndDevice;
}

SoundDevice::SoundDevice()
{
	mALCDevice = alcOpenDevice(nullptr); //Get default sound device
	//Throw an error if you cannot get a device	
	if(!mALCDevice)
		throw("failed to get sound device");
	//Create a context
	mALCContext = alcCreateContext(mALCDevice, nullptr);
	//Throw an error if you cannot get a device	
	if(!mALCContext)
		throw("failed to set sound context");
	//Attempt to make the context current (if you fail doing that, throw an error)	
	if(!alcMakeContextCurrent(mALCContext))
		throw("failed to make context current");

	//Output the name of the device
	const ALCchar* name = nullptr;
	if(alcIsExtensionPresent(mALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(mALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if(!name || alcGetError(mALCDevice) != AL_NO_ERROR)
		name = alcGetString(mALCDevice, ALC_DEVICE_SPECIFIER);
	printf("Opened %s\n", name);	
}

SoundDevice::~SoundDevice()
{
	if(!alcMakeContextCurrent(nullptr))
		throw("failed to set context to nullptr");
	alcDestroyContext(mALCContext);
	if(mALCContext)
		throw("failed to unset during close");
	if(!alcCloseDevice(mALCDevice))
		throw("failed to close sound device");
	printf("Successfully closed sound device\n");
}

