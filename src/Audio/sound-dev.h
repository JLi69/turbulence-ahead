#include <AL/alc.h>

class SoundDev
{
	SoundDev();
	~SoundDev();
	ALCdevice* mDevice;
	ALCcontext* mContext;
public:
	static SoundDev* get();
};
