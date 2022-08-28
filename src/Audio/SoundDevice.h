#ifndef SOUND_DEV_H
#include <AL/alc.h>

class SoundDevice
{	
	SoundDevice();
	~SoundDevice();
	ALCdevice* mALCDevice;
	ALCcontext* mALCContext;
public:	
	static SoundDevice* get();
};
#define SOUND_DEV_H
#endif
