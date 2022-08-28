#ifndef SOUND_BUFFER_H
#include <AL/al.h>
#include <vector>

class SoundBuffer
{
	SoundBuffer();
	~SoundBuffer();

	std::vector<ALuint> mSoundEffectBuffers;
public:
	static SoundBuffer* get();
	
	ALuint addSoundEffect(const char *filename);
	bool removeSoundEffect(const ALuint &buffer);
};
#define SOUND_BUFFER_H
#endif
